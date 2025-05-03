using System.Collections.Generic;
using System.Linq;


namespace CreditTask;

internal class FourOfAKind : NOfAKind
{
    protected override string name => "Four of a kind";

    public FourOfAKind() : base(4)
    {
    }

    public FourOfAKind(IEnumerable<Card> cards) : base(cards)
    {
    }

    protected override NOfAKind Construct(IEnumerable<Card> cards)
    {
        return new FourOfAKind(cards);
    }
    
}