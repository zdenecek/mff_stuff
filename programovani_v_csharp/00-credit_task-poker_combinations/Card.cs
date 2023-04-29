using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CreditTask;

record struct Card
{
    public CardValue Value;
    public Suit Suit;

    public Card(CardValue value, Suit suit)
    {
        Value = value;
        Suit = suit;
    }

    public override string ToString()
    {
        return $"{CardStringifier.ValueToString(Value)}{CardStringifier.SuitToString(Suit)}";
    }
}
