using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;

namespace task4.routing;

public class Router
{
    private readonly string domain;
    List<Route> routes = new List<Route>();

    public Router(string domain)
    {
        this.domain = domain;
    }


    public void RegisterRoute(string path, Func<Request, Response> action, bool addDomain = true)
    {
        Regex r = new Regex(@"\{([a-zA-Z\d]*)\}");
        var captures = r.Matches(path);
        var names = new string[captures.Count];

        for (int i = 0; i < captures.Count; i++)
        {
            // simplified, we have only numeric variables
            path = path.Replace(captures[i].Value, @"(\d+)");
            names[i] = captures[i].Groups[1].Value;
        }
        string s = addDomain ? this.domain : "";
        routes.Add(new Route(names, new Regex($"^{s}{path}$"), action));
    }

    public Route FindRoute(Request req) {

        foreach (Route route in routes)
        {
            if (!route.Regex.IsMatch(req.Url)) continue;
            var match = route.Regex.Match(req.Url);
            
            for (int i = 0; i < route.Variables.Count(); i++)
            {
                req.Variables.Add(route.Variables[i], int.Parse(match.Groups[i + 1].Value));
            }

            return route;
        }

        throw new Exception("No route.");
    }

   
}

public record struct Route
{
    public Func<Request, Response> Action { get; init; }
    public string[] Variables { get; init; }
    public Regex Regex { get; init; }
    public Route(string[] variables,
    Regex regex, Func<Request, Response> action)
    {
        Action = action;
        Variables = variables;
        Regex = regex;
    }
}


