using System.Collections.Generic;

namespace CreditTask
{
    internal class ThreeOfAKind : NOfAKind
    {

        protected override string name => "Three of a kind";

        public ThreeOfAKind() : base(3)
        {
        }

        public ThreeOfAKind(IEnumerable<Card> cards) : base(cards)
        {
        }

        protected override NOfAKind Construct(IEnumerable<Card> cards)
        {
            return new ThreeOfAKind(cards);
        }
    }
}