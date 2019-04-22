--insert into Book values('0002','Psychology','Floyd',19,0);--
--delete from Book where Book.Book_Id = '0002'--
update Book set Book.Book_Name = null where Book.Book_Id = '0002'
