using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CreditTask
{
    class Player
    {
        public List<Card> Cards { get; }

        public WinningCombination Combination { get; set; } = null;


        public string Name { get; set; }

        public Player(string name)
        {
            Name = name;
            Cards = new List<Card>();
        }
    }
}
