Na dnešním domácím úkolu **můžete spolupracovat** ve dvojicích, výsledných max. 10 bodů dostanete buďto sami nebo oba.

Zadání:

1.  podívejte se na příklad z přednášky
    
    [https://ksvi.mff.cuni.cz/~holan/py/obchod.txt](https://ksvi.mff.cuni.cz/~holan/py/obchod.txt)
    
    [https://ksvi.mff.cuni.cz/~holan/py/obchod\_data.txt](https://ksvi.mff.cuni.cz/~holan/py/obchod_data.txt)
    
    a zjistěte, jaká bude **průměrná doba, kterou zákazník stráví v obchodním domě** (hodnota **PDKZSVOD**) při počtu zákazníků 1, 11, 21, 31... až 501.
    
    Upřesnění zadání:
    
    -   budete muset program upravit tak, aby zákazníky generoval náhodně
        
    -   na to použijte náhodný generátor inicializovaný hodnotou 12345
        
        **Random rnd = new Random(12345);**
        
    -   náhodná čísla potom generujte funkcí **rnd.Next(min,max+1)**
        
    -   zákazníky vždy vytvářejte tak, že mají
        
        -   náhodný čas příchodu v rozsahu <0,600>
        -   náhodnou trpělivost v rozsahu <1,180>
        -   náhodný počet nákupů v rozsahu <1,20>
        -   náhodně vygenerované nákupy v daném počtu, kde typy nákupu/oddělení odpovídají pořadí oddělení na vstupu, tedy hodnota 0 je papírnictví, hodnota 1 potraviny atd.
    -   pokud zákazník má více nákupů ve stejném oddělení, musí pokaždé přijít znovu, i kdyby ty nákupy byly bezprostředně za sebou.
        
    -   pro omezení vlivu náhody spočítejte simulaci pro každý počet zákazníků DESETKRÁT a výsledná hodnota bude vždy průměr z těchto deseti meření... (postup výpočtu bude desetkrát pro každou hodnotu a pak teprve další hodnota (to potřebujete vědět kvůli náhodnému generátoru)), ...ze kterých ovšem vyloučíte největší a nejmenší hodnotu (takže ten průměr bude jen Z OSMI hodnot).
        
    -   náhodný generátor vytvořte jenom jednou na začátku (jinak by desetkrát vracel stejná "náhodná" čísla)
        
    
    Své výsledky porovnejte s kolegou a pokud Vám vyšla různá čísla, najděte chybu. (Mně je neposílejte, to je jen k tomu, abyste se ujistili, že Vám to funguje.)
    
2.  Když je počet zákazníků větší, vznikají fronty u oddělení i u výtahů. Zjistěte, co by se stalo, kdyby zákazníci byli tří druhů, podle jejich pořadového čísla P při generování:
    
    **PRVNÍ DRUH** (P%3==1): funguje stejně jako stávající zákazníci
    
    **DRUHÝ DRUH** (P%3==2): má **superschopnost S1**: ze svého seznamu nákupů může přednostně vybírat nákupy v tom patře, ve kterém právě je
    
    **TŘETÍ DRUH** (P%3==0): má **superschopnost S2**: má (od Modelu) informaci, jak jsou dlouhé fronty u jednotlivých oddělení i u výtahu a může si libovolně vybírat, do kterého oddělení půjde teď
    
    ...a zjistěte, jak se v takovém případě změní PDKZSVOD zákazníků jednotlivých druhů.
    
    Výsledky svého zjištení odevzdávejte jako text (**jednu větu**) podpořený čísly a grafy plus zdrojový kód.
    
    Na tomto domácím úkolu můžete spolupracovat s (jedním) svým kolegou, proto jako součást odpovědi napište, **kdo jsou autoři**, abych věděl, komu mám započítat body _(k započtení bodů je potřeba, aby každý odevzdal alespoň něco, třeba jen poznámku, že řešení je u kolegy)_ a také připojte jeden odstavec o tom, jak jste se o práci podělili.