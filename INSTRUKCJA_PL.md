===============================================================================
RXB v0.3.0 - Pelna instrukcja na Windows
===============================================================================

SPIS TRESCI:
1. Instalacja podstawowa
2. Konfiguracja zapory Windows (firewall)
3. Instalacja Tor i tworzenie Hidden Service
4. Tworzenie pliku konfiguracyjnego
5. Tworzenie wlasnego seed node onion
6. Hasla i bezpieczenstwo
7. Tworzenie mostow clearnet-tor
8. Wsparcie sieci RXB
9. Rozwiazywanie problemow

===============================================================================
1. INSTALACJA PODSTAWOWA
===============================================================================

OPCJA A: INSTALATOR (zalecane)
Plik: RXB-core -0.3.0-win64-setup-unsigned.exe (17 MB)

Kroki:
1. Pobierz plik instalatora
2. Kliknij prawym przyciskiem -> "Uruchom jako administrator"
3. Kliknij "Next" -> "Install" -> "Finish"
4. Ikona pojawi sie na pulpicie

OPCJA B: PORTABLE
Plik: rxb-0.3.0-win64-portable.zip (49 MB)

Kroki:
1. Pobierz plik ZIP
2. Kliknij prawym -> "Wypakuj wszystko"
3. Otwórz folder "release"
4. Uruchom: rxb-qt.exe

PIERWSZE URUCHOMIENIE:
- Program utworzy nowy portfel automatycznie
- Ustaw SILNE haslo portfela
- Zapisz haslo w bezpiecznym miejscu (NIE DA SIE ODZYSKAC!)
- Synchronizacja blockchainu rozpocznie sie automatycznie (10-60 min)
Uwaga zalecane jest po uruchominiu utworznie własnego portfela i utworzenie adresów w tym odbiorczych.


===============================================================================
2. KONFIGURACJA ZAPORY WINDOWS (FIREWALL)
===============================================================================

RXB uzywa portu 8327 dla komunikacji P2P z innymi wezlami.

KONFIGURACJA KROK PO KROKU:

Krok 1: Otworz Zapore Windows
- Nacisnij klawisz Windows
- Wpisz: "Zapora Windows Defender"
- Kliknij "Zapora Windows Defender w trybie zaawansowanym"

Krok 2: Utworz regule przychodzaca (Inbound)
- W lewym panelu kliknij: "Reguly ruchu przychodzacego"
- W prawym panelu kliknij: "Nowa regula"
- Wybierz: "Port" -> Dalej
- Wybierz: "TCP"
- Wybierz: "Okreslone porty lokalne"
- Wpisz: 8327
- Kliknij: Dalej
- Wybierz: "Zezwol na polaczenie"
- Kliknij: Dalej
- Zaznacz wszystkie profile (Domena, Prywatna, Publiczna)
- Kliknij: Dalej
- Nazwa: "RXB P2P Port 8327"
- Opis: "Port dla wezla RXB"
- Kliknij: Zakoncz

Krok 3: Utworz regule wychodzaca (Outbound)
- W lewym panelu kliknij: "Reguly ruchu wychodzacego"
- Powtórz kroki 2 z powyzszego
- Nazwa: "RXB P2P Outbound 8327"

Krok 4: Sprawdz router (opcjonalnie, dla pelnego wezla)
- Zaloguj sie do panelu routera (zwykle 192.168.1.1 lub 192.168.0.1)
- Znajdz sekcje "Port Forwarding" lub "Przekierowanie portow"
- Dodaj regule:
  - Port zewnetrzny: 8327
  - Port wewnetrzny: 8327
  - Protokol: TCP
  - IP urzadzenia: [twoje lokalne IP, np. 192.168.1.100]
- Zapisz zmiany


===============================================================================
3. INSTALACJA TOR I TWORZENIE HIDDEN SERVICE
===============================================================================

INSTALACJA TOR BROWSER:

Krok 1: Pobierz Tor Browser
- Wejdz na: https://www.torproject.org
- Pobierz "Tor Browser for Windows"
- Zainstaluj normalnie

Krok 2: Uruchom Tor Browser
- Uruchom Tor Browser
- Kliknij "Connect"
- Poczekaj na polaczenie z siecia Tor
- ZOSTAW TOR BROWSER WLACZONY (musi dzialac w tle)

TWORZENIE WLASNEGO SEED NODE ONION:

Krok 1: Instalacja Tor Expert Bundle (dla zaawansowanych)
- Wejdz na: https://www.torproject.org/download/tor/
- Pobierz "Expert Bundle for Windows"
- Wypakuj do folderu np: C:\Tor

Krok 2: Konfiguracja Tor jako uslugi
- Otwórz Notatnik jako administrator
- Utworz plik: C:\Tor\torrc
- Wklej nastepujaca konfiguracje:

SOCKSPort 9050
ControlPort 9051
HiddenServiceDir C:\Tor\rxb-hidden-service
HiddenServicePort 8327 127.0.0.1:8327

- Zapisz plik

Krok 3: Uruchom Tor
- Otwórz CMD jako administrator
- Przejdz do folderu: cd C:\Tor
- Uruchom: tor.exe -f torrc
- Poczekaj na komunikat: "Bootstrapped 100%: Done"

Krok 4: Odczytaj swoj adres onion
- Otwórz plik: C:\Tor\rxb-hidden-service\hostname
- Skopiuj adres (przyklad: abc123xyz456.onion)
- TO JEST TWOJ SEED NODE ONION!

Krok 5: Dodaj swoj seed node do konfiguracji
- Zobacz sekcje 4 ponizej


===============================================================================
4. TWORZENIE PLIKU KONFIGURACYJNEGO
===============================================================================

LOKALIZACJA PLIKU KONFIGURACYJNEGO:
C:\Users\[TWOJA_NAZWA]\AppData\Roaming\RXB\rxb.conf

TWORZENIE PLIKU:

Krok 1: Otwórz Eksplorator plikow
- Nacisnij Windows + R
- Wpisz: %APPDATA%\RXB
- Kliknij OK

Krok 2: Utworz nowy plik
- Kliknij prawym -> Nowy -> Dokument tekstowy
- Nazwij: rxb.conf (UWAGA: usun .txt z konca!)

Krok 3: Edytuj plik konfiguracyjny
Otwórz plik i wklej przykladowa konfiguracje:

# Podstawowa konfiguracja RXB

# Port P2P (domyslnie 8327)
port=8327

# Maksymalna liczba polaczen
maxconnections=125

# Tor proxy (jezeli uzywa Tor)
proxy=127.0.0.1:9050
listen=1
bind=127.0.0.1

# Twoj wlasny seed node onion (jezeli utworzyles)
# externalip=[TWOJ_ADRES].onion:8327

# Dodatkowe seed nodes (opcjonalnie)
# addnode=m4gcv7eaorwu5nsmflfmeo6whrh5gu3wwkxfsanzs7evqaotolf4ksyd.onion:8327

# RPC (dla zaawansowanych)
server=1
rpcuser=rxbuser
rpcpassword=[Wstaw tutaj twoje BEZPIECZNE_HASLO]
rpcallowip=127.0.0.1
rpcport=8328

# Opcjonalnie: ograniczenie uzycia dysku (pruning)
# prune=5000

Zapisz plik i zrestartuj wezel.


===============================================================================
5. TWORZENIE WLASNEGO SEED NODE ONION
===============================================================================

WYMAGANIA:
- Tor Expert Bundle zainstalowany (patrz sekcja 3)
- Port 8327 otwarty w zaporze (patrz sekcja 2)
- Stabilne polaczenie internetowe
- Komputer wlaczony 24/7 (zalecane)

KONFIGURACJA SEED NODE:

Krok 1: Upewnij sie ze Tor dziala
- Sprawdz czy tor.exe jest uruchomiony
- Sprawdz plik: C:\Tor\rxb-hidden-service\hostname
- Zapisz swoj adres .onion

Krok 2: Skonfiguruj rxb.conf
- Dodaj do pliku konfiguracyjnego:

# Seed node configuration
externalip=[TWOJ_ADRES].onion:8327
onion=127.0.0.1:9050
onlynet=onion
listen=1
discover=1

Krok 3: Uruchom wezel
- Uruchom rxbd.exe lub rxb-qt.exe
- Wezel automatycznie polacz sie z siecia przez Tor

Krok 4: Sprawdz status
- Otwórz CMD
- Przejdz do folderu instalacji RXB
- Uruchom: rxb-cli.exe getnetworkinfo
- Sprawdz czy "localaddresses" zawiera twoj adres .onion


===============================================================================
6. HASLA I BEZPIECZENSTWO
===============================================================================

ZASADY TWORZENIA BEZPIECZNEGO HASLA:

DO PORTFELA (wallet):
- Minimum 16 znakow
- Wielkie i male litery
- Cyfry
- Znaki specjalne dozwolone: !@#$%^&*()_+=[]{}

Przyklad: MyBr1xW@ll3t2025!Secure

DO KONFIGURACJI (rpcpassword):
Przy tworzeniu hasla do konfiguracji wezla mozesz uzywac znakow specjalnych 
i podkreslnikow. Jednak jezeli zauwazyc, ze znaki specjalne lub podkreslniki 
w haslach powoduja problemy podczas konfiguracji lub dzialania wezla, zalecamy 
utworzenie hasla skladajacego sie wylacznie z liter i cyfr. Takie proste hasla 
unikaja potencjalnych problemow z kompatybilnoscia w roznych konfiguracjach 
systemowych.

- Uzyj: liter i cyfr
- Opcjonalnie: podkreslnik _ (jezeli nie powoduje problemow)
- Minimum 20 znakow

Przyklad: RXBRPCPass2025SecureKey12345
Lub z podkreslnikami: RXBRPC_Pass_2025_Secure_Key_12345

GENEROWANIE BEZPIECZNEGO HASLA:
1. Uzyj generatora hasel online (np. passwordsgenerator.net)
2. Dlugossc: 20-30 znakow
3. Wlacz: wielkie litery, male litery, cyfry
4. Dla rpcpassword: wylacz znaki specjalne jezeli powoduja problemy
5. Zapisz haslo w menedzerze hasel (np. KeePass, Bitwarden)

BACKUP PORTFELA:
1. Zamknij RXB
2. Przejdz do: %APPDATA%\RXB
3. Skopiuj plik wallet.dat
4. Zapisz na dysku zewnetrznym, pendrive lub w chmurze (zaszyfrowanym!)
5. Rób backup co miesiac lub po kazdej waznej transakcji


===============================================================================
7. TWORZENIE MOSTOW CLEARNET-TOR
===============================================================================

CZYM JEST MOST?
Most (bridge) to wezel ktory laczy urzadzenia bez Tor (clearnet) 
z wezlami dzialajacymi przez Tor (onion). Mosty sa kluczowe 
dla dzialania sieci RXB.

JAK UTWORZYC MOST?

Wymagania:
- Publiczne IP (statyczne lub dynamiczne DNS)
- Port 8327 otwarty w routerze (port forwarding)
- Tor zainstalowany i skonfigurowany
- Stabilne polaczenie internetowe

KONFIGURACJA MOSTU:

Krok 1: Skonfiguruj Tor (patrz sekcja 3)

Krok 2: Skonfiguruj rxb.conf jako most:

# Most clearnet-tor configuration

# Nasluchuj na clearnet
bind=0.0.0.0:8327
listen=1

# Polacz z Tor
proxy=127.0.0.1:9050
onion=127.0.0.1:9050

# Zezwol na wszystkie sieci
onlynet=ipv4
onlynet=onion

# Twoj publiczny adres (jezeli masz statyczne IP)
# externalip=[TWOJE_PUBLICZNE_IP]:8327

# Lub uzyj domeny (jezeli masz DynamicDNS)
# externalip=twojwezel.ddns.net:8327

# Maksymalna liczba polaczen
maxconnections=125

Krok 3: Skonfiguruj DynamicDNS (opcjonalnie)
- Zarejestruj sie na No-IP.com lub DuckDNS.org (darmowe)
- Utworz domen np: twojadomenanetwork.ddns.net
- Zainstaluj klient DynamicDNS aby aktualizowac IP
- Dodaj do rxb.conf: externalip=twojadomenanetwork.ddns.net:8327

Krok 4: Przetestuj most
- Uruchom wezel
- Poczekaj 30 minut
- Sprawdz logi czy pojawiaja sie polaczenia przychodzace


===============================================================================
8. WSPARCIE SIECI RXB
===============================================================================

Siec RXB jest zdecentralizowana i dziala dzieki jej uzytkownikom.
Mozesz znaczaco wspomoc siec i zwiekszyc jej sile na nastepujace sposoby:

URUCHAMIANIE WEZLA:
- Kazdy aktywny wezel zwieksza bezpieczenstwo sieci
- Im wiecej wezlow, tym siec jest bardziej odporna
- Jezeli mozesz, pozostaw wezel wlaczony 24/7

TWORZENIE SEED NODE ONION:
- Wezly onion zapewniaja prywatnosc i anonimowość
- Jezeli masz stabilne polaczenie, utworz wlasny seed node (patrz sekcja 5)
- Kazdy seed node pomaga nowym uzrytkownikom polaczyc sie z siecia

TWORZENIE MOSTOW:
- Mosty stawiane do polaczenia urzytkownikow clearnet z siecia Tor
- Sa absolutnie kluczowe dla dzialania sieci
- Jezeli masz publiczne IP lub jezeli masz vps rozwaz utworzenie mostu (patrz sekcja 7)
- Mosty sa najbardziej wartosciowym wkladem w siec

UTRZYMYWANIE PELNEGO WEZLA:
- Pelny wezel przechowuje caly blockchain
- Pomaga nowym uzrytkownikom synchronizowac sie
- Wymaga ~50 GB miejsca na dysku
- W rxb.conf NIE dodawaj: prune=5000

JAK MOZESZ POMOC:
1. Uruchom wezel i pozostaw go wlaczonym
2. Jezeli masz Tor - utworz seed node onion
3. Jezeli masz publiczne IP - utworz most clearnet-tor
4. Jezeli masz VPS uzyj go aby laczyl sie z twoim wezlem i upublicznij ip vps aby vps byl przekaznikiem w sieci i 
mostem sieci 
5. Jezeli masz duzo miejsca - uruchom pelny wezel (bez pruning)
6. Pomoz innym uzrytkownikom w spolecznosci
7. Zalecane posiadanie dysku co najmniej 1 TB nawet pomimo ze mniejsze tez sie nadaja.

   KAZDY WEZEL SIE LICZY!

Nawet jezeli mozesz utrzymywac wezel tylko kilka godzin dziennie,
juz to pomaga sieci. 
Nie potrzebujesz zadnych specjalnych umiejetnosci
ani drogiego sprzetu. 
Twoj wklad jest cenny!


===============================================================================
9. ROZWIAZYWANIE PROBLEMOW
===============================================================================

Problem: Windows blokuje uruchomienie
Rozwiazanie:
1. Kliknij prawym przyciskiem na plik .exe
2. Wybierz "Wlasciwosci"
3. W zakladce "Ogolne" zaznacz "Odblokuj"
4. Kliknij "Zastosuj" i "OK"
5. Uruchom ponownie

Problem: Antywirus blokuje program
Rozwiazanie:
1. Dodaj folder RXB do wyjatkow antywirusa
2. Chwilowo wylacz antywirus podczas instalacji
3. Po instalacji wlacz antywirus ponownie

Problem: Portfel nie synchronizuje sie
Rozwiazanie:
1. Sprawdz polaczenie internetowe
2. Sprawdz czy port 8327 jest otwarty (patrz sekcja 2)
3. Poczekaj dluzej (moze trwac do 60 minut przy pierwszym uruchomieniu)
4. Zrestartuj program
5. Sprawdz logi: %APPDATA%\RXB\debug.log

Problem: Tor nie laczy sie
Rozwiazanie:
1. Upewnij sie ze Tor Browser jest uruchomiony PRZED uruchomieniem RXB
2. Sprawdz czy w rxb.conf jest: proxy=127.0.0.1:9050
3. Sprawdz czy Tor dziala: otwórz https://check.torproject.org w Tor Browser
4. Zrestartuj Tor Browser i RXB

Problem: Nie moge utworzyc seed node onion
Rozwiazanie:
1. Sprawdz czy tor.exe jest uruchomiony
2. Sprawdz czy plik torrc jest poprawnie skonfigurowany
3. Sprawdz logi Tor: C:\Tor\tor.log
4. Upewnij sie ze folder C:\Tor\rxb-hidden-service zostal utworzony

Problem: Most nie przyjmuje polaczen
Rozwiazanie:
1. Sprawdz czy port 8327 jest otwarty w routerze (port forwarding)
2. Sprawdz publiczne IP: https://whatismyipaddress.com
3. Dodaj do rxb.conf: externalip=[TWOJE_IP]:8327
4. Poczekaj 30-60 minut az wezel zostanie rozpoznany w sieci

Problem: Zapomnialem hasla portfela
Rozwiazanie:
Niestety nie ma mozliwosci odzyskania hasla. 
Jezeli nie masz kopii zapasowej
pliku wallet.dat, dostep do monet jest utracony na zawsze.
ZAWSZE ROB BACKUP PORTFELA!


===============================================================================
LOKALIZACJA WAZNYCH PLIKOW
===============================================================================

Folder danych:
C:\Users\[TWOJA_NAZWA]\AppData\Roaming\RXB\

Wazne pliki:
- wallet.dat           - Portfel (ROB BACKUP!)
- rxb.conf     - Konfiguracja
- debug.log            - Logi (pomocne przy problemach)
- peers.dat            - Lista znanych wezlow
- blocks\              - Blockchain
- chainstate\          - Stan lancucha


===============================================================================
INFORMACJE O PROJEKCIE
===============================================================================

Strona internetowa: https://rxbcore.org

Wersja: 0.3.0 Eksperymentalna to oznacznie experymntalna jest spowodownae ze jest to pierwsza wersja.
Data wydania: Styczen 2025
Technologia: RXB wykorzystuje biblioteki i strukture Bitcoin Core 20.2
Siec: Mainnet


===============================================================================
WYMAGANIA SYSTEMOWE
===============================================================================

Minimalne:
- System: Windows 7 SP1 (64-bit) lub nowszy
- Procesor: Dual-core 2.0 GHz
- RAM: 2 GB
- Dysk: 10 GB wolnego miejsca
- Internet: Polaczenie szerokopasmowe

Zalecane:
- System: Windows 10/11 (64-bit)
- Procesor: Quad-core 2.5 GHz lub lepszy
- RAM: 4 GB lub wiecej
- Dysk: SSD z 50 GB wolnego miejsca
- Internet: Polaczenie szerokopasmowe (bez limitu danych)

Dla mostow i seed nodes:
- Stabilne polaczenie 24/7
- Publiczne IP lub DynamicDNS
- Brak limitu transferu danych
- Minimum 100 GB wolnego miejsca


===============================================================================
LICENCJA I ZASTRZEZENIA
===============================================================================

RXB jest oprogramowaniem open source wydanym na licencji MIT.

ZASTRZEZENIE:
Oprogramowanie jest dostarczane "TAK JAK JEST", bez jakichkolwiek gwarancji.
Autor nie ponosi odpowiedzialnosci za jakiekolwiek straty finansowe
lub szkody wynikajace z uzytkowania tego oprogramowania.

ZAWSZE:
- Rób backup portfela
- Uzyj silnego hasla
- Nie udostepniaj swojego hasla nikomu
- Nie instaluj podejrzanego oprogramowania
- Sprawdzaj sumy kontrolne pobieranych plikow
- Nigdy ale to naprawde nigdy nie ujawniaj komukolwiek twoich kluczy prywatnych z portfela !

===============================================================================
SUMY KONTROLNE (CHECKSUMS)
===============================================================================

Przed uruchomieniem sprawdz czy pobrany plik jest autentyczny.
w przypadku wersji eksperymnetalnej mozliwe braki podpisow windows. 
Zawsze pobieraj tylko z oryginalnych RXB repo lub z rxbcore.org 
MD5:
[wygeneruj: md5sum *.exe *.zip]

SHA256:
[wygeneruj: sha256sum *.exe *.zip]

Jak sprawdzic sume kontrolna w Windows:
1. Otwórz PowerShell
2. Wpisz: Get-FileHash .\rxb-0.3.0-win64-setup-unsigned.exe
3. Porownaj wynik z powyzszym


===============================================================================

Dziekujemy za uzywanie RXB!

Szczesliwego kopania! 
Twoj wklad w utrzymywanie uruchomionych wezlow w tle jest nieoceniony. 
Kazdy wezel wzmacnia siec. 
Szczególnie wartosciowym wkladem jest uruchamianie wezlow mostowych, które lacza clearnet z siecia 
onion scalajac i jednoczac caly ekosystem RXB.

===============================================================================
