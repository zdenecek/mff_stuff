# Kvíz před cvičením (#7)

Kvíz musí být nahrán do GitLabu před začátkem vašeho cvičení.

Každá otázka je za 25 bodů.

Vaši odpověď vložte do označeného bloku (místo tří teček).



## Otázka Q1

Stručně napište, co pro vás bylo nejnáročnější na předchozím cvičení.


Vaše odpověď: **[A1]** Nejtěžší bylo vypracovat úlohy **[/A1]**



## Otázka Q2

Předpokládejme, že máme soubor `test.txt`, pro který `ls -l` vypíše následující:

```text
-rw-r----- 1 bjorn ursidae 13 Mar 21 14:54 test.txt
```

Kteří z následujících uživatelů můžou číst obsah souboru?

1. `bjorn` ve skupině `ursidae`
2. `bjorn` ve skupinách `carnivora` a `mammalia`
3. `iorek` ve skupině `ursidae`
4. `iorek` ve skupinách `carnivora` a `mammalia`
5. `root` (superuživatel)
6. všichni

Vaše odpověď: **[A2]** 1,2,3,5 **[/A2]**



## Otázka Q3

Předpokládejme, že soubor z předchozího příkladu je uložený v adresáři
`/data` s následujícími oprávněními vypsanými pomocí `ls -l`:

```text
rwxrwx-wx 3 bjorn ursidae 4096 Mar 21 14:53 data
```

Kteří z následujících uživatelů můžou soubor smazat?

1. `bjorn` ve skupině `ursidae`
2. `bjorn` ve skupinách `carnivora` a `mammalia`
3. `iorek` ve skupině `ursidae`
4. `iorek` ve skupinách `carnivora` a `mammalia`
5. `root` (superuživatel)
6. všichni

Můžete předpokládat, že kořenový adresář `/` může číst kdokoliv
a přistupovat do něj také.

Vaše odpověď: **[A3]** 1,2,3,4,5,6 **[/A3]**



## Otázka Q4

V návaznosti na předchozí otázky: Které příkazy můžeme použít, abychom
soubor `test.txt` učinili přístupný pro čtení a zápis pouze jeho vlastníkovi
a nikomu jinému?

1. `chmod u=rw test.txt`
2. `chmod =rw test.txt`
3. `chmod g= test.txt`
4. `chmod o= test.txt`
5. `chmod g=,o= test.txt`
6. `chmod g-r test.txt`
7. `chmod g-rwx test.txt`

Vaše odpověď: **[A4]** 3,4,5,6,7 **[/A4]** 6 7 nevím



