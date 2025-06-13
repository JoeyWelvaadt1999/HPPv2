[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/XyNCPEUm)
# Bucket Sort

In deze opdracht gaan jullie Bucket Sort implementeren en analyseren. Het is voor deze opdracht belangrijk dat je de pseudocode volgt. Dit is een alternatieve implementatie van bucket sort en het heeft geen zin of werkt wellicht zelfs averechts om van het internet oplossingen op te zoeken.

De leeruitkomsten die bij deze opdracht horen zijn:
- een complexiteitsanalyse kan uitvoeren op een bestaande implementatie en/of algoritme;
- code kan optimaliseren m.b.t. de gebruikte algoritmiek;

Bucket Sort begint met een een-dimensionale array van n nummers die gesorteerd worden en een twee-dimensionale array van integers, met rijen genummerd van 0 tot 9 en kolommen geïndexeerd van 0 tot n-1. Elke rij in de twee-dimensionale array heeft een bucket.  Het algoritme werkt dan alsvolgt:

1. Plaats elke waarde van de een-dimensionale array in een rij van de bucket array, gebaseerd op het meest rechtse cijfer in het getal (de "een"-waarde). Bijvoorbeeld, 97 wordt geplaatst in rij 7, 3 wordt geplaatst in rij 3 en 100 wordt geplaatst in rij 0. Deze stap heet de distribution pass.
2. Loop door de bucket array rij voor rij, en kopieer de waardes terug in de originele array. Deze stap heet de gathering pass. De volgorde van de hierboven genoemde getallen is dus nu 100, 3, 97.
3. Herhaal dit proces voor elke volgende digit-positie (dus voor de tientallen, honderdtallen, etc.). Na de laatste gathering pass is de array gesorteerd.
4. Verzin een aanpassing als oplossing voor de negatieve getallen, maar maak ook hierbij gebruik van een distribution en gathering pass

## Opdracht

1. Schrijf een implementatie in C++ van het bovenstaande Bucket Sort algoritme voor het sorteren van gehele getallen
2. Bepaal op basis van een timing test, de Big O waarde van Bucket Sort. Exporteer hiervoor timing data uit je C++ code, en gebruik Python om de resultaten te visualiseren.
   Op Canvas staat voorbeeld-code waarin Bubble-sort getest wordt op een aantal lijsten van n random waardes en worden de resultaten als CSV geprint.
3. Benader (beredeneer) zo precies mogelijk de tijdscomplexiteit van je algoritme zoals we dit in OAI hebben gezien.
4. Extra: Maak je BucketSort implementatie ook geschikt voor het sorteren van getallen met cijfers achter de komma
5. Extra: Laat bij je analyse zien dat je onderzoek hebt gedaan over wat wel en niet mee te nemen in je analyse (is een variable assignment een stap? hoeveel ruimte kost je datastructuur?)
6. Extra: Overleg met je docent als je andere ideeën hebt of deze kunnen gelden als extra voor een hoger cijfer

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
