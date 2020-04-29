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
    cout << " ================================== \n";
    cout << " Data processor class object created.\n";
    cout << " ================================== \n\n";
#endif /* DATA_PROC_CONSTR_DESTR_LOG */
    err_type_db errCode = err_type_db::ERR_OK;

    /* initialize data base handler ======================================== */
    dataBaseProcessor = make_shared<DDataBase>("testdb", 3306);
    /* connect to user databse */
    errCode = dataBaseProcessor->DDataBaseConnect("localhost", "adM1n34#184");
    if (errCode != err_type_db::ERR_OK) {
        std::cout << "Database connection has been failed.\n";
        return;
    }
    else {
        std::cout << "Database has  been connected.\n";
    }

    /* initialize JSON parser handler  ===================================== */
    jsonParser = make_shared<JJsonParser>();

    /* single thread for data base handler */
    dataProcHandlerThread = std::thread(&DDataProcess::handle, this);
    dataProcHandlerThread.detach();

}

/**
 * @brief Data processor destructor
 */
DDataProcess::~DDataProcess() {
#if DATA_PROC_CONSTR_DESTR_LOG
    cout << " ================================== \n";
    cout << " Data processor class object removed.\n";
    cout << " ================================== \n\n";
#endif /* DATA_PROC_CONSTR_DESTR_LOG */
    dataProcHandlerThread.~thread();
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

    std::string jsonResp, jsonReq;

    std::cout << "Data processor handler thread started.\n";
    while (1) {
        /* if data processor input queue is not empty */
        if (!dataProcReqsQueueEmpty()) {
            std::string dataProcReq;
            dataProcReq = pullDataProcReqsQueue();
            jsonParser->putJsonReqsQueue(dataProcReq);
        }
        /* if json parser output queue is not empty */
        else if (!jsonParser->jsonRespsQueueEmpty()) {
            jsonResp = jsonParser->pullJsonRespsQueue();
            pushDataProcRespsQueue(jsonResp);
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
void DDataProcess::pushDataProcRespsQueue(std::string sDataProcResponse) {
    dataProcRespsQueue.push(sDataProcResponse);
}

/***
 * @brief Get first response from data process class queue
 */
std::string DDataProcess::pullDataProcRespsQueue(void) {
    std::string dataProcResp = "";
    dataProcResp = dataProcRespsQueue.front();
    dataProcRespsQueue.pop();
    return dataProcResp;
}

/***
 * @brief Put new request in data process class queue
 */
void DDataProcess::pushDataProcReqsQueue(std::string sDataProcRequest) {
    dataProcReqsQueue.push(sDataProcRequest);
}

/***
 * @brief Get first request from data process class queue
 */
std::string DDataProcess::pullDataProcReqsQueue(void) {
    std::string dataProcReq = "";
    dataProcReq = dataProcReqsQueue.front();
    dataProcReqsQueue.pop();
    return dataProcReq;
}

#if UNIT_TEST_DATA_PROCESSOR
bool DDataProcess::GetUnitTestResult(void) {
    return unitTestResult;
}
#endif /* UNIT_TEST_DATA_PROCESSOR */