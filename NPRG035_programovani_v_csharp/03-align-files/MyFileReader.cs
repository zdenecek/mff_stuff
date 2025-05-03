using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace task3;

public class MyFileReader : TokenReader
{
    private IEnumerable<string> paths;
    private Func<string, TokenReader> readerFactory;
    private bool ignoreErrors;
    public MyFileReader(IEnumerable<string> paths, Func<string, TokenReader> readerFactory, bool ignoreErrors = true)
    {
        this.paths = paths;
        this.readerFactory = readerFactory;
        this.ignoreErrors = ignoreErrors;
    }

    public void Dispose() { }


    public IEnumerable<Token> Read()
    {
        foreach (string path in paths)
        {
            TokenReader r;
            try
            {
                r = readerFactory(path);
            }
            catch (Exception e) when (e is FileNotFoundException || e is UnauthorizedAccessException || e is FormatException)
            {
                if (ignoreErrors) continue;
                throw;
            }
            foreach (var token in r.Read())
            {
                yield return token;
            }
            yield return new FileBreakToken();
            r.Dispose();
        }
    }

}
