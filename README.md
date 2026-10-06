# Student Expense Tracker

A console-based C++ application that helps students record and monitor their daily and monthly spending.

## Features

- Add expenses with date, category, description and amount
- View all saved expenses in a table
- Total spending
- Spending organised by category (Food, Travel, Books, Entertainment, Other)
- Monthly spending summary (enter `MM-YYYY`)
- Delete an expense
- Data is saved automatically to `expenses.txt`, so records stay after you close the program
- Input validation for dates, amounts and menu choices

## Concepts used

Arrays, structs, loops, functions, conditional statements, file handling (`ifstream` / `ofstream`), string handling, input/output.

## Build and run

```
g++ main.cpp -o expense_tracker
./expense_tracker
```

On Windows: `g++ main.cpp -o expense_tracker.exe` then `expense_tracker.exe`.

## Example

```
1. Add expense
Date (DD-MM-YYYY): 06-10-2026
Category: 1 (Food)
Description: Lunch
Amount (Rs.): 120
```

## Possible improvements

- Set a monthly budget and warn when it is crossed
- Sort expenses by date or amount
- Export to CSV
