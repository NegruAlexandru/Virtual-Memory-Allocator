**Nume: Negru Alexandru-Nicolae**

**Grupă: 314CAb**

## Segregated Free lists (Tema 1 SDA)

### Descriere:

* Programul reprezinta un alocator de memorie virtuala. Acesta dispune de comenzi pentru manipularea memoriei precum: MALLOC, FREE, WRITE, READ.

* #### INIT_HEAP - creeaza heap-ul virtual

    Utilizare: INIT_HEAP <adresa_start> <nr_liste> <memorie_lista> <tip_free>

    Exemplu:
    ```bash
	INIT_HEAP 0x10 8 1024 0
    ``` 
   #### Proces:
   * Se aloca memorie pentru un vector de liste dublu inlantuite. 
   * Se adauga elemente in listele create in functie de parametrii introdusi.

* #### DUMP_MEMORY - afiseaza informatii despre heap

    Utilizare: DUMP_MEMORY
    Exemplu:
    ```bash
	DUMP_MEMORY
    ``` 
   #### Proces:
   * Foloseste variabile specifice si calcule pentru a determina informatii despre heap.
   * Afiseaza pe ecran rezultatele sub forma unui bloc.

* #### MALLOC - aloca memorie virtuala

    Utilizare: MALLOC <nr_bytes>

    Exemplu:
    ```bash
	MALLOC 10
    ``` 
   #### Proces:
   * Transfera blocuri de memorie libera, intregi sau partiale, in spatiul de memorie alocata care este disponibila utilizatorului.
   * Blocuri alocate partial vor fi impartite in doua blocuri, unul va fi alocat, al doilea va ramane liber.

* #### FREE - elibereaza memoria alocata

    Utilizare: FREE <adresa_memorie>

    Exemplu:
    ```bash
	FREE 0x41
    ``` 
   #### Proces:
   * Transfera blocurile de memorie alocata inapoi in heap.
   * Pentru modul de free 0 blocurile se vor transfera direct, iar pentru modul 1 se vor reintregi in blocurile originale.

* #### WRITE - scrie date in memoria alocata

    Utilizare: WRITE <adresa_memorie> <date_de_scris> <nr_bytes>

    Exemplu:
    ```bash
	WRITE 0x10 "gigel" 5
    ``` 
   #### Proces:
   * Verifica daca exista spatiu in care sa se scrie datele.
   * Scrie in memorie datele.

* #### READ - citeste datele scrise in memoria alocata

    Utilizare: READ <adresa_memorie> <nr_bytes>

    Exemplu:
    ```bash
	READ 0x10 5
    ``` 
   #### Proces:
   * Verifica daca memoria corespunzatoare datelor cerute este alocata.
   * Afiseaza pe ecran datele cerute.

* #### DESTROY_HEAP - opreste gratios programul

    Utilizare: DESTROY_HEAP

    Exemplu:
    ```bash
	DESTROY_HEAP
    ``` 
   #### Proces:
   * Elibereaza in intregime memoria alocata si iese din program.

### Comentarii asupra temei:

* Crezi că ai fi putut realiza o implementare mai bună?

Da, exista intotdeauna lucruri de imbunatatit. Ca si exemple, as putea spune ca sistemul de parsare a input-ului ar putea fi generalizat mai atent, iar totalitatea memoriei utilizate ar putea fi micsorata.

* Ce ai invățat din realizarea acestei teme?

Pana sa incep tema am invatat mai mult despre alocatoare de memorie si concepte precum liste, dar am invatat si lucruri noi, precum segregated free lists, sequential fit, buddy system. De asemenea, am invatat sa lucrez mai bine cu liste, sa imi modularizez codul mai mult si sa folosesc git pentru versionare (in repository privat).

