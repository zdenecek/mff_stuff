using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using task4.model;

namespace task4.views;

public abstract class BaseView : View
{
    private Customer customer;
    public BaseView(Customer customer)
    {
        this.customer = customer;
    }

    public override void Render()
    {

        wl("<!DOCTYPE html>");
        wl("<html lang=\"en\" xmlns=\"http://www.w3.org/1999/xhtml\">");
        wl("<head>");
        wl("	<meta charset=\"utf-8\" />");
        wl("	<title>Nezarka.net: Online Shopping for Books</title>");
        wl("</head>");
        wl("<body>");
        wl("	<style type=\"text/css\">");
        wl("		table, th, td {");
        wl("			border: 1px solid black;");
        wl("			border-collapse: collapse;");
        wl("		}");
        wl("		table {");
        wl("			margin-bottom: 10px;");
        wl("		}");
        wl("		pre {");
        wl("			line-height: 70%;");
        wl("		}");
        wl("	</style>");
        wl("	<h1><pre>  v,<br />Nezarka.NET: Online Shopping for Books</pre></h1>");
        wl($"	{customer.FirstName}, here is your menu:");
        wl("	<table>");
        wl("		<tr>");
        wl("			<td><a href=\"/Books\">Books</a></td>");
        wl($"			<td><a href=\"/ShoppingCart\">Cart ({customer.ShoppingCart.Items.Count})</a></td>");
        wl("		</tr>");
        wl("	</table>");

        RenderContent();
        
        wl("</body>");
        wl("</html>");
    }

    protected abstract void RenderContent();
}
