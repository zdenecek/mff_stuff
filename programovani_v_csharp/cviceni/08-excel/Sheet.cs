using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace task08_excel2;

public record struct CellAddress
{
    public int Sheet;
    public int Row;
    public int Col;
}


public class Sheet 
{

    public Sheet context;
    protected Stack<FormulaCell> valuationStack;
    public Sheet()
    {
        context = this;
        valuationStack = new();
    }

    public List<List<Cell>> Rows = new();

    public virtual  Cell this[CellAddress key]
    {
        get
        {
            if (Rows.Count < key.Row || Rows[key.Row -1].Count < key.Col) return Cell.Empty;

            var cell = Rows[key.Row - 1][key.Col - 1];

            if (cell is not FormulaCell formulaCell) return cell;

            if (!formulaCell.Evaluated) context.Evaluate(formulaCell);

            return formulaCell;
        }
        set
        {
            throw new InvalidOperationException("Invalid operation");
        }
    }

    public IEnumerable<IEnumerable<Cell>> GetRows()
    {
        return Rows;
    }

    public void Evaluate()
    {
        foreach (var row in Rows)
        {
            foreach (var cell in row)
            {
                if (cell is not FormulaCell formulaCell) continue;
                if (formulaCell.Evaluated) continue;


                context.Evaluate(formulaCell);
            }
        }
    }


    public void Evaluate(FormulaCell cell)
    {
        if (!cell.Evaluating)
        {
            cell.Evaluating = true;
            valuationStack.Push(cell);


            cell.Evaluate(context);

            if (cell.Evaluated)
            {
                return;
            }
            cell.Evaluated = true;
            cell.Evaluating = false;
            valuationStack.Pop();
        }
        else while (!cell.Evaluated)
            {
                cell.Error = CellError.CYCLE;
                cell.Evaluated = true;
                cell.Evaluating = false;
                cell = valuationStack.Pop();
            }
    }
}
