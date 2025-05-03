using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace cs
{
    static class WaterUtils
    {
        public static Dictionary<int, int> bfs(IEnumerable<WaterCup> cups)
        {
            var memory = new HashSet<WaterCupState>();
            var results = new Dictionary<int, int>();
            var que = new Queue<(WaterCupState, int)>();

            var factory = new WaterCupStateFactory(cups.Count());
            Func<WaterCupState, IEnumerable<WaterCupState>> getNextStates = state => factory.getNextStates(state);


            que.Enqueue((new WaterCupState(cups), 0));


            while (que.Count > 0)
            {

                (var currentState, var depth) = que.Dequeue();

                foreach (var cup in currentState.Cups)
                {
                    if (results.ContainsKey(cup.VolumeFilled)) continue;
                    results.Add(cup.VolumeFilled, depth);
                }

                var next = getNextStates(currentState);

                foreach (var newState in next)
                {
                    if (memory.Contains(newState)) continue;
                    memory.Add(newState);

                    que.Enqueue((newState, depth + 1));
                }
            }

            return results;
        }
    }

    struct WaterCup
    {
        public static bool operator ==(WaterCup a, WaterCup b) {
            return a.Volume == b.Volume && a.VolumeFilled == b.VolumeFilled;    
        }
        public static bool operator !=(WaterCup a, WaterCup b)
        {
            return !(a == b);
        }

        public int Volume { get; private set; }
        public int VolumeFilled { get; private set; }

        public bool isFull => VolumeFilled == Volume;
        public bool isEmpty => VolumeFilled == 0;

        public int emptyVolume => Volume - VolumeFilled;
        

        public WaterCup(int volume, int volumeFilled)
        {
            Volume = volume;
            VolumeFilled = volumeFilled;
        }

        public WaterCup pourOut(int toPourOut)
        {
            return new WaterCup(Volume, VolumeFilled - toPourOut);
        }

        public WaterCup pourIn(int toPourIn)
        {
            return new WaterCup(Volume, VolumeFilled + toPourIn);
        }

        public override bool Equals(object obj)
        {
            return obj is WaterCup cup &&
                   Volume == cup.Volume &&
                   VolumeFilled == cup.VolumeFilled;
        }

        public override int GetHashCode()
        {
            return HashCode.Combine(Volume, VolumeFilled);
        }
    }

    class WaterCupStateFactory
    {
        private readonly int cupCount;
        private readonly List<(int, int)> vectors;

        public WaterCupStateFactory(int cupCount)
        {
            this.cupCount = cupCount;
            var perms = MyMath.Permutations(cupCount, 2);
            vectors = new List<(int, int)>(perms.Count());
            foreach(var v in perms)
            {
                vectors.Add((v.ElementAt(0) - 1, v.ElementAt(1) - 1));
            }

        }


        public IEnumerable<WaterCupState> getNextStates(WaterCupState state)
        {
            foreach ((int fromIndex, int toIndex) in vectors)
            {
                var from = state.Cups[fromIndex];
                var to = state.Cups[toIndex];
                if (from.isEmpty || to.isFull) continue;
                int volumeTransfered = Math.Min(from.VolumeFilled, to.emptyVolume);
                WaterCup[] newCups = new WaterCup[state.Cups.Length];
                Array.Copy(state.Cups, newCups, state.Cups.Length);
                newCups[fromIndex] = from.pourOut(volumeTransfered);
                newCups[toIndex] = to.pourIn(volumeTransfered);
                var newState = new WaterCupState(newCups);
                yield return newState;
            }

        }
    }

    struct WaterCupState
    {

        public readonly WaterCup[] Cups;

        public WaterCupState(WaterCup[] cups)
        {
            Cups = cups;
        }

        public WaterCupState(IEnumerable<WaterCup> cups)
        {
            Cups = cups.ToArray();
        }

        public override bool Equals(object obj)
        {
            return obj is WaterCupState && 
                ((WaterCupState) obj).Cups.Zip(this.Cups)
                .Select(a => a.First != a.Second ).Any();
        }

        public override string ToString()
        {
            return String.Join(' ', (from cup in Cups select cup.VolumeFilled));
        }

        public override int GetHashCode()
        {
            // not sure this will work :)
            
            return Cups.Select((cup) => cup.GetHashCode()).Aggregate( (a,b) => HashCode.Combine(a,b));
        }
    }
}
