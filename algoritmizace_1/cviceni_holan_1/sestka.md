�estka - �e�en�
1.	seznam nejvzd�len�j��ch konfigurac�
4 5 0 1 2 3
2.	po�et tah� pot�ebn�ch k jejich dosa�en�
21
3.	popis, jak jste sv� �e�en� na�li.
Vzhledem k rozsahu �lohy nen� probl�m proj�t ve�ker� kombinace. 
Prohled�n�m do ���ky zajist�me, �e ka�d� nov� nalezen� stav je nejrychleji dosa�iteln� pou�it�m postupem.
�e�en� jsem implementoval podobn� jako u p�el�v�n� vody, uchov�v�me po�et tah� a n�jakou hashovou strukturu (dict) s ji� dosa�en�mi stavy, ka�d� stav zpracujeme tak, �e nejd��ve zaznamen�me stav do struktury, pak vytvo��me v�echny mo�n� soused�c� stavy a ty je�t� nenav�t�ven� p�id�me do fronty, stavy zpracov�v�me postupn� odeb�r�n�m z fronty, na za��tku do fronty p�id�me v�choz� stav. 
P�ikl�d�m implementaci v pythonu, postupn� jsem implementoval dvojrozm�rn� pole, frontu, pak samotnou tabulku a funkce main spolu s funkc� solve prov�d� samotn� prohled�v�n� do ���ky.
V�echny tahy (X = 0) 
1 2 3 4 5 X : 0
1 2 X 4 5 3 : 1
1 2 3 4 X 5 : 1
1 X 2 4 5 3 : 2
1 X 3 4 2 5 : 2
1 2 3 X 4 5 : 2
1 5 2 4 X 3 : 3
X 1 2 4 5 3 : 3
X 1 3 4 2 5 : 3
1 3 X 4 2 5 : 3
X 2 3 1 4 5 : 3
1 5 2 X 4 3 : 4
1 5 2 4 3 X : 4
4 1 2 X 5 3 : 4
4 1 3 X 2 5 : 4
1 3 5 4 2 X : 4
2 X 3 1 4 5 : 4
X 5 2 1 4 3 : 5
1 5 X 4 3 2 : 5
4 1 2 5 X 3 : 5
4 1 3 2 X 5 : 5
1 3 5 4 X 2 : 5
2 4 3 1 X 5 : 5
2 3 X 1 4 5 : 5
5 X 2 1 4 3 : 6
1 X 5 4 3 2 : 6
4 X 2 5 1 3 : 6
4 1 2 5 3 X : 6
4 X 3 2 1 5 : 6
4 1 3 2 5 X : 6
1 3 5 X 4 2 : 6
2 4 3 X 1 5 : 6
2 4 3 1 5 X : 6
2 3 5 1 4 X : 6
5 4 2 1 X 3 : 7
5 2 X 1 4 3 : 7
X 1 5 4 3 2 : 7
X 4 2 5 1 3 : 7
4 2 X 5 1 3 : 7
4 1 X 5 3 2 : 7
X 4 3 2 1 5 : 7
4 3 X 2 1 5 : 7
4 1 X 2 5 3 : 7
X 3 5 1 4 2 : 7
2 4 X 1 5 3 : 7
2 3 5 1 X 4 : 7
5 4 2 X 1 3 : 8
5 4 2 1 3 X : 8
5 2 3 1 4 X : 8
4 1 5 X 3 2 : 8
4 2 3 5 1 X : 8
4 X 1 5 3 2 : 8
4 3 5 2 1 X : 8
4 X 1 2 5 3 : 8
3 X 5 1 4 2 : 8
2 X 4 1 5 3 : 8
2 X 5 1 3 4 : 8
2 3 5 X 1 4 : 8
5 4 X 1 3 2 : 9
5 2 3 1 X 4 : 9
4 1 5 3 X 2 : 9
4 2 3 5 X 1 : 9
4 3 1 5 X 2 : 9
X 4 1 5 3 2 : 9
4 3 5 2 X 1 : 9
4 5 1 2 X 3 : 9
X 4 1 2 5 3 : 9
3 4 5 1 X 2 : 9
3 5 X 1 4 2 : 9
2 5 4 1 X 3 : 9
X 2 4 1 5 3 : 9
X 2 5 1 3 4 : 9
2 5 X 1 3 4 : 9
X 3 5 2 1 4 : 9
5 X 4 1 3 2 : 10
5 X 3 1 2 4 : 10
5 2 3 X 1 4 : 10
4 X 5 3 1 2 : 10
4 1 5 3 2 X : 10
4 X 3 5 2 1 : 10
4 2 3 X 5 1 : 10
4 3 1 X 5 2 : 10
4 3 1 5 2 X : 10
5 4 1 X 3 2 : 10
4 X 5 2 3 1 : 10
4 3 5 X 2 1 : 10
4 5 1 X 2 3 : 10
4 5 1 2 3 X : 10
2 4 1 X 5 3 : 10
3 4 5 X 1 2 : 10
3 4 5 1 2 X : 10
3 5 2 1 4 X : 10
2 5 4 X 1 3 : 10
2 5 4 1 3 X : 10
1 2 4 X 5 3 : 10
1 2 5 X 3 4 : 10
3 X 5 2 1 4 : 10
5 3 4 1 X 2 : 11
X 5 4 1 3 2 : 11
X 5 3 1 2 4 : 11
5 3 X 1 2 4 : 11
X 2 3 5 1 4 : 11
X 4 5 3 1 2 : 11
4 5 X 3 1 2 : 11
4 1 X 3 2 5 : 11
X 4 3 5 2 1 : 11
4 3 X 5 2 1 : 11
X 2 3 4 5 1 : 11
X 3 1 4 5 2 : 11
5 4 1 3 X 2 : 11
X 4 5 2 3 1 : 11
4 5 X 2 3 1 : 11
X 3 5 4 2 1 : 11
X 5 1 4 2 3 : 11
2 4 1 5 X 3 : 11
3 4 X 1 2 5 : 11
3 5 2 1 X 4 : 11
X 5 4 2 1 3 : 11
1 2 4 5 X 3 : 11
1 2 5 3 X 4 : 11
3 1 5 2 X 4 : 11
3 5 X 2 1 4 : 11
5 3 4 X 1 2 : 12
5 3 4 1 2 X : 12
1 5 4 X 3 2 : 12
1 5 3 X 2 4 : 12
2 X 3 5 1 4 : 12
4 5 2 3 1 X : 12
4 X 1 3 2 5 : 12
5 4 3 X 2 1 : 12
2 X 3 4 5 1 : 12
3 X 1 4 5 2 : 12
5 X 1 3 4 2 : 12
5 4 1 3 2 X : 12
2 4 5 X 3 1 : 12
3 X 5 4 2 1 : 12
5 X 1 4 2 3 : 12
2 X 1 5 4 3 : 12
2 4 1 5 3 X : 12
3 X 4 1 2 5 : 12
3 X 2 1 5 4 : 12
3 5 2 X 1 4 : 12
5 X 4 2 1 3 : 12
1 X 4 5 2 3 : 12
1 2 4 5 3 X : 12
1 X 5 3 2 4 : 12
1 2 5 3 4 X : 12
3 1 5 X 2 4 : 12
3 1 5 2 4 X : 12
3 5 4 2 1 X : 12
X 3 4 5 1 2 : 13
1 5 4 3 X 2 : 13
1 5 3 2 X 4 : 13
2 1 3 5 X 4 : 13
2 3 X 5 1 4 : 13
4 5 2 3 X 1 : 13
4 2 1 3 X 5 : 13
X 4 1 3 2 5 : 13
5 4 3 2 X 1 : 13
2 5 3 4 X 1 : 13
2 3 X 4 5 1 : 13
3 5 1 4 X 2 : 13
3 1 X 4 5 2 : 13
X 5 1 3 4 2 : 13
5 1 X 3 4 2 : 13
5 4 X 3 2 1 : 13
2 4 5 3 X 1 : 13
3 2 5 4 X 1 : 13
3 5 X 4 2 1 : 13
5 2 1 4 X 3 : 13
5 1 X 4 2 3 : 13
X 2 1 5 4 3 : 13
2 1 X 5 4 3 : 13
2 4 X 5 3 1 : 13
3 2 4 1 X 5 : 13
X 3 4 1 2 5 : 13
X 3 2 1 5 4 : 13
3 2 X 1 5 4 : 13
X 5 2 3 1 4 : 13
5 1 4 2 X 3 : 13
5 4 X 2 1 3 : 13
X 1 4 5 2 3 : 13
1 4 X 5 2 3 : 13
1 2 X 5 3 4 : 13
X 1 5 3 2 4 : 13
1 5 X 3 2 4 : 13
1 2 X 3 4 5 : 13
3 1 X 2 4 5 : 13
3 5 4 2 X 1 : 13
3 X 4 5 1 2 : 14
1 X 4 3 5 2 : 14
1 5 4 3 2 X : 14
1 X 3 2 5 4 : 14
1 5 3 2 4 X : 14
2 1 3 X 5 4 : 14
2 1 3 5 4 X : 14
2 3 4 5 1 X : 14
4 X 2 3 5 1 : 14
4 5 2 X 3 1 : 14
4 2 1 X 3 5 : 14
4 2 1 3 5 X : 14
3 4 1 X 2 5 : 14
5 X 3 2 4 1 : 14
5 4 3 2 1 X : 14
2 5 3 X 4 1 : 14
2 5 3 4 1 X : 14
2 3 1 4 5 X : 14
3 5 1 X 4 2 : 14
3 5 1 4 2 X : 14
3 1 2 4 5 X : 14
5 1 2 3 4 X : 14
5 X 4 3 2 1 : 14
2 X 5 3 4 1 : 14
2 4 5 3 1 X : 14
3 2 5 X 4 1 : 14
3 2 5 4 1 X : 14
5 2 1 X 4 3 : 14
5 2 1 4 3 X : 14
5 1 3 4 2 X : 14
2 X 4 5 3 1 : 14
3 2 4 X 1 5 : 14
3 2 4 1 5 X : 14
1 3 4 X 2 5 : 14
1 3 2 X 5 4 : 14
5 X 2 3 1 4 : 14
5 1 4 X 2 3 : 14
5 1 4 2 3 X : 14
1 4 3 5 2 X : 14
1 X 2 5 3 4 : 14
1 X 2 3 4 5 : 14
3 X 1 2 4 5 : 14
3 X 4 2 5 1 : 14
3 5 4 X 2 1 : 14
3 1 4 5 X 2 : 15
3 4 X 5 1 2 : 15
X 1 4 3 5 2 : 15
1 4 X 3 5 2 : 15
X 1 3 2 5 4 : 15
1 3 X 2 5 4 : 15
1 5 X 2 4 3 : 15
2 3 4 5 X 1 : 15
X 4 2 3 5 1 : 15
4 2 X 3 5 1 : 15
X 5 2 4 3 1 : 15
X 2 1 4 3 5 : 15
3 4 1 2 X 5 : 15
X 5 3 2 4 1 : 15
5 3 X 2 4 1 : 15
2 5 X 4 1 3 : 15
2 3 1 4 X 5 : 15
3 1 2 4 X 5 : 15
5 1 2 3 X 4 : 15
5 2 4 3 X 1 : 15
X 5 4 3 2 1 : 15
X 2 5 3 4 1 : 15
2 5 X 3 4 1 : 15
2 4 X 3 1 5 : 15
3 2 X 4 1 5 : 15
5 2 X 4 3 1 : 15
5 1 3 4 X 2 : 15
X 2 4 5 3 1 : 15
X 2 4 3 1 5 : 15
1 3 4 2 X 5 : 15
1 3 2 5 X 4 : 15
5 2 X 3 1 4 : 15
5 1 X 2 3 4 : 15
1 4 3 5 X 2 : 15
X 1 2 5 3 4 : 15
1 4 2 3 X 5 : 15
X 1 2 3 4 5 : 15
X 3 1 2 4 5 : 15
X 3 4 2 5 1 : 15
3 4 X 2 5 1 : 15
3 1 4 X 5 2 : 16
3 1 4 5 2 X : 16
3 4 2 5 1 X : 16
1 4 2 3 5 X : 16
1 3 4 2 5 X : 16
1 X 5 2 4 3 : 16
2 3 4 X 5 1 : 16
3 4 2 X 5 1 : 16
5 X 2 4 3 1 : 16
2 X 1 4 3 5 : 16
3 4 1 2 5 X : 16
5 3 1 2 4 X : 16
2 X 5 4 1 3 : 16
2 3 1 X 4 5 : 16
3 X 2 4 1 5 : 16
3 1 2 X 4 5 : 16
5 1 2 X 3 4 : 16
5 2 4 X 3 1 : 16
5 2 4 3 1 X : 16
2 5 1 3 4 X : 16
2 X 4 3 1 5 : 16
5 X 3 4 1 2 : 16
5 1 3 X 4 2 : 16
1 X 4 2 3 5 : 16
1 3 2 5 4 X : 16
5 X 1 2 3 4 : 16
1 X 3 5 4 2 : 16
1 4 3 X 5 2 : 16
1 4 2 X 3 5 : 16
3 1 X 5 2 4 : 17
3 4 2 5 X 1 : 17
1 4 5 2 X 3 : 17
X 1 5 2 4 3 : 17
5 3 2 4 X 1 : 17
2 1 X 4 3 5 : 17
5 3 1 2 X 4 : 17
2 1 5 4 X 3 : 17
X 2 5 4 1 3 : 17
X 3 2 4 1 5 : 17
2 5 1 3 X 4 : 17
2 1 4 3 X 5 : 17
X 5 3 4 1 2 : 17
5 3 X 4 1 2 : 17
X 1 3 5 4 2 : 17
X 1 4 2 3 5 : 17
1 4 X 2 3 5 : 17
1 3 X 5 4 2 : 17
X 5 1 2 3 4 : 17
X 4 3 1 5 2 : 17
X 4 2 1 3 5 : 17
3 X 1 5 2 4 : 18
3 X 2 5 4 1 : 18
1 4 5 X 2 3 : 18
1 4 5 2 3 X : 18
2 1 5 X 4 3 : 18
5 3 2 X 4 1 : 18
5 3 2 4 1 X : 18
2 1 5 4 3 X : 18
5 3 1 X 2 4 : 18
4 2 5 X 1 3 : 18
4 3 2 X 1 5 : 18
2 X 1 3 5 4 : 18
2 5 1 X 3 4 : 18
2 1 4 X 3 5 : 18
2 1 4 3 5 X : 18
4 5 3 X 1 2 : 18
4 X 3 1 5 2 : 18
4 X 2 1 3 5 : 18
3 2 1 5 X 4 : 19
X 3 1 5 2 4 : 19
X 3 2 5 4 1 : 19
3 2 X 5 4 1 : 19
X 4 5 1 2 3 : 19
4 2 5 1 X 3 : 19
4 3 2 1 X 5 : 19
X 2 1 3 5 4 : 19
2 1 X 3 5 4 : 19
4 5 3 1 X 2 : 19
4 3 X 1 5 2 : 19
4 2 X 1 3 5 : 19
3 2 1 X 5 4 : 20
3 2 1 5 4 X : 20
4 X 5 1 2 3 : 20
4 2 5 1 3 X : 20
4 3 2 1 5 X : 20
4 5 3 1 2 X : 20
4 5 X 1 2 3 : 21