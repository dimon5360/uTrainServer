/**
 * @file DDataProcess.cpp
 * @note Data processor class realization
 *
 * @date 08.04.2020
 * @author Kalmykov Dmitry
 *
 * @modified 08.04.2020
 * @version 0.1
 */

 /* main classes headers */
#include "main.h"

/* import async */
#include <future>
#include <thread>

/* data processor handler thread */
std::thread dataProcHandlerThread;

/**
 * @brief Data processor constructor
 */
DDataProcess::DDataProcess(void) {
#if DATA_PROC_CONSTR_DESTR_LOG
    ConsoleInfo("Data processor class object created.");
#endif /* DATA_PROC_CONSTR_DESTR_LOG */
    err_type_db errCode = err_type_db::ERR_OK;

    /* initialize data base handler ======================================== */
    //dataBaseProcessor = std::make_shared<DDataBase>("testdb", 3306);
    /* connect to user databse */
    /*errCode = dataBaseProcessor->DDataBaseConnect("localhost", "adM1n34#184");
    if (errCode != err_type_db::ERR_OK) {
        ConsoleError("Database connection has been failed.");
        return;
    }*/

    /* initialize HTTP handler  ============================================ */
    httpHandler = std::make_unique<HHttpHandler>();

    /* single thread for data base handler ================================= */
    std::thread(&DDataProcess::handle, this).detach();
    //handle();
}

/**
 * @brief Data processor destructor
 */
DDataProcess::~DDataProcess() {
}

/**
 * @brief Data processor destructor
 * @note If server received data then data processor takes and
 *       gives it to JSON parser. After this it waits a response.
 *       Then response transfers to data base handler and
 *       data processor waits a response. After response will be transfered
 *       to JSON wrapper and transmited by TCP/IP channel to client.
 *
 */
void DDataProcess::handle() {

    std::string httpHandlerReq, httpHandlerResp;

#if DATA_PROC_HANDLER_LOG
    ConsoleInfo("Data processor handler thread started.");
#endif /* DATA_PROC_HANDLER_LOG */

    while (1) {
        /* if data processor input queue is not empty */
        if (!dataProcReqsQueueEmpty()) {
#if DATA_PROC_HANDLER_LOG
            ConsoleComment("DataProcess queue is not empty.");
#endif /* DATA_PROC_HANDLER_LOG */
            httpHandlerReq = pullDataProcReqsQueue();
            httpHandler->pushHttpHandlerReqsQueue(std::move(httpHandlerReq));
        }
        /* if json parser output queue is not empty */
        else if (!httpHandler->httpHandlerRespsQueueEmpty()) {
#if DATA_PROC_HANDLER_LOG
            ConsoleComment("HTTP handler responses queue is not empty.");
#endif /* DATA_PROC_HANDLER_LOG */
            httpHandlerResp = httpHandler->pullHttpHandlerRespsQueue();
#if DATA_PROC_HANDLER_LOG
            ConsoleComment("Need to send the answer " + string("\"") +
                httpHandlerResp + string("\" ") + "from HTTP handler queue"
                "to DataProcess responses queue.");
#endif /* DATA_PROC_HANDLER_LOG */
            pushDataProcRespsQueue(std::move(httpHandlerResp));
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(THREAD_TIMEOUT));
    }
}

/***
 * @brief Returns true if queue with requests is empty
 */
bool DDataProcess::dataProcReqsQueueEmpty(void) {
    return dataProcReqsQueue.empty();
}

/***
 * @brief Returns true if queue with responses is empty
 */
bool DDataProcess::dataProcRespsQueueEmpty(void) {
    return dataProcRespsQueue.empty();
}

/***
 * @brief Put response in data process class queue 
 */
void DDataProcess::pushDataProcRespsQueue(std::string&& sDataProcResponse) {
    try {
        const std::lock_guard<std::mutex> lock(mutex_);
        dataProcRespsQueue.push(sDataProcResponse);
    }
    catch (std::exception& ex) {
        std::cout << ex.what() << std::endl;
    }
}

/***
 * @brief Get first response from data process class queue
 */
const std::string& DDataProcess::pullDataProcRespsQueue(void) {
    std::string dataProcResp;
    try {
        const std::lock_guard<std::mutex> lock(mutex_);
        dataProcResp = dataProcRespsQueue.front();
        dataProcRespsQueue.pop();
    }
    catch (std::exception& ex) {
        std::cout << ex.what() << std::endl;
    }
    return dataProcResp;
}

/***
 * @brief Put new request in data process class queue
 */
void DDataProcess::pushDataProcReqsQueue(std::string&& sDataProcRequest) {
    try {
        const std::lock_guard<std::mutex> lock(mutex_);
        dataProcReqsQueue.push(sDataProcRequest);
    }
    catch (std::exception& ex) {
        std::cout << ex.what() << std::endl;
    }
}

/***
 * @brief Get first request from data process class queue
 */
const std::string& DDataProcess::pullDataProcReqsQueue(void)  {
    std::string dataProcReq;
    try {
        const std::lock_guard<std::mutex> lock(mutex_);
        dataProcReq = dataProcReqsQueue.front();
        dataProcReqsQueue.pop();
    }
    catch (std::exception& ex) {
        std::cout << ex.what() << std::endl;
    }
    return dataProcReq;
}

#if UNIT_TEST_DATA_PROCESSOR
bool DDataProcess::GetUnitTestResult(void) {
    return unitTestResult;
}
#endif /* UNIT_TEST_DATA_PROCESSOR */