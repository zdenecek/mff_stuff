# Kvíz před cvičením (#10)

Kvíz musí být nahrán do GitLabu před začátkem vašeho cvičení.

Každá otázka je za 25 bodů.

Vaši odpověď vložte do označeného bloku (místo tří teček).



## Otázka Q1

Dejte nám, prosím, stručnou zpětnou vazbu k poslednímu cvičení.
Co bylo nejzajímavější, co bylo nejtěžší nebo libovolný další postřeh
k minulému cvičení. Prosím, odpovídejte rozumně.

Vaše odpověď: **[A1]** Nejtěžší bylo vypracovat úkoly, jiná poznámka mě nenapadá **[/A1]**



## Otázka Q2

Spustili jsme následující příkazy (oba skončily bez chyb).

```
dd if=/dev/zero of=disk.img bs=1024 count=$(( 512 * 64 ))
mkfs.fat disk.img
```

Vyberte, která z odpovědí níže _nejlépe_ vystihuje to, co jsme
udělali. Samozřejmě můžete použít manuálové stránky nebo si
dokonce příkazy sami pustit (pak také zvažte použití prográmku
na zjišťování typů souborů `file(1)`).

1. Smazali jsme soubor `disk.img` v aktuálním adresáři.
2. Vytvořili jsme prázdný soubor plný nul.
3. Vytvořili jsme prázdný obraz disku a zinicializovali ho
   na souborový systém FAT.
4. Vytvořili jsme souborový systém FAT nad `/dev/zero`.
5. Zkopírovali jsme 64 MB fotografií z fotoaparátu připojeného
    do `/dev/zero`.

Vaše odpověď: **[A2]** 3 **[/A2]**



## Otázka Q3

Podívejte se na následující fragment pseudo-Pythoního kódu
jednoduché služby.

```python
def on_signal(signal_number, frame_info):
    if signal_number == signal.SIGTERM:
        terminate = True
        signal_received = True
    elif signal_number == signal.SIGHUP:
        signal_received = True
    else:
        raise Exception("Unexpected signal")
def init():
    signal.signal(signal.SIGTERM, on_signal)
    signal.signal(signal.SIGHUP, on_signal)
    with open('./service.pid', 'w') as f:
        f.write(os.getpid())
def main():
    init()
    while not terminate:
        load_configuration()
        signal_received = False
        for req in get_requests():
            handle_request(req)
            if signal_received:
                break
    cleanup()
```

Vyberte všechna pravdivá tvrzení.

1. Služba (znovu) načte svoji konfiguraci při obdržení signálu SIGHUP.
2. Služba bude korektně (čistě) ukončena po obdržení signálu SIGTERM.
3. Služba bude korektně ukončena po obdržení signálu SIGINT.
4. Není žádný způsob, jak by služba mohla znovu načíst svojí konfiguraci
   (kromě ukončení a spuštění nanovo).
5. Soubor `service.pid` bude obsahovat ID procesu služby.
6. Příkaz `kill -TERM $( cat service.pid )` korektně (čistě) ukončí danou službu.
7. Příkaz `kill -HUP $( echo service.pid )` zařídí, že služba znovu načte svoji
   konfiguraci.
8. Zpracování požadavků může být přerušeno pomocí SIGTERM, ale ukončení
   proběhne až po zpracování aktuálně řešeného požadavku.

Vaše odpověď: **[A3]** 1,2,5,6,8 **[/A3]**



## Otázka Q4

Vyberte _všechna_ pravdivá tvrzení o připojování disků.

1. Kořenový souborový systém `/` musí být připojen jako první.
2. Svazek přimontovaný jako `/home` musí být vždy lokální fyzický disk.
3. `/home` musí být vždy připojen jako samostatný svazek (oddíl).
4. `/proc` funguje jako normální souborový systém, ale obsah souborů
   není nikde přímo uložen (tj. obsah je vytvářen podle potřeba za běhu).
5. Libovolný uživatel může odpojit `/`.
6. V podstatě není rozdíl, jestli uživatel pracuje se soubory připojenými
   ze skutečného disku nebo z obrazu disku.

Vaše odpověď: **[A4]** 1,4,6 **[/A4]**

