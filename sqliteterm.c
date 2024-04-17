#include <ncurses.h>
#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TABLES 100
#define MAX_TABLE_NAME_LENGTH 50
#define MAX_COLUMNS 50

// Global variables
sqlite3 *db;
char tables[MAX_TABLES][MAX_TABLE_NAME_LENGTH];
int num_tables;
char columns[MAX_COLUMNS][MAX_TABLE_NAME_LENGTH];
int num_columns;

// Function to open the SQLite database
int open_database(char *filename) {
    int rc = sqlite3_open(filename, &db);
    if (rc) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return 0;
    }
    return 1;
}

// Function to fetch tables from the database
int get_tables() {
    sqlite3_stmt *stmt;
    const char *query = "SELECT name FROM sqlite_master WHERE type='table';";
    int rc = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error preparing query: %s\n", sqlite3_errmsg(db));
        return 0;
    }
    int i = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        strcpy(tables[i], (char *)sqlite3_column_text(stmt, 0));
        i++;
    }
    num_tables = i;
    sqlite3_finalize(stmt);
    return 1;
}

// Function to fetch columns of a table
int get_columns(char *table_name) {
    char query[100];
    sprintf(query, "PRAGMA table_info(%s);", table_name);

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error preparing query: %s\n", sqlite3_errmsg(db));
        return 0;
    }
    int i = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        strcpy(columns[i], (char *)sqlite3_column_text(stmt, 1));
        i++;
    }
    num_columns = i;
    sqlite3_finalize(stmt);
    return 1;
}

// Function to display a table as a table
void display_table(char *table_name) {
    clear();
    printw("Contents of table '%s':\n\n", table_name);

    char query[100];
    sprintf(query, "SELECT * FROM %s;", table_name);

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        printw("Error preparing query: %s\n", sqlite3_errmsg(db));
        return;
    }

    int cols = sqlite3_column_count(stmt);
    for (int i = 0; i < cols; i++) {
        printw("%-20s", sqlite3_column_name(stmt, i));
    }
    printw("\n");
    for (int i = 0; i < cols; i++) {
        printw("--------------------");
    }
    printw("\n");

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        for (int i = 0; i < cols; i++) {
            printw("%-20s", sqlite3_column_text(stmt, i));
        }
        printw("\n");
    }

    sqlite3_finalize(stmt);
    printw("\nPress any key to return...");
    refresh();
    getch();
}

// Function to display table structure
void display_table_structure(char *table_name) {
    clear();
    printw("Structure of table '%s':\n\n", table_name);

    if (!get_columns(table_name)) {
        return;
    }

    printw("%-20s %-20s\n", "Column Name", "Data Type");
    for (int i = 0; i < num_columns; i++) {
        printw("%-20s %-20s\n", columns[i], "TEXT"); // Assuming all columns are text for simplicity
    }

    printw("\nPress any key to return...");
    refresh();
    getch();
}

// Function to setup NCURSES
void setup_ncurses() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
}

// Function to teardown NCURSES
void teardown_ncurses() {
    endwin();
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <sqlite-database>\n", argv[0]);
        return 1;
    }

    if (!open_database(argv[1])) {
        return 1;
    }

    if (!get_tables()) {
        sqlite3_close(db);
        return 1;
    }

    setup_ncurses();

    int choice;
    while (1) {
        clear();
        printw("Select an option:\n\n");
        printw("1. View tables\n");
        printw("2. View table structure\n");
        printw("0. Exit\n\nChoice: ");
        refresh();

        scanw("%d", &choice);
        if (choice == 0) {
            break;
        } else if (choice == 1) {
            clear();
            printw("Available tables:\n\n");
            for (int i = 0; i < num_tables; i++) {
                printw("%d. %s\n", i + 1, tables[i]);
            }
            printw("\nChoose a table (0 to go back): ");
            refresh();
            int table_choice;
            scanw("%d", &table_choice);
            if (table_choice > 0 && table_choice <= num_tables) {
                display_table(tables[table_choice - 1]);
            }
        } else if (choice == 2) {
            clear();
            printw("Available tables:\n\n");
            for (int i = 0; i < num_tables; i++) {
                printw("%d. %s\n", i + 1, tables[i]);
            }
            printw("\nChoose a table to view structure (0 to go back): ");
            refresh();
            int table_choice;
            scanw("%d", &table_choice);
            if (table_choice > 0 && table_choice <= num_tables) {
                display_table_structure(tables[table_choice - 1]);
            }
        }
    }

    teardown_ncurses();
    sqlite3_close(db);

    return 0;
}
