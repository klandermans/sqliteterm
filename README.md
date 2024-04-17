# SQLITETERM

SQLITETERM is a simple NCURSES-based program written in C that allows you to view the contents and structure of SQLite databases. You can select tables from the database and view their contents in a tabular format.

## Usage

1. **Compilation**: Compile the program using a C compiler. Make sure you have the necessary development libraries installed for NCURSES and SQLite3.

   ```bash
   gcc -o sqliteterm sqliteterm.c -lncurses -lsqlite3
   ```

2. **Execution**: Run the compiled program with the SQLite database file as an argument.

   ```bash
   ./sqliteterm your_database.db
   ```

## Features

- View a list of tables in the SQLite database.
- Select a table to view its contents in a tabular format.
- View the structure of tables, including column names and data types.

## Requirements

- C compiler (e.g., GCC)
- NCURSES library
- SQLite3 library

## Contributing

Contributions to improve SQLITETERM are welcome! If you find any issues or have suggestions for new features, please feel free to open an issue or submit a pull request.

## License

This project is licensed under the [MIT License](LICENSE).
