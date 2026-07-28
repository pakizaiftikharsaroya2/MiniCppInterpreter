#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <limits>
 
using namespace std;
 
struct Book {
    int id;
    string title;
    string author;
    bool issued;
    string issuedTo;
};
 
class Library {
private:
    vector<Book> books;
    const string fileName = "books.txt";
 
public:
    Library() {
        loadFromFile();
    }
 
    ~Library() {
        saveToFile();
    }
 
    void loadFromFile() {
        ifstream file(fileName);
        if (!file.is_open()) return;
 
        string line;
        while (getline(file, line)) {
            if (line.empty()) continue;
            stringstream ss(line);
            string idStr, title, author, issuedStr, issuedTo;
 
            getline(ss, idStr, '|');
            getline(ss, title, '|');
            getline(ss, author, '|');
            getline(ss, issuedStr, '|');
            getline(ss, issuedTo, '|');
 
            Book b;
            b.id = stoi(idStr);
            b.title = title;
            b.author = author;
            b.issued = (issuedStr == "1");
            b.issuedTo = issuedTo;
            books.push_back(b);
        }
        file.close();
    }
 
    void saveToFile() {
        ofstream file(fileName);
        for (auto &b : books) {
            file << b.id << "|" << b.title << "|" << b.author << "|"
                 << (b.issued ? "1" : "0") << "|" << b.issuedTo << "\n";
        }
        file.close();
    }
 
    int nextId() {
        int maxId = 0;
        for (auto &b : books) maxId = max(maxId, b.id);
        return maxId + 1;
    }
 
    void addBook() {
        Book b;
        b.id = nextId();
        cout << "Title: ";
        cin.ignore();
        getline(cin, b.title);
        cout << "Author: ";
        getline(cin, b.author);
        b.issued = false;
        b.issuedTo = "";
        books.push_back(b);
        cout << "Added \"" << b.title << "\" with id " << b.id << "\n";
    }
 
    void listBooks() {
        if (books.empty()) {
            cout << "No books yet.\n";
            return;
        }
        cout << "\nID  Title                          Author               Status\n";
        cout << "---------------------------------------------------------------\n";
        for (auto &b : books) {
            cout << b.id << "\t" << b.title << "\t" << b.author << "\t"
                 << (b.issued ? ("Issued to " + b.issuedTo) : "Available") << "\n";
        }
        cout << "\n";
    }
 
    Book* findBook(int id) {
        for (auto &b : books) {
            if (b.id == id) return &b;
        }
        return nullptr;
    }
 
    void issueBook() {
        int id;
        cout << "Book ID to issue: ";
        cin >> id;
        Book* b = findBook(id);
        if (!b) {
            cout << "No book with that id.\n";
            return;
        }
        if (b->issued) {
            cout << "Already issued to " << b->issuedTo << "\n";
            return;
        }
        cout << "Issue to (member name): ";
        cin.ignore();
        getline(cin, b->issuedTo);
        b->issued = true;
        cout << "Issued \"" << b->title << "\" to " << b->issuedTo << "\n";
    }
 
    void returnBook() {
        int id;
        cout << "Book ID to return: ";
        cin >> id;
        Book* b = findBook(id);
        if (!b) {
            cout << "No book with that id.\n";
            return;
        }
        if (!b->issued) {
            cout << "That book isn't issued out.\n";
            return;
        }
        cout << b->issuedTo << " returned \"" << b->title << "\"\n";
        b->issued = false;
        b->issuedTo = "";
    }
 
    void searchBook() {
        cout << "Search by title (partial ok): ";
        cin.ignore();
        string query;
        getline(cin, query);
 
        bool found = false;
        for (auto &b : books) {
            if (b.title.find(query) != string::npos) {
                cout << b.id << " - " << b.title << " by " << b.author
                     << (b.issued ? (" (out with " + b.issuedTo + ")") : " (available)") << "\n";
                found = true;
            }
        }
        if (!found) cout << "Nothing matched.\n";
    }
 
    void deleteBook() {
        int id;
        cout << "Book ID to remove: ";
        cin >> id;
        for (size_t i = 0; i < books.size(); i++) {
            if (books[i].id == id) {
                cout << "Removed \"" << books[i].title << "\"\n";
                books.erase(books.begin() + i);
                return;
            }
        }
        cout << "No book with that id.\n";
    }
};
 
void showMenu() {
    cout << "==== Library Menu ====\n";
    cout << "1. Add book\n";
    cout << "2. List all books\n";
    cout << "3. Issue book\n";
    cout << "4. Return book\n";
    cout << "5. Search by title\n";
    cout << "6. Delete book\n";
    cout << "0. Exit\n";
    cout << "Choice: ";
}
 
int main() {
    Library lib;
    int choice;
 
    while (true) {
        showMenu();
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
 
        switch (choice) {
            case 1: lib.addBook(); break;
            case 2: lib.listBooks(); break;
            case 3: lib.issueBook(); break;
            case 4: lib.returnBook(); break;
            case 5: lib.searchBook(); break;
            case 6: lib.deleteBook(); break;
            case 0:
                lib.saveToFile();
                cout << "Saved. Bye!\n";
                return 0;
            default:
                cout << "Not a valid option.\n";
        }
        cout << "\n";
    }
}