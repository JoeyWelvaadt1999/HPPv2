[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/QLVxutxE)
# Merge Sort

Deze opdracht bestaat uit 2 delen met aparte deadlines. Het eerste deel wordt beoordeeld met een VD/NVD en moet VD om dit vak te halen. Het tweede deel wordt met een cijfer beoordeeld, dat voor 35% je eindcijfer bepaalt.

## Opdracht 2a - Merge Sort
Maak een implementatie van Merge Sort, zoals gegeven in Python in Opdracht 2.1, maar dan in C++ (optioneel: Rust, Zig, Carbon). Je mag zelf kiezen welke versie, beide vormen geven hun eigen uitdagingen in opgave 2b, waar je dit algoritme multithreaded gaat maken.

## Opdracht 2b - Threaded Merge Sort
In deze opgave gaan we merge sort paralleliseren.

De leeruitkomsten die bij deze opdracht horen zijn:
- een complexiteitsanalyse kan uitvoeren op een bestaande implementatie en/of algoritme;
- code kan optimaliseren m.b.t. de gebruikte algoritmiek;
- kan onderbouwen welke invloed gemaakte keuzes hebben op de optimalisatie van een implementatie en/of algoritme;

Dit doen we in drie stappen:

1. Ontwerp op papier (a.h.v. een voorbeeld van een array van, zeg, 8 getallen) hoe je door middel van 1, 2, 4, 8 cores/processen/threads/... via het merge sort algoritme een lijst kan sorteren (dus laat voor alle aantallen een ontwerp zien). Denk hierbij goed na over hoe je de data distribueert over de verschillende processen, en hoe je de resultaten ook weer efficient samen brengt. Voor de basis mergesort mag je een implementatie van het internet pakken (uiteraard met bronverwijzing)
2. Analyseer je ontwerp, en bepaal wat de complexiteit van deze implementatie van merge sort is. Is deze vergelijkbaar met de complexiteit van van de sequentiële merge sort? Zo nee, waar ligt dat aan? Bepaal voor beide varianten de tijd- en ruimtecomplexiteit en druk deze uit in Big-O notatie. Kijk hierbij hoeveel sneller/trager je algoritme wordt door te multithreaden.
3. Bepaal de communicatie overhead van je ontwerp: als je aanneemt dat elke opsplitsing van de lijst, elke verplaatsing van data naar een ander process en elke samenvoeging van lijsten even lang duren (allemaal een bepaalde tijd eenheid), hoe verhoudt de hoeveelheid communicatie zich dan tot het aantal ingezette processoren en tot de grootte van de lijst?
4. Maak een PoC implementatie door middel van threading. Bepaal testmatig wat de run-time complexiteit van je implementatie is bij gebruik van 1, 2, 4, ... threads. Plot deze in een grafiek.
5. Extra: Maak een ontwerp op papier voor een implementatie die gebruik maakt van een thread pool met worker threads (waarbij nieuwe threads worden aangeroepen vanuit binnen het algoritme)
6. Extra: **Maak een alternatieve implementatie die gebruik maakt van worker threads door in de recursieve aanroep nieuwe threads aan te maken. Geef een korte, intuitieve analyse, over hoe deze verschilt van de threadpool-implementatie. Heb je meer of minder overhead? Hoeveel threads maak je aan? etc. Let op, deze opdracht is zeer lastig. Doe dit alleen als je tijd over hebt.**

## Inleveren HPP Opdrachten
Voor de opdrachten van High Performance Programming lever je een verslag in, in PDF formaat.

Begin het verslag met:

- De titel van de opdracht;
- Je naam en studentnummer;
- Een link naar je GitHub Classroom repository met je werk.

- Lees de hele opdracht goed door, stel alvast vragen als iets niet duidelijk is
- Voor ieder deel / vraag:
  -  Vermeld het nummer van het deel of de vraag;
  -  Maak de opdracht en/of beantwoord de vraag;
  -  Kies code snippets en/of screenshots om je oplossing te laten zien;
  -  Beschrijf je oplossing beknopt, waarbij je vooral duidelijk maakt hoe je het hebt aangepakt.

Bewaar / exporteer je verslag als PDF en lever die in.
