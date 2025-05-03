using System.Collections.Generic;
using System.Linq;

namespace CreditTask
{
    internal class Straight : WinningCombination
    {
        protected override string name => "Straight";

        public Straight()
        {
        }

        public Straight(IEnumerable<Card> cards) : base(cards)
        {
        }

        public override WinningCombination Match(IEnumerable<Card> hand)
        {
            if (IsStraight(hand))
            { 
                return new Straight(hand);
            }
            return null;
        }
    }
}