using System.Collections.Generic;
using System.Linq;

namespace CreditTask;

class Flush : WinningCombination
{



    public Flush()
    {
    }

    public Flush(IEnumerable<Card> cards) : base(cards)
    {
    }

    protected override string name => "Flush";

    public override WinningCombination Match(IEnumerable<Card> hand)
    {
        if (hand.All(c => c.Suit == hand.First().Suit))
        {
            return new Flush(hand);
        }
        return null;
    }
}