import sqlparse
import radb
import radb.ast
import sqlparse.sql
import sqlparse.tokens
from radb.parse import RAParser as sym

def translate(stmt):
    select_attrs = extract_select_attributes(stmt)
    from_relations = extract_from_relations(stmt)
    where_clause = extract_where_conditions(stmt)

    ra_from_expr = build_from_expression(from_relations)

    if where_clause:
        ra_where_expr = build_where_expression(where_clause, ra_from_expr)
    else:
        ra_where_expr = ra_from_expr

    ra_select_expr = build_select_expression(select_attrs, ra_where_expr)

    return ra_select_expr

def extract_select_attributes(stmt):
    select_attrs = []
    select_seen = False
    for token in stmt.tokens:
        if token.ttype is sqlparse.tokens.DML and token.value.upper() == 'SELECT':
            select_seen = True
            continue
        if select_seen:
            if token.ttype is sqlparse.tokens.Keyword and token.value.upper() == 'DISTINCT':
                continue 
            elif isinstance(token, sqlparse.sql.IdentifierList):
                for identifier in token.get_identifiers():
                    select_attrs.append(identifier)
                break
            elif isinstance(token, sqlparse.sql.Identifier):
                select_attrs.append(token)
                break
            elif token.ttype is sqlparse.tokens.Wildcard:
                select_attrs.append(token)
                break
            elif token.ttype is sqlparse.tokens.Whitespace:
                continue
            else:
                break
    return select_attrs

def extract_from_relations(stmt):
    from_relations = []
    from_seen = False
    for token in stmt.tokens:
        if token.ttype is sqlparse.tokens.Keyword and token.value.upper() == 'FROM':
            from_seen = True
            continue
        if from_seen:
            if isinstance(token, sqlparse.sql.IdentifierList):
                for identifier in token.get_identifiers():
                    from_relations.append(identifier)
                break
            elif isinstance(token, sqlparse.sql.Identifier):
                from_relations.append(token)
                break
            elif token.ttype is sqlparse.tokens.Whitespace:
                continue
            else:
                break
    return from_relations

def extract_where_conditions(stmt):
    for token in stmt.tokens:
        if isinstance(token, sqlparse.sql.Where):
            return token
    return None

def build_from_expression(from_relations):
    ra_expr = None
    for identifier in from_relations:
        relation = build_relation(identifier)
        if ra_expr is None:
            ra_expr = relation
        else:
            ra_expr = radb.ast.Cross(ra_expr, relation)
    return ra_expr

def build_relation(identifier):
    relation_name = identifier.get_real_name()
    alias = identifier.get_alias()
    relation = radb.ast.RelRef(relation_name)
    if alias:
        relation = radb.ast.Rename(relname=alias, attrnames=None, input=relation)
    return relation

def build_where_expression(where_clause, ra_expr):
    tokens = where_clause.tokens
    condition_tokens = []
    for token in tokens[1:]:
        if not token.is_whitespace:
            condition_tokens.append(token)
    condition_expr = parse_conditions(condition_tokens)
    ra_expr = radb.ast.Select(condition_expr, ra_expr)
    return ra_expr

def parse_conditions(condition_tokens):
    conditions = []
    current_condition = []
    for token in condition_tokens:
        if token.ttype is sqlparse.tokens.Keyword and token.value.upper() in ('AND', 'OR'):
            if current_condition:
                conditions.append(build_condition(current_condition))
                current_condition = []
            conditions.append(token)
        else:
            current_condition.append(token)
    if current_condition:
        conditions.append(build_condition(current_condition))

    while len(conditions) > 1:
        left = conditions.pop(0)
        op_token = conditions.pop(0)
        right = conditions.pop(0)
        operator = op_token.value.upper()
        if operator == 'AND':
            combined = radb.ast.ValExprBinaryOp(left, sym.AND, right)
        elif operator == 'OR':
            combined = radb.ast.ValExprBinaryOp(left, sym.OR, right)
        else:
            raise NotImplementedError(f"Logical operator '{operator}' not supported.")
        conditions.insert(0, combined)
    return conditions[0]

def build_condition(condition_tokens):
    tokens = [token for token in condition_tokens if not token.is_whitespace]
    if len(tokens) == 1 and isinstance(tokens[0], sqlparse.sql.Comparison):
        comparison = tokens[0]
        comparison_tokens = [t for t in comparison.tokens if not t.is_whitespace]
        if len(comparison_tokens) == 3:
            left_token, operator_token, right_token = comparison_tokens
            operator = operator_token.value.strip()
            left_expr = build_value_expression(left_token)
            right_expr = build_value_expression(right_token)
            if operator == '=':
                return radb.ast.ValExprBinaryOp(left_expr, sym.EQ,  right_expr)
            else:
                raise NotImplementedError(f"Operator '{operator}' not supported.")
        else:
            raise ValueError(f"Invalid comparison: {comparison}")
    elif len(tokens) == 3:
        left_token, operator_token, right_token = tokens
        operator = operator_token.value.strip()
        left_expr = build_value_expression(left_token)
        right_expr = build_value_expression(right_token)
        if operator == '=':
            return radb.ast.ConditionEQ(left_expr, right_expr)
        else:
            raise NotImplementedError(f"Operator '{operator}' not supported.")
    else:
        raise ValueError(f"Invalid condition: {condition_tokens}")

def build_value_expression(token):
    if isinstance(token, sqlparse.sql.Identifier):
        names = token.value.split('.')
        if len(names) == 2:
            return radb.ast.AttrRef(names[0], names[1])
        else:
            return radb.ast.AttrRef(None, names[0])
    elif token.ttype in (sqlparse.tokens.Literal.String.Single, sqlparse.tokens.Literal.String.Symbol):
        # value = token.value.strip("'\"")
        value = token.value
        return radb.ast.RAString(value)
    elif token.ttype in sqlparse.tokens.Number:
        value = token.value
        return radb.ast.RANumber(value)
    else:
        raise ValueError("Invalid")

def build_select_expression(select_attrs, ra_expr):
    if len(select_attrs) == 1 and select_attrs[0].ttype == sqlparse.tokens.Wildcard:
        return ra_expr
    else:
        proj_attrs = []
        for attr in select_attrs:
            if isinstance(attr, sqlparse.sql.Identifier):
                names = attr.value.split('.')
                if len(names) == 2:
                    proj_attrs.append(radb.ast.AttrRef(names[0], names[1]))
                else:
                    proj_attrs.append(radb.ast.AttrRef(None, names[0]))
        ra_expr = radb.ast.Project(proj_attrs, ra_expr)
        return ra_expr
