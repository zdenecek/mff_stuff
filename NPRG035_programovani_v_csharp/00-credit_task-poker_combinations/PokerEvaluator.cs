using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CreditTask
{
    class PokerEvaluator
    {
        // Combinations in order
        List<WinningCombination> combinations = new List<WinningCombination>();
          
        public PokerEvaluator()
        {
            combinations.Add(new StraightFlush());
            combinations.Add(new FourOfAKind());
            combinations.Add(new Flush());
            combinations.Add(new Straight());
            combinations.Add(new ThreeOfAKind());
            combinations.Add(new Pair());
            combinations.Add(new HighCard());
        }


        private bool ValidateInput(IEnumerable<Player> players)
        {
            if (players is null)
            {
                throw new ArgumentNullException();
            }

            if (players.Count() == 0)
            {
                throw new ArgumentException("Error: At least one player is required");
            }

            foreach (var player in players)
            {
                if (player.Cards.Count() != 5)
                {
                    throw new ArgumentException("Error: Each player must have 5 cards");
                }
            }

            HashSet<Card> cards = new();
            HashSet<Card> duplicates = new();

            foreach (var p in players)
            {
                foreach(var c in p.Cards)
                {
                    if (!cards.Add(c)) duplicates.Add(c);
                }
            }

            if(duplicates.Count > 0)
            {
                var s = String.Join(", ", duplicates.Select(c => CardStringifier.ToString(c)));
                throw new ArgumentException("There shall be no duplicate cards. Duplicate cards found: " + s);
            }

            return true;
        }

        public void Evaluate(IEnumerable<Player> players)
        {
            ValidateInput(players);


             foreach (var player in players)
            {
                foreach (var possible in combinations)
                {
                    var match = possible.Match(player.Cards);

                    if (match is not null)
                    {
                        player.Combination = match;
                        break;
                    }
                }
            }

            var c = new CombinationComparator();
            var ordered = players.OrderByDescending(p => p, c);

            var winner = ordered.First();
            var winners = ordered.TakeWhile(p => c.Compare(p, winner) == 0);

            printResults(players, winners);

        }

        private void printResults(IEnumerable<Player> players, IEnumerable<Player> winners) 
        {
            foreach (var player in players)
            {
                string winnerString = winners.Contains(player) ? "(WINNER)" : "";
                Console.WriteLine($"{player.Name}: {player.Combination} {winnerString}");
            }
        }


    }
}


