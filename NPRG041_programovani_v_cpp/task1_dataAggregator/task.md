# Zadání

Vytvořte agregátor dat. Program ze vstupu načte tabulku. Poté sloučí hodnoty v zadaném sloupci a hodnoty ve zbylých sloupcích agreguje pomocí zadané funkce. Jako oddělovač hodnot slouží středník - `;`. Na první řádce jsou zadané názvy jednotlivých sloupců. Druhý řádek obsahuje typy jednotlivých sloupců. Následuje číslo `N` značící počet řádků v tabulce. Další následují data. Na poslední řádce je napsán dotaz, který popisuje formát výstupu.

Program na výstup vypíše tabulku v podobném formátu jako vstup. Veškeré hodnoty budou odděleny středníkem. První řádka bude popisovat jména resp. operace v daném sloupci, ostatní řádky budou obsahovat samotná data.

## Vstup

```
<jmeno_sloupce_1>;<jmeno_sloupce_2>;...;<jmeno_sloupce_M>
<typ_sloupce_1>;<typ_sloupce_2>;...;<typ_sloupce_M>
<N>
<hodnota_1_1>;<hodnota_1_2>;...;<hodnota_1_M>
<hodnota_2_1>;<hodnota_2_2>;...;<hodnota_2_M>
...
<hodnota_N_1>;<hodnota_N_2>;...;<hodnota_N_M>
<dotaz>
```

První řádka obsahuje jména jednotlivých sloupců. Jednotlivá jména jsou typu `string`, neobsahují mezery a jsou odděleny středníkem - `;`. (např: `jmeno;vek;vzdalenost`)

Druhá řádka obsahuje typy jednotlivých sloupců. i-tý typ popisuje typ i-tého sloupce. K dispozici jsou 3 typy - `int`, `string`, `double`, které přesně odpovídají použitým typům v C++. Typy jsou oddělené středníkem - `;`. (např: `string;int;double`)

Třetí řádka obsahuje číslo `N` typu `size_t`, které udává počet následujících řádků s daty.

Následujících N řádků obsahuje popis dat. Řádky udávají řádky v tabulce, sloupce jsou odděleny středníkem. Např:

```
Anna;25;3.14
Borek;53;2.5
```

Na poslední řádce je obsažen dotaz. Dotaz má následující formát: `SELECT <popis_výstupní_tabulky> GROUP_BY <název_sloupce>`.

-   `<název_sloupce>` - Název sloupce, podle kterého se data v tabulce slučují. Obsahuje právě jeden sloupec. Hodnoty se slučují, pokud porovnání pomocí operatoru `==` vrátí `true`. Pozor na porovnávání typu `double`, `1.0 == 1.00000`
    
-   `<popis_výstupní_tabulky>` - Obsahuje názvy sloupců společně s funkcemi, které se mají provést na uskupených datech. K dispozici jsou tři agregační funkce - `SUM` (sčítání), `MIN` (minimum), `MAX` (maximum). Všechny funkce mohou být použity se všemi podporovanými typy - `int/double/string`. Operace `SUM` v případě typu `string` provádí spojování `std::string::operator+()` (nebo ekvivalentní operátor), v případě operace `MIN/MAX` porovná zadané hodnoty a vrátí menší resp. větší (`std::string::operator<()`). Operace mají jako svůj argument jméno sloupce. (např: `SELECT MIN(vek), SUM(vzdalenost) GROUP_BY jmeno`). Popis může rovněž obsahovat název sloupce, podle kterého se hodnoty slučují, v tom případě **nesmí** obsahovat název funkce. (např: `SELECT jmeno, MIN(vek), SUM(vzdalenost) GROUP_BY jmeno`). V tomto případě obsahuje sloupec uskupené hodnoty.
    
    Sloupce jsou odděleny **čárkou a mezerou**. Pořadí sloupců v dotazu může být libovolné, nemusí se zachovávat pořadí ze vstupu. Sloupce se mohou opakovat. (např: `SELECT SUM(vzdalenost), jmeno, MIN(vek), jmeno, SUM(vzdalenost) GROUP_BY jmeno`).
    
    **Formát**:
    
    -   `SELECT <operace_nebo_gid> GROUP_BY <gid>` nebo
    -   `SELECT <operace_nebo_gid>, <operace_nebo_gid>, ..., <operace_nebo_gid> GROUP_BY <gid>`
    
    kde `<operace_nebo_gid>` je:
    
    -   `SUM(<sloupec>)` nebo
    -   `MIN(<sloupec>)` nebo
    -   `MAX(<sloupec>)` nebo
    -   `<sloupec>`

## Výstup

```
<vystupni_sloupec_1>;<vystupni_sloupec_2>;...;<vystupni_sloupec_R>
<hodnota_1_1>;<hodnota_1_2>;...;<hodnota_1_R>
<hodnota_2_1>;<hodnota_2_2>;...;<hodnota_2_R>
...
<hodnota_S_1>;<hodnota_S_2>;...;<hodnota_S_R>
```

Na první řádce je popis výstupní tabulky. Je stejný, jako je v zadané části dotazu - obsahuje buď název operace společně s daným sloupcem, případně pouze název sloupce, pokud se jedná o sloupec, podle kterého slučujeme hodnoty. (např: Pro dotaz `SELECT SUM(vzdalenost), jmeno, MIN(vek), jmeno, SUM(vzdalenost) GROUP_BY jmeno` je popis výstupní tabulky `SUM(vzdalenost);jmeno;MIN(vek);jmeno;SUM(vzdalenost)`). Hodnoty jsou oddělené středníky.

Na dalších řádcích následují vypočítané hodnoty uspořádané v tabulce. Pořadí agregovaných řádků musí být stejné jako na vstupu.

## Příklad

### Vstup

```
key;jmeno;prijmeni;vek;deti;rodice;kralici;prumer
double;string;string;int;int;int;double;double
6
1;a;xxx;10;2;1;2.4;-0.5
2;b;yyy;56;3;0;123.8;-1.2
3;c;zzz;13;2;3;3.14;-2.2
4;d;xxx;23;1;6;1.41;3.1
5;a;xxx;76;3;10;8.833;5.2
6;b;zzz;99;3;22;2.44443;9.9
SELECT prijmeni, SUM(jmeno), MIN(key), MAX(key), MIN(vek), MAX(vek), SUM(deti), SUM(rodice), MIN(prumer), MAX(prumer) GROUP_BY prijmeni
```

### Výstup

```
prijmeni;SUM(jmeno);MIN(key);MAX(key);MIN(vek);MAX(vek);SUM(deti);SUM(rodice);MIN(prumer);MAX(prumer)
xxx;ada;1.000000;5.000000;10;76;6;17;-0.500000;5.200000
yyy;b;2.000000;2.000000;56;56;3;0;-1.200000;-1.200000
zzz;cb;3.000000;6.000000;13;99;5;25;-2.200000;9.900000
```

## Poznámky

-   Vysvětlení agregace na Wikipedii:
    -   [wiki/GROUP\_BY](https://cs.wikipedia.org/wiki/GROUP_BY)
    -   [wiki/Agregační\_funkce](https://cs.wikipedia.org/wiki/Agrega%C4%8Dn%C3%AD_funkce)
-   Všechny vstupy a data jsou **validní** a nejsou zákeřná, není je potřeba kontrolovat.
-   Pozor na mezery v dotazu.
-   Pokud máte problémy s načítáním vstupu, zkuste nahradit `std::getline` standardním `std::cin >> str`
-   Za poslední hodnotou se neuvádí středník.
-   Hodnoty typu double vypisujte s 6 desetinnými místy (`std::fixed`) (`int i; double d; string s; std::cout << i << ';' << std::fixed << d << ';' << s`.)
-   `GROUP_BY` obsahuje podtžítko
-   Narozdíl od vstupu se na výstup nevypisuje počet řádek v tabulce.
-   Pořadí řádků na výstupu musí být stejné jako na vstupu.
-   Testujte váš program přes přesměrování vstupu (podobně, jako to dělá ReCodex)
-   Vstupní a výstupní soubory pro přiklad:
    -   [test.input](https://recodex.mff.cuni.cz/api/v1/uploaded-files/4f1873bb-4b74-11ec-986f-f39926f24a9c/download)
    -   [test.output](https://recodex.mff.cuni.cz/api/v1/uploaded-files/75e36fe4-4b74-11ec-986f-f39926f24a9c/download)