ALTER TABLE STUDENT
    ADD CONSTRAINT fk_isbn FOREIGN KEY (isbn) REFERENCES BOOK(isbn);