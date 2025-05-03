using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CreditTask;

abstract class WinningCombination
{
    public IEnumerable<Card> Cards { get; } = null;

    public WinningCombination() { }
    protected WinningCombination(IEnumerable<Card> cards)
    {
        Cards = new List<Card>(cards.OrderBy(c => c.Suit).OrderBy(c => c.Value));
    }
    public abstract WinningCombination Match(IEnumerable<Card> cards);


    public static bool IsStraight(IEnumerable<Card> cards)
    {
        bool useLowerAces = cards.Any(c => c.Value == CardValue.Two);

        var values = cards.Select(c => (int) (c.Value != CardValue.Ace ? (int) c.Value : (useLowerAces ? 1 : 14))).OrderBy(a => a);

        for(int i = 0; i < 4; i++)
        {
            if (values.ElementAt(i + 1) - values.ElementAt(i) != 1) return false;
        }

        return true;
    }

    protected abstract string name { get; }

    public override string ToString()
    {
        var cs = string.Join(' ', Cards.Select(c => CardStringifier.ToString(c)));
        return $"{name} ({cs})";
    }
}
