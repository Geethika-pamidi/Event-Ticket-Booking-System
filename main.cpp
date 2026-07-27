#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>

using namespace std;

// ==================== DSA: EVENT STRUCTURE (Doubly Linked List) ====================
struct Event {
    int eventID;
    string eventName;
    string date;
    int availableSeats;
    Event* next;
    Event* prev;

    Event(int id, string name, string d, int seats) {
        eventID = id;
        eventName = name;
        date = d;
        availableSeats = seats;
        next = nullptr;
        prev = nullptr;
    }
};

class EventManager {
private:
    Event* head;

    void saveEvents() {
        ofstream fout("events.txt");
        Event* temp = head;
        while (temp != nullptr) {
            fout << temp->eventID << "|" << temp->eventName << "|" << temp->date << "|" << temp->availableSeats << "\n";
            temp = temp->next;
        }
        fout.close();
    }

public:
    EventManager() { head = nullptr; }

    void addEvent(int id, string name, string date, int seats, bool silent = false) {
        Event* newNode = new Event(id, name, date, seats);
        if (head == nullptr) {
            head = newNode;
        } else {
            Event* temp = head;
            while (temp->next != nullptr) temp = temp->next;
            temp->next = newNode;
            newNode->prev = temp;
        }
        saveEvents();
        if (!silent) cout << "\n[Success] Event '" << name << "' added successfully!\n";
    }

    void displayEvents() {
        if (head == nullptr) {
            cout << "\n-----------------------------------------\n";
            cout << "       NO UPCOMING EVENTS AVAILABLE      \n";
            cout << "-----------------------------------------\n";
            return;
        }
        Event* temp = head;
        cout << "\n==================================================\n";
        cout << "           AVAILABLE EVENTS FOR BOOKING           \n";
        cout << "==================================================\n";
        while (temp != nullptr) {
            cout << "Event ID       : " << temp->eventID << "\n";
            cout << "Event Name     : " << temp->eventName << "\n";
            cout << "Date           : " << temp->date << "\n";
            cout << "Available Seats: " << temp->availableSeats << "\n";
            cout << "--------------------------------------------------\n";
            temp = temp->next;
        }
    }

    Event* searchEvent(int id) {
        Event* temp = head;
        while (temp != nullptr) {
            if (temp->eventID == id) return temp;
            temp = temp->next;
        }
        return nullptr;
    }

    void loadEvents() {
        ifstream fin("events.txt");
        if (!fin) return;
        string name, date;
        string idStr, seatsStr;
        while (getline(fin, idStr, '|')) {
            int id = stoi(idStr);
            getline(fin, name, '|');
            getline(fin, date, '|');
            getline(fin, seatsStr);
            int seats = stoi(seatsStr);
            addEvent(id, name, date, seats, true);
        }
        fin.close();
    }
};

// ==================== DSA: TICKET BOOKING QUEUE ====================
struct TicketBooking {
    string customerName;
    int eventID;
    string ticketType; // VIP or Regular
    TicketBooking* next;

    TicketBooking(string name, int id, string type) {
        customerName = name;
        eventID = id;
        ticketType = type;
        next = nullptr;
    }
};

class BookingQueue {
private:
    TicketBooking* front;
    TicketBooking* rear;

    void saveBookings() {
        ofstream fout("bookings.txt");
        TicketBooking* temp = front;
        while (temp != nullptr) {
            fout << temp->customerName << "|" << temp->eventID << "|" << temp->ticketType << "\n";
            temp = temp->next;
        }
        fout.close();
    }

public:
    BookingQueue() { front = rear = nullptr; }

    void bookTicket(string name, int eventID, string type, EventManager &em, bool silent = false) {
        Event* ev = em.searchEvent(eventID);
        if (ev == nullptr) {
            if (!silent) cout << "\n[Error] Invalid Event ID! Booking failed.\n";
            return;
        }
        if (!silent && ev->availableSeats <= 0) {
            cout << "\n[Error] Sorry, this event is SOLD OUT!\n";
            return;
        }

        if (!silent) {
            ev->availableSeats--;
        }

        TicketBooking* newNode = new TicketBooking(name, eventID, type);
        
        // Priority Queue Logic: VIP jumps to front
        if (type == "VIP" && front != nullptr) {
            newNode->next = front;
            front = newNode;
        } else {
            if (rear == nullptr) {
                front = rear = newNode;
            } else {
                rear->next = newNode;
                rear = newNode;
            }
        }

        saveBookings();
        if (!silent) {
            cout << "\n[Success] Ticket Booked Successfully for " << name << " (" << type << ")\n";
            cout << "Remaining Seats for Event " << eventID << ": " << ev->availableSeats << "\n";
        }
    }

    void cancelBooking(string name, int eventID, EventManager &em) {
        if (front == nullptr) {
            cout << "\n[Error] No bookings found in queue.\n";
            return;
        }

        TicketBooking* temp = front;
        TicketBooking* prev = nullptr;
        bool found = false;

        while (temp != nullptr) {
            if (temp->customerName == name && temp->eventID == eventID) {
                found = true;
                break;
            }
            prev = temp;
            temp = temp->next;
        }

        if (!found) {
            cout << "\n[Error] Booking not found for " << name << " in Event ID " << eventID << "\n";
            return;
        }

        // Remove from queue
        if (temp == front) {
            front = front->next;
            if (front == nullptr) rear = nullptr;
        } else {
            prev->next = temp->next;
            if (temp == rear) rear = prev;
        }

        delete temp;
        saveBookings();

        // Restore seat (+1)
        Event* ev = em.searchEvent(eventID);
        if (ev != nullptr) {
            ev->availableSeats++;
        }

        cout << "\n[Success] Booking cancelled for " << name << ". Seat restored to Event " << eventID << "!\n";
    }

    void displayBookings() {
        if (front == nullptr) {
            cout << "\n-----------------------------------------\n";
            cout << "        NO ACTIVE BOOKINGS FOUND         \n";
            cout << "-----------------------------------------\n";
            return;
        }
        TicketBooking* temp = front;
        cout << "\n=========================================\n";
        cout << "         CURRENT BOOKING QUEUE           \n";
        cout << "=========================================\n";
        int i = 1;
        while (temp != nullptr) {
            cout << i++ << ". Name: " << temp->customerName 
                 << " | Event ID: " << temp->eventID 
                 << " | Type: " << temp->ticketType << "\n";
            temp = temp->next;
        }
    }

    void loadBookings(EventManager &em) {
        ifstream fin("bookings.txt");
        if (!fin) return;
        string name, type;
        string idStr;
        while (getline(fin, name, '|')) {
            getline(fin, idStr, '|');
            int id = stoi(idStr);
            getline(fin, type);
            bookTicket(name, id, type, em, true); // Load silently without re-decreasing seats unnecessarily
        }
        fin.close();
    }
};

// ==================== FRONTEND (CLI UI) ====================
void showHeader() {
    cout << "\n\t========================================\n";
    cout << "\t   EVENT TICKET BOOKING SYSTEM (DSA)\n";
    cout << "\t========================================\n";
}

int main() {
    EventManager eventManager;
    BookingQueue bookingQueue;

    // Load saved data from text files
    eventManager.loadEvents();
    bookingQueue.loadBookings(eventManager);

    // Add default events if none exist
    if (eventManager.searchEvent(101) == nullptr) {
        eventManager.addEvent(101, "Rock Music Concert", "25-Dec-2023", 5, true);
        eventManager.addEvent(102, "Tech Innovation Summit", "15-Jan-2024", 3, true);
    }

    int choice;
    do {
        showHeader();
        cout << "1. View All Events & Available Seats\n";
        cout << "2. Search for an Event by ID\n";
        cout << "3. Book a Ticket (Queue System)\n";
        cout << "4. Cancel a Ticket\n";
        cout << "5. View All Active Bookings in Queue\n";
        cout << "6. Add New Event (Admin)\n";
        cout << "7. Exit\n";
        cout << "----------------------------------------\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                eventManager.displayEvents();
                break;
            case 2: {
                int id;
                cout << "Enter Event ID to search: ";
                cin >> id;
                Event* ev = eventManager.searchEvent(id);
                if (ev != nullptr) {
                    cout << "\n[Found] Event: " << ev->eventName << " | Date: " << ev->date << " | Seats Left: " << ev->availableSeats << "\n";
                } else {
                    cout << "\n[Not Found] Event with ID " << id << " does not exist.\n";
                }
                break;
            }
            case 3: {
                string name, type;
                int id;
                eventManager.displayEvents();
                cout << "\nEnter Event ID to book: ";
                cin >> id;
                cin.ignore();
                cout << "Enter Customer Name: ";
                getline(cin, name);
                cout << "Enter Ticket Type (VIP/Regular): ";
                cin >> type;
                bookingQueue.bookTicket(name, id, type, eventManager);
                break;
            }
            case 4: {
                string name;
                int id;
                cout << "Enter Event ID for cancellation: ";
                cin >> id;
                cin.ignore();
                cout << "Enter Customer Name: ";
                getline(cin, name);
                bookingQueue.cancelBooking(name, id, eventManager);
                break;
            }
            case 5:
                bookingQueue.displayBookings();
                break;
            case 6: {
                int id, seats;
                string name, date;
                cout << "Enter Event ID: ";
                cin >> id;
                cin.ignore();
                cout << "Enter Event Name: ";
                getline(cin, name);
                cout << "Enter Date: ";
                getline(cin, date);
                cout << "Enter Total Seats: ";
                cin >> seats;
                eventManager.addEvent(id, name, date, seats);
                break;
            }
            case 7:
                cout << "\nExiting System. Data saved successfully. Goodbye!\n";
                break;
            default:
                cout << "\n[Error] Invalid Choice! Try again.\n";
        }
    } while (choice != 7);

    return 0;
}