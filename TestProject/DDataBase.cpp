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
err_type DDataBase::DDataBaseConnect(std::string hostAddr, std::string pass) {
    
    err_type errCode = err_type::ERR_OK;
    conn = mysql_real_connect(&mysql, hostAddr.c_str(), "root",
        pass.c_str(), dbName.c_str(), dbPort, NULL, 0);
    if (!conn) {
        std::cout << mysql_error(&mysql) << std::endl;
        return err_type::ERR_DB_CONNECT;
    }
    std::cout << "Database connected." << std::endl;

    /* db host information */
    std::string hostInfo = mysql_get_host_info(&mysql);
    if (hostInfo.empty()) {
        std::cout << mysql_error(&mysql) << std::endl;
        return err_type::ERR_NOT_GOT_HOST_INFO;
    }
    std::cout << hostInfo << std::endl;

    errCode = RequestDatabaseUserNumber();
    if (errCode != err_type::ERR_OK) {
        std::cout << "Database has no users.\n";
    }

    /* request for existing user */
    errCode = DDataBaseGetUserName("Dmitry");
    if (errCode != err_type::ERR_OK) {
        std::cout << "Database does not response.\n";
        return err_type::ERR_DB_DOESNT_RESPONSE;
    }

    errCode = DDataBaseGetUserName("Petya");
    if (errCode != err_type::ERR_OK) {
        std::cout << "Database does not response.\n";
        return err_type::ERR_DB_DOESNT_RESPONSE;
    }

    /* process database requests */
    //handle();
    return err_type::ERR_OK;
}

/**
 * @brief Get request for database to receive number of active users
 * @retval Error code
 */
err_type DDataBase::RequestDatabaseUserNumber() {
    int ret = 0;
    /* query to db */
    ret = mysql_query(conn, "select user_count()");
    if (ret != 0) {
        std::cout << "Incorrect request." << std::endl;
        return err_type::ERR_INCORRECT_REQUEST;
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
    return err_type::ERR_OK;
}

/**
 * @brief Get request for database to receive number of active users
 * @param userName Name of user that must be checked
 * @retval Error code
 */
err_type DDataBase::DDataBaseGetUserName(std::string userName) {
    int ret = 0;
    /* query to db */
    std::string query;
    query = "select fname from users where fname = \"" + userName + "\"";
    ret = mysql_query(conn, query.c_str());
    if (ret != 0) {
        std::cout << "Incorrect request." << std::endl;
        return err_type::ERR_INCORRECT_REQUEST;
    }
    /* store result */
    res = mysql_store_result(conn);
    /* print result */
    std::cout << "User name : " << std::endl;
    while ((row = mysql_fetch_row(res)) != NULL) {
        std::cout << row[0] << std::endl;
    }
    std::cout << std::endl;
    /* free result */
    mysql_free_result(res);
    return err_type::ERR_OK;
}

/**
 * @brief TODO:
 * @retval Error code
 */
err_type DDataBase::DDataBaseGetUserData(std::string login) {
    
    mysql_query(conn, "select * from ");
    return err_type::ERR_OK;
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

    while (1) {
        if (DDataBase::DDataBaseGetUserName("Dmitry") > 0) {
            DDataBaseProcRequest("", 0);
            /* TODO: */
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(3));
    }
}