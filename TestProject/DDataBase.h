#pragma once

#include <iostream>
#include <thread>
#include <chrono>
#include <memory>
#include <mysql.h>
#include <cstring>
#include <cstdint>
#include <cstdlib>
#include <xstring>
#include <string>

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
    /* Database handler */
    void handle();

    /* Get request for database to receive first name of defined user */
    err_type_db DDataBaseGetUserFirstName(std::string userName);
    /* Get request for database to receive password of defined user */
    err_type_db DDataBaseGetUserSecondName(std::string userName);
    /* Get request for database to receive id of defined user */
    err_type_db DDataBaseGetUserId(std::string userName);


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
    /* TODO: */
    void DDataBaseProcRequest(std::string request, uint32_t state);


};

