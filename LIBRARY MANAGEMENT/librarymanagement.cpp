#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

// Class to represent a book
class Book {
public:
    string title;
    string author;
    int id;
    bool available; // true if available, false if checked out

    Book(string t, string a, int i) : title(t), author(a), id(i), available(true) {}
};

// Class to represent a user
class User {
public:
    string username;
    string password;

    User(string u, string p) : username(u), password(p) {}
};

// Function declarations
void displayMenu();
void addBook(vector<Book>& library);
void displayBooks(const vector<Book>& library);
void checkoutBook(vector<Book>& library);
void returnBook(vector<Book>& library);
void addUser(vector<User>& users);
int authenticateUser(const vector<User>& users);

int main() {
    vector<Book> library;
    vector<User> users;
    int currentUser = -1; // No user logged in initially
    int choice;

    // Load data from files
    ifstream booksFile("books.txt");
    ifstream usersFile("users.txt");

    if (booksFile.is_open() && usersFile.is_open()) {
        while (!booksFile.eof()) {
            string title, author;
            int id;
            booksFile >> id >> ws; // Consume whitespace
            getline(booksFile, title);
            getline(booksFile, author);
            library.push_back(Book(title, author, id));
        }

        while (!usersFile.eof()) {
            string username, password;
            usersFile >> ws; // Consume whitespace
            getline(usersFile, username);
            getline(usersFile, password);
            users.push_back(User(username, password));
        }

        booksFile.close();
        usersFile.close();
    }

    do {
        if (currentUser == -1) {
            cout << "\n===== Welcome to the Library Management System =====\n";
            cout << "1. Log In\n";
            cout << "2. Add User\n";
            cout << "3. Exit\n";
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
                case 1:
                    currentUser = authenticateUser(users);
                    break;
                case 2:
                    addUser(users);
                    break;
                case 3:
                    // Save data to files
                    {
                        ofstream booksOutput("books.txt");
                        ofstream usersOutput("users.txt");

                        for (const Book& book : library) {
                            booksOutput << book.id << endl << book.title << endl << book.author << endl;
                        }

                        for (const User& user : users) {
                            usersOutput << user.username << endl << user.password << endl;
                        }
                    }
                    cout << "Exiting the program. Goodbye!\n";
                    break;
                default:
                    cout << "Invalid choice. Please enter a valid option.\n";
            }
        } else {
            displayMenu();
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
                case 1:
                    addBook(library);
                    break;
                case 2:
                    displayBooks(library);
                    break;
                case 3:
                    checkoutBook(library);
                    break;
                case 4:
                    returnBook(library);
                    break;
                case 5:
                    currentUser = -1; // Log out the current user
                    break;
                case 6:
                    // Save data to files
                    {
                        ofstream booksOutput("books.txt");
                        for (const Book& book : library) {
                            booksOutput << book.id << endl << book.title << endl << book.author << endl;
                        }
                    }
                    cout << "Exiting the program. Goodbye!\n";
                    break;
                default:
                    cout << "Invalid choice. Please enter a valid option.\n";
            }
        }
    } while (choice != 6);

    return 0;
}

void displayMenu() {
    cout << "\n===== Library Management System =====\n";
    cout << "1. Add a book\n";
    cout << "2. Display all books\n";
    cout << "3. Checkout a book\n";
    cout << "4. Return a book\n";
    cout << "5. Log Out\n";
    cout << "6. Exit\n";
}
void addUser(vector<User>& users) {
    string username, password;
    cout << "Enter username: ";
    cin.ignore(); // Clear the input buffer
    getline(cin, username);
    cout << "Enter password: ";
    getline(cin, password);
    users.push_back(User(username, password));
    cout << "User added successfully!\n";
}

void addBook(vector<Book>& library) {
    string title, author;
    cout << "Enter book title: ";
    cin.ignore(); // Clear the input buffer
    getline(cin, title);
    cout << "Enter author: ";
    getline(cin, author);
    library.push_back(Book(title, author, library.size() + 1));
    cout << "Book added successfully!\n";
}

void displayBooks(const vector<Book>& library) {
    if (library.empty()) {
        cout << "The library is empty.\n";
        return;
    }

    cout << "\n===== List of Books =====\n";
    for (const Book& book : library) {
        cout << "ID: " << book.id << endl;
        cout << "Title: " << book.title << endl;
               cout << "Author: " << book.author << endl;
        cout << "Availability: " << (book.available ? "Available" : "Checked Out") << endl;
        cout << "------------------------\n";
    }
}

void checkoutBook(vector<Book>& library) {
    if (library.empty()) {
        cout << "The library is empty. Cannot check out a book.\n";
        return;
    }

    int bookId;
    cout << "Enter the ID of the book you want to check out: ";
    cin >> bookId;

    if (bookId < 1 || bookId > library.size()) {
        cout << "Invalid book ID. Please enter a valid ID.\n";
        return;
    }

    if (library[bookId - 1].available) {
        library[bookId - 1].available = false; // Mark as checked out
        cout << "Book \"" << library[bookId - 1].title << "\" checked out successfully!\n";
    } else {
        cout << "Sorry, the book \"" << library[bookId - 1].title << "\" is already checked out.\n";
    }
}

void returnBook(vector<Book>& library) {
    if (library.empty()) {
        cout << "The library is empty. Cannot return a book.\n";
        return;
    }

    int bookId;
    cout << "Enter the ID of the book you want to return: ";
    cin >> bookId;

    if (bookId < 1 || bookId > library.size()) {
        cout << "Invalid book ID. Please enter a valid ID.\n";
        return;
    }

    if (!library[bookId - 1].available) {
        library[bookId - 1].available = true; // Mark as available
        cout << "Book \"" << library[bookId - 1].title << "\" returned successfully!\n";
    } else {
        cout << "Invalid operation. The book \"" << library[bookId - 1].title << "\" is already available.\n";
    }
}

int authenticateUser(const vector<User>& users) {
    string username, password;
    cout << "Enter username: ";
    cin.ignore(); // Clear the input buffer
    getline(cin, username);
    cout << "Enter password: ";
    getline(cin, password);

    for (int i = 0; i < users.size(); ++i) {
        if (users[i].username == username && users[i].password == password) {
            cout << "Authentication successful. Welcome, " << username << "!\n";
            return i; // Return the index of the authenticated user
        }
    }

    cout << "Authentication failed. Invalid username or password.\n";
    return -1; // Return -1 if authentication fails
}