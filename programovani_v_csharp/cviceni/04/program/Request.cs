using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace task4
{

    public enum Method
    {
        GET
    }

    public record Request
    {
        public string Url { get; init; }
        public Method Method { get; init; }
        public Dictionary<string, object> Variables { get; } = new();

       
    }
}
