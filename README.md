# Event-Ticket-Booking-System
A full-stack event ticket booking system utilizing DSA principles.
# 🎟️ Event Ticket Booking System

A full-stack Event Ticket Booking System featuring a **C++ Data Structures & Algorithms (DSA)** backend and a modern **HTML/CSS/JavaScript Frontend**.

---

## 🚀 Features
- **Browse Events:** View all upcoming events with real-time seat tracking.
- **Queue Management (DSA):** Implements a Queue (FIFO) for bookings, with a **Priority Queue mechanism for VIP tickets** (VIPs automatically jump to the front of the queue).
- **Seat Inventory Management:** Automatically decrements seats on booking and restores seats upon cancellation.
- **Admin Console:** Allows administrators to publish new events instantly.
- **Persistence:** Data is managed dynamically through system state and file storage.

---

## 🧠 Data Structures & Algorithms Used
1. **Doubly Linked List:** Used in the C++ backend to dynamically store, add, and manage events chronologically.
2. **Queue / Priority Queue:** Used to handle ticket bookings and user entry order.

---

## 🛠️ Tech Stack
- **Backend:** C++ (Pointers, Structs, Linked Lists, Queues, File Handling)
- **Frontend:** HTML5, CSS3, JavaScript (Local Storage State Management)

---

## 🏃‍♂️ How to Run Locally
1. Clone the repository or download the files.
2. Compile and run the C++ backend (`main.cpp`) in VS Code using:
   ```bash
   g++ main.cpp -o event_system
   ./event_system
