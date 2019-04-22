create or replace procedure updateBook(old_book_id in char,new_book_id in char) is
a number := 0;
begin
    select count(*) into a from Book where Book.Book_Id = new_book_id;
    if a <> 0 then
        raise_application_error(-20000,'ID�Ѵ���');
    else
        execute immediate 'alter table Borrow disable constraint FK1';
        update Book set book_id = new_book_id where book_id = old_book_id;
        update Borrow set book_id = new_book_id where book_id = old_book_id;
        execute immediate 'alter table Borrow enable constraint FK1';
    end if;
end updateBook;


--�洢�����в���ֱ��ʹ��create table����alter tableһ��Ķ���ṹ�����
/
