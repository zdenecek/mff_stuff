using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reflection.Metadata;
using System.Reflection.PortableExecutable;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;

using task3a;

namespace task08_excel2;

public class SheetFactory
{

    protected string operatorPattern;
    protected string addressPattern;
    protected string formulaCellPattern;
    protected int firstAddressGroup;
    protected int secondAddressGroup;
    protected int operatorGroup;

    public SheetFactory()
    {
        CellCache = new();
        firstAddressGroup = 1;
        secondAddressGroup = 3;
        operatorGroup = 2;
        operatorPattern = @"([*/+-])";
        addressPattern = @"([A-Z]+[0-9]+)";
        formulaCellPattern = "^" + addressPattern + operatorPattern + addressPattern + "$";
    }


    public virtual Sheet MakeSheet(StreamReader sr)
    {
        var reader = new WordReader(sr);

        var sheet = new Sheet();

        FillSheet(sheet, reader);
        
        return sheet;
    }


    public void FillSheet(Sheet sheet, WordReader reader)
    {

        var r = new List<Cell>();


        foreach (var token in reader.Read())
        {


            if (token is WordToken cell)
            {
                r.Add(MakeCell(cell.Word));

            } else
            {
               sheet.Rows.Add(r);
               r.Capacity = r.Count;
               r = new();
            }
        }

        sheet.Rows.Capacity = sheet.Rows.Count;
    }

    private  Dictionary<int, NumberCell> CellCache ;

    private  NumberCell NumberCell(int n)
    {
        if(!CellCache.ContainsKey(n)) CellCache[n] = new NumberCell(n);
        return CellCache[n];
    }




    public  Cell MakeCell( string data)
    {
        if (data == "[]")
            return Cell.Empty;

        if (data.StartsWith("="))
            return MakeFormulaCell( data.Substring(1));

        int value;
        bool isNumber = int.TryParse(data, out value);
        return isNumber ? NumberCell(value) : Cell.InvvalErrorCell;
    }


    protected virtual CellAddress parseReference(string reference)
    {
        var m = Regex.Match(reference, @"([A-Z]+)([0-9]+)");

        int row = int.Parse(m.Groups[2].Value);

        string colStr = m.Groups[1].Value;
        int col = 0;
        foreach (char c in colStr)
        {
            col *= 26;
            col += (c - 'A' + 1);
        }

        return new CellAddress { Row = row, Col = col };
    }

    private Cell MakeFormulaCell(string data)
    {
        var match = Regex.Match(data, formulaCellPattern);

        if (!match.Success)
            return Regex.IsMatch(data, operatorPattern) ? Cell.FormulaErrorCell : Cell.MissopErrorCell;


        var reference1 = parseReference(match.Groups[firstAddressGroup].Value);
        var reference2 = parseReference(match.Groups[secondAddressGroup].Value);
        var op = match.Groups[operatorGroup].Value[0];
        switch (op)
        {
            case '+': return new AdditionCell(reference1, reference2);
            case '-': return new SubtractionCell(reference1, reference2);
            case '/': return new DivisionCell(reference1, reference2);
            case '*': return new MultiplicationCell(reference1, reference2);
            default: throw new ArgumentException("Invalid operator");
        }

        
    }
}
