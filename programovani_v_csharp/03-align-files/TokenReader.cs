using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace task3;

public interface TokenReader : IDisposable
{
    public IEnumerable<Token> Read();
}
