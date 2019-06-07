CREATE DATABASE  IF NOT EXISTS `mydb` /*!40100 DEFAULT CHARACTER SET utf8 */;
USE `mydb`;
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
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2019-06-07 20:00:11
