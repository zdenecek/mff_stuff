using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using task4.model;

namespace task4.views;

public class CartView : BaseView
{
    private ShoppingCart cart;
    public CartView(Customer customer) : base(customer)
    {
        cart = customer.ShoppingCart;
    }

    protected override void RenderContent()
    {
        if (cart.Empty) RenderEmpty();
        else RenderNonEmpty();
    }

    private void RenderEmpty()
    {
        wl("	Your shopping cart is EMPTY.");
    }

    private void RenderNonEmpty()
    {
        wl("	Your shopping cart:");
        wl("	<table>");
        wl("		<tr>");
        wl("			<th>Title</th>");
        wl("			<th>Count</th>");
        wl("			<th>Price</th>");
        wl("			<th>Actions</th>");
        wl("		</tr>");
        foreach (var item in cart.Items) {
            wl("		<tr>");
            wl($"			<td><a href=\"/Books/Detail/{item.BookId}\">{item.Book.Title}</a></td>");
            wl($"			<td>{item.Count}</td>");
            if(item.Count == 1)
                wl($"			<td>{item.Price} EUR</td>");
            else
                wl($"			<td>{item.Count} * {item.Book.Price} = {item.Price} EUR</td>");
            wl($"			<td>&lt;<a href=\"/ShoppingCart/Remove/{item.BookId}\">Remove</a>&gt;</td>");
            wl("		</tr>");
        }
        wl("	</table>");
        wl($"	Total price of all items: {cart.Total} EUR");
    }
}
