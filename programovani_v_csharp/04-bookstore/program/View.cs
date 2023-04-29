using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace task4.views;

public abstract class View
{

    protected void wl(string s)
    {
        Console.WriteLine(s);
    }
    protected void w(string s)
    {
        Console.Write(s);
    }

    public abstract void Render();
}
