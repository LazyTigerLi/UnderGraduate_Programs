select Reader.reader_id, Reader.address from Reader where reader_name = 'Rose';

select Book.Book_Name, Borrow.Borrow_Date from Book, Reader, Borrow
       where Book.Book_Id = Borrow.Book_Id and Reader.Reader_Id = Borrow.Reader_Id
       and Reader.Reader_Name = 'Rose';

select distinct Reader.Reader_Name from Reader 
       where Reader.Reader_Id not in (select distinct Borrow.Reader_Id from Borrow);

select Book.Book_Name,Book.Price from Book where Book.Author = 'Ullman';

select Book.Book_Id,Book.Book_Name from Book,Borrow,Reader 
       where Book.Book_Id = Borrow.Book_Id and Reader.Reader_Id = Borrow.Reader_Id 
       and Reader.Reader_Name = '李林' and Borrow.Return_Date is null;

select distinct Reader.Reader_Name from Reader
       where Reader.Reader_Id in (select Reader.Reader_Id
       from Reader,Borrow where Reader.Reader_Id = Borrow.Reader_Id group by Reader.Reader_Id
       having count(Borrow.Book_Id) > 3);
       
select Reader.Reader_Id,Reader.Reader_Name from Reader
       where not exists
       (select Borrow.Book_Id from Borrow
       where Borrow.Reader_Id = Reader.Reader_Id and Borrow.Book_Id in
       (select distinct Borrow.Book_Id from Borrow,Reader 
       where Borrow.Reader_Id = Reader.Reader_Id and Reader.Reader_Name = '李林'));

select Book.Book_Id,Book.Book_Name from Book where Book.Book_Name like '%Oracle%';
       
select Reader_Borrow.Reader_Id,count(distinct Reader_Borrow.Book_Id) from Reader_Borrow
       where Reader_Borrow.Borrow_Date >= add_months(sysdate,-12)
       group by Reader_Borrow.Reader_Id;
       
