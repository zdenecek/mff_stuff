import radb
import radb.ast
from radb.parse import RAParser as sym

def remove_duplicate_entries(entries):
    """Remove duplicate entries from a list."""
    seen = set()
    return [entry for entry in entries if entry not in seen and not seen.add(entry)]

def decompose_expression(expr):
    """Break down an RA expression into its components."""
    if expr is None:
        return []
    result = [expr]
    if isinstance(expr, radb.ast.Select):
        result += decompose_expression(expr.cond)
    for input_expr in expr.inputs:
        result += decompose_expression(input_expr)
    return result

def construct_cross_product(dependency_dict, join_conditions, relations):
    """Construct a cross product of relations, incorporating conditions if possible."""
    current_relation = relations[0]
    for rel in relations[1:]:
        if isinstance(current_relation, radb.ast.Cross) and join_conditions:
            join_conditions, current_relation = apply_conditions_to_cross(dependency_dict, join_conditions, current_relation)
        current_relation = radb.ast.Cross(current_relation, rel)
    return current_relation

def apply_conditions_to_cross(dependency_dict, join_conditions, cross_relation):
    """Apply join conditions to a cross product."""
    for key in dependency_dict:
        for condition in join_conditions:
            if any(isinstance(attr, radb.ast.AttrRef) and str(attr.rel) == key for attr in condition.inputs):
                join_conditions.remove(condition)
                return join_conditions, combine_conditions([condition], cross_relation)
    return join_conditions, cross_relation

def combine_conditions(conditions, base_relation):
    """Combine conditions into a nested Select."""
    for condition in reversed(conditions):
        base_relation = radb.ast.Select(condition, base_relation)
    return base_relation

def rule_push_down_selections(ra, dependency_dict):
    """Push down selections in a relational algebra expression."""
    components = remove_duplicate_entries(decompose_expression(ra))

    binary_conditions = [item for item in components if isinstance(item, radb.ast.ValExprBinaryOp)]
    projection = [item for item in components if isinstance(item, radb.ast.Project)]
    relations = [item for item in components if isinstance(item, (radb.ast.RelRef, radb.ast.Rename))]
    join_conditions = []

    if len(relations) < 2:
        return ra

    for condition in binary_conditions[:]:
        if all(isinstance(input_item, radb.ast.AttrRef) for input_item in condition.inputs):
            join_conditions.append(condition)
            binary_conditions.remove(condition)

    if not binary_conditions and len(join_conditions) < 2:
        return ra

    for rel_name, attributes in dependency_dict.items():
        for attribute in attributes:
            for condition in binary_conditions[:]:
                if any(attribute in str(input_item) for input_item in condition.inputs) and any(rel_name == str(rel) for rel in relations):
                    relevant_relation = next(
                        (rename for rename in relations if isinstance(rename, radb.ast.Rename) and rename.inputs[0].rel == rel_name),
                        radb.ast.RelRef(rel_name)
                    )
                    select_relation = radb.ast.Select(condition, relevant_relation)
                    binary_conditions.remove(condition)

                    for idx, rel in enumerate(relations):
                        if ((isinstance(rel, radb.ast.RelRef) and str(rel.rel) == rel_name) or
                            (isinstance(rel, radb.ast.Rename) and rel.inputs[0].rel == rel_name)):
                            if isinstance(rel, radb.ast.Rename) and isinstance(select_relation.inputs[0], radb.ast.Rename) and rel.relname != select_relation.inputs[0].relname:
                                continue
                            relations[idx] = select_relation

    relations = remove_duplicate_entries(relations)
    joined_relations = construct_cross_product(dependency_dict, join_conditions, relations)

    if join_conditions:
        final_select = combine_conditions(join_conditions, joined_relations)
        if projection:
            projection[0].inputs[0] = final_select
            return projection[0]
        return final_select

    return joined_relations


def rule_break_up_selections(ra_expression):
    if isinstance(ra_expression, radb.ast.Select):
        predicates = split_conjunctive_predicate(ra_expression.cond)
        if len(predicates) > 1:
            nested_select = ra_expression.inputs[0]
            for predicate in reversed(predicates):
                nested_select = radb.ast.Select(predicate, nested_select)
            return rule_break_up_selections(nested_select)
        else:
            ra_expression.inputs[0] = rule_break_up_selections(ra_expression.inputs[0])
    elif hasattr(ra_expression, 'inputs') and ra_expression.inputs:
        ra_expression.inputs = [rule_break_up_selections(inp) for inp in ra_expression.inputs]
    return ra_expression



def split_conjunctive_predicate(predicate):
    if isinstance(predicate, radb.ast.ValExprBinaryOp) and predicate.op == sym.AND:
        left_preds = split_conjunctive_predicate(predicate.inputs[0])
        right_preds = split_conjunctive_predicate(predicate.inputs[1])
        return left_preds + right_preds
    else:
        return [predicate]
    


def get_relations(condition):
    attrs = extract_predicate_attribudes(condition)
    return set(rel for rel, _ in attrs if rel is not None)


def extract_predicate_attribudes(confition):
    attrs = set()
    if isinstance(confition, radb.ast.AttrRef):
        attrs.add((confition.rel, confition.name))
    elif isinstance(confition, radb.ast.ValExprBinaryOp):
        attrs.update(extract_predicate_attribudes(confition.inputs[0]))
        attrs.update(extract_predicate_attribudes(confition.inputs[1]))
    elif isinstance(confition, radb.ast.ValExprUnaryOp):
        attrs.update(extract_predicate_attribudes(confition.inputs[0]))
    return attrs


def get_children_attributes(node, schema):
    if isinstance(node, radb.ast.RelRef):
        attributes = schema.get(node.rel, {}).keys()
        return {(node.rel, attr) for attr in attributes}
    elif isinstance(node, radb.ast.Select) or isinstance(node, radb.ast.Project):
        return get_children_attributes(node.inputs[0], schema)
    elif isinstance(node, radb.ast.Rename):
        base_attributes = get_children_attributes(node.inputs[0], schema)
        return {(node.relname, attribute[1]) for attribute in base_attributes}
    elif isinstance(node, radb.ast.Cross):
        left_attributes = get_children_attributes(node.inputs[0], schema)
        right_attributes = get_children_attributes(node.inputs[1], schema)
        return left_attributes.union(right_attributes)
    else:
        return set()


def predicate_applies_to_relation(predicate_attrs, relation_attrs):
    return all(
        (relation, name) in relation_attrs if relation is not None
        else any(attr_name == name for _, attr_name in relation_attrs)
        for relation, name in predicate_attrs
    )


def predicate_only_on_relation(predicate, relation, data_dict):
    relation_aliases = get_all_relation_aliases(relation)
    alias_to_base = get_alias_to_base_mapping(relation)

    attributes = collect_attributes(predicate)

    predicate_relations = set()
    for attr in attributes:
        if attr.rel is None:
            possible_relations = []
            for alias in relation_aliases:
                base_name = alias_to_base[alias]
                if attr.name in data_dict[base_name]:
                    possible_relations.append(alias)
            if len(possible_relations) == 1:
                predicate_relations.add(possible_relations[0])
            else:
                return False
        else:
            if attr.rel not in relation_aliases:
                return False
            predicate_relations.add(attr.rel)

    return len(predicate_relations) == 1


def get_all_relation_aliases(relation):
    aliases = set()
    if isinstance(relation, radb.ast.Rename):
        aliases.add(relation.relname)
        aliases.update(get_all_relation_aliases(relation.inputs))
    elif isinstance(relation, radb.ast.RelRef):
        aliases.add(relation.rel)
    elif isinstance(relation, (radb.ast.Cross, radb.ast.Join)):
        aliases.update(get_all_relation_aliases(relation.inputs[0]))
        aliases.update(get_all_relation_aliases(relation.inputs[1]))
    elif hasattr(relation, 'inputs') and relation.inputs:
        for inp in relation.inputs:
            aliases.update(get_all_relation_aliases(inp))
    return aliases

def get_alias_to_base_mapping(relation, current_alias=None):
    mapping = {}
    if isinstance(relation, radb.ast.Rename):
        base_name = get_base_relation_name(relation.inputs)
        mapping[relation.relname] = base_name
        mapping.update(get_alias_to_base_mapping(relation.inputs, relation.relname))
    elif isinstance(relation, radb.ast.RelRef):
        alias = current_alias if current_alias else relation.rel
        mapping[alias] = relation.rel
    elif isinstance(relation, (radb.ast.Cross, radb.ast.Join)):
        mapping.update(get_alias_to_base_mapping(relation.inputs[0]))
        mapping.update(get_alias_to_base_mapping(relation.inputs[1]))
    elif hasattr(relation, 'inputs') and relation.inputs:
        for inp in relation.inputs:
            mapping.update(get_alias_to_base_mapping(inp))
    return mapping

def get_all_relation_aliases(relation):
    aliases = set()
    if isinstance(relation, radb.ast.Rename):
        aliases.add(relation.relname)
        aliases.update(get_all_relation_aliases(relation.inputs))
    elif isinstance(relation, radb.ast.RelRef):
        aliases.add(relation.rel)
    elif isinstance(relation, (radb.ast.Cross, radb.ast.Join)):
        aliases.update(get_all_relation_aliases(relation.inputs[0]))
        aliases.update(get_all_relation_aliases(relation.inputs[1]))
    elif hasattr(relation, 'inputs') and relation.inputs:
        for inp in relation.inputs:
            aliases.update(get_all_relation_aliases(inp))
    return aliases

def get_alias_or_name(expr):
    if isinstance(expr, radb.ast.Rename):
        return expr.relname
    elif isinstance(expr, radb.ast.RelRef):
        return expr.rel
    else:
        return None

def get_base_relation_name(expr):
    if isinstance(expr, radb.ast.Rename):
        return get_base_relation_name(expr.input)
    elif isinstance(expr, radb.ast.RelRef):
        return expr.rel
    else:
        return None


def collect_attributes(expr):
    attrs = []
    if isinstance(expr, radb.ast.AttrRef):
        attrs.append(expr)
    elif isinstance(expr, radb.ast.ValExprBinaryOp):
        attrs.extend(collect_attributes(expr.inputs[0]))
        attrs.extend(collect_attributes(expr.inputs[1]))
    elif isinstance(expr, radb.ast.ValExprUnaryOp):
        attrs.extend(collect_attributes(expr.input))
    elif isinstance(expr, radb.ast.Literal):
        pass  
    else:
        pass  
    return attrs

def get_relation_name(expr):
    if isinstance(expr, radb.ast.RelRef):
        return expr.rel
    elif isinstance(expr, radb.ast.Rename):
        return expr.relname
    elif hasattr(expr, 'inputs') and expr.inputs:
        return get_relation_name(expr.inputs[0])
    else:
        return None

def ensure_valexpr(expr):
    if isinstance(expr, radb.ast.ValExpr):
        return expr
    elif isinstance(expr, radb.ast.Literal):
        return radb.ast.ValExprConst(expr.val)
    else:
        raise TypeError(f"Cannot convert {type(expr)} to ValExpr")

def rule_merge_selections(ra_expression):
    if isinstance(ra_expression, radb.ast.Select):
        ra_expression.inputs[0] = rule_merge_selections(ra_expression.inputs[0])
        child = ra_expression.inputs[0]

        if isinstance(child, radb.ast.Select):
            cond1 = ensure_valexpr(ra_expression.cond)
            cond2 = ensure_valexpr(child.cond)

            merged_predicate = radb.ast.ValExprBinaryOp(cond1, sym.AND, cond2)
            return radb.ast.Select(merged_predicate, child.inputs[0])
        else:
            return ra_expression
    elif hasattr(ra_expression, 'inputs') and ra_expression.inputs:
        ra_expression.inputs = [rule_merge_selections(input_expr) for input_expr in ra_expression.inputs]
    return ra_expression

def rule_introduce_joins(ra_expression):
    if isinstance(ra_expression, radb.ast.Select):
        ra_expression.inputs[0] = rule_introduce_joins(ra_expression.inputs[0])
        child = ra_expression.inputs[0]

        if isinstance(child, radb.ast.Cross):
            left, right = child.inputs
            predicates = split_conjunctive_predicate(ra_expression.cond)
            join_conditions = []
            residual_conditions = []

            for predicate in predicates:
                if is_join_condition(predicate, left, right):
                    join_conditions.append(predicate)
                else:
                    residual_conditions.append(predicate)

            if join_conditions:
                join_cond = join_conditions[0]
                for cond in join_conditions[1:]:
                    join_cond = radb.ast.ValExprBinaryOp(join_cond,sym.AND,  cond)

                new_child = radb.ast.Join( left, join_cond, right)

                if residual_conditions:
                    residual_cond = residual_conditions[0]
                    for cond in residual_conditions[1:]:
                        residual_cond = radb.ast.ValExprBinaryOp(sym.AND, residual_cond, cond)
                    new_expr = radb.ast.Select(residual_cond, new_child)
                else:
                    new_expr = new_child
                return rule_introduce_joins(new_expr)
        return ra_expression
    elif hasattr(ra_expression, 'inputs') and ra_expression.inputs:
        ra_expression.inputs = [rule_introduce_joins(inp) for inp in ra_expression.inputs]
    return ra_expression


def is_join_condition(predicate, left_relation, right_relation):
    predicate_attrs = collect_attributes(predicate)
    left_aliases = get_all_relation_aliases(left_relation)
    right_aliases = get_all_relation_aliases(right_relation)
    left_pred_attrs = [attr for attr in predicate_attrs if attr.rel in left_aliases]
    right_pred_attrs = [attr for attr in predicate_attrs if attr.rel in right_aliases]
    return bool(left_pred_attrs and right_pred_attrs)

