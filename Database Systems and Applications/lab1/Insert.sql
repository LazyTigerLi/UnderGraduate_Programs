/*insert into Book values('0001','Python','Turing',20,0);
insert into Book values('0002','Psychology','Floyd',19,0);
insert into Book values('0003','Physics','Einstein',18,0);
insert into Book values('0004','Philosophy','Plato',17,0);
insert into Book values('0005','- Oracle -','Me',16,0);
insert into Book values('0006','Unknown','Ullman',15,0);
insert into Book values('0007','Robot','Asimov',14,0);
insert into Book values('0008','History','Unknown',13,0);
insert into Book values('0009','ICS','Yale',12,0);
insert into Book values('0010','hahaha','Me',11,0);

insert into Reader values('0001','Rose','20','America');
insert into Reader values('0002','李林','21','China');
insert into Reader values('0003','Trump','73','Earth');
insert into Reader values('0004','Linus','50','Finland');
insert into Reader values('0005','Obama','58','America');
insert into Reader values('0006','Trump','1','America');*/

insert into Borrow values('0001','0004',to_date('12-08-2018','dd-mm-yyyy'),null);    --linus python--
insert into Borrow values('0002','0001',to_date('13-04-2019','dd-mm-yyyy'),null);    --rose psychology--
insert into Borrow values('0005','0001',to_date('12-08-2016','dd-mm-yyyy'),to_date('13-08-2016','dd-mm-yyyy')); --rose oracle--
insert into Borrow values('0006','0001',to_date('07-12-2018','dd-mm-yyyy'),null);    --rose unknown--
insert into Borrow values('0008','0001',to_date('12-03-2019','dd-mm-yyyy'),null);    --rose history--
insert into Borrow values('0005','0002',to_date('12-02-2019','dd-mm-yyyy'),null);    --李林 oracle--
insert into Borrow values('0009','0002',to_date('12-08-2018','dd-mm-yyyy'),to_date('13-08-2018','dd-mm-yyyy')); --李林 ICS--
insert into Borrow values('0007','0003',to_date('10-08-2018','dd-mm-yyyy'),to_date('11-08-2018','dd-mm-yyyy')); --trump robot--
--insert into Borrow values('0001','0003',to_date('10-08-2018','dd-mm-yyyy'),to_date('11-08-2018','dd-mm-yyyy'));
--insert into Borrow values('0001','0003',to_date('14-08-2018','dd-mm-yyyy'),to_date('15-08-2018','dd-mm-yyyy'));
insert into Borrow values('0003','0003',to_date('12-08-2010','dd-mm-yyyy'),to_date('13-08-2013','dd-mm-yyyy')); --trump physics--
insert into Borrow values('0004','0003',to_date('12-02-2018','dd-mm-yyyy'),to_date('13-02-2018','dd-mm-yyyy')); --trump philosophy--
