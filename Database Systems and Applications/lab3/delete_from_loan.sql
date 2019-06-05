delimiter $
CREATE DEFINER = CURRENT_USER TRIGGER `mydb`.`Loan_BEFORE_DELETE` BEFORE DELETE ON `Loan` FOR EACH ROW
BEGIN
	if old.`Status`= '发放中' then
		signal sqlstate 'HY000' set message_text = 'The loan is in distribution!';
	else
		delete from `Client_has_Loan` where `Client_has_Loan`.`Loan ID` = old.`Loan ID`;
        delete from `LoanDistribution` where `LoanDistribution`.`Loan ID` = old.`Loan ID`;
	end if;
END$