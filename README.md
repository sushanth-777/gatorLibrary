# Gator Library Management System

This project is a library management system implemented in C++. It includes a Red-Black Tree-based book storage system with reservation handling.

## Table of Contents

- [Features](#features)
- [Getting Started](#getting-started)
  - [Prerequisites](#prerequisites)
  - [Installation](#installation)
- [Usage](#usage)
  - [Adding a Book](#adding-a-book)
  - [Removing a Book](#removing-a-book)
  - [Borrowing a Book](#borrowing-a-book)
  - [Returning a Book](#returning-a-book)
  - [Viewing Books](#viewing-books)

## Features

- Red-Black Tree data structure for efficient book storage
- Reservation system for books with priority handling
- Borrowing and returning functionality
- Command-line interface for user interaction

## Getting Started

### Prerequisites

- C++ compiler (supporting C++11 or later)
- Git

### Installation

Clone the repository:
   git clone https://github.com/your-username/gatorLibrary.git
   cd gatorLibrary

Compile the C++ code:
  g++ -o main main.cpp

### Usage

##Adding a book:
  InsertBook(BookID, BookName, Author, Availability)

##Removing a book:
  DeleteBook(BookID)
  
##Borrowing a book:
  BorrowBook(BookID, PatronID, Priority)

##Viewing books:
  PrintBooks(BookID1, BookID2)
  



