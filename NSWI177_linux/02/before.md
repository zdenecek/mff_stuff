# Kvíz před cvičením (#2)

Kvíz musí být nahrán do GitLabu před začátkem vašeho cvičení.

Každá otázka je za 25 bodů.

Vaši odpověď vložte do označeného bloku (místo tří teček).



## Otázka Q1

Stručně napište, co pro vás bylo nejnáročnější na předchozím cvičení.


Vaše odpověď: **[A1]** Upřímně nejnáročnější bylo se na cvičení dostavit, jelikož se konalo v devět ráno. **[/A1]**



## Otázka Q2

Seznam níže obsahuje různé cesty. Vyberte všechny absolutní cesty ze
seznamu (oddělte jednotlivá čísla čárkou nebo mezerou).

1. `/etc/`
2. `../intro/`
3. `.bashrc`
4. `/dev/../etc/os-release`
5. `../share/man/man3/lseek64.3.gz`

Vaše odpověď: **[A2]** 1,4 **[/A2]**



## Otázka Q3

Zkraťte následující cestu tak, aby neobsahovala žádné relativní odkazy
(tj. převeďte ji na absolutní cestu bez `..` a `.`).

```
/home/../usr/./share/./man/../../lib/../../etc/ssh/.././os-release
```

Vaše odpověď: **[A3]** /etc/os-release **[/A3]**



## Otázka Q4

Vyberte z možností níže tu, které **nejlépe vystihuje** účel
následujího útržku Pythoního programu (komentáře jsme odstranili).

Chceme po vás vybrat nejlepší možnost (tj. nejpřesnější): odpověď
_je to Pythoní kód, který něco vytiskne_ je sice pravdivá, ale neřeší
podstatu otázky

```python
stats = {}
with open('/proc/meminfo', 'r') as f:
    for line in f:
        parts = line.split(":")
        stats[parts[0].strip()] = parts[1].split()[0].strip()
print(float(stats['MemFree'])/float(stats['MemTotal']))
```

1. Vytiskne první dva řádky souboru `/proc/meminfo`.
2. Vytiskne druhý sloupeček souboru `/proc/meminfo`, jednotlivé sloupečky
   jsou odděleny dvojtečkou (`:`).
3. Vytiskne odhad volného množství paměti v procentech.
4. Zkontroluje, že `/proc/meminfo` obsahuje platná data.
5. Přečte `/proc/meminfo` a ověří, jestli je ve správném formátu.

Vaše odpověď: **[A4]** 3 **[/A4]**



