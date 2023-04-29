using System;
using System.Collections.Generic;
using System.Linq;

namespace ExpressionCalculator
{
    // Abstract base class for expressions
    public abstract class Expression
    {
        public abstract void Accept(IExpressionVisitor visitor);
    }

    // Class for representing integer constants
    public class IntegerConstant : Expression
    {
        public int Value { get; set; }

        public IntegerConstant(int value)
        {
            Value = value;
        }

        public override void Accept(IExpressionVisitor visitor)
        {
            visitor.Visit(this);
        }
    }

    // Class for representing floating-point constants
    public class DoubleConstant : Expression
    {
        public double Value { get; set; }

        public DoubleConstant(double value)
        {
            Value = value;
        }

        public override void Accept(IExpressionVisitor visitor)
        {
            visitor.Visit(this);
        }
    }

    // Class for representing binary operators
    public class BinaryOperator : Expression
    {
        public Expression Left { get; set; }
        public Expression Right { get; set; }
        public char Operator { get; set; }

        public BinaryOperator(Expression left, Expression right, char op)
        {
            Left = left;
            Right = right;
            Operator = op;
        }

        public override void Accept(IExpressionVisitor visitor)
        {
            visitor.Visit(this);
        }
    }

    // Class for representing unary operators
    public class UnaryOperator : Expression
    {
        public Expression Operand { get; set; }
        public char Operator { get; set; }

        public UnaryOperator(Expression operand, char op)
        {
            Operand = operand;
            Operator = op;
        }

        public override void Accept(IExpressionVisitor visitor)
        {
            visitor.Visit(this);
        }
    }

    // Interface for expression visitors
    public interface IExpressionVisitor
    {
        void Visit(IntegerConstant constant);
        void Visit(DoubleConstant constant);
        void Visit(BinaryOperator binaryOperator);
        void Visit(UnaryOperator unaryOperator);
    }

    // Class for evaluating expressions using integer arithmetic
    public class IntegerEvaluator : IExpressionVisitor
    {
        public int Result { get; set; }

        public void Visit(IntegerConstant constant)
        {
            Result = constant.Value;
        }

        public void Visit(DoubleConstant constant)
        {
            Result = (int)constant.Value;
        }

        public void Visit(BinaryOperator binaryOperator)
        {
            binaryOperator.Left.Accept(this);
            int left = Result;
            binaryOperator.Right.Accept(this);
            int right = Result;

            checked
            {

                switch (binaryOperator.Operator)
                {
                    case '+':
                        Result = left + right;
                        break;
                    case '-':
                        Result = left - right;
                        break;
                    case '*':
                        Result = left * right;
                        break;
                    case '/':
                        if (right == 0)
                        {
                            throw new DivideByZeroException();
                        }
                        Result = left / right;
                        break;
                }
            }
        }

        public void Visit(UnaryOperator unaryOperator)
        {
            unaryOperator.Operand.Accept(this);
            if (unaryOperator.Operator == '~')
            {
                Result = -Result;
            }
        }
    }

    // Class for evaluating expressions using floating-point arithmetic
    public class DoubleEvaluator : IExpressionVisitor
    {
        public double Result { get; set; }

        public void Visit(IntegerConstant constant)
        {
            Result = constant.Value;
        }

        public void Visit(DoubleConstant constant)
        {
            Result = constant.Value;
        }

        public void Visit(BinaryOperator binaryOperator)
        {
            binaryOperator.Left.Accept(this);
            double left = Result;
            binaryOperator.Right.Accept(this);
            double right = Result;

            checked
            {
                switch (binaryOperator.Operator)
                {
                    case '+':
                        Result = left + right;
                        break;
                    case '-':
                        Result = left - right;
                        break;
                    case '*':
                        Result = left * right;
                        break;
                    case '/':
                        Result = left / right;
                        break;
                }
            }
        }

        public void Visit(UnaryOperator unaryOperator)
        {
            unaryOperator.Operand.Accept(this);
            if (unaryOperator.Operator == '~')
            {
                Result = -Result;
            }
        }
    }

    // Class for parsing expressions
    public class ExpressionParser
    {
        private static readonly char[] Operators = { '+', '-', '*', '/', '~' };
        private static readonly char[] Digits = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };

        public static Expression Parse(string input)
        {
            // Split the input into tokens
            var tokens = Tokenize(input);

            // Parse the tokens
            return ParseTokens(tokens);
        }

        private static Expression ParseTokens(Queue<string> tokens)
        {
            string token;
            try
            {
            // Peek at the next token
            token = tokens.Peek();

            } catch (InvalidOperationException)
            {
                throw new FormatException();
            }


            // If the token is an operator, parse it as a unary or binary operator
            if (Operators.Contains(token[0]))
            {
                tokens.Dequeue();
                if (token[0] == '~')
                {
                    // Unary operator
                    return new UnaryOperator(ParseTokens(tokens), token[0]);
                }
                else
                {
                    // Binary operator
                    return new BinaryOperator(ParseTokens(tokens), ParseTokens(tokens), token[0]);
                }
            }
            // If the token is a digit,
            // If the token is a digit, parse it as a constant
            else if (Digits.Contains(token[0]) || (token[0] == '-' && token.Length > 1 && Digits.Contains(token[1])))
            {
                tokens.Dequeue();
                bool isDouble = token.Contains('.');
                if (isDouble)
                {
                    return new DoubleConstant(double.Parse(token));
                }
                else
                {
                    return new IntegerConstant(int.Parse(token));
                }
            }
            // If the token is invalid, throw an exception
            else
            {
                throw new FormatException();
            }
        }

        private static Queue<string> Tokenize(string input)
        {
            // Initialize the queue of tokens
            var tokens = new Queue<string>();

            // Initialize the current token
            string currentToken = "";

            // Iterate through the input string
            foreach (char c in input)
            {
                // If the character is an operator or a space, add the current token to the queue and reset it
                if (Operators.Contains(c) || c == ' ')
                {
                    if (currentToken != "")
                    {
                        tokens.Enqueue(currentToken);
                        currentToken = "";
                    }
                    if (Operators.Contains(c))
                    {
                        tokens.Enqueue(c.ToString());
                    }
                }
                // Otherwise, append the character to the current token
                else
                {
                    currentToken += c;
                }
            }

            // Add the final token to the queue
            if (currentToken != "")
            {
                tokens.Enqueue(currentToken);
            }

            return tokens;
        }
    }

    // Main program class
    public class Program
    {
        public static void Main(string[] args)
        {
            // Initialize the expression
            Expression expression = null;

            // Read input lines
            string line;
            while ((line = Console.ReadLine()) != null)
            {
                // Trim leading and trailing white space
                line = line.Trim();

                // If the line is empty, skip it
                if (line == "")
                {
                    continue;
                }

                // If the line is "end", exit the loop
                if (line == "end")
                {
                    break;
                }

                // If the line starts with "=", parse the expression
                if (line.StartsWith("="))
                {
                    try
                    {
                        expression = ExpressionParser.Parse(line.Substring(2));
                    }
                    catch (Exception e) when (e is FormatException or ArgumentOutOfRangeException or OverflowException)
                    {
                        expression = null;
                        Console.WriteLine("Format Error");
                    }

                }
                // If the line is "i", evaluate the expression using integer arithmetic
                else if (line == "i")
                {
                    if (expression == null)
                    {
                        Console.WriteLine("Expression Missing");
                    }
                    else
                    {
                        try
                        {
                            var evaluator = new IntegerEvaluator();
                            expression.Accept(evaluator);
                            Console.WriteLine(evaluator.Result);
                        }
                        catch (DivideByZeroException)
                        {
                            Console.WriteLine("Divide Error");
                        }
                        catch (OverflowException)
                        {
                            Console.WriteLine("Overflow Error");
                        }
                    }
                }
                // If the line is "d", evaluate the expression using floating-point arithmetic
                else if (line == "d")
                {
                    if (expression == null)
                    {
                        Console.WriteLine("Expression Missing");
                    }
                    else
                    {
                        try
                        {
                            var evaluator = new DoubleEvaluator();
                            expression.Accept(evaluator);

                            if(Double.IsInfinity(evaluator.Result) ) 
                                Console.WriteLine((evaluator.Result < 0 ? "-" : "") + "Infinity");
                            else
                            Console.WriteLine(evaluator.Result.ToString("F5"));
                        }

                        catch (DivideByZeroException)
                        {
                            Console.WriteLine("Divide Error");
                        }
                        catch (OverflowException)
                        {
                            Console.WriteLine("Overflow Error");
                        }
                    }
                }
                // If the line is invalid, print "Format Error"
                else
                {
                    Console.WriteLine("Format Error");
                }
            }
        }
    }
}
