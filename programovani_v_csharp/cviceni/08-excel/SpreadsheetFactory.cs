using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.ConstrainedExecution;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
using task3a;

namespace task08_excel2;

public class SpreadsheetFactory : SheetFactory
{
	public SpreadsheetFactory()
	{
        firstAddressGroup = 1;
        secondAddressGroup = 4;
        operatorGroup = 3;
        operatorPattern = @"([*/+-])";
        addressPattern = @"((\w+!)?[A-Z]+[0-9]+)";
        formulaCellPattern = "^" + addressPattern + operatorPattern + addressPattern + "$";
    }
    

    private Spreadsheet currentSpreadsheet;
    protected int currentSheetRef;
    Dictionary<string, int> sheetRefs;
    Stack<(string, string)> pathsToParse;

    private Func<string, string> nameToPath;

    public Spreadsheet MakeSpreadsheet(string path)
    {
        currentSpreadsheet = new();
        sheetRefs = new();
        pathsToParse = new();


        var dir = Path.GetDirectoryName(path);
        nameToPath = (n) => Path.Combine(dir, Path.ChangeExtension(Path.GetFileName(n),  ".sheet"));

        string name = Path.GetFileNameWithoutExtension(path);
        int primary = 0;
        sheetRefs[name] = 0;
        pathsToParse.Push((name, path));
        
        while(pathsToParse.Count > 0)
        {

            (var nm, var p) = pathsToParse.Pop();

            try
            {
                currentSpreadsheet.AddSheet(sheetRefs[nm], nm, MakeSpreadheetSheet(nm, p));

            } catch (Exception e) when (e is AccessViolationException or FileNotFoundException or UnauthorizedAccessException or FormatException or FileNotFoundException)
            {
                continue;
            }
        }

        if(currentSpreadsheet.Sheets.ContainsKey(primary)) currentSpreadsheet.PrimarySheet = currentSpreadsheet.Sheets[primary];
        else throw new FileNotFoundException();

        return currentSpreadsheet;
    }

    private SpreadsheetSheet MakeSpreadheetSheet(string name, string path)
    {
        using(StreamReader sr = new(path))
        {
            var wordReader = new WordReader(sr);
            var sh = new SpreadsheetSheet(currentSpreadsheet);

            currentSheetRef = getSheetRef(name);

            FillSheet(sh, wordReader);

            return sh;
        }
    }



    private int getSheetRef(string name)
    {
        if(!sheetRefs.ContainsKey(name))
        {
        sheetRefs[name] = sheetRefs.Count;
        pathsToParse.Push((name, nameToPath(name)));

        }
        return sheetRefs[name];
    }

    protected override CellAddress parseReference(string reference)
    {
        var match = Regex.Match(reference, @"(\w+!)?([A-Z]+[0-9]+)");

        bool sheet = match.Groups[1].Success;

        var address = base.parseReference(match.Groups[2].Value);

        if (sheet)
        {
            int sheetRef = getSheetRef(match.Groups[1].Value.Substring(0, match.Groups[1].Value.Length - 1));
            address.Sheet = sheetRef;
        }
        else
            address.Sheet = currentSheetRef;

        return address;
    }
}
