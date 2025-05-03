import radb
import radb.ast


# Break up selections that use AND conditions into smaller selections
def rule_break_up_selections(node):
    if isinstance(node, radb.ast.Select):
        predicates = _split_conjunctive_predicate(node.cond)
        if len(predicates) > 1:
            inner = node.inputs[0]
           
            for pred in reversed(predicates):
                inner = radb.ast.Select(pred, inner)
            return rule_break_up_selections(inner) 
        else:
            node.inputs[0] = rule_break_up_selections(node.inputs[0])
            return node
    elif hasattr(node, "inputs"):
        for idx in range(len(node.inputs)):
            node.inputs[idx] = rule_break_up_selections(node.inputs[idx])
        return node
    else:
        return node


# Try to push down selections closer to base relations
def rule_push_down_selections(node, dd):
    if isinstance(node, radb.ast.Select):
        child = rule_push_down_selections(node.inputs[0], dd)
        node.inputs[0] = child

        pred_attrs = _get_attributes_in_predicate(node.cond)
        child_attrs = _get_attributes_in_relation(child, dd)

        if _predicate_applies_to_relation(pred_attrs, child_attrs):
            if isinstance(child, radb.ast.Select):
                outer_rels = _get_relations_in_predicate(node.cond)
                inner_rels = _get_relations_in_predicate(child.cond)

                if outer_rels < inner_rels:
                    new_node = radb.ast.Select(
                        child.cond, radb.ast.Select(node.cond, child.inputs[0])
                    )
                    return rule_push_down_selections(new_node, dd)
                else:
                    return node
            elif isinstance(child, radb.ast.Cross):
                left_child = child.inputs[0]
                right_child = child.inputs[1]
                left_attrs = _get_attributes_in_relation(left_child, dd)
                right_attrs = _get_attributes_in_relation(right_child, dd)

                if _predicate_applies_to_relation(pred_attrs, left_attrs):
                    new_left = rule_push_down_selections(
                        radb.ast.Select(node.cond, left_child), dd
                    )
                    return radb.ast.Cross(new_left, right_child)
                elif _predicate_applies_to_relation(pred_attrs, right_attrs):
                    new_right = rule_push_down_selections(
                        radb.ast.Select(node.cond, right_child), dd
                    )
                    return radb.ast.Cross(left_child, new_right)
                else:
                    return node
            else:
                return node
        else:
            return node

    elif isinstance(node, (radb.ast.Project, radb.ast.Rename)):
        node.inputs[0] = rule_push_down_selections(node.inputs[0], dd)
        return node
    elif isinstance(node, radb.ast.Cross):
        node.inputs[0] = rule_push_down_selections(node.inputs[0], dd)
        node.inputs[1] = rule_push_down_selections(node.inputs[1], dd)
        return node
    else:
        return node


# Combine multiple nested selections into one with AND in the condition
def rule_merge_selections(node):
    if isinstance(node, radb.ast.Select):
        child = rule_merge_selections(node.inputs[0])
        node.inputs[0] = child

        if isinstance(child, radb.ast.Select):
            new_cond = radb.ast.ValExprBinaryOp(node.cond, radb.ast.sym.AND, child.cond)
            new_node = radb.ast.Select(new_cond, child.inputs[0])
            return new_node
        else:
            return node
    elif hasattr(node, "inputs"):
        for idx in range(len(node.inputs)):
            node.inputs[idx] = rule_merge_selections(node.inputs[idx])
        return node
    else:
        return node


# Turn a selection over a cross product into a join if possible
def rule_introduce_joins(node):
    if hasattr(node, "inputs"):
        node.inputs = [rule_introduce_joins(child) for child in node.inputs]

    if isinstance(node, radb.ast.Select) and isinstance(node.inputs[0], radb.ast.Cross):
        child = node.inputs[0]
        left_child = child.inputs[0]
        right_child = child.inputs[1]

        left_rels = _get_relations_in_relation(left_child)
        right_rels = _get_relations_in_relation(right_child)

        if _is_join_condition(node.cond, left_rels, right_rels):
            new_node = radb.ast.Join(cond=node.cond, left=left_child, right=right_child)
            return new_node
    return node


def _split_conjunctive_predicate(cond):
    if isinstance(cond, radb.ast.ValExprBinaryOp):
        if cond.op == radb.ast.sym.AND:
            left_preds = _split_conjunctive_predicate(cond.inputs[0])
            right_preds = _split_conjunctive_predicate(cond.inputs[1])
            return left_preds + right_preds
    return [cond]


def _get_relations_in_predicate(cond):
    attrs = _get_attributes_in_predicate(cond)
    return set(rel for rel, _ in attrs if rel is not None)


def _get_attributes_in_predicate(cond):
    attrs = set()
    if isinstance(cond, radb.ast.AttrRef):
        attrs.add((cond.rel, cond.name))
    elif isinstance(cond, radb.ast.ValExprBinaryOp):
        attrs.update(_get_attributes_in_predicate(cond.inputs[0]))
        attrs.update(_get_attributes_in_predicate(cond.inputs[1]))
    elif isinstance(cond, radb.ast.ValExprUnaryOp):
        attrs.update(_get_attributes_in_predicate(cond.inputs[0]))
    return attrs


def _get_attributes_in_relation(rel, dd):
    if isinstance(rel, radb.ast.RelRef):
        attrs = dd.get(rel.rel, {}).keys()
        return set((rel.rel, attr) for attr in attrs)
    elif isinstance(rel, radb.ast.Rename):
        attrs = _get_attributes_in_relation(rel.inputs[0], dd)
        return set((rel.relname, attr[1]) for attr in attrs)
    elif isinstance(rel, radb.ast.Select):
        return _get_attributes_in_relation(rel.inputs[0], dd)
    elif isinstance(rel, radb.ast.Cross):
        left_attrs = _get_attributes_in_relation(rel.inputs[0], dd)
        right_attrs = _get_attributes_in_relation(rel.inputs[1], dd)
        return left_attrs.union(right_attrs)
    elif isinstance(rel, radb.ast.Project):
        return _get_attributes_in_relation(rel.inputs[0], dd)
    else:
        return set()


def _predicate_applies_to_relation(pred_attrs, rel_attrs):
    for pred_rel, pred_name in pred_attrs:
        if pred_rel is not None:
            if (pred_rel, pred_name) not in rel_attrs:
                return False
        else:
            if not any(name == pred_name for (_, name) in rel_attrs):
                return False
    return True


def _is_join_condition(cond, left_rels, right_rels):
    pred_attrs = _get_attributes_in_predicate(cond)
    attr_rels = {rel for rel, _ in pred_attrs}

    if attr_rels & left_rels and attr_rels & right_rels:
        return True
    else:
        return False


def _get_relations_in_relation(rel):
    if isinstance(rel, radb.ast.RelRef):
        return {rel.rel}
    elif isinstance(rel, radb.ast.Rename):
        return {rel.relname}
    elif hasattr(rel, "inputs"):
        rels = set()
        for inp in rel.inputs:
            rels.update(_get_relations_in_relation(inp))
        return rels
    else:
        return set()
