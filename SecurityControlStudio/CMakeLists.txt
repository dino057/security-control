cmake_minimum_required(VERSION 3.16)

project(SecurityControlStudio VERSION 1.0 LANGUAGES CXX)

# Qt Creator dobrze rozumie te ustawienia. AUTOMOC i AUTOUIC sprawiaja,
# ze CMake sam uruchamia narzedzia Qt potrzebne do klas z Q_OBJECT
# oraz do pliku mainwindow.ui.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# C++17 jest tutaj uzyty dlatego, ze jest standardowo wspierany przez
# nowe wersje Qt Creator. W kodzie celowo nie uzywam trudnych konstrukcji.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Projekt dziala z Qt 6 oraz z Qt 5. Qt Creator sam podstawi odpowiedni
# zestaw bibliotek z wybranego zestawu kompilatora.
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Gui Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Widgets)

add_executable(SecurityControlStudio
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui

    AlarmSystem.cpp
    AlarmSystem.h
    BuildingView.cpp
    BuildingView.h
    Building.cpp
    Building.h
    Door.cpp
    Door.h
    Employee.cpp
    Employee.h
    EventLog.cpp
    EventLog.h
    Intruder.cpp
    Intruder.h
    Guard.cpp
    Guard.h
    Person.cpp
    Person.h
    Room.cpp
    Room.h
    Sensor.cpp
    Sensor.h
    Simulation.cpp
    Simulation.h
)

target_include_directories(SecurityControlStudio PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(SecurityControlStudio PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Widgets
)


add_custom_command(TARGET SecurityControlStudio POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/mapa_mala.map
        $<TARGET_FILE_DIR:SecurityControlStudio>/mapa_mala.map
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_CURRENT_SOURCE_DIR}/mapa_duza.map
        $<TARGET_FILE_DIR:SecurityControlStudio>/mapa_duza.map
)
