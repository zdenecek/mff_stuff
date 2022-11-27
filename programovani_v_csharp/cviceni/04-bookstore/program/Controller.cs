using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using task4.model;

namespace task4.controller;

public abstract class Controller
{
	protected ModelStore model;
	public Controller(ModelStore model)
	{
		this.model = model;
	}

    protected Customer GetCustomer(int id) => model.GetCustomer(id);
}
