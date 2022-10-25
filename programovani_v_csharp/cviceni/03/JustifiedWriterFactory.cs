using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace task3;

class JustifiedWriterFactory
{
    public static JustifiedWriter MakeJustifiedWriter(TextWriter tw, int lineLength, bool highlight)
    {
        var spaceChar = highlight ? '.' : ' ';
        var append = highlight ? "<-" : null;
        var jw = new JustifiedWriter(tw, lineLength, spaceChar, append);
        return jw;
    }
}
