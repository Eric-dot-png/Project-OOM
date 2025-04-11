/*M!999999\- enable the sandbox mode */ 
-- MariaDB dump 10.19  Distrib 10.11.10-MariaDB, for Linux (x86_64)
--
-- Host: localhost    Database: DSDB
-- ------------------------------------------------------
-- Server version	10.11.10-MariaDB

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

use DSDB;

--
-- Table structure for table `FriendRequest`
--

DROP TABLE IF EXISTS `FriendRequest`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `FriendRequest` (
  `sender` varchar(100) NOT NULL,
  `receiver` varchar(100) NOT NULL,
  PRIMARY KEY (`sender`,`receiver`),
  KEY `receiver` (`receiver`),
  CONSTRAINT `FriendRequest_ibfk_1` FOREIGN KEY (`sender`) REFERENCES `User` (`username`),
  CONSTRAINT `FriendRequest_ibfk_2` FOREIGN KEY (`receiver`) REFERENCES `User` (`username`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `FriendRequest`
--

LOCK TABLES `FriendRequest` WRITE;
/*!40000 ALTER TABLE `FriendRequest` DISABLE KEYS */;
/*!40000 ALTER TABLE `FriendRequest` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `Friends`
--

DROP TABLE IF EXISTS `Friends`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `Friends` (
  `u1` varchar(100) NOT NULL,
  `u2` varchar(100) NOT NULL,
  PRIMARY KEY (`u1`,`u2`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `Friends`
--

LOCK TABLES `Friends` WRITE;
/*!40000 ALTER TABLE `Friends` DISABLE KEYS */;
/*!40000 ALTER TABLE `Friends` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `GroupInfo`
--

DROP TABLE IF EXISTS `GroupInfo`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `GroupInfo` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `owner` varchar(100) NOT NULL,
  `name` varchar(100) DEFAULT NULL,
  `deleted` tinyint(1) DEFAULT 0,
  PRIMARY KEY (`id`),
  KEY (`owner`,`name`),
  CONSTRAINT `GroupInfo_ibfk_1` FOREIGN KEY (`owner`) REFERENCES `User` (`username`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `GroupInfo`
--

LOCK TABLES `GroupInfo` WRITE;
/*!40000 ALTER TABLE `GroupInfo` DISABLE KEYS */;
/*!40000 ALTER TABLE `GroupInfo` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `GroupMember`
--

DROP TABLE IF EXISTS `GroupMember`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `GroupMember` (
  `groupId` int(11) NOT NULL,
  `user` varchar(100) NOT NULL,
  PRIMARY KEY (`groupId`,`user`),
  KEY `user` (`user`),
  CONSTRAINT `GroupMember_ibfk_1` FOREIGN KEY (`groupId`) REFERENCES `GroupInfo` (`id`),
  CONSTRAINT `GroupMember_ibfk_2` FOREIGN KEY (`user`) REFERENCES `User` (`username`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `GroupMember`
--

LOCK TABLES `GroupMember` WRITE;
/*!40000 ALTER TABLE `GroupMember` DISABLE KEYS */;
/*!40000 ALTER TABLE `GroupMember` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `GroupMessage`
--

DROP TABLE IF EXISTS `GroupMessage`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `GroupMessage` (
  `groupId` int(11) NOT NULL,
  `sender` varchar(100) NOT NULL,
  `sentAt` timestamp NULL DEFAULT current_timestamp(),
  `message` text NOT NULL,
  `deletedmsg` tinyint(1) DEFAULT 0,
  KEY `groupId` (`groupId`),
  KEY `sender` (`sender`),
  CONSTRAINT `GroupMessage_ibfk_1` FOREIGN KEY (`groupId`) REFERENCES `GroupInfo` (`id`),
  CONSTRAINT `GroupMessage_ibfk_2` FOREIGN KEY (`sender`) REFERENCES `User` (`username`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `GroupMessage`
--

LOCK TABLES `GroupMessage` WRITE;
/*!40000 ALTER TABLE `GroupMessage` DISABLE KEYS */;
/*!40000 ALTER TABLE `GroupMessage` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `PrivMessage`
--

DROP TABLE IF EXISTS `PrivMessage`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `PrivMessage` (
  `receiver` varchar(100) NOT NULL,
  `sender` varchar(100) NOT NULL,
  `sentAt` timestamp NULL DEFAULT current_timestamp(),
  `message` text NOT NULL,
  `deleted` tinyint(1) DEFAULT 0,
  KEY `receiver` (`receiver`),
  KEY `sender` (`sender`),
  CONSTRAINT `PrivMessage_ibfk_1` FOREIGN KEY (`receiver`) REFERENCES `User` (`username`),
  CONSTRAINT `PrivMessage_ibfk_2` FOREIGN KEY (`sender`) REFERENCES `User` (`username`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `PrivMessage`
--

LOCK TABLES `PrivMessage` WRITE;
/*!40000 ALTER TABLE `PrivMessage` DISABLE KEYS */;
INSERT INTO `PrivMessage` VALUES
('test1','test2','2025-04-01 21:49:56','DOES THIS WORK???',0);
/*!40000 ALTER TABLE `PrivMessage` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `Registration`
--

DROP TABLE IF EXISTS `Registration`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `Registration` (
  `username` varchar(100) NOT NULL,
  `password` varchar(100) DEFAULT NULL,
  `email` varchar(100) DEFAULT NULL,
  `permissions` tinyint(1) DEFAULT NULL,
  `validTimeout` datetime DEFAULT NULL,
  `code` char(6) DEFAULT NULL,
  PRIMARY KEY (`username`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `Registration`
--

LOCK TABLES `Registration` WRITE;
/*!40000 ALTER TABLE `Registration` DISABLE KEYS */;
/*!40000 ALTER TABLE `Registration` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `User`
--

DROP TABLE IF EXISTS `User`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `User` (
  `username` varchar(100) NOT NULL,
  `password` varchar(100) NOT NULL,
  `email` varchar(100) NOT NULL,
  `permissions` tinyint(1) NOT NULL DEFAULT 0,
  `deleted` tinyint(1) NOT NULL DEFAULT 0,
  KEY `username` (`username`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1 COLLATE=latin1_swedish_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `User`
--

LOCK TABLES `User` WRITE;
/*!40000 ALTER TABLE `User` DISABLE KEYS */;
INSERT INTO `User` VALUES
('test1','1809192717','bspat11037@gmail.com',0,0),
('test2','1713550986','bspat11037@gmail.com',0,0);
/*!40000 ALTER TABLE `User` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2025-04-01 14:52:22
