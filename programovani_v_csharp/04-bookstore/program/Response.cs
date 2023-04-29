using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using task4.views;

namespace task4
{
    public class Response
    {

        public static Response Redirect(string Url)
        {
            return new RedirectResponse { RedirectUrl = Url };
        }

        public static Response Html(View view)
        {
            return new HtmlResponse { view = view };
        }

    }

    public class RedirectResponse : Response
    {
        public string RedirectUrl { get; init; }
    }


    public class HtmlResponse : Response
    {
        public View view { get; init; }
    }
}
