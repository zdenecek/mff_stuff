Program čte ze vstupu čtveřice celých čísel X, Y, K a L,

-   X a Y v rozsahu 0..49,
-   K a L v rozsahu 1..20,
-   čísla jsou oddělená mezerou, každá čtveřice na novém řádku,
-   vstup končí prázdným řádkem

a vytiskne LES, sestávající z jednotlivých stromů (viz předchozí stromečkové úlohy)  
s rozměry K a L, jejichž levý horní roh je na souřadnicích X a Y.

Počet řádků a sloupců výstupu bude nejmenší takový, do kterého se vejde celý les a zároveň pozice \[0,0\], tedy nebude obsahovat prázdné řádky dole ani prázdné sloupce napravo.

Stromy na vstupu nejsou nijak uspořádány.  
Ve výsledném lese se jednotlivé stromy mohou překrývat.

Použijte funkce!

**Příklad:**

_Vstup:_

0 0 5 5  
9 1 3 1

_Výstup:_

```
....*.........
...***.....*..
..*****...***.
.*******.*****
*********..*..
....*.........
....*.........
....*.........
....*.........
....*.........
```