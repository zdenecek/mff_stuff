
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using task4;
using task4.controller;
using task4.model;
using task4.routing;
using task4.views;

namespace task4
{
    internal class Program
    {
        static void Main(string[] args)
        {
            App app;
            try
            {
                app = new App();
            }
            catch (ModelException)
            {
                Console.WriteLine("Data error.");
                return;
            }
            app.Run();
        }
    }



public class ModelException : Exception { }
class App
{

    private Router router;
    private Dispatcher dispatcher;
    private ModelStore model;
    private CartController cartController;
    private BookController bookController;


    public App()
    {
        // No DI container available...
        model = ModelStore.LoadFrom(Console.In);

        if (model == null) throw new ModelException();
        dispatcher = new Dispatcher();
        bookController = new(model);
        cartController = new(model);

        router = new Router("http://www.nezarka.net");
        RegisterRoutes();
    }

    public void Run()
    {
        Request req;
        for (; ; )
        {
            try
            {
                req = parseRequest();
                if (req == null) return; // end of life
            }
            catch
            {
                ShowInvalid();
                continue;
            }
            Handle(req);
        }

    }

    private void Handle(Request req)
    {
        var route = router.FindRoute(req);
        var response = dispatcher.Dispatch(route, req);
        if (response is RedirectResponse)
            Handle(new Request { Method = req.Method, Url = ((RedirectResponse)response).RedirectUrl });
        if (response is HtmlResponse)
        {
            ((HtmlResponse)response).view.Render();
            Console.WriteLine("====");
        }
    }

    private void ShowInvalid()
    {
        new InvalidRequestView().Render();
        Console.WriteLine("====");
    }
    private Request parseRequest()
    {
        var s = Console.ReadLine();
        if (s == null) return null;
        var a = s.Split(" ");
        if (a.Length != 3) throw new Exception();
        if (a[0] != "GET") throw new Exception();

        var r = new Request
        {
            Url = a[2],
            Method = Method.GET
        };
        int userId;
        if (!int.TryParse(a[1], out userId)) throw new Exception();

        r.Variables.Add("UserId", userId);
        return r;
    }

    private void RegisterRoutes()
    {
        router.RegisterRoute("/Books", bookController.ShowBooks);
        router.RegisterRoute("/Books/Detail/{id}", bookController.ShowBookDetail);
        router.RegisterRoute("/ShoppingCart", cartController.Show);
        router.RegisterRoute("/ShoppingCart/Add/{id}", cartController.Add);
        router.RegisterRoute("/ShoppingCart/Remove/{id}", cartController.Remove);
        router.RegisterRoute(".*", (req) => Response.Html(new InvalidRequestView()), false);
    }
}
}