##Krabièky — nesestrojitelné hodnoty
1.	seznam èísel, která nelze vytvoøit
213, 237, 239, 241, 281, 283
2.	popis, jak jste své øešení našli.
Øešení jsem nalezl programaticky a to rekurzivním prùchodem možností. Rekurzivní metoda dostane na vstupu zbývající operace a dosud dostupná èísla, a pro každou možnou kombinaci (první èinitel, druhý èinitel, operace) operaci provede, výsledek pøidá k dostupným èíslùm a zavolá sama sebe s novými dostupnými èísly a operacemi.
Mohlo by se zdát, že postup neprojde všechny kombinace, když tøeba v tøetím zanoøení rekurze získáme èíslo zoperováním dvou èísel, která jsme mìli dostupné už v zanoøení hloubce jedna, mohli jsme tuto operaci provést v hloubce dva a hloubku tøi obohatit o možnost využití novì vypoèteného èísla.
Ve skuteènosti ale i takovýto bìh program prozkoumá, protože prozkoumá rekurzí redundatnì úplnì všechny možnosti (nevyužijeme ani komutativity operací).
Taková míra zanedbání optimalizace je možná díky rozmìrùm úlohy. I tak ale musíme optimalizovat alespoò opìtovné volání funkce s úplnì stejnými parametry, a to ukládáním vypoèítaných hodnot pro všechny vstupy. Vzhledem k velmi rekurzivnímu charakteru úlohy tak znaènì snížime poèet volání øešící funkce a èas øešení dostaneme na pøijatelnou úroveò.
Vizte pøipojené programatické øešení na ~50 øádkù. V øešení hashuji stavy podle øetìzce, což asi není nejoptimálnìjší, ale staèí to (jiné øešení vzhledem k promìnnému rozmìru množiny dostupných èísel mì nenapadá).
