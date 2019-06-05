delimiter $
CREATE DEFINER = CURRENT_USER TRIGGER `mydb`.`Account_AFTER_INSERT` AFTER INSERT ON `Account` FOR EACH ROW
BEGIN
	insert into `Client_has_Account`
    (`ID`,`Account ID`,`Type`,`Bank`)
    values
    (new.`Client ID`,new.`Account ID`,new.`Type`,new.`Bank`);
END$
