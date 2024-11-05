// 2023/2024
// TASK The MarkupBuilder in Groovy can transform a hierarchy of method calls and nested closures into a valid XML document.
// Create a NumericExpressionBuilder builder that will read a user-specified hierarchy of simple math expressions and build a tree representation of it.
// The basic arithmetics operations as well as the power (aka '^') operation must be supported.
// It will feature a toString() method that will pretty-print the expression tree into a string with the same semantics, as verified by the assert on the last line.
// This means that parentheses must be placed where necessary with respect to the mathematical operator priorities.
// Change or add to the code in the script. Reuse the infrastructure code at the bottom of the script.
class NumericExpressionBuilder extends BuilderSupport {
    private Item root

    @Override
    protected void setParent(Object parent, Object child) {
        if (parent instanceof Item && child instanceof Item) {
            parent.operands << child
        }
    }

    @Override
    protected Object createNode(Object name) {
        return createNode(name, [:], null)
    }

    @Override
    protected Object createNode(Object name, Object value) {
        return createNode(name, [:], value)
    }

    @Override
    protected Object createNode(Object name, Map attributes) {
        return createNode(name, attributes, null)
    }

    @Override
    protected Object createNode(Object name, Map attributes, Object value) {
        Item item = new Item()
        item.operator = name.toString()
        if (attributes?.value != null) {
            item.value = attributes.value
        } else if (value != null) {
            item.value = value
        }
        if (root == null) {
            root = item
        }
        return item
    }

    @Override
    protected void nodeCompleted(Object parent, Object node) {
        // No additional actions required upon node completion
    }

    Item rootItem() {
        return root
    }
}

class Item {
    String operator
    Object value
    List<Item> operands = []

    int getPrecedence() {
        switch (operator) {
            case 'number':
            case 'variable':
                return 4
            case 'power':
                return 3
            case '*':
            case '/':
                return 2
            case '+':
            case '-':
                return 1
            default:
                return 0
        }
    }

    @Override
    public String toString() {
        if (operator == 'number' || operator == 'variable') {
            return value.toString()
        } else {
            // Operator symbols mapping
            def opSymbols = ['+': '+', '-': '-', '*': '*', '/': '/', 'power': '^']
            def symbol = opSymbols[operator]
            def operandStrings = []
            operands.each { operand ->
                boolean needsParens = operand.precedence < this.precedence
                def str = operand.toString()
                operandStrings << (needsParens ? "(${str})" : str)
            }
            return operandStrings.join(" ${symbol} ")
        }
    }
}


//------------------------- Do not modify beyond this point!

def build(builder, String specification) {
    def binding = new Binding()
    binding['builder'] = builder
    new GroovyShell(binding).evaluate(specification)
}

//Custom expression to display. It should be eventually pretty-printed as 10 + x * (2 - 3) / 8 ^ (9 - 5)
String description = '''
builder.'+' {
    number(value: 10)
    '*' {
        variable(value: 'x')
        '/' {
            '-' {
                number(value: 2)
                number(value: 3)
            }
            power {
                number(value: 8)
                '-' {
                    number(value: 9)
                    number(value: 5)
                }
            }
        }
    }
}
'''

//XML builder building an XML document
build(new groovy.xml.MarkupBuilder(), description)

//NumericExpressionBuilder building a hierarchy of Items to represent the expression
def expressionBuilder = new NumericExpressionBuilder()
build(expressionBuilder, description)
def expression = expressionBuilder.rootItem()
println (expression.toString())
assert '10 + x * (2 - 3) / 8 ^ (9 - 5)' == expression.toString()
