# Library-Management-System-in-Qt
This repository contains Library Management System created by me using Qt Framework. I have used the SQLite database to accomplish my task and have used Qt c++ libraries. 

**DESCRIPTION**
Hy guys,
I have assumed that admin will use my application.
(Make sure you have shadow build off for my application cause my databse is stores in the same project file)
There are 5 tables I am using in my application, their description are given below:
1. users:- Will store user credentials (Username and hash 256 generated password in hex form)
2. books:- It will store details of books added by admin after log in.
3. authors:- This table stores author details.
4. students:- contains students details added by admin after login.
5. issueBook :- Contains details of books issued to students.

Their attributes explanation:-

1. users table attributes:-

 - email_id :- contains username or email_id of user
 - password :- contains hash256 generated passwprd in headecimal form.
 
 2 books table attributes:-
 
 - book_id :- It is AUTOINCREMENT attribute used to be our primary key here.
 - book_name :- Stores name of the books.
 - author_id :- stores the author_id of the author of that book (author_id will be obtain from authors table)
 - stock :- stores the stock of book (It will be decrease by one whenever a book is issued to student and vice versa in case of return )
 - availability :- when stock values comes to zero availability becomes 0 (means book is not available).
 
 3 authors table attributes :-
 
 - author_id :- t is AUTOINCREMENT attribute used to be our primary key here.
 - author_name :- store name of author.
 
 4. students table attributes :-

- stu_id :- it is AUTOINCREMENT attribute used to be our primary key here.
- stu_name :- store student name.
- stu_rollNo :- store roll number of student (Unique key)
- stu_branch :- branch in which student studying (e.g- cse, ece etc)
- book_taken :- shows the value of how much book student has obtain from library (if this values become 5 that student won't take more book from library until he/she return a book).

5. issueBook table attributes :-

- issue_id :-  it is AUTOINCREMENT attribute used to be our primary key here.
- stu_id :-  stores the student id of student who taken the book having book_id.
- book_id :- stores the book_id issued by student having student_id.
