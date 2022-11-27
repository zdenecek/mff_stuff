using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace task4.views
{
    public class InvalidRequestView : View
    {
        public override void Render()
        {
            wl("<!DOCTYPE html>");
            wl("<html lang=\"en\" xmlns=\"http://www.w3.org/1999/xhtml\">");
            wl("<head>");
            wl("	<meta charset=\"utf-8\" />");
            wl("	<title>Nezarka.net: Online Shopping for Books</title>");
            wl("</head>");
            wl("<body>");
            wl("<p>Invalid request.</p>");
            wl("</body>");
            wl("</html>");
        }
    }
}
