using System.Collections.Generic;
using System.Linq;

namespace CreditTask
{
    internal class Pair : WinningCombination
    {

        protected override string name => "One pair";

        public Pair()
        {
        }

        public Pair(IEnumerable<Card> cards) : base(cards)
        {
        }

        public override WinningCombination Match(IEnumerable<Card> hand)
        {
            var groups = hand.GroupBy(c => c.Value).OrderByDescending(g => g.Key);
            var pair = groups.FirstOrDefault(g => g.Count() == 2);
            if (pair != null)
            {
                return new Pair(pair);
            }
            return null;
        }
    }
}