# Kvíz před cvičením (#11)

Kvíz musí být nahrán do GitLabu před začátkem vašeho cvičení.

Každá otázka je za 25 bodů.

Vaši odpověď vložte do označeného bloku (místo tří teček).



## Otázka Q1

Dejte nám, prosím, stručnou zpětnou vazbu k poslednímu cvičení.
Co bylo nejzajímavější, co bylo nejtěžší nebo libovolný další postřeh
k minulému cvičení. Prosím, odpovídejte rozumně.

Vaše odpověď: **[A1]** Nejtěžší bylo odevzdat vypracování kvízu včas, protože při virtuálním cvičení je deadline již v úterý a lehce se na to zapomíná **[/A1]**



## Otázka Q2

Vyberte z odstavců níže ten, který _nejlépe_ vystihuje důležitost
a užitečnost virtuálních prostředí.


1. Virtuální prostředí umožní uživateli naklonovat projekt z Gitového
   repozitáře aniž by bylo nutné měnit soubory na počítači.
2. Virtuální prostředí umožní uživateli upgradovat závislosti v projektu
   aniž by bylo nutné aktualizovat operační systém.
3. Virtuální prostředí umožní vývojáři testovat program na různých
   operačních systémech.
4. Virtuální prostředí zajistí izolaci a opakovatelnost prostředí beze
   změn do operačního systému.
5. Virtuální prostředí umožní uživateli instalovat závislosti z PyPI
   mnohem jednodušeji.

Vaše odpověď: **[A2]** 4 **[/A2]**



## Otázka Q3

Vyberte z odstavců níže ty, které dávají správné důvody proč nejsou
adresáře s virtuálním prostředím obvykle commitovány do verzovacích
systémů (repozitářů).

Více odpovědí je možných.

1. Protože obsahují soubory specifické pro konkrétní operační systém
   a na jiným by nebyly stejně k ničemu.
2. Protože se obecně necommitují soubory, které lze snadno znovu vytvořit
   nebo stáhnout.
3. Protože Git neumí verzovat adresáře.
4. Protože mohou obsahovat konfigurační soubory s absolutní cestou.
5. Protože počet souborů v takovém adresáři je mnohem větší, než co
   Git dovolí (čili by stejně nebylo možné je všechny commitnout).

Vaše odpověď: **[A3]** 1,2,3 **[/A3]**



## Otázka Q4

Přečtěte si o sémantickém verzování na https://semver.org/ (k dispozici je i
mnoho překladů).

Potom rozhodněte, která z následujících tvrzení jsou pravdivá. Používáme slovo
_vynutit_ abychom řekli, že je _zcela nutné_ zvýšit číslo verze, abychom
neporušili pravidla sémantického verzování. V tomto ohledu oprava špatného
komentáře může způsobit aktualizaci _patch_ verze (pro knihovnu může být
špatný komentář vlastně chybou), ale změna verze často není nutná.

Budeme se bavit v kontextu C knihovny, která nabízí následující funkci
(pro jednoduchost nezobrazujeme další funkce, ale můžeme předpokládat,
že knihovna není jen o jedné funkci).

Když mluvíme o změnách, předpokládáme, že zbytek knihovny se nezměnil.

```c
int factorial(int number) {
    if (number < 0) {
        return -1;
    } else if (number == 0) {
        return 0;
    } else if (number == 1) {
        return 1;
    } else {
        return number * factorial(number - 1);
    }
}
```

1. Odstranění výše zmíněné funkce `factorial` vynutí zvýšení
   čísla _major_ verze.
2. Odstranění výše zmíněné funkce `factorial` vynutí zvýšení
   čísla _minor_ verze.
3. Odstranění výše zmíněné funkce `factorial` vynutí zvýšení
   čísla _patch_ verze.
4. Přidání nové funkce `long factorial_l(long number)` vynutí
   zvýšení čísla _major_ verze.
5. Přidání nové funkce `long factorial_l(long number)` vynutí
   zvýšení čísla _minor_ verze.
6. Přidání nové funkce `long factorial_l(long number)` vynutí
   zvýšení čísla _patch_ verze.
7. Oprava pro hodnotu `0` vynutí zvýšení čísla _major_ verze.
8. Oprava pro hodnotu `0` vynutí zvýšení čísla _minor_ verze.
9. Oprava pro hodnotu `0` vynutí zvýšení čísla _patch_ verze.
10. Změna implementace na použití for-cyklu místo rekurze
    vynutí zvýšení čísla _major_ verze.
11. Změna implementace na použití for-cyklu místo rekurze
    vynutí zvýšení čísla _minor_ verze.
12. Změna implementace na použití for-cyklu místo rekurze
    vynutí zvýšení čísla _patch_ verze.

Vaše odpověď: **[A4]** 1,5,9,12 **[/A4]**



