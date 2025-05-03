using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using task4.model;
using task4.views;

namespace task4.view;

class BookDetailView : BaseView
{
    private Book book;
    public BookDetailView(Customer customer, Book book) : base(customer)
    {
        this.book = book;
    }

    protected override void RenderContent()
    {
        wl("	Book details:");
        wl($"	<h2>{book.Title}</h2>");
        wl("	<p style=\"margin-left: 20px\">");
        wl($"	Author: {book.Author}<br />");
        wl($"	Price: {book.Price} EUR<br />");
        wl("	</p>");
        wl($"	<h3>&lt;<a href=\"/ShoppingCart/Add/{book.Id}\">Buy this book</a>&gt;</h3>");
    }
}
