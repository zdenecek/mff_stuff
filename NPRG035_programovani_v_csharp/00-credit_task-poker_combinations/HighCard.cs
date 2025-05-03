using System.Collections.Generic;
using System.Linq;

namespace CreditTask;

internal class HighCard : WinningCombination
{
    protected override string name => "High card";

    public HighCard()
    {
    }

    public HighCard(IEnumerable<Card> cards) : base(cards)
    {
    }

    public override WinningCombination Match(IEnumerable<Card> cards)
    {
        return new HighCard(cards.OrderByDescending(a => a.Value).Take(1));
    }

}