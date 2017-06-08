# Problem ucztujących filozofów
Problem ucztujących filozofów to klasyczny informatyczny problem synchronizacji. Jest to teoretyczne wyjaśnienie zakleszczenia i uniemożliwienia innym jednostkom korzystania z zasobów poprzez założenie, że każdy z filozofów bierze po jednym widelcu / po jednej pałeczce (do jedzenia ryżu), a dopiero potem próbuje zdobyć drugi / drugą. Zakłada się, że jedzenie jednym widelcem / jedną pałeczką jest niemożliwe.

![at_the_table](https://mieszkomakuch.github.io/problemy-synchronizacyjne/img/at_the_table.png)

Problem ucztujących filozofów jest prezentacją problemu synchronizacji pracujących współbieżnie procesów.

W projekcie zaprezentowane zostały 2 rozwiązania:
- **rozwiązanie z hierarchią zasobów**
- **rozwiązanie z kelnerem/arbitrem**

## Opis problemu

Pięciu filozofów siedzi przy stole i każdy wykonuje jedną z dwóch czynności – albo je, albo rozmyśla. Stół jest okrągły, przed każdym z nich znajduje się miska ze spaghetti, a pomiędzy każdą sąsiadującą parą filozofów leży widelec, a więc każda osoba ma przy sobie dwie sztuki – po swojej lewej i prawej stronie. Ponieważ jedzenie potrawy jest trudne przy użyciu jednego widelca, zakłada się, że każdy filozof korzysta z dwóch. Dodatkowo nie ma możliwości skorzystania z widelca, który nie znajduje się bezpośrednio przed daną osobą. Problem ucztujących filozofów jest czasami przedstawiany przy użyciu ryżu, który musi być jedzony dwiema pałeczkami, co lepiej obrazuje sytuację.

Filozofowie nigdy nie rozmawiają ze sobą, co stwarza zagrożenie **zakleszczenia** w sytuacji, gdy każdy z nich zabierze lewy widelec i będzie czekał na prawy (lub na odwrót).

## Rozwiązanie z hierarchią zasobów / widelców

Jednym z rozwiązań tego problemu jest możliwe przez ustalenie hierarchii dla widelców, którymi posługują się filozofowie. i wprowadzenie zasady, że kolejność dostępu do widelców jest ustalona przez ten porządek, a ich zwalnianie następuje w odwrotnej kolejności. Ponadto dwa widelce, które nie są ze sobą w relacje nie mogą zostać użyte przez jednego filozofa w tym samym czasie.

### Opis rozwiązania

Ustalmy zatem hierarchię i ponumerujmy widelce od 1 do 5:

![forks_with_numbers](https://mieszkomakuch.github.io/problemy-synchronizacyjne/img/forks.png)

Zgodnie z zasadami, aby użyć dwóch widelców filozof musi najpierw podnieść widelec z niższym numerem. Sytuację te przedstawia poniższa grafika:

![resource_hierarchy](https://mieszkomakuch.github.io/problemy-synchronizacyjne/img/resource_hierarchy.png)

Zwróćmy uwagę, że zaznaczony na czerwono filozof **nie może podnieść widelca 5!** Jest to związane z przyjętą zasadą - najpierw musiałby podnieść widelec o niższym numerze.

Oznacza to, że widelec 5 może zostać podniesiony przez sąsiadującego filozofa, przez co jest on teraz w stanie konsumować danie, co przedstawia poniższa grafika:

![resource_hierarchy_result](https://mieszkomakuch.github.io/problemy-synchronizacyjne/img/resource_hierarchy_result.png)

Jak widać, dzięki hierarchii zasobów udało się **uniknąć zakleszczenia**.

### Pseudokod rozwiązania
```cpp
var
  widelce: tablica semaforów [0..4] := (1, 1, 1, 1, 1);

process Filozof (i: 0..4);
begin
  repeat
    Myśli;
    Wait (widelce[i]);
    Wait (widelce[(i+1) mod 5];
    Je danie;
    Post (widelce[i]);
    Post (widelce[(i+1) mod 5];
  until false
end;

```
### [Pełne rozwiązanie z hierarchią zasobów](https://mieszkomakuch.github.io/problemy-synchronizacyjne/doc/dining-philosophers-resourceh-code.html)

## Rozwiązanie z kelnerem / arbitrem

Kolejnym nieskomplikowanym rozwiązaniem jest wprowadzenie kelnera. Filozofowie będą pytać go o pozwolenie przed wzięciem widelca. Ponieważ kelner jest świadomy, które widelce są aktualnie w użyciu, może nimi rozporządzać zapobiegając zakleszczeniom.

### Opis rozwiązania

Jeśli dostępne jest 5 widelców to tylko 4 filozofów będzie mogło siedzieć przy stole. Kelner zarządza tym kto i kiedy może usiąść przy stole: jeśli jest mniej niż 4 filozofów przy stole kolejny może dołączyć, jeśli nie kolejny musi czekać na pozwolenie kelnera.

Sytuację prezentuje poniższa grafika:

![semaphore](https://mieszkomakuch.github.io/problemy-synchronizacyjne/img/semaphore.png)

Rozwiązanie to **zapobiega zakleszczeniu**, ale wciąż może pojawić się **głodzenie**. Filozof może czekać w nieskończoność i nigdy nie zasiąść do stołu.

### Pseudokod rozwiązania
```cpp
var
  kelner: semafor := 4;
  widelce: tablica semaforów [0..4] := (1, 1, 1, 1, 1);

process Filozof (i: 0..4);
begin
  repeat
    Myśli;
    Wait (kelner);
    Wait (widelce[i]);
    Wait (widelce[(i+1) mod 5];
    Je;
    Post (widelce[i]);
    Post (widelce[(i+1) mod 5];
    Post (kelner)
  until false
end;
```
### [Pełne rozwiązanie z kelnerem / arbitrem](https://mieszkomakuch.github.io/problemy-synchronizacyjne/doc/dining-philosophers-waiter-code.html)