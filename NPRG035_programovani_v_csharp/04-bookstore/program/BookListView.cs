using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using task4.model;
using task4.views;

namespace task4.view;

class BookListView : BaseView
{
    private IEnumerable<Book> books;
    public BookListView(Customer customer, IEnumerable<Book> books): base(customer)
    {
        this.books = books;
    }

    protected override void RenderContent()
    {
        wl("	Our books for you:");
        wl("	<table>");
        if (books.Count() > 0)
        {
            wl("		<tr>");
            int i = 0;
            foreach (Book book in books)
            {
                wl("			<td style=\"padding: 10px;\">");
                wl($"				<a href=\"/Books/Detail/{book.Id}\">{book.Title}</a><br />");
                wl($"				Author: {book.Author}<br />");
                wl($"				Price: {book.Price} EUR &lt;<a href=\"/ShoppingCart/Add/{book.Id}\">Buy</a>&gt;");
                wl("			</td>");
                i++;
                if (i % 3 == 0)
                {
                    wl("		</tr>");
                    wl("		<tr>");
                }
            }
            if (i % 3 != 0) wl("		</tr>");
        }
        wl("	</table>");
    }
}
