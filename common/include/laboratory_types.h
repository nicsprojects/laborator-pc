#ifndef LABORATORY_TYPES_H
#define LABORATORY_TYPES_H

enum LedState {
    ON  ,
    OFF 
};

enum ButtonState {
    PRESSED     ,
    NOT_PRESSED 
};

enum I2cResponse {
    ACKNOWLEDGE     ,
    NOT_ACKNOWLEDGE
};

#endif // LABORATORY_TYPES_H
/*
==================================================
EXPLICAȚIE FIȘIER: laboratory_types.h
==================================================

Scopul fișierului:
- Definește tipuri de date comune folosite în laboratoare.
- Aceste tipuri fac programul mai ușor de citit.
- În loc să folosim direct valori numerice, folosim nume clare precum ON, OFF, PRESSED sau NOT_PRESSED.

Tipul: enum LedState
- Reprezintă starea unui LED.
- ON înseamnă LED aprins.
- OFF înseamnă LED stins.
- Este folosit de funcțiile care controlează LED-urile.

Tipul: enum ButtonState
- Reprezintă starea unui buton.
- PRESSED înseamnă că butonul este apăsat.
- NOT_PRESSED înseamnă că butonul nu este apăsat.
- Este folosit la citirea butonului și la logica de comutare a LED-ului.

Tipul: enum I2cResponse
- Reprezintă răspunsul așteptat pe magistrala I2C.
- ACKNOWLEDGE înseamnă că dispozitivul confirmă primirea datelor.
- NOT_ACKNOWLEDGE înseamnă că dispozitivul nu mai confirmă sau că citirea se termină.
- Este folosit în funcțiile I2C pentru a decide dacă se trimite ACK sau NACK.

Observație:
- Fișierul conține doar definiții de tipuri, nu implementează logică efectivă.
- Directiva #ifndef / #define / #endif previne includerea multiplă a aceluiași fișier.
*/