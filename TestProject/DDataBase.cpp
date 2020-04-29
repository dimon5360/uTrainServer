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

 /* main classes headers */
#include "main.h"

/* data base handler thread */
std::thread dataBaseHandlerThread;

/**
 * @brief DDataBase class constructor
 */
DDataBase::DDataBase(std::string s_name, uint32_t u_port) {
#if DATA_BASE_CONSTR_DESTR_LOG
    cout << " ================================== \n";
    cout << " Data base class object created.\n";
    cout << " ================================== \n\n";
 #endif /* DATA_BASE_CONSTR_DESTR_LOG */
    this->dbName = s_name;
    this->dbPort = u_port;
    DDataBaseInit();

    /* single thread for data base handler */
    dataBaseHandlerThread = std::thread(&DDataBase::handle, this);
    dataBaseHandlerThread.detach();
}

/**
 * @brief DDataBase class destructor
 */
DDataBase::~DDataBase(void) {
#if DATA_BASE_CONSTR_DESTR_LOG
    cout << " ================================== \n";
    cout << " Data base class object removed.\n";
    cout << " ================================== \n\n";
#endif /* DATA_BASE_CONSTR_DESTR_LOG */
    mysql_close(conn);
    dataBaseHandlerThread.~thread();
}

/**
 * @brief Init database connection 
 */
void DDataBase::DDataBaseInit(void) {
    mysql_init(&mysql);
}

/***
 * @brief Returns true if queue is empty
 */
bool DDataBase::dataBaseQueueEmpty(void) {
    return dataBaseQueue.empty();
}

/***
 * @brief Put new request in data base class queue
 */
void DDataBase::DDataBasePushRequest(std::string sDataBaseRequest) {
    dataBaseQueue.push(sDataBaseRequest);
}

/***
 * @brief Get new request from data base class queue
 */
std::string DDataBase::DDataBasePullRequest(void) {
    std::string dataBaseReq;
    dataBaseReq = dataBaseQueue.front();
    dataBaseQueue.pop();
    return dataBaseReq;
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
    else {
        return errCode;
    }
    return errCode;
}

/**
 * @brief Get request for database to receive number of active users
 * @retval Error code
 */
err_type_db DDataBase::RequestDatabaseUserNumber(void) {
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
 * @brief Get request for database to receive data of defined user
 * @param userName Name of defined user to get data
 * @param req Parameter of defined user that need to get
 * @retval Error code
 */
err_type_db DDataBase::DDataBaseUserDataReq(std::string req, std::string userName) {

    int ret = 0;
    /* query to db */
    std::string query;
    query = "select " + req + " from users where fname = \"" + userName + "\"";
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
        std::cout << "User " + req + ": ";
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
    errCode = DDataBaseUserDataReq("fname", userName);
    if (errCode != err_type_db::ERR_OK) {
        return errCode;
    }
    /* request for existing user */
    errCode = DDataBaseUserDataReq("sname", userName);
    if (errCode != err_type_db::ERR_OK) {
        return errCode;
    }
    /* request for existing user */
    errCode = DDataBaseUserDataReq("id", userName);
    if (errCode != err_type_db::ERR_OK) {
        return errCode;
    }
    return err_type_db::ERR_OK;
}

/**
 * @brief Process input request
 *
 * @param request Request to process
 * @param state State for request to process
 * @retval None
 */
void DDataBase::DDataBaseProcRequest(std::string request, uint32_t state) {

    switch (state) {

    case 0:
        DDataBaseGetUserData(request);
        break;

    case 1:
        break;

    default:
        return;
    }
}

/**
 * @brief Data base thread handler
 * @retval None
 */
void DDataBase::handle() {

    std::cout << "Data base handler thread started.\n";

    while (1) {


        std::this_thread::sleep_for(std::chrono::milliseconds(3));
    }
}