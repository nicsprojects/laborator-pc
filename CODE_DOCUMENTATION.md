# Documentație Completă - Laborator Protocoale de Comunicații

## Descriere Generală a Proiectului

Acest proiect implementează drivere pentru protocoale de comunicații pe microcontrollerul **ATmega328P** (Arduino UNO). Fiecare protocol are o clasă de nivel înalt care abstractizează detaliile hardware-ului și oferă o interfață ușor de utilizat.

---

## STRUCTURA MODULELOR

### 1. **COMMON/ - Modulele de Bază Partajate**

Folderul `common/` conține implementările de bază ale protocolurilor care sunt reutilizate în mai multe proiecte.

---

## FIȘIERE HEADER (INTERFEȚE)

### **common/include/laboratory_types.h**

**Scopul**: Definiție de tipuri și enumerații utilizate în toat proiectul

**Ce conține**:
- `LedState` - enum cu stări LED: ON, OFF
- `ButtonState` - enum cu stări buton: PRESSED, NOT_PRESSED  
- `I2cResponse` - enum pentru răspunsuri I2C: ACKNOWLEDGE (ACK), NOT_ACKNOWLEDGE (NACK)

**De ce**: Standardizează tipurile de date folosite în toată aplicația, asigurând consistență și facilitând citibilitatea codului.

---

### **common/include/Uart.h**

**Scopul**: Definiția clasei `Uart` - interfață de nivel înalt pentru Protocol UART (Universal Asynchronous Receiver Transmitter)

**Componente (Metode Publice)**:

1. **`void init()`**
   - Ce face: Inițializează perifericul UART pe ATmega328P
   - Configurează: frecvență baud rate 9600, pinii de RX/TX, frame format (8 biti, 1 stop bit, fără paritate)
   - De ce: UART trebuie configurat înainte de a putea trimite/primi date

2. **`void writeByte(const char& d)`**
   - Ce face: Trimite un octet (8 biți) prin UART TX
   - Funcționare: Blochează execuția până când bufferul de transmisie este gol
   - De ce: Permite comunicare sincronă garantând completarea transmisiei

3. **`bool available()`**
   - Ce face: Verifică dacă sunt date în bufferul de recepție UART
   - Returnează: true dacă există date, false altfel
   - De ce: Permite cod non-blocant pentru verificarea sosirii datelor

4. **`char readByte()`**
   - Ce face: Citește un octet din bufferul RX
   - Funcționare: Blochează dacă nu sunt date disponibile
   - De ce: Oferă citire directă și simplă a datelor primite

5. **`void writeString(const char* msg)`**
   - Ce face: Trimite un șir de caractere prin UART
   - De ce: Convenință pentru trimiterea mesajelor text

6. **`void writeIntegerNumber(const int& number, const int& base = 10)`**
   - Ce face: Convertește un număr întreg în bază 2-36 și îl trimite prin UART
   - Parametri: `base` - 10 (decimal), 2 (binar), 16 (hexazecimal), etc.
   - De ce: Permite debug și logging numere în diferite formate

---

### **common/include/I2cMaster.h**

**Scopul**: Definiția clasei `I2cMaster` - interfață de nivel înalt pentru Protocol I2C (Inter-Integrated Circuit) în mod MASTER

**Componente Protejate (Private)**:

1. **`void _writeByte(const char& data)`**
   - Ce face: Trimite un octet brut pe magistrala I2C
   - Detaliu low-level: Scrie în registrul TWDR, setează flag TWINT
   - De ce: Operație de bază necesară pentru protocoale I2C

2. **`void _readByte(char& data)`**
   - Ce face: Citește un octet brut din magistrala I2C
   - Detaliu low-level: Aşteaptă interrupt TWINT, citește din TWDR
   - De ce: Operație de bază pentru primire date I2C

**Componente Publice (Interfață Utilizator)**:

1. **`void init()`**
   - Ce face: Configurează perifericul I2C cu parametrii standard
   - Configurare: Frecvență SCL = 100kHz, enable pull-up rezistori pe SCL/SDA
   - De ce: I2C necesită configurare precisă și pull-up rezistori activi

2. **`void sendStart()`**
   - Ce face: Trimite condiția START a protocolului I2C
   - Detaliu: Pune liniile SCL și SDA în starea inițială pentru comunicare
   - De ce: START condiția initializează fiecare dialog I2C

3. **`bool writeAddrWrite(const char& addr)`**
   - Ce face: Trimite adresa slave-ului + bit de WRITE (0)
   - Returnează: true dacă slave-ul a răspuns cu ACK
   - De ce: Prima fază a unei operații I2C - selectarea slave-ului pentru scriere

4. **`bool writeAddrRead(const char& addr)`**
   - Ce face: Trimite adresa slave-ului + bit de READ (1)
   - Returnează: true dacă slave-ul a răspuns cu ACK
   - De ce: Prima fază a unei operații I2C - selectarea slave-ului pentru citire

5. **`bool writeByte(const char& d, I2cResponse expectedResponse = ACKNOWLEDGE)`**
   - Ce face: Trimite un octet de date pe magistrala I2C
   - Parametri: `expectedResponse` - ce răspuns așteptăm (ACK/NACK)
   - Returnează: true dacă răspunsul primit corespunde cu cel așteptat
   - De ce: Trimitere date cu verificare de integritate

6. **`bool readByte(char& d, I2cResponse expectedResponse = ACKNOWLEDGE)`**
   - Ce face: Citește un octet și trimite ACK
   - De ce: Pentru citire date în mijlocul unei comunicații

7. **`bool readLastByte(char& d)`**
   - Ce face: Citește ultimul octet și trimite NACK (pentru a termina citire)
   - De ce: Semnalizează slave-ului că comunicarea se termină

8. **`void sendStop()`**
   - Ce face: Trimite condiția STOP a protocolului I2C
   - De ce: Încheie orice dialog I2C și eliberează magistrala

---

### **common/include/SpiMaster.h**

**Scopul**: Definiția clasei `SpiMaster` - interfață de nivel înalt pentru Protocol SPI (Serial Peripheral Interface) în mod MASTER

**Componente (Metode Publice)**:

1. **`void init()`**
   - Ce face: Configurează perifericul SPI pentru funcționare master
   - Configurare: 
     - Pinii MOSI, MISO, SCK sunt setați corespunzător
     - CS (Chip Select) este configurat
     - Setări SPI: mode CPOL=1, CPHA=1, prescaler corespunzător
   - De ce: SPI necesită configurare specifică a polarității și fazei clock-ului

2. **`char transmit(const char& d)`**
   - Ce face: Transmite un octet pe magistrala SPI și primește răspunsul
   - Parametru: `d` - octetul de trimis pe MOSI
   - Returnează: Octetul primit pe MISO în același timp
   - Funcționare: Full-duplex - comunicare simultană în ambele direcții
   - De ce: SPI este protocol sincron care permite comunicare full-duplex

---

### **common/include/IAdxlTransport.h**

**Scopul**: Interfață abstractă pentru comunicația cu senzorul ADXL345 (accelerometru)

**Componente (Metode Virtuale)**:

1. **`virtual void init() = 0`**
   - Ce face: Inițializare transport layer - pur virtual, implementat în subclase
   - De ce: Permite mai multe moduri de transport (I2C sau SPI)

2. **`virtual bool read(int addr, char* d, int len = 1)`**
   - Ce face: Citește `len` bytes din registrul ADXL345 la adresa `addr`
   - Parametri:
     - `addr` - adresa registrului ADXL345
     - `d` - buffer unde se stochează datele
     - `len` - numărul de bytes de citit (implicit 1)
   - Returnează: true dacă citirea a fost cu succes
   - De ce: Interfață unificată pentru citire din accelerometru indiferent de transport

3. **`virtual bool write(int addr, const char* d, int len = 1)`**
   - Ce face: Scrie `len` bytes în registrul ADXL345 la adresa `addr`
   - Parametri:
     - `addr` - adresa registrului ADXL345
     - `d` - buffer cu datele de scris
     - `len` - numărul de bytes de scris (implicit 1)
   - Returnează: true dacă scrierea a fost cu succes
   - De ce: Interfață unificată pentru scriere în accelerometru indiferent de transport

---

## FIȘIERE IMPLEMENTARE (.cpp)

### **common/src/Uart.cpp**

**Scopul**: Implementarea metodelor clasei Uart

**Ce implementează**:
- `init()` - Configurează registrele UART: DDRD, UBRR0H/L, UCSR0B, UCSR0C
- `writeByte()` - Așteaptă UDRE0 flag, scrie în UDR0
- `available()` - Verifică RXC0 flag în UCSR0A
- `readByte()` - Așteaptă RXC0 flag, citește din UDR0
- `writeString()` - Loop prin caractere și apelează writeByte()
- `writeIntegerNumber()` - Folosește itoa() pentru conversie, apoi writeString()

**De ce aceste implementări**:
- Gestionează detaliile hardware register-level pentru ATmega328P
- Oferă abstracție simplă peste complexitatea perifericului UART

---

### **common/src/I2cMaster.cpp**

**Scopul**: Implementarea metodelor clasei I2cMaster

**Ce implementează**:
- `_writeByte()` - Scrie în TWDR, setează TWINT și TWEN, așteaptă TWINT
- `_readByte()` - Setează TWINT și TWEN, așteaptă TWINT, citește din TWDR
- `init()` - Configurează TWSR (prescaler), TWBR (baud rate 100kHz), enable TWI, pull-up rezistori
- `sendStart()` - Setează TWSTA flag
- `writeAddrWrite()/writeAddrRead()` - Formează adresa cu bit R/W, apelează _writeByte()
- `writeByte()` - Apelează _writeByte(), verifică status
- `readByte()` - Apelează _readByte()
- `readLastByte()` - Apelează _readByte() fără ACK
- `sendStop()` - Setează TWSTO flag

**De ce aceste implementări**:
- Respectă protocolul I2C standard
- Gestionează handshake-ul ACK/NACK
- Oferă nivel de abstracție util pentru aplicații

---

### **common/src/SpiMaster.cpp**

**Scopul**: Implementarea metodelor clasei SpiMaster

**Ce implementează**:
- `init()` - Setează direcții pinii (MOSI/SCK output, MISO input), configurează SPCR și DDRB
- `transmit()` - Scrie în SPDR, așteaptă SPIF flag, returnează valoare SPDR

**De ce aceste implementări**:
- Configurare SPI master standard
- Protocol full-duplex sincron
- Suportă comunicare cu senzori SPI precum ADXL345

---

## CLASE DE TRANSPORT (OSI/)

### **OSI/include/I2cTransport.h și SpiTransport.h**

**Scopul**: Implementări concrete ale interfeței IAdxlTransport

**Ce sunt**:
- `I2cTransport` - Transport layer pentru ADXL345 via I2C
- `SpiTransport` - Transport layer pentru ADXL345 via SPI

**De ce separate**:
- Modelează conceptul OSI: Transport layer abstractizează protocolul fizic
- Permite schimbarea protocolului fără a modifica cod aplicație

---

## PROIECTE PRACTICE (HelloWorld, I2C, SPI, UART, OSI, Introduction)

Fiecare folder conține un proiect platformio cu:

### **platformio.ini**
- Configurație PlatformIO pentru Arduino UNO
- Specifică framework-ul, placa, și portul

### **src/main.cpp**
- Aplicație practică demonstrând utilizarea modulelor

### **include/** și **lib/**
- Conține header-uri și biblioteci proiect

### **test/**
- Spațiu pentru teste unitare

---

## RELAȚII ÎNTRE COMPONENTE

```
laboratory_types.h (tipuri)
    ↓
Uart.h, I2cMaster.h, SpiMaster.h (drivere hardware)
    ↓
IAdxlTransport.h (interfață generică)
    ↓
I2cTransport.h, SpiTransport.h (implementări concrete)
    ↓
main.cpp (aplicații practice)
```

---

## REZUMAT FIȘIERE

| Fișier | Tip | Scop |
|--------|-----|------|
| **laboratory_types.h** | Header | Tipuri și enumerații standard |
| **Uart.h** | Header | Interfață UART |
| **Uart.cpp** | Implementare | Cod UART ATmega328P |
| **I2cMaster.h** | Header | Interfață I2C Master |
| **I2cMaster.cpp** | Implementare | Cod I2C ATmega328P |
| **SpiMaster.h** | Header | Interfață SPI Master |
| **SpiMaster.cpp** | Implementare | Cod SPI ATmega328P |
| **IAdxlTransport.h** | Header | Interfață transport ADXL345 |
| **I2cTransport.h** | Header | Implementare ADXL345 via I2C |
| **SpiTransport.h** | Header | Implementare ADXL345 via SPI |

---

## HARDWARE FOLOSIT

- **Microcontroller**: ATmega328P (Arduino UNO)
- **Senzor**: ADXL345 (accelerometru 3-axe)
- **Protocoale**: UART (9600 baud), I2C (100kHz), SPI (1MHz)

---

## NOTE IMPORTANTE

1. **NU MODIFICA CODUL** - Această documentație explică comportamentul curent
2. Fiecare modul este autoinclus - poți folosi doar ce ai nevoie
3. Fișierele din `common/` sunt bazele; folderele de proiecte le reutilizează
4. Interfața IAdxlTransport permite schimbarea protocolului transport ușor
5. Configurări hardware (baud rate, frecvență I2C, etc.) sunt hardcodificate în init()
