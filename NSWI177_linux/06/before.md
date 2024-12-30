# Kvíz před cvičením (#6)

Kvíz musí být nahrán do GitLabu před začátkem vašeho cvičení.

Každá otázka je za 25 bodů.

Vaši odpověď vložte do označeného bloku (místo tří teček).



## Otázka Q1

Stručně napište, co pro vás bylo nejnáročnější na předchozím cvičení.


Vaše odpověď: **[A1]** Nejtěžší bylo dohledat nutné informace. Poskytnuté texty byly nedostatečné. **[/A1]**



## Otázka Q2

Předpokládejme, že jsme v adresáři, který obsahuje pouze skript `ioargs.py`
a žádné další soubory (pochopitelně, speciální adresáře `.` a `..` tam budou, ale
žádné další skryté soubory).

Skript `ioargs.py` má následující obsah (a executable bit nastaven).

```python
#!/usr/bin/env python3

import sys

def main():
    for i, arg in enumerate(sys.argv):
        print("argv[{}]: '{}'".format(i, arg))
    for line in sys.stdin:
        print("stdin: '{}'".format(line.rstrip('\n')))
    sys.exit(0)

if __name__ == '__main__':
    main()
```

Skript spustíme následujícím způsobem.

```shell
./ioargs.py *.python "< ioargs.py"
```

Vyberte všechna pravdivá tvrzení o tomto spuštění.
Slovo _výstup_ v následujícím  text odpovídá standardnímu výstupu,
tj. tomu, co bychom viděli na obrazovce po spuštění programu.

1. Spuštění není možné, shell oznámí chybu a nic nespustí.
2. Výstup bude obsahovat `argv[0]: './ioargs.py'`.
3. Výstup bude obsahovat `stdin: '.'`.
4. Výstup bude obsahovat zdroják `ioargs.py` (s `stdin` na
   začátku řádky a v apostrofech).
5. `argv[1]` bude prázdné, protože neexistuje žádný `*.python` soubor.
6. Program bude čekat na vstup od uživatele.
7. Výstup bude obsahovat `argv[1]: '< ioargs.py'`.
8. Standardní vstup bude přesměrován z `ioargs.py`.

Vaše odpověď: **[A2]** 1,6 **[/A2]**



## Otázka Q3

Vyberte všechna pravdivá tvrzení. Zvažte použití manuálových stránek,
pokud potřebujete zjistit, co jednotlivé programy dělají.

1. `uname -r` vytiskne informace o vydání (verzi, release) jádra (kernelu)
2. `date +%C` vytiskne současný rok.
3. `mkdir a && mkdir a` vždy selže (pokud žádný další program neběží).
4. `fold` čte řádky na standardním vstupu a tiskne je v opačném
   pořadí na standardní výstup.
5. `echo -n Hello` vytiskne řetězec `Hello` bez koncového znaku nového řádku.
6. `ls --full-colors` skončí s nulovým exit kódem (předpokládáme
   standardní `ls(1)`).

Vaše odpověď: **[A3]** 3,5,6 **[/A3]**



## Otázka Q4

Vyberte všechna pravdivá tvrzení. V této otázce se zaměřujeme
na Git.

1. `git clone` nahraje nové commity na server
2. `git add` řekne Gitu, že dané soubory budou v dalším commitu.
3. `git log` tiskne informace o existujících commitech.
4. `git status` nemůže fungovat bez připojení k Internetu.
5. `git commit -m "Typo"` vytvoří commit s názvem (commit message) `Typo`
   (tj. commit je vytvořen bez spuštění interaktivního editoru).
6. `git log --format='%s'` vytiskne první řádku názvu commitu
   (předmět, subject) když bude tisknout informace o commitech.

Vaše odpověď: **[A4]** 2,3,5,6 **[/A4]**



