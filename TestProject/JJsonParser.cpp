/**
 *  @file JJsonParser.cpp
 *  @note Json parser class realization
 *
 *  @date 08.04.2020
 *  @author Kalmykov Dmitry
 *
 *  @modified 08.04.2020
 *  @version 0.1
 */

#include "main.h"


/**
 * @brief JSON parser class constructor
 */
JJsonParser::JJsonParser(void) {
    /* start JSON handler  */
    jsonHandler();
}

/**
 * @brief JSON parser class destructor
 */
JJsonParser::~JJsonParser(void) {

}

/**
 * @brief Put JSON data to queue
 */
void JJsonParser::putJsonReqQueue(string jsonReq) {
#if JSON_PROC_PUSH_PULL_LOG
    cout << "Msg put to queue: " << jsonReq << endl;
#endif /* JSON_PROC_PUSH_PULL_LOG */
    jsonQueue.push(jsonReq);
}

/**
 * @brief Get JSON data from queue
 */
string JJsonParser::getJsonReqQueue(void) {
    string jsonString;
    jsonString = jsonQueue.front();
#if JSON_PROC_PUSH_PULL_LOG
    cout << "Msg got from queue: " << jsonString << endl;
#endif /* JSON_PROC_PUSH_PULL_LOG */
    return jsonString;
}

uint32_t jsonParse(string jsonDoc) {

    return 0;
}

/**
 * @brief JSON parser main handler
 */
void JJsonParser::jsonHandler(void) {

    string jsonReq;
    while (true) {

        /* check if queue is empty */
        if (!jsonQueue.empty()) {
            jsonReq = getJsonReqQueue();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(3));
    }
}

/* UNIT_TESTS -------------------------------------------------------------- */
#if UNIT_TEST_QUEUE_EXCHANGE
/**
    * @brief JSON parser class constructor
    */
JJsonParser::JJsonParser(string testReq) {
    inputData.push_back(testReq);
    /* put in queue test request */
    putJsonReqQueue(testReq);
    /* check if queue is empty */
    if (!jsonQueue.empty()) {
        string jsonReq = getJsonReqQueue();
        if (jsonReq == inputData.back()) {
            inputData.clear();
            unitTestResult = true;
        }
    }
    return;
}

bool JJsonParser::GetUnitTestResult(void) {
    return unitTestResult;
}
#endif /* UNIT_TEST_QUEUE_EXCHANGE */