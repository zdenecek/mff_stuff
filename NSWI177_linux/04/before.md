# Kvíz před cvičením (#4)

Kvíz musí být nahrán do GitLabu před začátkem vašeho cvičení.

Každá otázka je za 25 bodů.

Vaši odpověď vložte do označeného bloku (místo tří teček).



## Otázka Q1

Stručně napište, co pro vás bylo nejnáročnější na předchozím cvičení.


Vaše odpověď: **[A1]** Nejtěžší bylo napsat službu na automatické mountování windows disku, protože je velmi málo zdrojů, které popisují jak to udělat, a jsou často nepřehledné. **[/A1]**



## Otázka Q2

Setkali jsme se s argumenty (`sys.argv`) a standardním vstupem (`sys.stdin`).
Vyberte všechna platná tvrzení:

1. `sys.argv` je ve skutečnosti totéž, co `sys.stdin`.
2. standardní vstup je vždycky z klávesnice.
3. `sys.argv` se obvykle používá ke změně chování programu.
4. `sys.stdin` se obvykle používá pro čtení vstupních dat.
5. `sys.argv[1]` je vždycky jméno souboru, které čteme na standardním vstupu jako `sys.stdin`.
6. Na `sys.stdin` se tisknou chybové zprávy.
7. `sys.argv` se dá použít pro uložení exit kódu programu.

Vaše odpověď: **[A2]** 3,4 **[/A2]**



## Otázka Q3

Přečtěte si následující jednoduchý program v Pythonu.

```
import sys
def usage(output):
    print("Usage: {} NUMBER".format(sys.argv[0]), file=output)
    print("...")
def main():
    if len(sys.argv) != 2:
        usage(sys.stderr)
        sys.exit(2)
    if sys.argv[1] == "--help":
        usage(sys.stdout)
        sys.exit(0)
    try:
        number = int(sys.argv[1])
        if number % 2 == 0:
            sys.exit(0)
        else:
            sys.exit(1)
    except ValueError:
        usage(sys.stderr)
        sys.exit(2)
if __name__ == '__main__':
    main()
```

Vyberte všechna platná tvrzení:

1. Jestliže program nespustíme správně, skončí s exit kódem 2.
2. Je-li zadáno číslo, program rozhodne, zda je liché či sudé, a výsledek uloží do exit kódu.
3. Vstupní číslo se čte ze standardního vstupu.
4. Program za žádných okolností netiskne na standardní výstup.
5. Jestliže program nespustíme správně, vytiskne krátkou nápovědu na stderr.
6. Spuštění programu s `--help` (jako jediným argumentem) vytiskne tuto krátkou nápovědu.
7. Spuštění programu s `--help` (jako jediným argumentem) způsobí, že program skončí s nenulovým exit kódem.
8. Program se nejspíš bude jmenovat `is_even`, nebo nějak podobně.


Vaše odpověď: **[A3]** 1,2,5,6,8 **[/A3]**



## Otázka Q4

Vyberte všechna platná tvrzení o stdout a stderr:

1. Stdout a stderr se chovají jako otevřené (textové) soubory.
2. Stdout a stderr můžou být připojené ke stejnému výstupnímu zařízení, ale logicky jsou odlišné.
3. V Pythonu vždy platí `sys.stdout == sys.stderr`.
4. Aby program zjistil, zda má být stdout přesměrován, musí zkontrolovat, zda je `>` první znak posledního prvku v `sys.argv`.
5. Stdout je přesměrován předtím, než se spustí program.

Vaše odpověď: **[A4]** 1,2,5 **[/A4]**
