# Problem producenta i konsumenta

W problemie producenta i konsumenta występują dwa rodzaje procesów: producent i konsument, którzy dzielą wspólny zasób - bufor dla produkowanych (konsumowanych) jednostek. Zadaniem producenta jest wytworzenie produktu, umieszczenie go w buforze i rozpoczęcie pracy od nowa. W tym samym czasie konsument ma pobrać produkt z bufora. Problemem jest taka synchronizacja procesów, żeby producent nie dodawał nowych jednostek gdy bufor jest pełny, a konsument nie pobierał gdy bufor jest pusty.

![woman-reader_2256250b](https://mieszkomakuch.github.io/problemy-synchronizacyjne/img/producers-consumers.jpg)

## Opis rozwiązania

Rozwiązaniem dla producenta jest uśpienie procesu w momencie gdy bufor jest pełny. Pierwszy konsument, który pobierze element z bufora budzi proces producenta, który uzupełnia bufor. W analogiczny sposób usypiany jest konsument próbujący pobrać z pustego bufora. Pierwszy producent, po dodaniu nowego produktu umożliwi dalsze działanie konsumentowi. Nieprawidłowe rozwiązanie może skutkować **zakleszczeniem**.

### Pseudokod rozwiązania
```cpp
int BUFFER[BUFFER_SIZE]

int ilosc_producentow
int ilosc_konsumentow

mutex buffer_mutex
semafor zapelnione_miejsce // inicjalizuj 0
semafor wolne_miejsce // inicjalizuj BUFFER_SIZE

producent() {
    while (1) {
        // Produce item
        item = produce_item
        
        sem_(&wolne_miejsce)
        lock_mutex(&buffer_mutex)

        // Wstaw przedmiot do bufora
        wstaw_przedmiot(item)

        unlock_mutex(&buffer_mutex)
        post(&zapelnione_miejsce)
    }
}

konsument() {
    while (1) {
        sem_(&zapelnione_miejsce)
        lock_mutex(&buffer_mutex)

        // Pobierz przedmiot z bufora
        item = pobierz_przedmiot()

        unlock_mutex(&buffer_mutex)
        post(&wolne_miejsce)

        // Consume item
    }
    return 0
}

```
### [Pełne rozwiązanie](https://mieszkomakuch.github.io/problemy-synchronizacyjne/doc/producers-consumers-source.html)

