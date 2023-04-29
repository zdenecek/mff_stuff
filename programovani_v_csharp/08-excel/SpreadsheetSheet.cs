using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace task08_excel2;

public class SpreadsheetSheet : Sheet
{

	public SpreadsheetSheet(Spreadsheet ctx)
	{
		context = ctx;

		valuationStack = null;
	}


}
