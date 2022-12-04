using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace task08_excel2;



public enum CellError
{
    ERROR,
    INVVAL,
    CYCLE,
    DIV0,
    FORMULA,
    MISSOP,
}

public abstract class Cell
{


    static Cell()
    {
    FormulaErrorCell = new ErrorCell(CellError.FORMULA);
    MissopErrorCell = new ErrorCell(CellError.MISSOP);
    InvvalErrorCell = new ErrorCell(CellError.INVVAL);
    ErrorCell = new ErrorCell(CellError.ERROR);
    Empty = new EmptyCell();
    }

    public static Cell FormulaErrorCell ;
    public static Cell MissopErrorCell ;
    public static Cell InvvalErrorCell ;
    public static Cell ErrorCell ;
    public static Cell Empty;
    public abstract int Value { get; }
    public abstract string String { get; }
    public virtual bool IsError { get => false; }
  
}

public class EmptyCell : Cell
{
    public override int Value => 0;

    public override string String => "[]";
}


public class NumberCell : Cell
{
    private int value;
    public override int Value => value;
    public override string String => $"{value}";

    public NumberCell(int value)
    {
        this.value = value;
    }
}

public class ErrorCell : Cell
{

    public override int Value => 0;
    public override bool IsError { get => Error != null; }
    public CellError? Error { get; set; } = null;

    public string ErrorString
    {
        get
        {
            switch (this.Error)
            {
                case CellError.ERROR: return "#ERROR";
                case CellError.INVVAL: return "#INVVAL";
                case CellError.CYCLE: return "#CYCLE";
                case CellError.DIV0: return "#DIV0";
                case CellError.FORMULA: return "#FORMULA";
                case CellError.MISSOP: return "#MISSOP";
                default:
                    return null;
            }

        }
    }
    public override string String => ErrorString;
    public ErrorCell(CellError cellError) => Error = cellError;
    public ErrorCell() { }


}

public abstract class FormulaCell : ErrorCell
{
    public bool Evaluating = false;
    public bool Evaluated = false;

    private int value;
    public override int Value
    {
        get
        {
            if (!Evaluated) throw new Exception("Not evaluated");
            return this.value;
        }
    }
    public override string String => IsError ? ErrorString : Value.ToString();

    public CellAddress Address1 { get; }
    public CellAddress Address2 { get; }


    public void Evaluate(Sheet ctx)
    {
        var cell1 = ctx[Address1];
        var cell2 = ctx[Address2];

        if (this.Evaluated) return;

        if (cell1.IsError || cell2.IsError)
        {
            this.Error = CellError.ERROR;
            return;
        }

        this.value = doEvaluation( cell1.Value,  cell2.Value);

    }

    protected abstract int doEvaluation(int a, int b);

    public FormulaCell(CellAddress address1, CellAddress address2)
    {
        Address1 = address1;
        Address2 = address2;
    }
}


public class AdditionCell : FormulaCell
{
    public AdditionCell(CellAddress address1, CellAddress address2) : base(address1, address2) { }
    protected override int doEvaluation(int a, int b) => a + b;
}

public class SubtractionCell : FormulaCell
{
    public SubtractionCell(CellAddress address1, CellAddress address2) : base(address1, address2) { }
    protected override int doEvaluation(int a, int b) => a - b;
}

public class MultiplicationCell : FormulaCell
{
    public MultiplicationCell(CellAddress address1, CellAddress address2) : base(address1, address2) { }


    protected override int doEvaluation(int a, int b) => a * b;
}

public class DivisionCell : FormulaCell
{
    public DivisionCell(CellAddress address1, CellAddress address2) : base(address1, address2) { }


    protected override int doEvaluation(int a, int b)
    {
        if (b != 0) return a / b;

        Error = CellError.DIV0;
        return 0;
    }
}