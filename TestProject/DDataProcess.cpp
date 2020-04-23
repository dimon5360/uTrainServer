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

 /* presprocessor configuration */
#include "config.h"
#include "main.h"

/* import async */
#include <future>
#include <thread>

std::thread dataBaseThread;

/**
 * @brief Data processor constructor
 */
DDataProcess::DDataProcess(void) {
    err_type_db errCode = err_type_db::ERR_OK;
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

    jsonParser = make_shared<JJsonParser>();

    /* single thread for data base handler */
    dataBaseThread = std::thread(&DDataProcess::handle, this);
    dataBaseThread.detach();
}

/**
 * @brief Data processor destructor
 */
DDataProcess::~DDataProcess() {
    dataBaseThread.~thread();
}

/***
 * @brief Put new request in data process class queue 
 */
void DDataProcess::pushDataProcReqQueue(std::string sDataProcRequest) {
#if DATA_PROC_PUSH_PULL_LOG
    cout << "Msg put to queue: " << sDataProcRequest << endl;
#endif /* DATA_PROC_PUSH_PULL_LOG */
    inDataProcQueue.push(sDataProcRequest);
}

/***
 * @brief Get new request from data process class queue
 */
std::string DDataProcess::pullDataProcRespQueue(void) {
    std::string dataProcReq = "";
    if (!outDataProcQueue.empty()) {
        dataProcReq = outDataProcQueue.front();
        outDataProcQueue.pop();
#if DATA_PROC_PUSH_PULL_LOG
        cout << "Msg got from queue: " << dataProcReq << endl;
#endif /* DATA_PROC_PUSH_PULL_LOG */
    }
    return dataProcReq;
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

    std::string dataProcReq;
    while (1) {
        if (!inDataProcQueue.empty()) {
            dataProcReq = inDataProcQueue.front();
            std::cout << dataProcReq << std::endl;

            jsonParser->putJsonReqQueue(dataProcReq);

            jsonParser->jsonHandle();

            // TODO: transmit to JSON parser
            inDataProcQueue.pop();
        }
        else if(inDataProcQueue.size() == 0) { // while uses unit tests
            unitTestResult = true;
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(3));
    }
}

#if UNIT_TEST_DATA_PROCESSOR
bool DDataProcess::GetUnitTestResult(void) {
    return unitTestResult;
}
#endif /* UNIT_TEST_DATA_PROCESSOR */