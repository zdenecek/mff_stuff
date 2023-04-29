using System.Collections.Generic;
using System.Linq;

namespace CreditTask
{
    internal class StraightFlush : WinningCombination
    {

        protected override string name => "Straight flush";

        public StraightFlush()
        {
        }

        public StraightFlush(IEnumerable<Card> cards) : base(cards)
        {
        }

        public override WinningCombination Match(IEnumerable<Card> hand)
        {
            var suit = hand.First().Suit;
            if (IsStraight(hand) && hand.All(c => c.Suit == suit))
            {
                return new StraightFlush(hand);
            }
            return null;
        }
    }
}