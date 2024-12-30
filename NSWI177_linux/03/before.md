# Kvíz před cvičením (#3)

Kvíz musí být nahrán do GitLabu před začátkem vašeho cvičení.

Každá otázka je za 25 bodů.

Vaši odpověď vložte do označeného bloku (místo tří teček).



## Otázka Q1

Stručně napište, co pro vás bylo nejnáročnější na předchozím cvičení.


Vaše odpověď: **[A1]** Vyplnit tento kvíz, protože jsem to dělal na cvičení, a protože Čtení před cvičením zdaleka nezmiňuje všechny informace potřebné ke zodpovězení. **[/A1]**



## Otázka Q2

Předpokládejme, že soubor `script.py` má shebang `#!/usr/bin/env python3` a uživatel
spustí následující příkaz.

```shell
./script.py alpha.txt
```

Vyberte který příkaz ze seznamu nejlépe odpovídá tomu, co bude skutečně spuštěno.

1. `python3 alpha.txt`
2. `/usr/bin/env python3 script.py alpha.txt`
3. `/usr/bin/env python3 script.py`
4. Nic z výše uvedeného, protože příkaz nedává smysl.
5. Nejde určit, protože to záleží na pracovním (aktuálním) adresáři.

Vaše odpověď: **[A2]** 2 **[/A2]**



## Otázka Q3

Přepokládejme, že soubor `script.php` je napsán v PHP PHP a má shebang
`#!/usr/bin/env php7` a uživatel spustí následující příkaz.

```shell
php3 ./script.php bravo.txt
```

Vyberte _všechna pravdivá tvrzení_ ze sezamu níže (předpokládáme, že interpretry
pro PHP 3 i PHP 7 jsou nainstalovány).

1. Shebang je ignorován.
2. Skript je spuštěn s interpretrem PHP 3.
3. Systém (tj. shell) odmítne skript spustit.
4. Skript je spuštěn s interpretrem PHP 7.
5. Shell se uživatele zeptá, jaký interpretr se má použít (jestli PHP 3 nebo 7).
6. Executable bit souboru `script.php` je zapnut.
7. Skript může selhat, protože byl napsán pro novější verzy PHP.


Vaše odpověď: **[A3]** 1,2,7 **[/A3]**



## Otázka Q4

Vyberte nejlepší popis toho, co dělá příkaz `git add script.sh`.

1. Odešle aktuální stav `script.sh` do GitLabu.
2. Aktuální stav `script.sh` bude součástí dalšího commitu (revize).
3. Aktualizuje soubor `script.sh` změnami z GitLabu.
4. Ukáže historii změn pro soubor `script.sh`.
5. Zkontroluje, že `script.sh` má nastavený executable bit a správný shebang.
6. Spustí skript `script.sh` za pomoci Gitového interpretru.

Vaše odpověď: **[A4]** 2 **[/A4]**



