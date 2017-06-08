# Problem czytelników i pisarzy
Problem czytelników i pisarzy to kolejny, znany problem synchronizacji. Dotyczy on dostępu do jednego zasobu dwóch rodzajów procesów: dokonujących w nim zmian (pisarzy) i niedokonujących w nim zmian (czytelników).

Jednoczesny dostęp do zasobu może uzyskać dowolna liczba czytelników. Pisarz może otrzymać tylko dostęp wyłączny. Równocześnie z pisarzem dostępu do zasobu nie może otrzymać ani inny pisarz, ani czytelnik, gdyż mogłoby to spowodować błędy.

![woman-reader_2256250b](https://mieszkomakuch.github.io/problemy-synchronizacyjne/img/woman-reader_2256250b.jpg)

**W projekcie zaimplementowanych zostało 6 rozwiązań**:
1. Wariant faworyzujący czytelników
  - wykorzystując do synchronizacji semafory
  - wykorzystując do synchronizacji muteks
2. Wariant faworyzujący pisarzy
  - wykorzystując do synchronizacji semafory
  - wykorzystując do synchronizacji muteks
3. Wariant bez zagłodzenia
  - wykorzystując do synchronizacji semafory
  - wykorzystując do synchronizacji muteks

## Wariant faworyzujący czytelników

Czytelnicy nie mają obowiązku czekania na otrzymanie dostępu do zasobu, jeśli w danym momencie nie otrzymał go pisarz. Ponieważ pisarz może otrzymać tylko dostęp wyłączny, musi czekać na opuszczenie zasobu przez wszystkie inne procesy. Jeśli czytelnicy przybywają odpowiednio szybko, może dojść do zagłodzenia pisarza: w tej sytuacji będzie on w nieskończoność czekał na zwolnienie zasobu przez wciąż napływających nowych czytelników.

### Pseudokod rozwiązania
```cpp
semafor zrodlo=1;
semafor rmutex=1;
licznosc_czytelnikow=0;

pisarz() {
    zrodlo.wait();

    <Sekcja krytyczna>
    // Pisanie do źródła

    <EXIT Section>
    zrodlo.post();
}

czytelnik() {
    rmutex.wait();
    <Sekcja krytyczna>
    licznosc_czytelnikow++;
    if (licznosc_czytelnikow == 1)
        zrodlo.wait();
    <EXIT Sekcja krytyczna>
    rmutex.post();

    // Czytenie ze źródła

    rmutex.wait();
    <Sekcja krytyczna>
    licznosc_czytelnikow--;
    if (licznosc_czytelnikow == 0)
        zrodlo.post();
    <EXIT Sekcja krytyczna>
    rmutex.post();
}

```
### [Pełne rozwiązanie](https://mieszkomakuch.github.io/problemy-synchronizacyjne/doc/readers-writers-rwp1.html)

## Wariant faworyzujący pisarzy

Czytelnicy nie mogą otrzymać dostępu do zasobu, jeżeli oczekuje na niego pisarz. W tej sytuacji oczekujący pisarz otrzymuje dostęp najwcześniej, jak to jest możliwe, czyli zaraz po opuszczeniu zasobu przez ostatni proces, który przybył przed nim. W tym wariancie może dojść do zagłodzenia oczekujących czytelników.

### Pseudokod rozwiązania
```cpp
int ilosc_czytelnikow, ilosc_pisarzy; //(wartosc poczatkowa = 0)
semafor rmutex, wmutex, readTry, zrodlo; //(wartosc poczatkowa = 1)

//czytelnik
czytelnik() {
<ENTRY Sekcja>
  readTry.wait();
  rmutex.wait();
  ilosc_czytelnikow++;
  if (ilosc_czytelnikow == 1)
    zrodlo.wait();
  rmutex.post();
  readTry.post();

<Sekcja krytyczna>
 // Odbywa się czytanie

<EXIT Sekcja>
  rmutex.wait();
  ilosc_czytelnikow--;
  if (ilosc_czytelnikow == 0)
    zrodlo.post();
  rmutex.post();
}

//PISARZ
pisarz() {
<ENTRY Sekcja>
  wmutex.wait();
  ilosc_pisarzy++;
  if (ilosc_pisarzy == 1)
    readTry.wait();
  wmutex.post();

<Sekcja krytyczna>
  zrodlo.wait();
   // Odbywa się pisanie
  zrodlo.post();

<EXIT Sekcja>
  wmutex.wait();
  ilosc_pisarzy--;
  if (ilosc_pisarzy == 0)
    readTry.post();
  wmutex.post();
}
```
### [Pełne rozwiązanie](https://mieszkomakuch.github.io/problemy-synchronizacyjne/doc/readers-writers-rwp2.html)

## Wariant bez głodzenia

Wariant ten zakłada równoczesne wyeliminowanie możliwości zagłodzenia obu typów procesów.

### Pseudokod rozwiązania
```cpp
int ilosc_czytelnikow;                  // inicjalizuj do 0

// wszystkie semafory zainicjalizowane 1
semafor zrodlo;       
semafor ilosc_czytelnikow;      
semafor kolejka;         

void pisarz()
{
    kolejka.wait();          
    // <ENTER>
    zrodlo.wait();         
    // </ENTER>
    kolejka.post();           
    
    // <PISZ>
    writezrodlo();            // Odbywa się pisanie
    // </PISZ>
    
    // <EXIT>
    zrodlo.post();         
    // </EXIT>
}

void czytelnik()
{
    kolejka.wait();           
    ilosc_czytelnikow.wait();        
    // <ENTER>
    if (ilosc_czytelnikow == 0)         
        zrodlo.wait();     
    ilosc_czytelnikow++;               
    // </ENTER>
    kolejka.post();           
    ilosc_czytelnikow.post();       
    
    // <READ>
    readzrodlo();             
    // </READ>
    
    ilosc_czytelnikow.wait();        
    // <EXIT>
    ilosc_czytelnikow--;                
    if (ilosc_czytelnikow == 0)        
        zrodlo.post();    
    // </EXIT>
    ilosc_czytelnikow.post();        
}
```
### [Pełne rozwiązanie](https://mieszkomakuch.github.io/problemy-synchronizacyjne/doc/readers-writers-rwp3.html)
