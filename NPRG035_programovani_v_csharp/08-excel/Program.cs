using System;
using System.IO;

namespace task08_excel2;

internal class Program
{

    public static void Main(string[] args)
    {

        try
        {
            var factory = new SpreadsheetFactory();
            var sheet = factory.MakeSpreadsheet(args[0]);
            factory = null;
            sheet.PrimarySheet.Evaluate();

            var s = sheet.PrimarySheet;
            sheet = null;
            s.context = null;

            using (StreamWriter sw = new(args[1]))
            {
                SheetSerializer.SerializeSheet(s, sw);
            }
        }
        catch (Exception ex) 
        {
            Console.WriteLine("File Error");
            return;
        }
    }
}