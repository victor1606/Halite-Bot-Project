# Halite-Bot-Project
Reasonably intelligent Halite bot

Proiect Halite 2022

Instructiuni compilare:
    - make: compileaza sursele si creeaza executabilul MyBot
    - make clean: sterge executabilul
    - make run: ruleaza MyBot

Structura proiectului:
    - MyBot.cpp: contine logica principala; sunt apelate functii auxiliare;
    - bot_functions.cpp: functii ajutatoare;
    - hlt.hpp: identic ca in starter_pack, cu exceptia celor 3 campuri adaugate
        structurii site: direction, score, coords;
    -networking.hpp: fisier neschimbat fata de schelet;

Abordarea algoritmica:

    - Pas I: atribuim fiecarui camp aflat la granita teritoriului bot-ului un
    scor; scorului fiecarui site este calculat pe baza parametrilor sai -
    production si strength - folosind urmatoarea formula (constantele sunt 
    alese experimental): score = (5 * production) - (0.7 * strength); Pe masura
    ce atribuim scoruri, adaugam fiecare camp de granita intr-o coada;

    - Pas II: coada realizata anterior este sortata in functie de scorul site-
    urilor; cat timp coada nu este goala, extragem site-ul cu cel mai mare
    scor, atribuim scoruri vecinilor sai si pe ei ii adaugam la randul lor in
    coada; coada este pastrata sortata in permanenta; la finalul pasului, 
    avem scoruri atribuite pentru fiecare camp al carui owner este bot-ul
    nostru + pentru campurile de granita;

    - Pas III: alegem cele mai bune mutari pentru fiecare site pe baza
    scorurilor obtinute la pasul II; miscarile sunt propagate spre casutele cu
    scoruri cat mai mari; pentru a realiza expansiunea, sunt adaugate diverse
    conditii:
        - se efectueaza mutarea doar daca site-ul cucerit are productia
    mai mare si strength-ul mai mic decat celula cuceritoare;
        - se efectueaza mutarea doar daca productia este de 5 ori mai mare
        decat strength-ul site-ului;
        - nu se efectueaza o mutare pe un site cu strength-ul mai mare de 255;
        - o celula neutra este atacata doar daca cucerirea este garantata;

Complexitate: functia populate_move_set itereaza prin fiecare Site al hartii
    (n * n) si apeleaza functia insert in setul de mutari (log m, m = size-ul
    set-ului, variaza pe parcursul executiei); se insereaza mutarea obtinuta
    cu functia get_best_move, care are complexitate liniara; 
        ===> complexitatea temporala = O(n^2 * log m)

Referinte:
    - ideea atribuirii scorurilor pentru site-uri este inspirata de pe aceasta
    pagina: http://braino.org/thoughts/halite_the_simple_way.html

Responsabilitati membri:
    - Research - Calugaritoiu Victor
    - Implementare - Rizea Adelina, Dicu Stefania, Calugaritoiu Victor
    - Debug - Rizea Adelina, Dicu Stefania, Calugaritoiu Victor
