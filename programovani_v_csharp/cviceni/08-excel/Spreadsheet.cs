using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace task08_excel2;



public class Spreadsheet : Sheet
{

    public Dictionary<int, SpreadsheetSheet> Sheets { get; } = new();

    public override Cell this[CellAddress key]
    {
        get
        {
            if (!Sheets.ContainsKey(key.Sheet)) return Cell.ErrorCell;

            var cell = Sheets[key.Sheet][key];

            if (cell is not FormulaCell formulaCell) return cell;

            if (!formulaCell.Evaluated) Evaluate(formulaCell);

            return formulaCell;
        }
        set
        {
            throw new InvalidOperationException();
        }
    }
    public SpreadsheetSheet PrimarySheet { get; set; }
    public Dictionary<string, int> RefByName { get; } = new();

    public void AddSheet(int id, string name, SpreadsheetSheet sheet)  {

        if (Sheets.ContainsKey(id)) throw new InvalidOperationException("Sheet already present");

        Sheets[id] = sheet;
        RefByName[name] = id;
    }


}
