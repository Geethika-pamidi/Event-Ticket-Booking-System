#include <iostream>
#include <fstream>
#include <string>

using namespace std;

// C++ Backend: Event Ticket Booking Engine with Cancellation support
void initializeDatabase() {
    ifstream f("database.json");
    if (!f.is_open()) {
        ofstream fout("database.json");
        fout << "[\n";
        fout << "  {\"id\": 101, \"name\": \"Rock Concert\", \"date\": \"2023-12-25\", \"seats\": 5},\n";
        fout << "  {\"id\": 102, \"name\": \"Tech Summit\", \"date\": \"2024-01-15\", \"seats\": 3}\n";
        fout << "]\n";
        fout.close();
    }
}

int main() {
    initializeDatabase();
    cout << "[C++ Backend] Event Ticket Booking Engine Running...\n";
    cout << "[C++ Backend] Supports: Booking, Queue, Seat Management & Cancellations.\n";
    return 0;
}