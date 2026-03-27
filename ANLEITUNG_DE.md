===============================================================================
RXB-core v0.1.0 - Vollständige Anleitung für Windows
===============================================================================

INHALTSVERZEICHNIS:
1. Grundinstallation
2. Windows Firewall-Konfiguration
3. Tor-Installation und Hidden Service-Erstellung
4. Konfigurationsdatei erstellen
5. Eigenen Onion Seed Node erstellen
6. Passwörter und Sicherheit
7. Clearnet-Tor-Brücken erstellen
8. RXBNetwork unterstützen
9. Fehlerbehebung

===============================================================================
1. GRUNDINSTALLATION
===============================================================================

OPTION A: INSTALLATIONSPROGRAMM (empfohlen)
Datei: rxb-0.1.0-win64-setup-unsigned.exe (17 MB)

Schritte:
1. Installationsdatei herunterladen
2. Rechtsklick -> "Als Administrator ausführen"
3. Klicken Sie auf "Weiter" -> "Installieren" -> "Fertig stellen"
4. Symbol erscheint auf dem Desktop

OPTION B: PORTABLE
Datei: rxb-0.1.0-win64-portable.zip (49 MB)

Schritte:
1. ZIP-Datei herunterladen
2. Rechtsklick -> "Alle extrahieren"
3. Öffnen Sie den Ordner "release"
4. Ausführen: rxb-qt.exe

ERSTER START:
- Programm erstellt automatisch eine neue Wallet
- Legen Sie ein STARKES Wallet-Passwort fest
- Speichern Sie das Passwort an einem sicheren Ort (KANN NICHT WIEDERHERGESTELLT WERDEN!)
- Blockchain-Synchronisation startet automatisch (10-60 Min)


===============================================================================
2. WINDOWS FIREWALL-KONFIGURATION
===============================================================================

RXBNetwork verwendet Port 8327 für P2P-Kommunikation mit anderen Nodes.

SCHRITT-FÜR-SCHRITT-KONFIGURATION:

Schritt 1: Windows Firewall öffnen
- Windows-Taste drücken
- Eingeben: "Windows Defender Firewall"
- Klicken Sie auf "Windows Defender Firewall mit erweiterter Sicherheit"

Schritt 2: Eingehende Regel erstellen
- Im linken Bereich klicken: "Eingehende Regeln"
- Im rechten Bereich klicken: "Neue Regel"
- Wählen: "Port" -> Weiter
- Wählen: "TCP"
- Wählen: "Bestimmte lokale Ports"
- Eingeben: 8327
- Klicken: Weiter
- Wählen: "Verbindung zulassen"
- Klicken: Weiter
- Alle Profile markieren (Domäne, Privat, Öffentlich)
- Klicken: Weiter
- Name: "RXBNetwork P2P Port 8327"
- Beschreibung: "Port für RXBNetwork Node"
- Klicken: Fertig stellen

Schritt 3: Ausgehende Regel erstellen
- Im linken Bereich klicken: "Ausgehende Regeln"
- Wiederholen Sie die Schritte von oben
- Name: "RXBNetwork P2P Outbound 8327"

Schritt 4: Router prüfen (optional, für Full Node)
- Melden Sie sich im Router-Panel an (normalerweise 192.168.1.1 oder 192.168.0.1)
- Finden Sie den Abschnitt "Portweiterleitung"
- Regel hinzufügen:
  - Externer Port: 8327
  - Interner Port: 8327
  - Protokoll: TCP
  - Geräte-IP: [Ihre lokale IP, z.B. 192.168.1.100]
- Änderungen speichern


===============================================================================
3. TOR-INSTALLATION UND HIDDEN SERVICE-ERSTELLUNG
===============================================================================

TOR BROWSER-INSTALLATION:

Schritt 1: Tor Browser herunterladen
- Gehen Sie zu: https://www.torproject.org
- Laden Sie "Tor Browser for Windows" herunter
- Normal installieren

Schritt 2: Tor Browser ausführen
- Tor Browser starten
- Klicken Sie auf "Verbinden"
- Warten Sie auf Verbindung zum Tor-Netzwerk
- TOR BROWSER LAUFEN LASSEN (muss im Hintergrund laufen)

EIGENEN ONION SEED NODE ERSTELLEN:

Schritt 1: Tor Expert Bundle installieren (für fortgeschrittene Benutzer)
- Gehen Sie zu: https://www.torproject.org/download/tor/
- Laden Sie "Expert Bundle for Windows" herunter
- Extrahieren Sie in einen Ordner z.B.: C:\Tor

Schritt 2: Tor als Dienst konfigurieren
- Öffnen Sie Notepad als Administrator
- Erstellen Sie die Datei: C:\Tor\torrc
- Fügen Sie folgende Konfiguration ein:

SOCKSPort 9050
ControlPort 9051
HiddenServiceDir C:\Tor\rxb-hidden-service
HiddenServicePort 8327 127.0.0.1:8327

- Datei speichern

Schritt 3: Tor ausführen
- Öffnen Sie CMD als Administrator
- Navigieren Sie zum Ordner: cd C:\Tor
- Ausführen: tor.exe -f torrc
- Warten Sie auf die Nachricht: "Bootstrapped 100%: Done"

Schritt 4: Ihre Onion-Adresse auslesen
- Öffnen Sie die Datei: C:\Tor\rxb-hidden-service\hostname
- Kopieren Sie die Adresse (Beispiel: abc123xyz456.onion)
- DAS IST IHR SEED NODE ONION!

Schritt 5: Ihren Seed Node zur Konfiguration hinzufügen
- Siehe Abschnitt 4 unten


===============================================================================
4. KONFIGURATIONSDATEI ERSTELLEN
===============================================================================

SPEICHERORT DER KONFIGURATIONSDATEI:
C:\Users\[IHR_NAME]\AppData\Roaming\RXBNetwork\rxb.conf

DATEI ERSTELLEN:

Schritt 1: Datei-Explorer öffnen
- Drücken Sie Windows + R
- Eingeben: %APPDATA%\RXBNetwork
- Klicken Sie auf OK

Schritt 2: Neue Datei erstellen
- Rechtsklick -> Neu -> Textdokument
- Benennen Sie es: rxb.conf (HINWEIS: Entfernen Sie .txt am Ende!)

Schritt 3: Konfigurationsdatei bearbeiten
Öffnen Sie die Datei und fügen Sie Beispielkonfiguration ein:

# Grundlegende RXBNetwork-Konfiguration

# P2P-Port (Standard 8327)
port=8327

# Maximale Verbindungen
maxconnections=125

# Tor-Proxy (wenn Tor verwendet wird)
proxy=127.0.0.1:9050
listen=1
bind=127.0.0.1

# Ihr eigener Onion Seed Node (falls erstellt)
# externalip=[IHRE_ADRESSE].onion:8327

# Zusätzliche Seed Nodes (optional)
# addnode=m4gcv7eaorwu5nsmflfmeo6whrh5gu3wwkxfsanzs7evqaotolf4ksyd.onion:8327

# RPC (für fortgeschrittene Benutzer)
server=1
rpcuser=rxbuser
rpcpassword=[SICHERES_PASSWORT]
rpcallowip=127.0.0.1
rpcport=8328

# Optional: Festplattenbegrenzung (Pruning)
# prune=5000

Datei speichern und Node neu starten.


===============================================================================
5. EIGENEN ONION SEED NODE ERSTELLEN
===============================================================================

ANFORDERUNGEN:
- Tor Expert Bundle installiert (siehe Abschnitt 3)
- Port 8327 in Firewall geöffnet (siehe Abschnitt 2)
- Stabile Internetverbindung
- Computer läuft 24/7 (empfohlen)

SEED NODE-KONFIGURATION:

Schritt 1: Stellen Sie sicher, dass Tor läuft
- Prüfen Sie, ob tor.exe läuft
- Prüfen Sie Datei: C:\Tor\rxb-hidden-service\hostname
- Speichern Sie Ihre .onion-Adresse

Schritt 2: rxb.conf konfigurieren
- Fügen Sie zur Konfigurationsdatei hinzu:

# Seed Node-Konfiguration
externalip=[IHRE_ADRESSE].onion:8327
onion=127.0.0.1:9050
onlynet=onion
listen=1
discover=1

Schritt 3: Node starten
- Führen Sie rxbd.exe oder rxb-qt.exe aus
- Node verbindet sich automatisch über Tor mit dem Netzwerk

Schritt 4: Status prüfen
- Öffnen Sie CMD
- Navigieren Sie zum RXBNetwork-Installationsordner
- Ausführen: rxb-cli.exe getnetworkinfo
- Prüfen Sie, ob "localaddresses" Ihre .onion-Adresse enthält


===============================================================================
6. PASSWÖRTER UND SICHERHEIT
===============================================================================

REGELN FÜR SICHERE PASSWÖRTER:

FÜR WALLET:
- Mindestens 16 Zeichen
- Groß- und Kleinbuchstaben
- Zahlen
- Sonderzeichen erlaubt: !@#$%^&*()_+=[]{}

Beispiel: MyBr1xW@ll3t2025!Secure

FÜR KONFIGURATION (rpcpassword):
Beim Erstellen von Passwörtern für die Node-Konfiguration können Sie 
Sonderzeichen und Unterstriche verwenden. Wenn Sie jedoch feststellen, 
dass Sonderzeichen oder Unterstriche in Passwörtern während der Konfiguration 
oder des Node-Betriebs Probleme verursachen, empfehlen wir, ein Passwort zu 
erstellen, das nur aus Buchstaben und Zahlen besteht. Solche einfachen 
Passwörter vermeiden potenzielle Kompatibilitätsprobleme in verschiedenen 
Systemkonfigurationen.

- Verwenden Sie: Buchstaben und Zahlen
- Optional: Unterstrich _ (wenn es keine Probleme verursacht)
- Mindestens 20 Zeichen

Beispiel: RXBRPCPass2025SecureKey12345
Oder mit Unterstrichen: RXBRPC_Pass_2025_Secure_Key_12345

SICHERE PASSWORT-GENERIERUNG:
1. Verwenden Sie einen Online-Passwortgenerator (z.B. passwordsgenerator.net)
2. Länge: 20-30 Zeichen
3. Aktivieren: Großbuchstaben, Kleinbuchstaben, Zahlen
4. Für rpcpassword: Sonderzeichen deaktivieren, wenn sie Probleme verursachen
5. Passwort im Passwort-Manager speichern (z.B. KeePass, Bitwarden)

WALLET-BACKUP:
1. Schließen Sie RXB
2. Navigieren Sie zu: %APPDATA%\RXBNetwork
3. Kopieren Sie die Datei wallet.dat
4. Speichern Sie auf externem Laufwerk, USB oder Cloud (verschlüsselt!)
5. Backup monatlich oder nach jeder wichtigen Transaktion erstellen


===============================================================================
7. CLEARNET-TOR-BRÜCKEN ERSTELLEN
===============================================================================

WAS IST EINE BRÜCKE?
Eine Brücke ist ein Node, der Geräte ohne Tor (Clearnet) 
mit Nodes verbindet, die über Tor (Onion) laufen. Brücken sind entscheidend 
für den Betrieb von RXBNetwork.

WIE ERSTELLT MAN EINE BRÜCKE?

Anforderungen:
- Öffentliche IP (statisch oder dynamisches DNS)
- Port 8327 im Router geöffnet (Portweiterleitung)
- Tor installiert und konfiguriert
- Stabile Internetverbindung

BRÜCKEN-KONFIGURATION:

Schritt 1: Tor konfigurieren (siehe Abschnitt 3)

Schritt 2: rxb.conf als Brücke konfigurieren:

# Clearnet-Tor-Brücken-Konfiguration

# Auf Clearnet lauschen
bind=0.0.0.0:8327
listen=1

# Mit Tor verbinden
proxy=127.0.0.1:9050
onion=127.0.0.1:9050

# Alle Netzwerke zulassen
onlynet=ipv4
onlynet=onion

# Ihre öffentliche Adresse (wenn Sie statische IP haben)
# externalip=[IHRE_ÖFFENTLICHE_IP]:8327

# Oder Domain verwenden (wenn Sie DynamicDNS haben)
# externalip=yournode.ddns.net:8327

# Maximale Verbindungen
maxconnections=125

Schritt 3: DynamicDNS konfigurieren (optional)
- Registrieren Sie sich bei No-IP.com oder DuckDNS.org (kostenlos)
- Erstellen Sie eine Domain z.B.: yourdomainnetwork.ddns.net
- Installieren Sie DynamicDNS-Client zur IP-Aktualisierung
- Fügen Sie zu rxb.conf hinzu: externalip=yourdomainnetwork.ddns.net:8327

Schritt 4: Brücke testen
- Node starten
- 30 Minuten warten
- Logs auf eingehende Verbindungen prüfen


===============================================================================
8. RXB UNTERSTÜTZEN
===============================================================================

RXBNetwork ist dezentralisiert und läuft dank seiner Benutzer.
Sie können das Netzwerk erheblich unterstützen und seine Stärke auf folgende 
Weise erhöhen:

EINEN NODE BETREIBEN:
- Jeder aktive Node erhöht die Netzwerksicherheit
- Je mehr Nodes, desto widerstandsfähiger das Netzwerk
- Wenn möglich, lassen Sie Ihren Node 24/7 laufen

ONION SEED NODES ERSTELLEN:
- Onion-Nodes bieten Privatsphäre und Anonymität
- Wenn Sie eine stabile Verbindung haben, erstellen Sie Ihren eigenen Seed Node (siehe Abschnitt 5)
- Jeder Seed Node hilft neuen Benutzern, sich mit dem Netzwerk zu verbinden

BRÜCKEN ERSTELLEN:
- Brücken verbinden Clearnet-Benutzer mit dem Tor-Netzwerk
- Sie sind absolut entscheidend für den Netzwerkbetrieb
- Wenn Sie eine öffentliche IP haben, erwägen Sie, eine Brücke zu erstellen (siehe Abschnitt 7)
- Brücken sind der wertvollste Beitrag zum Netzwerk

FULL NODE BETREIBEN:
- Full Node speichert die gesamte Blockchain
- Hilft neuen Benutzern bei der Synchronisation
- Benötigt ~50 GB Festplattenspeicher
- In rxb.conf NICHT hinzufügen: prune=5000

WIE SIE HELFEN KÖNNEN:
1. Betreiben Sie einen Node und halten Sie ihn online
2. Wenn Sie Tor haben - erstellen Sie Onion Seed Node
3. Wenn Sie öffentliche IP haben - erstellen Sie Clearnet-Tor-Brücke
4. Wenn Sie viel Speicherplatz haben - betreiben Sie Full Node (ohne Pruning)
5. Helfen Sie anderen Benutzern in der Community

JEDER NODE ZÄHLT!
Auch wenn Sie Ihren Node nur einige Stunden täglich laufen lassen können,
hilft es dem Netzwerk. Sie benötigen keine besonderen Fähigkeiten
oder teure Hardware. Ihr Beitrag ist wertvoll!


===============================================================================
9. FEHLERBEHEBUNG
===============================================================================

Problem: Windows blockiert Ausführung
Lösung:
1. Rechtsklick auf .exe-Datei
2. Wählen Sie "Eigenschaften"
3. Auf der Registerkarte "Allgemein" markieren Sie "Zulassen"
4. Klicken Sie auf "Übernehmen" und "OK"
5. Erneut ausführen

Problem: Antivirus blockiert Programm
Lösung:
1. Fügen Sie RXBNetwork-Ordner zu Antivirus-Ausnahmen hinzu
2. Deaktivieren Sie Antivirus vorübergehend während der Installation
3. Aktivieren Sie Antivirus nach der Installation wieder

Problem: Wallet synchronisiert nicht
Lösung:
1. Internetverbindung prüfen
2. Prüfen Sie, ob Port 8327 geöffnet ist (siehe Abschnitt 2)
3. Länger warten (kann beim ersten Start bis zu 60 Minuten dauern)
4. Programm neu starten
5. Logs prüfen: %APPDATA%\RXBNetwork\debug.log

Problem: Tor verbindet nicht
Lösung:
1. Stellen Sie sicher, dass Tor Browser läuft BEVOR Sie RXB starten
2. Prüfen Sie, ob rxb.conf hat: proxy=127.0.0.1:9050
3. Prüfen Sie, ob Tor funktioniert: öffnen Sie https://check.torproject.org im Tor Browser
4. Tor Browser und RXB neu starten

Problem: Kann Onion Seed Node nicht erstellen
Lösung:
1. Prüfen Sie, ob tor.exe läuft
2. Prüfen Sie, ob torrc-Datei korrekt konfiguriert ist
3. Tor-Logs prüfen: C:\Tor\tor.log
4. Stellen Sie sicher, dass Ordner C:\Tor\rxb-hidden-service erstellt wurde

Problem: Brücke akzeptiert keine Verbindungen
Lösung:
1. Prüfen Sie, ob Port 8327 im Router geöffnet ist (Portweiterleitung)
2. Öffentliche IP prüfen: https://whatismyipaddress.com
3. Zu rxb.conf hinzufügen: externalip=[IHRE_IP]:8327
4. 30-60 Minuten warten, bis Node im Netzwerk erkannt wird

Problem: Wallet-Passwort vergessen
Lösung:
Leider gibt es keine Möglichkeit, das Passwort wiederherzustellen. Wenn Sie kein Backup
der wallet.dat-Datei haben, ist der Zugang zu den Coins für immer verloren.
ERSTELLEN SIE IMMER EIN WALLET-BACKUP!


===============================================================================
WICHTIGE DATEISPEICHERORTE
===============================================================================

Datenordner:
C:\Users\[IHR_NAME]\AppData\Roaming\RXBNetwork\

Wichtige Dateien:
- wallet.dat           - Wallet (BACKUP!)
- rxb.conf     - Konfiguration
- debug.log            - Logs (hilfreich bei Problemen)
- peers.dat            - Liste bekannter Nodes
- blocks\              - Blockchain
- chainstate\          - Chain-Status


===============================================================================
PROJEKTINFORMATIONEN
===============================================================================

Website: https://rxbcore.org

Version: 0.1.0
Veröffentlichungsdatum: Januar 2025
Technologie: RXBNetwork nutzt Bitcoin Core 20.2 Bibliotheken und Struktur
Netzwerk: Mainnet


===============================================================================
SYSTEMANFORDERUNGEN
===============================================================================

Minimal:
- System: Windows 7 SP1 (64-bit) oder neuer
- Prozessor: Dual-Core 2.0 GHz
- RAM: 2 GB
- Festplatte: 10 GB freier Speicherplatz
- Internet: Breitbandverbindung

Empfohlen:
- System: Windows 10/11 (64-bit)
- Prozessor: Quad-Core 2.5 GHz oder besser
- RAM: 4 GB oder mehr
- Festplatte: SSD mit 50 GB freiem Speicherplatz
- Internet: Breitbandverbindung (unbegrenzte Daten)

Für Brücken und Seed Nodes:
- Stabile 24/7-Verbindung
- Öffentliche IP oder DynamicDNS
- Keine Datentransferbegrenzung
- Mindestens 100 GB freier Speicherplatz


===============================================================================
LIZENZ UND HAFTUNGSAUSSCHLUSS
===============================================================================

RXBNetwork ist Open-Source-Software, veröffentlicht unter MIT-Lizenz.

HAFTUNGSAUSSCHLUSS:
Die Software wird "WIE BESEHEN" bereitgestellt, ohne jegliche Garantien.
Die Autoren sind nicht verantwortlich für finanzielle Verluste
oder Schäden, die aus der Nutzung dieser Software resultieren.

IMMER:
- Wallet sichern
- Starkes Passwort verwenden
- Passwort niemals mit jemandem teilen
- Keine verdächtige Software installieren
- Prüfsummen heruntergeladener Dateien überprüfen


===============================================================================
PRÜFSUMMEN (CHECKSUMS)
===============================================================================

Überprüfen Sie vor der Ausführung, ob die heruntergeladene Datei authentisch ist.

MD5:
[generieren: md5sum *.exe *.zip]

SHA256:
[generieren: sha256sum *.exe *.zip]

So überprüfen Sie Prüfsumme in Windows:
1. PowerShell öffnen
2. Eingeben: Get-FileHash .\rxb-0.1.0-win64-setup-unsigned.exe
3. Ergebnis mit obigem vergleichen

Wenn Prüfsummen nicht übereinstimmen - DATEI NICHT AUSFÜHREN!


===============================================================================

Vielen Dank für die Nutzung von RXBNetwork!

Frohes Mining! Ihr Beitrag zur Aufrechterhaltung von Nodes im Hintergrund 
ist von unschätzbarem Wert. Jeder Node stärkt das Netzwerk. Besonders 
wertvoll ist der Betrieb von Brücken-Nodes, die Clearnet mit dem Onion-
Netzwerk verbinden und das gesamte RXBNetwork-Ökosystem integrieren und 
vereinen.

===============================================================================
