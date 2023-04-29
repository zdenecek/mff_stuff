using System;
using System.Collections.Generic;
using System.Linq;

namespace CreditTask;

abstract internal class NOfAKind : WinningCombination
{
    private int n;
    protected NOfAKind(int n)
    {
        this.n = n;
    }
    protected NOfAKind(IEnumerable<Card> cards) : base(cards) { }

    abstract protected NOfAKind Construct(IEnumerable<Card> cards);

    public override WinningCombination Match(IEnumerable<Card> cards)
    {
        foreach (CardValue val in cards.Select(c=>c.Value))
        {
            var matches = cards.Where(c => c.Value == val).ToList();
            if (matches.Count == n)
            {
                return Construct(matches);
            }
        }
        return null;
    }
}