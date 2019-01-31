/*
 * Description: Catalog Database storing the information of 100 books and allowing the user to display/edit that information.
 * Input: cs202 - a txt file of book information to populate the database.
 * Output: cs202 - a txt file to reflect the changes made to the database by the user.
 */

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>

using namespace std;

const int ID = 0;
const int TITLE = 1;

// changes made to database
int changes = 0;

struct Book {
    long id;
    string title;
    string name;
    string lname;
    int pages;
};

struct Catalog {
    int stock_number;
    double price;
    Book book;
};

// function declarations
int searchHelper (Catalog database[], int numBooks, int specifier);
void add (Catalog database[], int& numBooks);
void sort (Catalog database[], int numBooks);
int maxDigits(Catalog database[], int numBooks, string varType);
void search (Catalog database[], int numBooks);
void remove (Catalog database[], int& numBooks);
void update (Catalog database[], int numBooks);
void print (Catalog database[], int numBooks);

/*
 * function_identifier: populates database array of structs, creates main program loop,
                        and calls appropriate functions, depending on user selection.
 * parameters: none
 * return value: 0
 */

int main() {
    const int ELEMENTS_IN_DB = 100;
    int totalBooks = 100;
    
    bool loop = true;
    int code;
    
    // declaring an array of 100 structs to serve as database
    Catalog database[ELEMENTS_IN_DB];
    
    // temporary string variables to store information from input file
    string line, price, stock_number, id, empty, empty2, title, name, lname, pages;
    
    ifstream infile;
    infile.open("cs202");
    
    // looping through input file and storing each value in a string
    for (int i = 0; i < totalBooks; i++) {
        // getline is used with appropriate delimeters for each data input value
        getline(infile, line, '\n');
        stringstream ss;
        ss << line;
        
        getline(ss, price, ',');
        getline(ss, stock_number, ',');
        getline(ss, id, ',');
        // 'empty' variables store empty characters when parsing title due to
        // combination of commas and quotation marks within title
        getline(ss, empty, '"');
        getline(ss, title, '"');
        getline(ss, empty2, ',');
        getline(ss, name, ',');
        getline(ss, lname, ',');
        getline(ss, pages, ',');
        
        // add quotation marks back to title
        title = "\"" + title + "\"";
        
        // conversion from string to appropriate data types and storage into struct
        database[i].price = atof(price.c_str());
        database[i].stock_number = atoi(stock_number.c_str());
        database[i].book.id = atol(id.c_str());
        database[i].book.title = title;
        database[i].book.name = name;
        database[i].book.lname = lname;
        database[i].book.pages = atoi(pages.c_str());
        
    }
    
    // infinite loop governed by loop variable
    while (loop) {
        cout << endl;
        cout << "Catalog Database" << endl;
        cout << "================" << endl;
        cout << "(1) Add a book" << endl;
        cout << "(2) Search a book" << endl;
        cout << "(3) Remove a book" << endl;
        cout << "(4) Sort database by" << endl;
        cout << "(5) Print all books" << endl;
        cout << "(6) Update database" << endl;
        cout << "(7) exit program" << endl;
        cout << "Enter operation code: ";
        cin >> code;
        
        while (cin.fail() || !(code>0 && code<8)) {
            cout << endl;
            cin.clear();
            cin.ignore(100, '\n');
            cout << "ERROR: Please enter a valid code (1-7): ";
            cin >> code;
        }
        
        switch (code) {
            case 1:
                add(database, totalBooks);
                break;
            case 2:
                search(database, totalBooks);
                break;
            case 3:
                remove(database, totalBooks);
                break;
            case 4:
                sort(database, totalBooks);
                break;
            case 5:
                print(database, totalBooks);
                break;
            case 6:
                update(database, totalBooks);
                break;
            case 7:
                cout << "Changes made to the database: " << changes << endl;
                cout << "writing changes to cs202.txt.." << endl;
                
                ofstream outfile;
                outfile.open("cs202");
                
                // ensures file is empty
                outfile << "";
                
                for (int i = 0; i < totalBooks; i++) {
                    // if not last item, include new line
                    if (i != (totalBooks-1)) {
                    outfile << database[i].price << "," << database[i].stock_number << "," << database[i].book.id << "," << database[i].book.title << "," << database[i].book.name << "," << database[i].book.lname << "," << database[i].book.pages << endl;
                    } else {
                    // don't include the new line if it's the last item
                        outfile << database[i].price << "," << database[i].stock_number << "," << database[i].book.id << "," << database[i].book.title << "," << database[i].book.name << "," << database[i].book.lname << "," << database[i].book.pages;
                    }
                }
                outfile.close();
                infile.close();
                cout << "done..." << endl << "MAY THE FORCE BE WITH YOU" << endl;
                
                // end main loop
                loop = false;
                break;
        }
    }
    return 0;
}

/*
 * function_identifier: asks user what to sort by, converts that data into a string, and
                        performs bubble sort of database with respect to user's choice.
 * parameters: database struct array, total number of books in the database
 * return value: none
 */

void sort (Catalog database[], int numBooks) {
    char code;
    const int ELEMENTS_IN_DB = 100;
    
    // initially empty array of either prices, stock numbers, or titles
    string sortBy[ELEMENTS_IN_DB] = {};
    
    // bubble sort variables
    Catalog tempStruct;
    string tempString;
    int bubbleEnd = 0;
    bool swap = true;
    
    cout << " (p) Sort database by price" << endl;
    cout << " (n) Sort database by stock number" << endl;
    cout << " (t) Sort database by title" << endl;
    cout << "Enter operation code: ";
    cin >> code;
    
    // input validation
    code = tolower(code);
    while (cin.fail() || !(code == 'p' || code == 'n' || code == 't')) {
        cout << endl;
        cin.clear();
        cin.ignore(100, '\n');
        cout << "ERROR: Please enter a valid option: ";
        cin >> code;
    }
    
    // the variable the user chooses to sort by is converted into a string
    // and stored into an array that will govern the bubble sort on the database
    if (code == 'p') {
        
        // formatting the price as a string by adding spaces (which have lower ascii value than numbers)
        // where they are needed for propper string comparison
        for (int i = 0; i<numBooks; i++) {
            stringstream ss;
            ss << database[i].price;
            string inputBefore;
            string inputAfter;
            string spacesBefore;
            string spacesAfter;
            string price;
            getline(ss, inputBefore, '.');
            
            // adding spaces to prices before the decimal if they have less digits than
            // the price with the maximum number of digits
            for (int j = 0; j < maxDigits(database, numBooks, "priceBeforeDec") - inputBefore.length(); j++) {
                spacesBefore = " " + spacesBefore;
            }

            // stores new string into the array
            inputBefore = spacesBefore + inputBefore;
            
            getline(ss, inputAfter);
            
            // adding spaces to prices after the decimal if they have less digits than
            // the price with the maximum number of digits after the decimal
            for (int j = 0; j < maxDigits(database, numBooks, "priceAfterDec") - inputAfter.length(); j++) {
                spacesAfter = " " + spacesAfter;
            }
            
            //adding the final string together and storing it into the array governing the bubble sort
            inputAfter = inputAfter + spacesAfter;
            price = inputBefore + "." + inputAfter;
            sortBy[i] = price;
        }
    } else if (code == 'n') {
        for (int i = 0; i<numBooks; i++) {
            stringstream ss;
            ss << database[i].stock_number;
            string input;
            string spaces;
            input = ss.str();

            // adds spaces to any stock number that has less digits than maximum stock number
            for (int j = 0; j < maxDigits(database, numBooks, "stock_number") - input.length(); j++) {
                spaces = " " + spaces;
            }

            // stores new string into the array
            input = spaces + input;
            sortBy[i] = input;
        }
    } else {
        for (int i = 0; i<numBooks; i++) {
            stringstream ss;
            ss << database[i].book.title;
            sortBy[i] = ss.str();
        }
    }
    
    // Bubble sort begins
    while (swap) {
        swap = false;
        bubbleEnd++;
        for (int i = 0; i < numBooks-bubbleEnd; i++) {
            // string comparison governs conditional
            if (sortBy[i].compare(sortBy[i+1]) > 0) {
                // selected variable governs the bubble sort performed on all elements of database
                tempStruct = database[i+1];
                tempString = sortBy[i+1];
                database[i+1] = database[i];
                sortBy[i+1] = sortBy[i];
                database[i] = tempStruct;
                sortBy[i] = tempString;
                
                swap = true;
            }
        }
    }
    cout << "Database was successfully sorted" << endl;
}

/*
 * function_identifier: goes through database struct array and searches for index of book user entered
 * parameters: database struct array, total number of books in the database, title or id constant
 * return value: index of book or -1 if book wasn't found
 */

int searchHelper (Catalog database[], int numBooks, int specifier) {
    long id;
    string title;
    
    // returning index when found if element is found by either id or title
    if (specifier == 0) {
        cout << "Enter id: ";
        cin >> id;
        //check that there are only n characters in the number as well
        while (cin.fail()) {
            cin.clear();
            cin.ignore(100, '\n');
            cout << "ERROR: Please enter a valid, numerical id: ";
            cin >> id;
        }
        for (int i = 0; i < numBooks; i++) {
            if (database[i].book.id == id)
                return i;
        }
    } else {
        cout << "Enter title: ";
        cin.ignore(100, '\n');
        getline(cin, title);
        for (int j = 0; j < numBooks; j++) {
            if (database[j].book.title == title)
                return j;
        }
    }
    
    // no book was found by either title or id
    return -1;
}

/*
 * function_identifier: prompts user for book to add and either increments that
                        book's stock number if the book is already part of database or
                        adds a new entry if it is a unique book and there is space available.
 * parameters: database struct array, total number of books in the database
 * return value: none
 */

void add (Catalog database[], int& numBooks) {
    long id;
    double price;
    string title;
    string name;
    string lname;
    int pages;
    // initially assuming index isn't found
    int index = -1;
    
    cout << "Enter id: ";
    cin >> id;
    
    // input validation
    while (cin.fail()) {
        cout << endl;
        cin.clear();
        cin.ignore(100, '\n');
        cout << "ERROR: Please enter a valid, numerical id: ";
        cin >> id;
    }
    
    // if index is found, -1 becomes replaced with index
    for (int i = 0; i < numBooks; i++) {
        if (database[i].book.id == id)
            index = i;
    }
    
    // if the book isnt found and there are too many books in the database, return prematurely
    if (index == -1 && numBooks >= 100) {
        cout << "ERROR: The database is full. You may not add a new book, try adding to an existing one instead." << endl;
        cout << endl;
        return;
    }
    
    cout << "Enter title: ";
    cin.clear();
    cin.ignore(100, '\n');
    getline(cin, title);
    cout << "Enter author: ";
    cin >> name >> lname;
    cout << "Enter number of pages: ";
    cin >> pages;
    
    // book was found but database is full, so existing book's stock number is incremented
    if (index != -1) {
        database[index].stock_number++;
        changes++;
        cout << "Book successfully added. The total amount in stock is " << database[index].stock_number << endl;
        
    // book wasn't found and database isn't full, so new book can be added
    } else {
        cout << "Enter the price: ";
        cin >> price;
        database[numBooks].price = price;
        database[numBooks].stock_number = 1;
        database[numBooks].book.id = id;
        database[numBooks].book.title = title;
        database[numBooks].book.name = name;
        database[numBooks].book.lname = lname;
        database[numBooks].book.pages = pages;
        
        // increase total number of items (books in database by 1)
        numBooks++;
        changes++;
        cout << "New book successfully added. It is now the " << numBooks << "th" << " in the database" << endl;
    }
    
}

/*
 * function_identifier: prompts user to search by id or title
                        and displays information about that book, if found.
 * parameters: database struct array, total number of books in the database
 * return value: none
 */

void search (Catalog database[], int numBooks) {
    char code;
    int bookIndex;
    
    cout << " (i) Search book by id" << endl;
    cout << " (t) Search book by title" << endl;
    cout << "Enter operation code: ";
    cin >> code;
    
    // input validation
    while (cin.fail() || !(code == 'i' || code == 't')) {
        cout << endl;
        cin.clear();
        cin.ignore(100, '\n');
        cout << "ERROR: Please enter a valid option: ";
        cin >> code;
    }
    
    if (code == 'i')
        bookIndex = searchHelper(database, numBooks, ID);
    else
        bookIndex = searchHelper(database, numBooks, TITLE);
    
    if (bookIndex != -1) {
        cout << endl;
        cout << "==================== Database ====================" << endl;
        cout << "price: " << "$" << database[bookIndex].price << endl;
        cout << "stock number: " << database[bookIndex].stock_number << endl;
        cout << "id: " << database[bookIndex].book.id << endl;
        cout << "title: " << database[bookIndex].book.title << endl;
        cout << "number of pages: " << database[bookIndex].book.pages << endl;
        cout << "author: " << database[bookIndex].book.name << " " << database[bookIndex].book.lname << endl;
        cout << "==================================================" << endl;
    } else {
        cout << "ERROR: No record found." << endl;
    }
}

/*
 * function_identifier: loops through all stock numbers to find which number has the most digits.
                        Used to aid in string comparison performed in the sort function.
 * parameters:  database struct array,
                total number of books in the database,
                the variable to calculate digits of.
 * return value: maximum number of digits in the variable the user is requesting to check
 */

int maxDigits(Catalog database[], int numBooks, string varType) {
    int max = 0;
    string input;
    
    if (varType == "stock_number") {
        for (int i = 0; i < numBooks; i++) {
            // stock number is converted to string to be analyzed as array of characters
            stringstream ss;
            ss << database[i].stock_number;
            input = ss.str();
            
            if (input.length() > max) {
                max = input.length();
            }
        }
    } else if (varType == "priceBeforeDec") {
        for (int i = 0; i < numBooks; i++) {
            // price before decimal is converted to string to be analyzed as array of characters
            stringstream ss;
            string beforeDec;
            ss << database[i].price;
            
            // gets all information before the decimal
            getline(ss, beforeDec, '.');
            if (beforeDec.length() > max) {
                max = beforeDec.length();
            }
        }
    } else if (varType == "priceAfterDec") {
        for (int i = 0; i < numBooks; i++) {
            // price after decimal is converted to string to be analyzed as array of characters
            stringstream ss;
            string beforeDec;
            string afterDec;
            ss << database[i].price;
            getline(ss, beforeDec, '.');
            
            // information after the decimal is used
            getline(ss, afterDec);
            if (afterDec.length() > max) {
                max = afterDec.length();
            }
        }
    }
    return max;
}

/*
 * function_identifier: prompts user to remove a book by id or title.
                        If the book has more than 1 copy, removes 1 copy,
                        otherwise removes that instance of the book and shifts
                        all other books up by 1.
 * parameters: database struct array, total number of books in the database
 * return value: none
 */

void remove (Catalog database[], int& numBooks) {
    char code;
    int bookIndex;
    
    cout << " (i) Remove book by id" << endl;
    cout << " (t) Remove book by title" << endl;
    
    cout << "Enter operation code: ";
    cin >> code;
    cin.clear();
    
    while (cin.fail() || !(code == 'i' || code == 't')) {
        cout << endl;
        cin.clear();
        cin.ignore(100, '\n');
        cout << "ERROR: Please enter a valid option: ";
        cin >> code;
    }
    
    if (code == 'i') {
        bookIndex = searchHelper(database, numBooks, ID);
    } else {
        bookIndex = searchHelper(database, numBooks, TITLE);
    }
    
    if (bookIndex == -1 && code == 'i') {
        cout << "ERROR: Invalid id, no record found." << endl;
        return;
    } else if (bookIndex == -1 && code == 't') {
        cout << "ERROR: Invalid title, no record found." << endl;
        return;
    } else if (bookIndex >= 0) {
        if (database[bookIndex].stock_number > 1) {
            database[bookIndex].stock_number -= 1;
            changes++;
            cout << "Book successfully removed. The total amount in stock is " << database[bookIndex].stock_number << endl;
        } else {
            // decrement total items in the database
            numBooks -= 1;
            
            // shifting all of the indexes up by one
            for (int i = bookIndex; i < numBooks; i++) {
                database[i] = database[i+1];
            }
            
            changes++;
            cout << "Last copy of this book was removed. There are now " << numBooks << " books in the database" << endl;
        }
    }
}

/*
 * function_identifier: prompts user to search for a book by id or title
                        and update that book's information if it is found.
 * parameters: database struct array, total number of books in the database
 * return value: none
 */

void update (Catalog database[], int numBooks) {
    char code;
    char subcode;
    int bookIndex;
    string price;
    int stock_number;
    string title;
    int pages;
    
    cout << " (i) Search book by id" << endl;
    cout << " (t) Search book by title" << endl;
    cout << "Enter operation code: ";
    cin >> code;
    
    // input validation
    while (cin.fail() || !(code == 'i' || code == 't')) {
        cout << endl;
        cin.clear();
        cin.ignore(100, '\n');
        cout << "ERROR: Please enter a valid option: ";
        cin >> code;
    }
    
    if (code == 'i') {
        bookIndex = searchHelper(database, numBooks, ID);
    } else {
        bookIndex = searchHelper(database, numBooks, TITLE);
    }
    
    if (bookIndex == -1 && code == 'i') {
        cout << "ERROR: Invalid id, no record found." << endl;
        return;
    } else if (bookIndex == -1 && code == 't') {
        cout << "ERROR: Invalid title, no record found." << endl;
        return;
    } else {
        cout << endl;
        cout << " (p) Update price" << endl;
        cout << " (s) Update stock number" << endl;
        cout << " (t) Update title" << endl;
        cout << " (n) Update number of pages" << endl;
        cout << "Enter operation code: ";
        cin >> subcode;
        cout << endl;
        
        // input validation
        while (cin.fail() || !(subcode == 'p' || subcode == 's' || subcode == 't' || subcode == 'n')) {
            cout << endl;
            cin.clear();
            cin.ignore(100, '\n');
            cout << "ERROR: Please enter a valid code: ";
            cin >> subcode;
        }
        
        switch (subcode) {
            case 'p': {
                cout << "Enter price: ";
                cin >> price;
                
                // input validation
                while (cin.fail()) {
                    cout << endl;
                    cin.clear();
                    cin.ignore(100, '\n');
                    cout << "ERROR: Please enter a number: ";
                    cin >> price;
                }

                string beforedecimal;
                
                // add a '.' if user's price doesn't include one
                // necessary for string comparison in bubble sort
                if (price[price.length()-1] != '.') {
                    price = price + ".0";
                }
                
                // store double back into price
                database[bookIndex].price = atof(price.c_str());
                changes++;
                break;
            }
            case 's':
                cout << "Enter stock number: ";
                cin >> stock_number;
                
                // input validation
                while (cin.fail()) {
                    cout << endl;
                    cin.clear();
                    cin.ignore(100, '\n');
                    cout << "ERROR: Please enter a number: ";
                    cin >> stock_number;
                }
                
                database[bookIndex].stock_number = stock_number;
                changes++;
                break;
            case 't':
                cout << "Enter title: ";
                cin >> title;
                database[bookIndex].book.title = title;
                changes++;
                break;
            case 'n':
                cout << "Enter pages: ";
                cin >> pages;
                
                // input validation
                while (cin.fail()) {
                    cout << endl;
                    cin.clear();
                    cin.ignore(100, '\n');
                    cout << "ERROR: Please enter a number: ";
                    cin >> pages;
                }
                
                database[bookIndex].book.pages = pages;
                changes++;
                break;
        }
        cout << "Database was successfully updated." << endl;
    }
}

/*
 * function_identifier: prints all of the books, reflecting changes made by the user
 * parameters: database struct array, total number of books in the database
 * return value: none
 */

void print (Catalog database[], int numBooks) {
    cout << "==================== Database ====================" << endl;
    for (int i = 0; i < numBooks; i++) {
        cout << "price: " << "$" << database[i].price << endl;
        cout << "stock number: " << database[i].stock_number << endl;
        cout << "id: " << database[i].book.id << endl;
        cout << "title: " << database[i].book.title << endl;
        cout << "number of pages: " << database[i].book.pages << endl;
        cout << "author: " << database[i].book.name << " " << database[i].book.lname << endl;
        cout << "==================================================" << endl;
        
    }
}
