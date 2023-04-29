using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using task4.model;
using task4.view;
using task4.views;

namespace task4.controller;

public class CartController : Controller
{
    public CartController(ModelStore model) : base(model)
    {
    }


    // - přídá 1 položku(1 kus) BookId do nákupního košíku aktuálního zákazníka(dle CustId); pokud kniha BookId již v košíku je, tak se jen zvětší počet kusů o 1; zobrazí ObsahKošíku
    public Response Add(Request req)
    {
        var c = GetCustomer((int)req.Variables["UserId"]);
        if (c == null) return Response.Redirect("invalid");

        var b = model.GetBook((int)req.Variables["id"]);
        if (b == null) return Response.Redirect("invalid");

        var i = c.ShoppingCart.GetItemByBookId(b.Id);


        if (i == null)
            c.ShoppingCart.Items.Add(new ShoppingCartItem
            {
                Book = b,
                Count = 1
            });
        else
            i.Count++;

        return Response.Html(new CartView(c));
    }

    //- odebere 1 položku(1 kus) BookId z nákupního košíku(tj.zmenší počet kusů o 1); pokud je kniha BookId v košíku jen jednou, tak z košíku odebere celý záznam pro tuto knihu; zobrazí ObsahKošíku
    public Response Remove(Request req)
    {
        var c = GetCustomer((int)req.Variables["UserId"]);
        if (c == null) return Response.Redirect("invalid");

        var b = model.GetBook((int)req.Variables["id"]);
        if (b == null) return Response.Redirect("invalid");

        var i = c.ShoppingCart.GetItemByBookId(b.Id);
        if (i == null)
            return Response.Redirect("invalid");
  


        else if (i.Count == 1)
            c.ShoppingCart.Items.Remove(i);
        else
            i.Count--;

        return Response.Html(new CartView(c));
    }

    //- beze změny dat, zobrazí ObsahKošíku pro aktuálního zákazníka(dle CustId)
    public Response Show(Request req)
    {
        var c = GetCustomer((int)req.Variables["UserId"]);
        if (c == null) return Response.Redirect("invalid");


        return Response.Html(new CartView(c));

    }
}
