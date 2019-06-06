-- MySQL dump 10.13  Distrib 8.0.15, for Linux (x86_64)
--
-- Host: localhost    Database: mydb
-- ------------------------------------------------------
-- Server version	8.0.15

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
 SET NAMES utf8 ;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `Account`
--

DROP TABLE IF EXISTS `Account`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
 SET character_set_client = utf8mb4 ;
CREATE TABLE `Account` (
  `Account ID` int(11) NOT NULL,
  `Client ID` char(18) NOT NULL,
  `Bank` varchar(10) NOT NULL,
  `Type` enum('储蓄','支票') NOT NULL,
  `Balance` float NOT NULL,
  `Date` datetime NOT NULL,
  `Rate` float DEFAULT NULL,
  `Currency` enum('','人民币','美元','英镑','卢布','欧元') DEFAULT NULL,
  `Overdraft` float DEFAULT NULL,
  PRIMARY KEY (`Account ID`),
  KEY `fk_Account_Bank1_idx` (`Bank`),
  KEY `fk_Account_Client1_idx` (`Client ID`),
  CONSTRAINT `fk_Account_Bank1` FOREIGN KEY (`Bank`) REFERENCES `Bank` (`Name`),
  CONSTRAINT `fk_Account_Client1` FOREIGN KEY (`Client ID`) REFERENCES `Client` (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER `Account_BEFORE_INSERT` BEFORE INSERT ON `Account` FOR EACH ROW BEGIN
	declare cnt int;
	select count(*) into cnt from `Account`,`Client_has_Account`
    where `Account`.`Account ID` = `Client_has_Account`.`Account ID`
    and `Client_has_Account`.`Client ID` = new.`Client ID`
    and `Account`.`Bank` = new.`Bank` and `Account`.`Type` = new.`Type`;
    
    if cnt >= 1 then
		signal sqlstate 'HY000' set message_text = 'The client has already owned an account of this type!';
    elseif new.`Type` = '储蓄' and new.`Rate` is null and new.`Currency` is null then
		signal sqlstate 'HY000' set message_text = 'The rate and the currency shouldn\'t be null!';
    elseif new.`Type` = '支票' and new.`Overdraft` is null then
		signal sqlstate 'HY000' set message_text = 'The overdraft shouldn\'t be null!';
	end if;
    
    
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER `Account_AFTER_INSERT` AFTER INSERT ON `Account` FOR EACH ROW BEGIN
	insert into `Client_has_Account`
    (`Client ID`,`Account ID`,`Recent Access Time`)
    values
    (new.`Client ID`,new.`Account ID`,new.`Date`);
    
    insert into `Account Change Log`
    (`Client ID`,`Account ID`,`Date`,`Money`)
    values
    (new.`Client ID`,new.`Account ID`,new.`Date`,new.`Balance`);
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER `Account_BEFORE_UPDATE` BEFORE UPDATE ON `Account` FOR EACH ROW BEGIN
	if old.`Account ID` <> new.`Account ID` then
		signal sqlstate 'HY000' set message_text = 'The account id mustn\'t be modified!';
	elseif new.`Type` = '储蓄' and new.`Rate` is null and new.`Currency` is null then
		signal sqlstate 'HY000' set message_text = 'The rate and the currency shouldn\'t be null!';
    elseif new.`Type` = '支票' and new.`Overdraft` is null then
		signal sqlstate 'HY000' set message_text = 'The overdraft shouldn\'t be null!';
	end if;
    -- 一开始把'储蓄'写成了`储蓄`,找了半天错误 -- 
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER `Account_AFTER_UPDATE` AFTER UPDATE ON `Account` FOR EACH ROW BEGIN
	declare accessTime datetime;
    declare clientID char(18);
    
    select max(`Client_has_Account`.`Recent Access Time`) ,
    `Client_has_Account`.`Client ID` into accessTime,clientID
    from `Client_has_Account`
    where `Client_has_Account`.`Account ID` = new.`Account ID`;
    
    insert into `Account Change Log`(`Client ID`,`Account ID`,`Date`,`Money`)
    values (clientID,new.`Account ID`,accessTime,new.`Balance` - old.`Balance`);
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER `Account_BEFORE_DELETE` BEFORE DELETE ON `Account` FOR EACH ROW BEGIN
	delete from `Client_has_Account` where `Account ID` = old.`Account ID`;
    delete from `Account Change Log` where `Account ID` = old.`Account ID`;
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;

--
-- Table structure for table `Account Change Log`
--

DROP TABLE IF EXISTS `Account Change Log`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
 SET character_set_client = utf8mb4 ;
CREATE TABLE `Account Change Log` (
  `Account ID` int(11) NOT NULL,
  `Client ID` char(18) NOT NULL,
  `Date` datetime NOT NULL,
  `Money` float NOT NULL,
  PRIMARY KEY (`Account ID`,`Client ID`,`Date`),
  KEY `fk_Account Change Log_2_idx` (`Client ID`),
  CONSTRAINT `fk_Account Change Log_1` FOREIGN KEY (`Account ID`) REFERENCES `Account` (`Account ID`),
  CONSTRAINT `fk_Account Change Log_2` FOREIGN KEY (`Client ID`) REFERENCES `Client` (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `Bank`
--

DROP TABLE IF EXISTS `Bank`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
 SET character_set_client = utf8mb4 ;
CREATE TABLE `Bank` (
  `Name` varchar(10) NOT NULL,
  `City` varchar(6) NOT NULL,
  `Fund` float NOT NULL,
  PRIMARY KEY (`Name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `Client`
--

DROP TABLE IF EXISTS `Client`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
 SET character_set_client = utf8mb4 ;
CREATE TABLE `Client` (
  `ID` char(18) NOT NULL,
  `Name` varchar(5) NOT NULL,
  `Telephone` varchar(15) NOT NULL,
  `Address` varchar(30) NOT NULL,
  `Contact Name` varchar(5) NOT NULL,
  `Contact Telephone` varchar(15) NOT NULL,
  `Contact Email` varchar(20) NOT NULL,
  `Relationship with Contact` varchar(6) NOT NULL,
  `Staff ID` char(18) DEFAULT NULL,
  `Relationship with Staff` tinyint(4) DEFAULT NULL,
  PRIMARY KEY (`ID`),
  KEY `fk_Client_Staff1_idx` (`Staff ID`),
  CONSTRAINT `fk_Client_Staff1` FOREIGN KEY (`Staff ID`) REFERENCES `Staff` (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Table structure for table `Client_has_Account`
--

DROP TABLE IF EXISTS `Client_has_Account`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
 SET character_set_client = utf8mb4 ;
CREATE TABLE `Client_has_Account` (
  `Client ID` char(18) NOT NULL,
  `Account ID` int(11) NOT NULL,
  `Recent Access Time` datetime NOT NULL,
  PRIMARY KEY (`Client ID`,`Account ID`),
  KEY `fk_Client_has_Account_Client1_idx` (`Client ID`),
  KEY `fk_Client_has_Account_Account1_idx` (`Account ID`),
  CONSTRAINT `fk_Client_has_Account_Account1` FOREIGN KEY (`Account ID`) REFERENCES `Account` (`Account ID`),
  CONSTRAINT `fk_Client_has_Account_Client1` FOREIGN KEY (`Client ID`) REFERENCES `Client` (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER `Client_has_Account_BEFORE_INSERT` BEFORE INSERT ON `Client_has_Account` FOR EACH ROW BEGIN
	declare cnt int;
    declare bankName varchar(10);
    declare accountType char(2);
    select `Account`.`Bank` into bankName from `Account` 
		where `Account`.`Account ID` = new.`Account ID`;
    select `Account`.`Type` into accountType from `Account` 
		where `Account`.`Account ID` = new.`Account ID`;    
    
	select count(*) into cnt from `Account`,`Client_has_Account`
    where `Account`.`Account ID` = `Client_has_Account`.`Account ID`
    and `Client_has_Account`.`Client ID` = new.`Client ID`
    and `Account`.`Bank` = bankName and `Account`.`Type` = accountType;
    
    if cnt >= 1 then
		signal sqlstate 'HY000' set message_text = 'The client has already owned an account of this type!';
	end if;
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;

--
-- Table structure for table `Client_has_Loan`
--

DROP TABLE IF EXISTS `Client_has_Loan`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
 SET character_set_client = utf8mb4 ;
CREATE TABLE `Client_has_Loan` (
  `ID` char(18) NOT NULL,
  `Loan ID` int(11) NOT NULL,
  PRIMARY KEY (`ID`,`Loan ID`),
  KEY `fk_Client_has_Loan_Loan1_idx` (`Loan ID`),
  KEY `fk_Client_has_Loan_Client1_idx` (`ID`),
  CONSTRAINT `fk_Client_has_Loan_Client1` FOREIGN KEY (`ID`) REFERENCES `Client` (`ID`),
  CONSTRAINT `fk_Client_has_Loan_Loan1` FOREIGN KEY (`Loan ID`) REFERENCES `Loan` (`Loan ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER `Client_has_Loan_BEFORE_UPDATE` BEFORE UPDATE ON `Client_has_Loan` FOR EACH ROW BEGIN
	signal sqlstate 'HY000' set message_text = 'The infomation mustn\'t be modified!';
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER `Client_has_Loan_BEFORE_DELETE` BEFORE DELETE ON `Client_has_Loan` FOR EACH ROW BEGIN
	declare loanStatus varchar(4);
    declare clientID char(18);
	select `Loan`.`Status` into loanStatus from `Loan`
		where old.`Loan ID` = `Loan`.`Loan ID`;
    select `Loan`.`Client ID` into clientID from `Loan`
		where old.`Loan ID` = `Loan`.`Loan ID`;
    
	if loanStatus = '发放中' then
		signal sqlstate 'HY000' set message_text = 'The loan is in distribution!';
	elseif clientID = old.`ID` and loanStatus = '发放中' then
		signal sqlstate 'HY000' set message_text = 'The loan must have a client!';
    end if;
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;

--
-- Table structure for table `Loan`
--

DROP TABLE IF EXISTS `Loan`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
 SET character_set_client = utf8mb4 ;
CREATE TABLE `Loan` (
  `Loan ID` int(11) NOT NULL,
  `Bank` varchar(10) NOT NULL,
  `Loan` float NOT NULL,
  `Status` enum('未发放','发放中','发放完毕') NOT NULL DEFAULT '未发放',
  `Client ID` char(18) NOT NULL,
  PRIMARY KEY (`Loan ID`,`Bank`),
  UNIQUE KEY `id_UNIQUE` (`Loan ID`),
  KEY `fk_Loan_Bank1_idx` (`Bank`),
  KEY `fk_Loan_Client1_idx` (`Client ID`),
  CONSTRAINT `fk_Loan_Bank1` FOREIGN KEY (`Bank`) REFERENCES `Bank` (`Name`),
  CONSTRAINT `fk_Loan_Client1` FOREIGN KEY (`Client ID`) REFERENCES `Client` (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER `Loan_AFTER_INSERT` AFTER INSERT ON `Loan` FOR EACH ROW BEGIN
	insert into `Client_has_Loan`(`ID`,`Loan ID`)
    values
    (new.`Client ID`,new.`Loan ID`);
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER `Loan_BEFORE_UPDATE` BEFORE UPDATE ON `Loan` FOR EACH ROW BEGIN
	declare s float;
select sum(`LoanDistribution`.`Money`) into s
	from `LoanDistribution` where `LoanDistribution`.`Loan ID` = old.`Loan ID`;

if old.`Loan ID` <> new.`Loan ID` or old.`Bank` <> new.`Bank`
		or old.`Loan` <> new.`Loan` or old.`Client ID` <> new.`Client ID` then
		signal sqlstate 'HY000' set message_text = 'The infomation mustn\'t be modified!';
elseif not ((s = 0 and new.`Status`='未发放') or 
		(s = old.`Loan` and new.`Status`='发放完毕') or 
        (s > 0 and s < old.`Loan` and new.`Status`='发放中')) then
        signal sqlstate 'HY000' set message_text = 'The status is wrong!';
end if;
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER `Loan_BEFORE_DELETE` BEFORE DELETE ON `Loan` FOR EACH ROW BEGIN
	if old.`Status`= '发放中' then
		signal sqlstate 'HY000' set message_text = 'The loan is in distribution!';
	else
		delete from `Client_has_Loan` where `Client_has_Loan`.`Loan ID` = old.`Loan ID`;
        delete from `LoanDistribution` where `LoanDistribution`.`Loan ID` = old.`Loan ID`;
	end if;
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;

--
-- Table structure for table `LoanDistribution`
--

DROP TABLE IF EXISTS `LoanDistribution`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
 SET character_set_client = utf8mb4 ;
CREATE TABLE `LoanDistribution` (
  `Date` datetime NOT NULL,
  `Money` float NOT NULL,
  `Loan ID` int(11) NOT NULL,
  PRIMARY KEY (`Date`,`Loan ID`),
  KEY `fk_LoanDistribution_Loan_idx` (`Loan ID`),
  CONSTRAINT `fk_LoanDistribution_Loan` FOREIGN KEY (`Loan ID`) REFERENCES `Loan` (`Loan ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER `LoanDistribution_AFTER_INSERT` AFTER INSERT ON `LoanDistribution` FOR EACH ROW BEGIN
	declare s float;
    declare loan float;
    select sum(`LoanDistribution`.`Money`) into s from `LoanDistribution`
    where `LoanDistribution`.`Loan ID` = new.`Loan ID`
    group by `LoanDistribution`.`Loan ID`;
    
    select `Loan`.`Loan` into loan from `Loan`
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
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER `LoanDistribution_BEFORE_UPDATE` BEFORE UPDATE ON `LoanDistribution` FOR EACH ROW BEGIN
	signal sqlstate 'HY000' set message_text = 'The infomation mustn\'t be modified!';
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8mb4 */ ;
/*!50003 SET character_set_results = utf8mb4 */ ;
/*!50003 SET collation_connection  = utf8mb4_0900_ai_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
/*!50003 CREATE*/ /*!50017 DEFINER=`root`@`localhost`*/ /*!50003 TRIGGER `LoanDistribution_BEFORE_DELETE` BEFORE DELETE ON `LoanDistribution` FOR EACH ROW BEGIN
	declare loanStatus varchar(4);
	select `Loan`.`Status` into loanStatus from `Loan`
		where old.`Loan ID` = `Loan`.`Loan ID`;
    
	if loanStatus = '发放中' then
		signal sqlstate 'HY000' set message_text = 'The loan is in distribution!';
	end if;
END */;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;

--
-- Temporary view structure for view `Month Loan Statics`
--

DROP TABLE IF EXISTS `Month Loan Statics`;
/*!50001 DROP VIEW IF EXISTS `Month Loan Statics`*/;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8mb4;
/*!50001 CREATE VIEW `Month Loan Statics` AS SELECT 
 1 AS `time`,
 1 AS `bank`,
 1 AS `money`,
 1 AS `count`*/;
SET character_set_client = @saved_cs_client;

--
-- Temporary view structure for view `Month Saving Statics`
--

DROP TABLE IF EXISTS `Month Saving Statics`;
/*!50001 DROP VIEW IF EXISTS `Month Saving Statics`*/;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8mb4;
/*!50001 CREATE VIEW `Month Saving Statics` AS SELECT 
 1 AS `time`,
 1 AS `bank`,
 1 AS `money`,
 1 AS `count`*/;
SET character_set_client = @saved_cs_client;

--
-- Temporary view structure for view `Season Loan Statics`
--

DROP TABLE IF EXISTS `Season Loan Statics`;
/*!50001 DROP VIEW IF EXISTS `Season Loan Statics`*/;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8mb4;
/*!50001 CREATE VIEW `Season Loan Statics` AS SELECT 
 1 AS `time`,
 1 AS `bank`,
 1 AS `money`,
 1 AS `count`*/;
SET character_set_client = @saved_cs_client;

--
-- Temporary view structure for view `Season Saving Statics`
--

DROP TABLE IF EXISTS `Season Saving Statics`;
/*!50001 DROP VIEW IF EXISTS `Season Saving Statics`*/;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8mb4;
/*!50001 CREATE VIEW `Season Saving Statics` AS SELECT 
 1 AS `time`,
 1 AS `bank`,
 1 AS `money`,
 1 AS `count`*/;
SET character_set_client = @saved_cs_client;

--
-- Table structure for table `Staff`
--

DROP TABLE IF EXISTS `Staff`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
 SET character_set_client = utf8mb4 ;
CREATE TABLE `Staff` (
  `ID` char(18) NOT NULL,
  `Name` varchar(5) NOT NULL,
  `Is Manager` tinyint(4) NOT NULL,
  `Department` varchar(10) NOT NULL,
  `Telephone` varchar(15) NOT NULL,
  `Address` varchar(30) NOT NULL,
  `Employed Date` date NOT NULL,
  `Manager ID` varchar(18) NOT NULL,
  `Bank` varchar(10) NOT NULL,
  PRIMARY KEY (`ID`),
  KEY `fk_Staff_Bank1_idx` (`Bank`),
  CONSTRAINT `fk_Staff_Bank1` FOREIGN KEY (`Bank`) REFERENCES `Bank` (`Name`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Temporary view structure for view `Year Loan Statics`
--

DROP TABLE IF EXISTS `Year Loan Statics`;
/*!50001 DROP VIEW IF EXISTS `Year Loan Statics`*/;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8mb4;
/*!50001 CREATE VIEW `Year Loan Statics` AS SELECT 
 1 AS `time`,
 1 AS `bank`,
 1 AS `money`,
 1 AS `count`*/;
SET character_set_client = @saved_cs_client;

--
-- Dumping routines for database 'mydb'
--

--
-- Final view structure for view `Month Loan Statics`
--

/*!50001 DROP VIEW IF EXISTS `Month Loan Statics`*/;
/*!50001 SET @saved_cs_client          = @@character_set_client */;
/*!50001 SET @saved_cs_results         = @@character_set_results */;
/*!50001 SET @saved_col_connection     = @@collation_connection */;
/*!50001 SET character_set_client      = utf8mb4 */;
/*!50001 SET character_set_results     = utf8mb4 */;
/*!50001 SET collation_connection      = utf8mb4_0900_ai_ci */;
/*!50001 CREATE ALGORITHM=UNDEFINED */
/*!50013 DEFINER=`root`@`localhost` SQL SECURITY DEFINER */
/*!50001 VIEW `Month Loan Statics` AS select date_format(floor(`LoanDistribution`.`Date`),'%Y-%M-01 00:00:00') AS `time`,`Loan`.`Bank` AS `bank`,sum(`LoanDistribution`.`Money`) AS `money`,count(distinct `Client_has_Loan`.`ID`) AS `count` from ((`Client_has_Loan` join `LoanDistribution`) join `Loan`) where ((`Client_has_Loan`.`Loan ID` = `LoanDistribution`.`Loan ID`) and (`Client_has_Loan`.`Loan ID` = `Loan`.`Loan ID`)) group by `time`,`Loan`.`Bank` order by `time` desc */;
/*!50001 SET character_set_client      = @saved_cs_client */;
/*!50001 SET character_set_results     = @saved_cs_results */;
/*!50001 SET collation_connection      = @saved_col_connection */;

--
-- Final view structure for view `Month Saving Statics`
--

/*!50001 DROP VIEW IF EXISTS `Month Saving Statics`*/;
/*!50001 SET @saved_cs_client          = @@character_set_client */;
/*!50001 SET @saved_cs_results         = @@character_set_results */;
/*!50001 SET @saved_col_connection     = @@collation_connection */;
/*!50001 SET character_set_client      = utf8mb4 */;
/*!50001 SET character_set_results     = utf8mb4 */;
/*!50001 SET collation_connection      = utf8mb4_0900_ai_ci */;
/*!50001 CREATE ALGORITHM=UNDEFINED */
/*!50013 DEFINER=`root`@`localhost` SQL SECURITY DEFINER */
/*!50001 VIEW `Month Saving Statics` AS select date_format(floor(`Account Change Log`.`Date`),'%Y-%M-01 00:00:00') AS `time`,`Account`.`Bank` AS `bank`,sum(`Account Change Log`.`Money`) AS `money`,count(distinct `Account Change Log`.`Client ID`) AS `count` from (`Account Change Log` join `Account`) where (`Account Change Log`.`Account ID` = `Account`.`Account ID`) group by `time`,`Account`.`Bank` order by `time` desc */;
/*!50001 SET character_set_client      = @saved_cs_client */;
/*!50001 SET character_set_results     = @saved_cs_results */;
/*!50001 SET collation_connection      = @saved_col_connection */;

--
-- Final view structure for view `Season Loan Statics`
--

/*!50001 DROP VIEW IF EXISTS `Season Loan Statics`*/;
/*!50001 SET @saved_cs_client          = @@character_set_client */;
/*!50001 SET @saved_cs_results         = @@character_set_results */;
/*!50001 SET @saved_col_connection     = @@collation_connection */;
/*!50001 SET character_set_client      = utf8mb4 */;
/*!50001 SET character_set_results     = utf8mb4 */;
/*!50001 SET collation_connection      = utf8mb4_0900_ai_ci */;
/*!50001 CREATE ALGORITHM=UNDEFINED */
/*!50013 DEFINER=`root`@`localhost` SQL SECURITY DEFINER */
/*!50001 VIEW `Season Loan Statics` AS select concat(year(`LoanDistribution`.`Date`),' ',quarter(`LoanDistribution`.`Date`)) AS `time`,`Loan`.`Bank` AS `bank`,sum(`LoanDistribution`.`Money`) AS `money`,count(distinct `Client_has_Loan`.`ID`) AS `count` from ((`Client_has_Loan` join `LoanDistribution`) join `Loan`) where ((`Client_has_Loan`.`Loan ID` = `LoanDistribution`.`Loan ID`) and (`Client_has_Loan`.`Loan ID` = `Loan`.`Loan ID`)) group by `time`,`Loan`.`Bank` order by `time` desc */;
/*!50001 SET character_set_client      = @saved_cs_client */;
/*!50001 SET character_set_results     = @saved_cs_results */;
/*!50001 SET collation_connection      = @saved_col_connection */;

--
-- Final view structure for view `Season Saving Statics`
--

/*!50001 DROP VIEW IF EXISTS `Season Saving Statics`*/;
/*!50001 SET @saved_cs_client          = @@character_set_client */;
/*!50001 SET @saved_cs_results         = @@character_set_results */;
/*!50001 SET @saved_col_connection     = @@collation_connection */;
/*!50001 SET character_set_client      = utf8mb4 */;
/*!50001 SET character_set_results     = utf8mb4 */;
/*!50001 SET collation_connection      = utf8mb4_0900_ai_ci */;
/*!50001 CREATE ALGORITHM=UNDEFINED */
/*!50013 DEFINER=`root`@`localhost` SQL SECURITY DEFINER */
/*!50001 VIEW `Season Saving Statics` AS select concat(year(`Account Change Log`.`Date`),' ',quarter(`Account Change Log`.`Date`)) AS `time`,`Account`.`Bank` AS `bank`,sum(`Account Change Log`.`Money`) AS `money`,count(distinct `Account Change Log`.`Client ID`) AS `count` from (`Account Change Log` join `Account`) where (`Account Change Log`.`Account ID` = `Account`.`Account ID`) group by `time`,`Account`.`Bank` order by `time` desc */;
/*!50001 SET character_set_client      = @saved_cs_client */;
/*!50001 SET character_set_results     = @saved_cs_results */;
/*!50001 SET collation_connection      = @saved_col_connection */;

--
-- Final view structure for view `Year Loan Statics`
--

/*!50001 DROP VIEW IF EXISTS `Year Loan Statics`*/;
/*!50001 SET @saved_cs_client          = @@character_set_client */;
/*!50001 SET @saved_cs_results         = @@character_set_results */;
/*!50001 SET @saved_col_connection     = @@collation_connection */;
/*!50001 SET character_set_client      = utf8mb4 */;
/*!50001 SET character_set_results     = utf8mb4 */;
/*!50001 SET collation_connection      = utf8mb4_0900_ai_ci */;
/*!50001 CREATE ALGORITHM=UNDEFINED */
/*!50013 DEFINER=`root`@`localhost` SQL SECURITY DEFINER */
/*!50001 VIEW `Year Loan Statics` AS select date_format(floor(`LoanDistribution`.`Date`),'%Y-01-01 00:00:00') AS `time`,`Loan`.`Bank` AS `bank`,sum(`LoanDistribution`.`Money`) AS `money`,count(distinct `Client_has_Loan`.`ID`) AS `count` from ((`Client_has_Loan` join `LoanDistribution`) join `Loan`) where ((`Client_has_Loan`.`Loan ID` = `LoanDistribution`.`Loan ID`) and (`Client_has_Loan`.`Loan ID` = `Loan`.`Loan ID`)) group by `time`,`Loan`.`Bank` order by `time` desc */;
/*!50001 SET character_set_client      = @saved_cs_client */;
/*!50001 SET character_set_results     = @saved_cs_results */;
/*!50001 SET collation_connection      = @saved_col_connection */;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2019-06-06 18:03:43
