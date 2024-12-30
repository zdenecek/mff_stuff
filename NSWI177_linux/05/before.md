# Kvíz před cvičením (#5)

Kvíz musí být nahrán do GitLabu před začátkem vašeho cvičení.

Každá otázka je za 25 bodů.

Vaši odpověď vložte do označeného bloku (místo tří teček).



## Otázka Q1

Stručně napište, co pro vás bylo nejnáročnější na předchozím cvičení.


Vaše odpověď: **[A1]** Nalézt v textu před cvičením odpovědi na otázky kvízu, některé věci mi jsou stále nejasné, ale asi to příliš nevadí.  **[/A1]**



## Otázka Q2

Uvažujme následující fragment shellového skriptu.
Jaký text se objeví na obrazovce?

```shell
COMMAND=echo
MESSAGE=Hello.txt
TARGET=/dev/null
$COMMAND $MESSAGE 2>$TARGET
```

1. Nic.
2. Obsah souboru Hello.txt.
3. Řetězec _Hello.txt_.

Vaše odpověď: **[A2]** 3 **[/A2]**



## Otázka Q3

Uvažujme následující fragment shellového skriptu.
Jaké příkazy budou spuštěny? Můžete předpokládat, že `/etc/` je adresářem.

```shell
if test -d /etc/; then
    head -n 1 /etc/passwd
else
    tail -n 1 /etc/group
fi
```

1. Jenom `head`.
2. Jenom `tail`.
3. `test` a `head`.
4. `test` a `tail`.
5. `test`, `head` a `tail`.
6. Jen `head` a `tail`.

Vaše odpověď: **[A3]** 3 **[/A3]**



## Otázka Q4

Vyberte všechna pravdivá tvrzení o `$PATH`.

1. `$PATH` je proměnná prostředí.
2. Jednotlivé položka `$PATH` jsou oddělené čárkou `,`.
3. Prázdná `$PATH` znamená, že nemůžeme spustit žádný program z shellu
   (ale built-in jako `cd` budou dál fungovat).
4. `/usr/bin/env` prohledává `$PATH` když spouští program.
5. `$PATH` obsahuje adresáře s Gitovými projekty.
6. `$PATH` není prohledávána, pokud je příkaz zadán s cestou, jako např. `../ls`.

Vaše odpověď: **[A4]** 1,4,6 **[/A4]**

 