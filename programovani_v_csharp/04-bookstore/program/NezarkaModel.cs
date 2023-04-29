using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Linq;

namespace task4.model;

public class ModelStore
{
    private List<Book> books = new List<Book>();
    private List<Customer> customers = new List<Customer>();

    public IList<Book> GetBooks()
    {
        return books;
    }

    public Book GetBook(int id)
    {
        return books.Find(b => b.Id == id);
    }

    public Customer GetCustomer(int id)
    {
        return customers.Find(c => c.Id == id);
    }

    public static ModelStore LoadFrom(TextReader reader)
    {
        var store = new ModelStore();

        try
        {
            if (reader.ReadLine() != "DATA-BEGIN")
            {
                return null;
            }
            for(;;)
            {
                string line = reader.ReadLine();
                if (line == null)
                {
                    return null;
                }
                else if (line == "DATA-END")
                {
                    break;
                }

                string[] tokens = line.Split(';');
                switch (tokens[0])
                {
                    case "BOOK":
                        store.books.Add(new Book
                        {
                            Id = int.Parse(tokens[1]),
                            Title = tokens[2],
                            Author = tokens[3],
                            Price = decimal.Parse(tokens[4])
                        });
                        break;
                    case "CUSTOMER":
                        store.customers.Add(new Customer
                        {
                            Id = int.Parse(tokens[1]),
                            FirstName = tokens[2],
                            LastName = tokens[3]
                        });
                        break;
                    case "CART-ITEM":
                        var customer = store.GetCustomer(int.Parse(tokens[1]));
                        if (customer == null)
                        {
                            return null;
                        }
                        customer.ShoppingCart.Items.Add(new ShoppingCartItem
                        {
                            BookId = int.Parse(tokens[2]),
                            Count = int.Parse(tokens[3])
                        });
                        break;
                    default:
                        return null;
                }
            }
        }
        catch (Exception ex)
        {
            if (ex is FormatException || ex is IndexOutOfRangeException)
            {
                return null;
            }
            throw;
        }

        // wiring
        foreach(var customer in store.customers) {
            foreach(var item in customer.ShoppingCart.Items)
            {
                item.Book = store.GetBook(item.BookId);
            }
        }

        return store;
    }
}

public class Book
{
    public int Id { get; set; }
    public string Title { get; set; }
    public string Author { get; set; }
    public decimal Price { get; set; }
}

public class Customer
{
    private ShoppingCart shoppingCart;

    public int Id { get; set; }
    public string FirstName { get; set; }
    public string LastName { get; set; }

    public ShoppingCart ShoppingCart
    {
        get
        {
            if (shoppingCart == null)
            {
                shoppingCart = new ShoppingCart();
            }
            return shoppingCart;
        }
        set
        {
            shoppingCart = value;
        }
    }
}

public class ShoppingCartItem
{
   
    private Book book;
    public Book Book { get => book; set  { book = value; BookId = value.Id; } }
    public int BookId;
    public int Count { get; set; }
    public decimal Price { get => Book.Price * Count; }
}

public class ShoppingCart
{
    public int CustomerId { get; set; }
    public List<ShoppingCartItem> Items = new List<ShoppingCartItem>();

    public ShoppingCartItem GetItemByBookId(int id)
    {
        return Items.FirstOrDefault(i => i.BookId == id, null);
    }

    public bool Empty
    {
        get => Items.Count == 0;
    }

    public decimal Total
    {
        get => Items.Select(a => a.Price).Sum();
    }
}
