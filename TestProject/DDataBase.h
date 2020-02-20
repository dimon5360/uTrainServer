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

enum class err_type {
    ERR_OK = 0,
    ERR_DB_CONNECT,
    ERR_NOT_GOT_HOST_INFO,
    ERR_DB_DOESNT_RESPONSE,
    ERR_INCORRECT_REQUEST,
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
    err_type RequestDatabaseUserNumber();
    /* Database handler */
    void handle();

public:
    /* constructor */
    DDataBase(std::string s_name, uint32_t u_port);
    /* destructor */
    ~DDataBase();

    /* database connect */
    err_type DDataBaseConnect(std::string hostAddr,
        std::string pass);
    /* TODO: */
    err_type DDataBaseGetUserData(std::string login);
    /* TODO: */
    err_type DDataBaseGetUserName(std::string userName);



    /* TODO: */
    void DDataBaseProcRequest(std::string request, uint32_t state);


};

