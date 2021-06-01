/**
 * @file DDataProcess.h
 * @note Data processor class prototype
 *
 * @date 08.04.2020
 * @author Kalmykov Dmitry
 *
 * @modified 08.04.2020
 * @version 0.1
 */ 

#pragma once
 /* main classes headers */
#include "main.h"
/* import data base class */
#include "DDataBase.h"
/* import JSON parser class */
#include "HHttpHandler.h"

/* import data type string */
#include <string>
#include <cstdint>
/* import data queue */
#include <queue>
/* import unique pointer */
#include <memory>
/* import mutex class*/
#include <mutex>


 /* error codes for udata process class ------------------------------------ */
enum class err_type_dp {
    ERR_OK = 0,

    ERR_ALL_CODES_MOUNT
};

class DDataProcess {

private:
    /* queue for input requests */
    std::queue<std::string> dataProcReqsQueue;
    /* queue for output reesponses */
    std::queue<std::string> dataProcRespsQueue;

    /* pointer to data base processor class */
    std::shared_ptr<DDataBase> dataBaseProcessor;
    //std::shared_ptr<JJsonParser> jsonParser;
    std::unique_ptr<HHttpHandler> httpHandler;
    /* mutex object to avoid data race */
    mutable std::mutex mutex_;

public:
    /* data process handler */
    void handle();
    /* public prototypes */
    DDataProcess();
    ~DDataProcess();

    /* get new response from data process class queue */
    void pushDataProcRespsQueue(std::string&& sDataProcRequest);
    /* get new response from data process class queue */
    const std::string& pullDataProcRespsQueue(void);
    /* put new request in data process class queue */
    void pushDataProcReqsQueue(std::string&& sDataProcRequest);
    /* get new request in data process class queue */
    const std::string& pullDataProcReqsQueue(void);

    bool dataProcReqsQueueEmpty(void);
    bool dataProcRespsQueueEmpty(void);

    /* UNIT_TESTS ---------------------------------------------------------- */
private:

#if UNIT_TEST_DATA_PROCESSOR
    bool unitTestResult = false;
#endif /* UNIT_TEST_DATA_PROCESSOR */

public:
#if UNIT_TEST_DATA_PROCESSOR
    /* unit test result for test queue */
    bool GetUnitTestResult(void);
#endif /* UNIT_TEST_DATA_PROCESSOR */
};

