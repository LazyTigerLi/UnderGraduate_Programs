delimiter $
CREATE DEFINER = CURRENT_USER TRIGGER `mydb`.`LoanDistribution_AFTER_INSERT` AFTER INSERT ON `LoanDistribution` FOR EACH ROW
BEGIN
	declare s float;
    declare loan float;
    select sum(`LoanDistribution`) into s from `LoanDistribution`
    where `LoanDistribution`.`Loan ID` = new.`Loan ID`
    group by `LoanDistribution`.`Loan ID`;
    
    select `Loan`.`Money` into loan from `Loan`
    where `Loan`.`Loan ID` = new.`Loan ID`;
    
    if s > loan then
		delete from `LoanDistribution` where `LoanDistribution`.`Date` = new.`Date` 
        and `LoanDistribution`.`Loan ID` = new.`Loan ID`;
        signal sqlstate 'HY001' set message_text = 'The money distributed is wrong!';
	elseif s = loan then
		update `Loan` set `Loan`.`Status` = '发放完毕' where `Loan`.`Loan ID` = new.`Loan ID`;
	elseif s > 0 then
		update `Loan` set `Loan`.`Status` = '发放中' where `Loan`.`Loan ID` = new.`Loan ID`;
    end if;
END$