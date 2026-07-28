Library Management System

Simple console app to manage a small library — add books, issue/return them, search, and delete records. Data is saved to a text file so it persists between runs.

Features
Add new books (auto-assigned IDs)
List all books with their status (available / issued to whom)
Issue a book to a member
Return a book
Search by title (partial match)
Delete a book record
Everything saves to books.txt automatically on exit
Build & Run
g++ -std=c++17 -o library library.cpp
./library
Notes

Storage is a plain pipe-delimited text file (books.txt), created automatically the first time you add a book. Not meant to handle concurrent access or anything fancy — this was mainly a project to practice classes, file I/O, and basic CRUD logic in C++.
