/**
 *  @file DDataBase.cpp
 *  @note MySQL data base processor class realization
 *
 *  @date 08.04.2020
 *  @author Kalmykov Dmitry
 *
 *  @modified 08.04.2020
 *  @version 0.1
 */

#include "DDataBase.h"

/**
 * @brief DDataBase class constructor
 */
DDataBase::DDataBase(std::string s_name, uint32_t u_port) {
    this->dbName = s_name;
    this->dbPort = u_port;
    DDataBaseInit();
}

/**
 * @brief DDataBase class destructor
 */
DDataBase::~DDataBase() {
    mysql_close(conn);
}

/**
 * @brief Init database connection 
 */
void DDataBase::DDataBaseInit() {
    mysql_init(&mysql);
}

/**
 * @brief Connect to database
 * @param hostAddr Database host ip address
 * @param pass Database root user password
 * @retval Error code
 */
err_type_db DDataBase::DDataBaseConnect(std::string hostAddr, std::string pass) {
    
    err_type_db errCode = err_type_db::ERR_OK;
    conn = mysql_real_connect(&mysql, hostAddr.c_str(), "root",
        pass.c_str(), dbName.c_str(), dbPort, NULL, 0);
    if (!conn) {
        std::cout << mysql_error(&mysql) << std::endl;
        return err_type_db::ERR_DB_CONNECT;
    }
    std::cout << "Database connected." << std::endl;

    /* db host information */
    std::string hostInfo = mysql_get_host_info(&mysql);
    if (hostInfo.empty()) {
        std::cout << mysql_error(&mysql) << std::endl;
        return err_type_db::ERR_NOT_GOT_HOST_INFO;
    }
    std::cout << hostInfo << std::endl;

    errCode = RequestDatabaseUserNumber();
    if (errCode != err_type_db::ERR_OK) {
        std::cout << "Database has no users.\n";
    }
      
    /* process database requests */
    handle();
    return err_type_db::ERR_OK;
}

/**
 * @brief Get request for database to receive number of active users
 * @retval Error code
 */
err_type_db DDataBase::RequestDatabaseUserNumber() {
    int ret = 0;
    /* query to db */
    ret = mysql_query(conn, "select user_count()");
    if (ret != 0) {
        std::cout << "Incorrect request." << std::endl;
        return err_type_db::ERR_INCORRECT_REQUEST;
    }
    /* store result */
    res = mysql_store_result(conn);
    /* print result */
    std::cout << "Number of active users : ";
    while ((row = mysql_fetch_row(res)) != NULL) {
        std::cout << row[0] << std::endl;
    }
    std::cout << std::endl;
    /* free result */
    mysql_free_result(res);
    return err_type_db::ERR_OK;
}

/**
 * @brief Get request for database to receive first name of defined user
 * @param userName Name of user to get first name of user
 * @retval Error code
 */
err_type_db DDataBase::DDataBaseGetUserFirstName(std::string userName) {
    int ret = 0;
    /* query to db */
    std::string query;
    query = "select fname from users where fname = \"" + userName + "\"";
    ret = mysql_query(conn, query.c_str());
    if (ret != 0) {
        std::cout << "Incorrect request." << std::endl;
        return err_type_db::ERR_INCORRECT_REQUEST;
    }
    /* store result */
    res = mysql_store_result(conn);
    /* check that user found */
    if ((row = mysql_fetch_row(res)) == NULL) {
        std::cout << "User not found." << std::endl;
        /* free result */
        mysql_free_result(res);
        return err_type_db::ERR_USER_NOT_FOUND;
    }
    else {
        /* print result */
        std::cout << "User first name : ";
        std::cout << row[0] << std::endl;
        while ((row = mysql_fetch_row(res)) != NULL) {
            std::cout << row[0] << std::endl;
        }
    }
    /* free result */
    mysql_free_result(res);
    return err_type_db::ERR_OK;
}

/**
 * @brief Get request for database to receive second name of defined user
 * @param userName Name of user to get second name of user
 * @retval Error code
 */
err_type_db DDataBase::DDataBaseGetUserSecondName(std::string userName) {
    int ret = 0;
    /* query to db */
    std::string query;
    query = "select sname from users where fname = \"" + userName + "\"";
    ret = mysql_query(conn, query.c_str());
    if (ret != 0) {
        std::cout << "Incorrect request." << std::endl;
        return err_type_db::ERR_INCORRECT_REQUEST;
    }
    /* store result */
    res = mysql_store_result(conn);
    /* check that user found */
    if ((row = mysql_fetch_row(res)) == NULL) {
        std::cout << "User second name not found." << std::endl;
        /* free result */
        mysql_free_result(res);
        return err_type_db::ERR_USER_NOT_FOUND;
    }
    else {
        /* print result */
        std::cout << "User second name : ";
        std::cout << row[0] << std::endl;
        while ((row = mysql_fetch_row(res)) != NULL) {
            std::cout << row[0] << std::endl;
        }
    }
    /* free result */
    mysql_free_result(res);
    return err_type_db::ERR_OK;
}

/**
 * @brief Get request for database to receive id of defined user
 * @param userName Name of user to get second name of user
 * @retval Error code
 */
err_type_db DDataBase::DDataBaseGetUserId(std::string userName) {
    int ret = 0;
    /* query to db */
    std::string query;
    query = "select id from users where fname = \"" + userName + "\"";
    ret = mysql_query(conn, query.c_str());
    if (ret != 0) {
        std::cout << "Incorrect request." << std::endl;
        return err_type_db::ERR_INCORRECT_REQUEST;
    }
    /* store result */
    res = mysql_store_result(conn);
    /* check that user found */
    if ((row = mysql_fetch_row(res)) == NULL) {
        std::cout << "User id not found." << std::endl;
        /* free result */
        mysql_free_result(res);
        return err_type_db::ERR_USER_NOT_FOUND;
    }
    else {
        /* print result */
        std::cout << "User id : ";
        std::cout << row[0] << std::endl;
        while ((row = mysql_fetch_row(res)) != NULL) {
            std::cout << row[0] << std::endl;
        }
    }
    /* free result */
    mysql_free_result(res);
    return err_type_db::ERR_OK;
}

/**
 * @brief Get request for database to receive data of defined user
 * @param userName Name to get user data
 * @retval Error code
 */
err_type_db DDataBase::DDataBaseGetUserData(std::string userName) {

    err_type_db errCode = err_type_db::ERR_OK;
    std::cout << "\nRequest for user \"" << userName << "\"." << std::endl;
    /* request for existing user */
    errCode = DDataBaseGetUserFirstName(userName);
    if (errCode != err_type_db::ERR_OK) {
        return errCode;
    }
    /* request for existing user */
    errCode = DDataBaseGetUserSecondName(userName);
    if (errCode != err_type_db::ERR_OK) {
        return errCode;
    }
    /* request for existing user */
    errCode = DDataBaseGetUserId(userName);
    if (errCode != err_type_db::ERR_OK) {
        return errCode;
    }
    return err_type_db::ERR_OK;
}

/**
 * @brief TODO:
 * @retval None
 */
void DDataBase::DDataBaseProcRequest(std::string request, uint32_t state) {
    (void)request;
    (void)state;

    switch (state) {

    case 0:
        DDataBase::DDataBaseGetUserData(request);
        break;

    case 1:
        break;

    default:
        return;
    }
}

/**
 * @brief Database handler
 * @retval None
 */
void DDataBase::handle() {

    err_type_db errCode = err_type_db::ERR_OK;
    /* request for existing user */
    errCode = DDataBaseGetUserData("Dmitry");
    if (errCode != err_type_db::ERR_OK) {
        std::cout << "Database does not response.\n";
    }

    errCode = DDataBaseGetUserData("Petya");
    if (errCode != err_type_db::ERR_OK) {
        std::cout << "Database does not response.\n";
    }

    errCode = DDataBaseGetUserData("Denis");
    if (errCode != err_type_db::ERR_OK) {
        std::cout << "Database does not response.\n";
    }
}