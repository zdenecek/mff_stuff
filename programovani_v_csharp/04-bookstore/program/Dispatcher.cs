using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using task4.routing;

namespace task4
{
    public class Dispatcher
    {

        public Response Dispatch(Route route, Request req)
        {
             return route.Action(req);
        }
    }
}
