using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using task4.model;
using task4.view;
using task4.views;

namespace task4.controller;

public class BookController : Controller
{
    public BookController(ModelStore model) : base(model)
    {
    }


    //- beze změny dat, zobrazí InformaceOKnize pro knihu BookId
    public Response ShowBookDetail(Request req)
    {
        var c = GetCustomer((int)req.Variables["UserId"]);
        if (c == null) return Response.Redirect("invalid");

        var bookId = (int)req.Variables["id"];
        var book = model.GetBook(bookId);

        // would be done with redirect
        if (book == null) return Response.Redirect("invalid");

        return Response.Html(new BookDetailView(c, book));
    }

    // beze změny dat, zobrazí SeznamKnih
    public Response ShowBooks(Request req)
    {
        var c = GetCustomer( (int) req.Variables["UserId"] );
        if (c == null) Response.Redirect("invalid");

        return Response.Html(new BookListView(c, model.GetBooks().ToList()));
    }
}
