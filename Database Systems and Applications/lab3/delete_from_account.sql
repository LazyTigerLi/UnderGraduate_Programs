delimiter $
CREATE DEFINER = CURRENT_USER TRIGGER `mydb`.`Account_BEFORE_DELETE` BEFORE DELETE ON `Account` FOR EACH ROW
BEGIN
	delete from `Client_has_Account` where `Account ID` = old.`Account ID`;
END$
