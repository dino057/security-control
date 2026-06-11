Security Control Studio - projekt Qt/CMake
========================================

Jak uruchomic w Qt Creatorze:
1. Otworz Qt Creator.
2. Wybierz File -> Open File or Project.
3. Wskaz plik CMakeLists.txt z tego katalogu.
4. Wybierz swoj zestaw kompilatora Qt.
5. Kliknij Configure Project.
6. Zbuduj i uruchom projekt.

Najwazniejsze pliki:
- mainwindow.ui - prosty interfejs graficzny Qt Designer.
- mainwindow.h / mainwindow.cpp - podpiecie przyciskow Start i Krok.
- BuildingView.h / BuildingView.cpp - rysowanie ukladu budynku, drzwi i ludzi.
- Simulation.h / Simulation.cpp - glowna logika programu i delete dla obiektow.
- Building.h / Building.cpp - kontener na pokoje, usuwa Room*.
- Person, Employee, Intruder - dziedziczenie i polimorfizm.
- Sensor, AlarmSystem - wykrywanie intruza i blokowanie drzwi.
- sample_building.map - przykladowy uklad budynku.

Uwagi do obrony:
- Person jest klasa abstrakcyjna, bo ma metode virtual bool hasAccess() = 0.
- Employee i Intruder dziedzicza po Person i inaczej odpowiadaja na hasAccess().
- Sensor trzyma wskaznik Person*, ale dzieki virtual wywoluje dobra wersje metody.
- Simulation jest glownym menedzerem pamieci dla Person*, Door* i Sensor*.
- Building jest wlascicielem Room* i usuwa je w destruktorze.
- Room nie usuwa drzwi, osob ani czujnika, bo tylko przechowuje ich adresy.
- BuildingView nie zmienia logiki symulacji. On tylko czyta dane z Simulation
  i rysuje pokoje, drzwi oraz osoby w oknie programu.
