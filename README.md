# [Dokumentacja online](https://mieszkomakuch.github.io/problemy-synchronizacyjne/)

# Wprowadzenie
Problem synchronizacji procesów pojawia się wszędzie tam, gdzie mamy do czynienia ze współpracującymi ze sobą współbieżnymi procesami. Oto najczęściej spotykane przyczyny, dla których konieczna jest synchronizacja współpracujących procesów:
- Procesy współdzielą pewną strukturę danych
- Wyniki działania jednego procesu stanowią dane dla innego procesu.
- Procesy korzystają z pewnej wspólnej puli zasobów, które pobierają i zwalniają wedle potrzeb.

Projekt ma na celu analizę i prezentację problemów synchronizacji
współpracujących ze sobą procesów współbierznych. Mechanizmy i techniki
progamistyczne służące do synchronizowana procesów zostaną
zaprezentowane na **3 znanych problemach synchronizacyjnych**.

## Problem ucztujących filozofów
Problem ucztujących filozofów to klasyczny informatyczny problem synchronizacji. Jest to teoretyczne wyjaśnienie zakleszczenia i uniemożliwienia innym jednostkom korzystania z zasobów poprzez założenie, że każdy z filozofów bierze po jedym widelcu / po jednej pałeczce (do jedzenia ryżu), a dopiero potem próbuje zdobyć drgugi / drugą. Zakłada się, że jedzenie jedym widalecem / jedną pałeczką jest niemożliwe.

![at_the_table](https://mieszkomakuch.github.io/problemy-synchronizacyjne/img/at_the_table.png)

Problem ucztujących filozofów jest prezentacją problemu synchronizacji pracujących współbieżnie procesów. Istnieje kilka możliwych rozwiązań tego problemu, które zostały rozwiązane w tym projekcie.

## Problem czytelników i pisarzy
Problem czytelników i pisarzy to kolejny, znany problem synchronizacji. Dotyczy on dostępu do jednego zasobu dwóch rodzajów procesów: dokonujących w nim zmian (pisarzy) i niedokonujących w nim zmian (czytelników).

Jednoczesny dostęp do zasobu może uzyskać dowolna liczba czytelników. Pisarz może otrzymać tylko dostęp wyłączny. Równocześnie z pisarzem dostępu do zasobu nie może otrzymać ani inny pisarz, ani czytelnik, gdyż mogłoby to spowodować błędy.

## Problem producenta i konsumenta
Ostatnim problemem synchronizacyjnym, który zostanie przestawiony jest problem producenta i konsumenta. W problemie występują dwa rodzaje procesów: producent i konsument, którzy dzielą wspólny zasób - bufor dla produkowanych (konsumowanych) jednostek. Zadaniem producenta jest wytworzenie produktu, umieszczenie go w buforze i rozpoczęcie pracy od nowa. W tym samym czasie konsument ma pobrać produkt z bufora. Problemem jest taka synchronizacja procesów, żeby producent nie dodawał nowych jednostek gdy bufor jest pełny, a konsument nie pobierał gdy bufor jest pusty.