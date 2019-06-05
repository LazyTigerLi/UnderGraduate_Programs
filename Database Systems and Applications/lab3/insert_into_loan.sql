delimiter $
CREATE DEFINER = CURRENT_USER TRIGGER `mydb`.`Loan_AFTER_INSERT` AFTER INSERT ON `Loan` FOR EACH ROW
BEGIN
	insert into `Client_has_Loan`(`ID`,`Loan ID`)
    values
    (new.`Client ID`,`Loan ID`);
END$
