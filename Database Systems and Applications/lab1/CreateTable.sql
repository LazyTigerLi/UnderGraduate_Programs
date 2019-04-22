create table Book
(
       book_id char(8) constraint PK_Book primary key,
       book_name varchar2(10) not null,
       author varchar2(10),
       price float,
       status int default 0 check(status in (0,1))
);

create table Reader
(
       reader_id char(8) constraint PK_Reader primary key,
       reader_name varchar2(10),
       age int,
       address varchar2(20)
);

create table Borrow
(
       book_id char(8),
       reader_id char(8),
       borrow_date date,
       return_date date,
       constraint PK_Borrow primary key(book_id,reader_id),
       constraint FK1 foreign key(book_id) references Book(book_id),
       constraint FK2 foreign key(reader_id) references Reader(reader_id)
);

create view Reader_Borrow(reader_id,reader_name,book_id,book_name,borrow_date)
       as select Reader.Reader_Id,Reader.Reader_Name,Book.Book_Id,Book.Book_Name,Borrow.Borrow_Date
       from Reader,Book,Borrow where Reader.Reader_Id = Borrow.Reader_Id and Book.Book_Id = Borrow.Book_Id;
