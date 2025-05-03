using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CreditTask
{
    internal class CombinationComparator : Comparer<Player>
    {
        private int getOuterScore(WinningCombination c)
        {
            return c switch
            {
                StraightFlush a => 10,
                FourOfAKind a => 9,
                Flush a => 8,
                Straight a => 7,
                ThreeOfAKind a => 6,
                Pair a => 5,
                HighCard a => 4,
                _ => throw new NotImplementedException(),
            }; 
        }
        public override int Compare(Player x, Player y)
        {
            if (x is null || y is null)
            {
                throw new ArgumentNullException();
            }

            int scoreX = getOuterScore(x.Combination);
            int scoreY = getOuterScore(y.Combination);

            if (scoreX != scoreY) return scoreX - scoreY;

            scoreX = (int)x.Combination.Cards.Max(c => c.Value);
            scoreY = (int)y.Combination.Cards.Max(c => c.Value);

            if (x.Combination is Straight or StraightFlush)
            {
                if (x.Combination.Cards.Any(c => c.Value == CardValue.Two) &&
                    x.Combination.Cards.Any(c => c.Value == CardValue.Ace))

                    scoreX = (int)CardValue.Five;

                if (y.Combination.Cards.Any(c => c.Value == CardValue.Two) &&
                    y.Combination.Cards.Any(c => c.Value == CardValue.Ace))

                    scoreY = (int)CardValue.Five;
            }
            
            

            return scoreX - scoreY;
        }
    }
}
