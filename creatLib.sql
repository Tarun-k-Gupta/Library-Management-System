CREATE DATABASE Library;
USE Library;
CREATE TABLE STUDENT(
    name VARCHAR(30) NOT NULL,
    roll_no VARCHAR(30) NOT NULL,
    cgpa FLOAT NOT NULL,
    isbn VARCHAR(30),
    CONSTRAINT pk_stud PRIMARY KEY (roll_no)
);

CREATE TABLE BOOK(
    title VARCHAR(30) NOT NULL,
    author VARCHAR(30) NOT NULL,
    price FLOAT NOT NULL,
    isbn VARCHAR(30) NOT NULL,
    issued BOOLEAN, 
    CONSTRAINT pk_book PRIMARY KEY (isbn)
);
CREATE TABLE LIB_OWNER(
    name_Owner VARCHAR(30) NOT NULL,
    id INTEGER NOT NULL,
    age INTEGER NOT NULL,
    experience_in_yr INTEGER NOT NULL,
    CONSTRAINT pk_owner PRIMARY KEY (id)
);