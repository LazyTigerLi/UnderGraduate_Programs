create or replace trigger updateStatus
  after insert or update
  on borrow 
  for each row
declare  
   --maxborrowDate date;--
   a number := 0;
pragma autonomous_transaction;
begin
    if :new.return_date is null then
        update Book set status = 1 where Book.Book_Id = :new.book_id;
        commit;
    else
        /*select max(Borrow.Borrow_Date) into maxBorrowDate from Borrow 
               where Borrow.Book_Id = :new.book_id group by Borrow.Book_Id;
        if :new.borrow_date >= maxBorrowDate then
           update Book set status = 0 where Book.Book_Id = :new.book_id;
           commit;
        end if;*/
        select count(*) into a from Borrow where Borrow.Book_Id = :new.book_id and
            Borrow.Return_Date is null;
        if a = 0 then
            update Book set status = 0 where Book.Book_Id = :new.book_id;
            commit;
        end if;
    end if;
    
/*exception
    when no_data_found then
        update Book set status = 0 where Book.Book_Id = :new.book_id;
        commit;*/

end updateStatus;
/
