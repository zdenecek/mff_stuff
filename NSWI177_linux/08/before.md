# Kvíz před cvičením (#8)

Kvíz musí být nahrán do GitLabu před začátkem vašeho cvičení.

Každá otázka je za 25 bodů.

Vaši odpověď vložte do označeného bloku (místo tří teček).



## Otázka Q1

Stručně napište, co pro vás bylo nejnáročnější na předchozím cvičení.


Vaše odpověď: **[A1]** Nejnáročnější bylo vyplnit kvíz. **[/A1]**



## Otázka Q2

Mějme následující rozšířený regulární výraz.

```
^(a[bc]+[d-z]|A.C*)$
```

Vyberte, které z řetězců budou tímto výrazem zachyceny.
1. `abcd`
2. `acacz`
3. `abbbe`
4. `az`
5. `A.C`
6. `AA`
7. `A`
8. `ABCABC`
9. `ABCCCC`

Vaše odpověď: **[A2]** 1,3,4,5,6,9 **[/A2]**



## Otázka Q3

Vyberte nejlepší popis toho, co dělá následující příkaz.

```
egrep '[0-9]{4,6}'
```

1. Vypíše řádky obsahující přesně `[0-9]{4,6}`.
2. Vypíše všechna čísla ze stdin s 4--6 číslicemi, každé na samostatný řádek.
3. Vypíše právě ty řádky, které jsou celé tvořeny číslem s 4--6 číslicemi.
4. Vypíše právě řádky obsahující číslo, které má aspoň 4 číslice.
5. Vypíše právě řádky obsahující číslo, které má aspoň 4 číslice ale ne více než 6.

Vaše odpověď: **[A3]** 5 **[/A3]**



## Otázka Q4

Vyberte, která nahrazení budou provedena následujícím příkazem.

```
sed 's=\(https\{0,1\}\)://[^/ ]*=\1://example.com='
```

1. `https0://myweb.org` -> `https0://example.com`
2. `://aaa` -> `://example.com`
3. `http://a/b/c` -> `http://example.com`
4. `https://otherweb.com` -> `https://example.com`
5. `http://web1.cz and https://web2.cz` -> `http://example.com and https://example.com`
6. `ftp://abc.org/file1, https://def.org/file2` -> `ftp://abc.org/file1, https://example.com/file2`
7. `HTTPS://ghi.io/` -> `HTTPS://example.com/`
8. `download("http://company.com","~/homepage.html")` -> `download("http://example.com","~/homepage.html")`

Vaše odpověď: **[A4]** 4,6 **[/A4]**
