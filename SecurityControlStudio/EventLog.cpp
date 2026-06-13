#include "EventLog.h"

#include <sstream>

EventLog::EventLog()
{
    // Pusty konstruktor - wektor 'entries' sam tworzy się jako pusty,
    // więc nie musimy mu tutaj nic dopisywać.
}

void EventLog::add(const std::string& message)
{
    // Funkcja push_back po prostu dokleja nowy komunikat na sam koniec naszej wektorowej listy.
    entries.push_back(message);
}

void EventLog::clear()
{
    // Funkcja clear() wbudowana w wektor usuwa wszystkie jego elementy i zmniejsza jego rozmiar do zera.
    entries.clear();
}

std::string EventLog::toString() const
{
    // Używamy znowu ostringstream - naszego "budowniczego tekstu", co jest wydajniejsze niż dodawanie stringów przy takiej ilości tekstu.
    std::ostringstream stream;

    for (std::size_t i = 0; i < entries.size(); ++i)
    {
        // Doklejamy każdą kolejną wiadomość z listy, a po niej dorzucamy znak nowej linii '\n'
        stream << entries[i] << '\n';
    }

    // Gdy cała historia jest już sklejona, zamieniamy ją w zwykły string i zwracamy do wyświetlenia na ekranie.
    return stream.str();
}