/**
 *  @file DDataBase.h
 *  @note MySQL data base processor class prototype
 *
 *  @date 08.04.2020
 *  @author Kalmykov Dmitry
 *
 *  @modified 08.04.2020
 *  @version 0.1
 */

#pragma once
#include "config.h"
#include "main.h"

 /* import mysql prototypes */
#include <mysql.h>
#include <xstring>
 /* import data queue */
#include <queue>

/* error codes for data base */
enum class err_type_db {
    ERR_OK = 0,
    ERR_DB_CONNECT,
    ERR_NOT_GOT_HOST_INFO,
    ERR_DB_DOESNT_RESPONSE,
    ERR_INCORRECT_REQUEST,
    ERR_USER_NOT_FOUND,
};

class DDataBase
{
private:
    std::queue<std::string> dataBaseQueue;
    std::string dbName;
    uint32_t dbPort;

    MYSQL * conn, mysql;
    MYSQL_ROW row;
    MYSQL_RES * res;
    uint32_t userCount = 0;
    
    /* Init database connection */
    void DDataBaseInit();
    /* Get request for database to receive number of active users */
    err_type_db RequestDatabaseUserNumber();
    /* Get request for database to receive data of defined user */
    err_type_db DDataBaseUserDataReq(std::string req, std::string userName);


public:
    /* constructor */
    DDataBase(std::string s_name, uint32_t u_port);
    /* destructor */
    ~DDataBase();

    /* database connect */
    err_type_db DDataBaseConnect(std::string hostAddr,
        std::string pass);

    /*  Get request for database to receive data of defined user */
    err_type_db DDataBaseGetUserData(std::string userName);

    void DDataBasePushRequest(std::string sDataBaseRequest);
    std::string DDataBasePullRequest(void);

    /* TODO: */
    void DDataBaseProcRequest(std::string request, uint32_t state);
    /* Database handler */
    void handle();

};

