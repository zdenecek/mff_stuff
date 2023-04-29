##Krabi�ky � nesestrojiteln� hodnoty
1.	seznam ��sel, kter� nelze vytvo�it
213, 237, 239, 241, 281, 283
2.	popis, jak jste sv� �e�en� na�li.
�e�en� jsem nalezl programaticky a to rekurzivn�m pr�chodem mo�nost�. Rekurzivn� metoda dostane na vstupu zb�vaj�c� operace a dosud dostupn� ��sla, a pro ka�dou mo�nou kombinaci (prvn� �initel, druh� �initel, operace) operaci provede, v�sledek p�id� k dostupn�m ��sl�m a zavol� sama sebe s nov�mi dostupn�mi ��sly a operacemi.
Mohlo by se zd�t, �e postup neprojde v�echny kombinace, kdy� t�eba v t�et�m zano�en� rekurze z�sk�me ��slo zoperov�n�m dvou ��sel, kter� jsme m�li dostupn� u� v zano�en� hloubce jedna, mohli jsme tuto operaci prov�st v hloubce dva a hloubku t�i obohatit o mo�nost vyu�it� nov� vypo�ten�ho ��sla.
Ve skute�nosti ale i takov�to b�h program prozkoum�, proto�e prozkoum� rekurz� redundatn� �pln� v�echny mo�nosti (nevyu�ijeme ani komutativity operac�).
Takov� m�ra zanedb�n� optimalizace je mo�n� d�ky rozm�r�m �lohy. I tak ale mus�me optimalizovat alespo� op�tovn� vol�n� funkce s �pln� stejn�mi parametry, a to ukl�d�n�m vypo��tan�ch hodnot pro v�echny vstupy. Vzhledem k velmi rekurzivn�mu charakteru �lohy tak zna�n� sn�ime po�et vol�n� �e��c� funkce a �as �e�en� dostaneme na p�ijatelnou �rove�.
Vizte p�ipojen� programatick� �e�en� na ~50 ��dk�. V �e�en� hashuji stavy podle �et�zce, co� asi nen� nejoptim�ln�j��, ale sta�� to (jin� �e�en� vzhledem k prom�nn�mu rozm�ru mno�iny dostupn�ch ��sel m� nenapad�).
