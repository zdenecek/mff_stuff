using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace task08_excel2;

public class SheetSerializer
{

    public static void SerializeSheet(Sheet sheet, StreamWriter sw)
    {

        foreach (var row in sheet.GetRows())
        {
            foreach (var cell in row)
            {
                sw.Write($"{cell.String} ");
            }
            sw.WriteLine();
        }
    }
}
