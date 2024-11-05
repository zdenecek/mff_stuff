// 2023/2024
import java.lang.annotation.*
import java.lang.reflect.Modifier

import org.codehaus.groovy.ast.*
import org.codehaus.groovy.ast.expr.*
import org.codehaus.groovy.ast.stmt.*
import org.codehaus.groovy.control.*
import org.codehaus.groovy.syntax.*
import org.codehaus.groovy.transform.*

import org.codehaus.groovy.ast.builder.AstBuilder
import org.codehaus.groovy.ast.tools.GeneralUtils

import static org.codehaus.groovy.ast.tools.GeneralUtils.*
import static org.codehaus.groovy.control.CompilePhase.*

@Retention(RetentionPolicy.SOURCE)
@Target([ElementType.TYPE])
@GroovyASTTransformationClass("CreatedAtTransformation")
public @interface CreatedAt {
    String name() default "";
}

@GroovyASTTransformation(phase = SEMANTIC_ANALYSIS)
public class CreatedAtTransformation implements ASTTransformation {

    public void visit(ASTNode[] nodes, SourceUnit source) {
        if (nodes == null || nodes.length < 2) return;

        AnnotationNode annotationNode = (AnnotationNode) nodes[0];
        AnnotatedNode annotatedNode = (AnnotatedNode) nodes[1];

        if (!(annotatedNode instanceof ClassNode)) return;

        ClassNode classNode = (ClassNode) annotatedNode;

        // Get the 'name' parameter
        Expression nameExpr = annotationNode.getMember("name");
        String methodName = "timestamp"; // default name

        if (nameExpr != null && nameExpr instanceof ConstantExpression) {
            String value = ((ConstantExpression) nameExpr).getValue().toString();
            if (value != null && !value.isEmpty()) {
                methodName = value;
            }
        }

        // Add private long field with initial value
        String TIMESTAMP_FIELD_NAME = "__timestamp_field__";
        FieldNode timestampField = new FieldNode(
            TIMESTAMP_FIELD_NAME,
            Modifier.PRIVATE,
            ClassHelper.long_TYPE,
            classNode,
            new MethodCallExpression(
                new ClassExpression(ClassHelper.make(System)),
                "currentTimeMillis",
                ArgumentListExpression.EMPTY_ARGUMENTS
            )
        );

        classNode.addField(timestampField);

        // Add public final method returning the value stored in the field
        MethodNode timestampMethod = new MethodNode(
            methodName,
            Modifier.PUBLIC | Modifier.FINAL,
            ClassHelper.long_TYPE,
            Parameter.EMPTY_ARRAY,
            ClassNode.EMPTY_ARRAY,
            new ReturnStatement(
                new FieldExpression(timestampField)
            )
        );

        classNode.addMethod(timestampMethod);

        // Add clearTimestamp() method
        MethodNode clearTimestampMethod = new MethodNode(
            "clearTimestamp",
            Modifier.PUBLIC,
            ClassHelper.VOID_TYPE,
            Parameter.EMPTY_ARRAY,
            ClassNode.EMPTY_ARRAY,
            new ExpressionStatement(
                new BinaryExpression(
                    new FieldExpression(timestampField),
                    Token.newSymbol("=", -1, -1),
                    new ConstantExpression(0L)
                )
            )
        );

        classNode.addMethod(clearTimestampMethod);

        // Modify existing methods
        List<MethodNode> methods = new ArrayList<>(classNode.getMethods());

        for (MethodNode method : methods) {
            if (method.isSynthetic()) continue;
            if (method.isStatic()) continue;
            if (method.isConstructor()) continue;
            if (method.getName().equals(timestampMethod.getName())) continue;
            if (method.getName().equals("clearTimestamp")) continue;

            if (method.getCode() == null) continue;

            BlockStatement methodBody;

            if (method.getCode() instanceof BlockStatement) {
                methodBody = (BlockStatement) method.getCode();
            } else {
                methodBody = new BlockStatement();
                methodBody.addStatement(method.getCode());
                method.setCode(methodBody);
            }

            // Build the if statement
            // if (System.currentTimeMillis() - timestampField > 1000L) {
            //     timestampField = System.currentTimeMillis();
            // }

            MethodCallExpression currentTimeMillisCall1 = new MethodCallExpression(
                new ClassExpression(ClassHelper.make(System)),
                "currentTimeMillis",
                ArgumentListExpression.EMPTY_ARGUMENTS
            );

            MethodCallExpression currentTimeMillisCall2 = new MethodCallExpression(
                new ClassExpression(ClassHelper.make(System)),
                "currentTimeMillis",
                ArgumentListExpression.EMPTY_ARGUMENTS
            );

            BinaryExpression timeDifference = new BinaryExpression(
                currentTimeMillisCall1,
                Token.newSymbol("-", -1, -1),
                new FieldExpression(timestampField)
            );

            BooleanExpression condition = new BooleanExpression(
                new BinaryExpression(
                    timeDifference,
                    Token.newSymbol(">", -1, -1),
                    new ConstantExpression(1000L)
                )
            );

            ExpressionStatement thenStatement = new ExpressionStatement(
                new BinaryExpression(
                    new FieldExpression(timestampField),
                    Token.newSymbol("=", -1, -1),
                    currentTimeMillisCall2
                )
            );

            IfStatement ifStatement = new IfStatement(
                condition,
                thenStatement,
                EmptyStatement.INSTANCE
            );

            // Insert at beginning of method body
            methodBody.getStatements().add(0, ifStatement);
        }
    }
}

final calculator = new GroovyShell(this.class.getClassLoader()).evaluate('''
@CreatedAt(name = "timestamp")
class Calculator {
    int sum = 0
    
    def add(int value) {
        int v = sum + value
        sum = v
    }

    def subtract(int value) {
        sum -= value
    }
}

new Calculator()
''')

assert System.currentTimeMillis() >= calculator.timestamp()
assert calculator.timestamp() == calculator.timestamp()
def oldTimeStamp = calculator.timestamp()

sleep(1500)
calculator.add(10)
assert calculator.sum == 10

assert oldTimeStamp < calculator.timestamp()
//The timestamp should have been updated since the pause was longer than 1s
assert calculator.timestamp() == calculator.timestamp()
oldTimeStamp = calculator.timestamp()

sleep(1500)
calculator.subtract(1)
assert calculator.sum == 9
//The timestamp should have been updated since the pause was longer than 1s
assert oldTimeStamp < calculator.timestamp()
assert calculator.timestamp() == calculator.timestamp()

oldTimeStamp = calculator.timestamp()
sleep(100)
calculator.subtract(1)
assert calculator.sum == 8
//The timestamp should not have been updated since the pause was shorter than 1s
assert oldTimeStamp == calculator.timestamp()
assert calculator.timestamp() == calculator.timestamp()

calculator.clearTimestamp()
assert calculator.timestamp() == 0

println 'well done'