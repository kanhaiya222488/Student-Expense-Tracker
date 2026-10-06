// Student Expense Tracker
// A console-based C++ application to record and manage daily student expenses.
// Expenses are saved to expenses.txt so they are available the next time you run it.

#include <iostream>
#include <fstream>
#include <iomanip>
#include <limits>
#include <string>
using namespace std;

const int MAX_EXPENSES = 500;
const int CATEGORY_COUNT = 5;
const string CATEGORIES[CATEGORY_COUNT] = {"Food", "Travel", "Books", "Entertainment", "Other"};
const string DATA_FILE = "expenses.txt";

struct Expense {
    string date;      // DD-MM-YYYY
    string category;
    string note;
    double amount;
};

Expense expenses[MAX_EXPENSES];
int expenseCount = 0;

// ---------------------------- Input helpers ----------------------------
int readInt(const string &prompt, int minVal, int maxVal) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value && value >= minVal && value <= maxVal) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "  Invalid input. Enter a number between " << minVal << " and " << maxVal << ".\n";
    }
}

double readAmount(const string &prompt) {
    double value;
    while (true) {
        cout << prompt;
        if (cin >> value && value > 0) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "  Invalid amount. Enter a number greater than 0.\n";
    }
}

bool isDigits(const string &s) {
    for (size_t i = 0; i < s.size(); i++) {
        if (s[i] < '0' || s[i] > '9') return false;
    }
    return !s.empty();
}

// Checks the format DD-MM-YYYY with sensible ranges.
bool isValidDate(const string &d) {
    if (d.length() != 10 || d[2] != '-' || d[5] != '-') return false;
    string dd = d.substr(0, 2), mm = d.substr(3, 2), yy = d.substr(6, 4);
    if (!isDigits(dd) || !isDigits(mm) || !isDigits(yy)) return false;
    int day = stoi(dd), month = stoi(mm), year = stoi(yy);
    return day >= 1 && day <= 31 && month >= 1 && month <= 12 && year >= 2000 && year <= 2100;
}

string readDate(const string &prompt) {
    string date;
    while (true) {
        cout << prompt;
        getline(cin, date);
        if (isValidDate(date)) return date;
        cout << "  Invalid date. Use DD-MM-YYYY (example: 06-10-2026).\n";
    }
}

string readNote(const string &prompt) {
    string note;
    cout << prompt;
    getline(cin, note);
    if (note.empty()) note = "-";
    for (size_t i = 0; i < note.size(); i++) {
        if (note[i] == '|') note[i] = '/';   // '|' is used as the file separator
    }
    return note;
}

// ------------------------------ File handling --------------------------
void saveToFile() {
    ofstream out(DATA_FILE.c_str());
    if (!out) {
        cout << "Could not save to file.\n";
        return;
    }
    for (int i = 0; i < expenseCount; i++) {
        out << expenses[i].date << "|" << expenses[i].category << "|"
            << expenses[i].note << "|" << expenses[i].amount << "\n";
    }
}

void loadFromFile() {
    ifstream in(DATA_FILE.c_str());
    if (!in) return;   // first run: no file yet
    string line;
    while (expenseCount < MAX_EXPENSES && getline(in, line)) {
        size_t p1 = line.find('|');
        size_t p2 = (p1 == string::npos) ? string::npos : line.find('|', p1 + 1);
        size_t p3 = (p2 == string::npos) ? string::npos : line.find('|', p2 + 1);
        if (p3 == string::npos) continue;   // skip damaged lines
        expenses[expenseCount].date = line.substr(0, p1);
        expenses[expenseCount].category = line.substr(p1 + 1, p2 - p1 - 1);
        expenses[expenseCount].note = line.substr(p2 + 1, p3 - p2 - 1);
        expenses[expenseCount].amount = atof(line.substr(p3 + 1).c_str());
        expenseCount++;
    }
}

// ------------------------------ Features -------------------------------
void addExpense() {
    if (expenseCount >= MAX_EXPENSES) {
        cout << "Expense storage is full.\n";
        return;
    }
    cout << "\n--- Add Expense ---\n";
    string date = readDate("Date (DD-MM-YYYY): ");

    cout << "Category:\n";
    for (int i = 0; i < CATEGORY_COUNT; i++) {
        cout << "  " << i + 1 << ". " << CATEGORIES[i] << "\n";
    }
    int cat = readInt("Choose category: ", 1, CATEGORY_COUNT);

    string note = readNote("Description: ");
    double amount = readAmount("Amount (Rs.): ");

    expenses[expenseCount].date = date;
    expenses[expenseCount].category = CATEGORIES[cat - 1];
    expenses[expenseCount].note = note;
    expenses[expenseCount].amount = amount;
    expenseCount++;
    saveToFile();
    cout << "Expense added.\n";
}

void displayExpenses() {
    if (expenseCount == 0) {
        cout << "\nNo expenses recorded yet.\n";
        return;
    }
    cout << "\n" << left << setw(5) << "No." << setw(13) << "Date" << setw(16) << "Category"
         << setw(22) << "Description" << "Amount\n";
    cout << string(66, '-') << "\n";
    for (int i = 0; i < expenseCount; i++) {
        cout << left << setw(5) << i + 1 << setw(13) << expenses[i].date << setw(16)
             << expenses[i].category << setw(22) << expenses[i].note << fixed << setprecision(2)
             << expenses[i].amount << "\n";
    }
}

void showTotal() {
    double total = 0;
    for (int i = 0; i < expenseCount; i++) total += expenses[i].amount;
    cout << "\nTotal spending: Rs. " << fixed << setprecision(2) << total << "\n";
}

void showByCategory() {
    if (expenseCount == 0) {
        cout << "\nNo expenses recorded yet.\n";
        return;
    }
    cout << "\n--- Spending by Category ---\n";
    for (int c = 0; c < CATEGORY_COUNT; c++) {
        double sum = 0;
        for (int i = 0; i < expenseCount; i++) {
            if (expenses[i].category == CATEGORIES[c]) sum += expenses[i].amount;
        }
        cout << left << setw(16) << CATEGORIES[c] << "Rs. " << fixed << setprecision(2) << sum << "\n";
    }
}

void showMonthly() {
    string key;
    while (true) {
        cout << "Enter month (MM-YYYY): ";
        getline(cin, key);
        if (key.length() == 7 && key[2] == '-' && isDigits(key.substr(0, 2)) && isDigits(key.substr(3, 4))) break;
        cout << "  Invalid format. Example: 10-2026\n";
    }
    double sum = 0;
    int count = 0;
    for (int i = 0; i < expenseCount; i++) {
        if (expenses[i].date.substr(3) == key) {
            sum += expenses[i].amount;
            count++;
        }
    }
    cout << "\nSpending in " << key << ": Rs. " << fixed << setprecision(2) << sum
         << " (" << count << " expenses)\n";
}

void deleteExpense() {
    if (expenseCount == 0) {
        cout << "\nNo expenses to delete.\n";
        return;
    }
    displayExpenses();
    int n = readInt("\nEnter the number to delete (0 to cancel): ", 0, expenseCount);
    if (n == 0) return;
    for (int i = n - 1; i < expenseCount - 1; i++) expenses[i] = expenses[i + 1];
    expenseCount--;
    saveToFile();
    cout << "Expense deleted.\n";
}

// -------------------------------- Main ---------------------------------
int main() {
    loadFromFile();
    int choice;
    cout << "=== Student Expense Tracker ===\n";
    do {
        cout << "\n1. Add expense\n2. View all expenses\n3. Total spending\n"
             << "4. Spending by category\n5. Monthly spending\n6. Delete an expense\n7. Exit\n";
        choice = readInt("Enter choice: ", 1, 7);
        switch (choice) {
            case 1: addExpense(); break;
            case 2: displayExpenses(); break;
            case 3: showTotal(); break;
            case 4: showByCategory(); break;
            case 5: showMonthly(); break;
            case 6: deleteExpense(); break;
            case 7: cout << "Goodbye!\n"; break;
        }
    } while (choice != 7);
    return 0;
}
