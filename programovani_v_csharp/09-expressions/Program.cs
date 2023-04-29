


using System.Collections.Generic;
using System.IO;
using System;
using System.Linq;
using static System.Net.Mime.MediaTypeNames;

namespace task09_expr;


class MalformedExpressionException : Exception
{

}

public class Program

{

    public static void Main2(string[] args)
    {
        
        var line = Console.ReadLine();  

        try
        {
            
            var tree = createTree(line);
            Console.Write(tree);
            
        } catch (DivideByZeroException)
        {
            Console.WriteLine("Divide Error");
        } catch (MalformedExpressionException)
        {
            Console.WriteLine("Format Error");

        } catch (OverflowException)
        {
            Console.WriteLine("Overflow Error");
        }

    }
    public static int createTree(string e)
    {
        checked
        {
            if (e.Length == 0) throw new MalformedExpressionException();

            var stack = new Stack<int>();

            foreach (String token in e.Split().Reverse())
            {
                if (token.Length == 0) continue;
                if (token == "+")
                {
                    if (stack.Count < 2) throw new MalformedExpressionException();
                    var r = stack.Pop() + stack.Pop();
                    stack.Push(r);
                }
                else if (token == "-")
                {
                    if (stack.Count < 2) throw new MalformedExpressionException();
                    var x = stack.Pop();
                    var y = stack.Pop();

                    var r = x - y;
                    stack.Push(r);
                }
                else if (token == "*")
                {
                    if (stack.Count < 2) throw new MalformedExpressionException();
                    var r = stack.Pop() * stack.Pop();
                    stack.Push(r);
                }
                else if (token == "/")
                {
                    if (stack.Count < 2) throw new MalformedExpressionException();
                    var x = stack.Pop();
                    var y = stack.Pop();
                    if (y == 0) throw new DivideByZeroException();
                    var r = x / y;
                    stack.Push(r);

                }
                else if (token == "~")
                {
                    if (stack.Count < 1) throw new MalformedExpressionException();
                    var x = stack.Pop();
                    stack.Push(-x);
                }
                else
                {
                    try
                    {
                        var i = int.Parse(token);
                        if(i <= 0) throw new MalformedExpressionException();
                        stack.Push(i);
                    }
                    catch (Exception ex) when (ex is OverflowException or FormatException)
                    {
                        throw new MalformedExpressionException();
                    }
                }
            }
            if (stack.Count != 1)
            {
                throw new MalformedExpressionException();
            }
            return stack.Pop();
        }
    }
}
