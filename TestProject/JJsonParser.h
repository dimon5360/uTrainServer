/**
 *  @file JJsonParser.h
 *  @note Json parser class prototype
 *
 *  @date 08.04.2020
 *  @author Kalmykov Dmitry
 *
 *  @modified 08.04.2020
 *  @version 0.1
 */

#pragma once
#include "config.h"

#include <cstring>
#include <cstdlib>
#include <vector>
#include <string>
#include <cstdint>
#include <queue>
#include <map>
#include <thread>

using namespace std;

class JJsonParser
{
private:
    vector<string> inputData;
    queue<string> jsonQueue;

    /* Handle ------------------------------------- */
    /* main json thread handler */
    void jsonHandler(void);

    /* interfaces for json queue ----------------- */
    /* Get JSON data from queue */
    string getJsonReqQueue(void);
    /* Put JSON data to queue */
    void putJsonReqQueue(string);

public:
    /* constructor */
    JJsonParser();
    /* destructor */
    ~JJsonParser();



/* UNIT_TESTS -------------------------------------------------------------- */
private:
#if UNIT_TEST_QUEUE_EXCHANGE
    bool unitTestResult = false;
#endif /* UNIT_TEST_QUEUE_EXCHANGE */

public:
#if UNIT_TEST_QUEUE_EXCHANGE
    /* redefined constructor for unit test */
    JJsonParser(string testReq);
    /* unit test result for test queue */
    bool GetUnitTestResult(void);
#endif /* UNIT_TEST_QUEUE_EXCHANGE */
       
};

/* test JSON parser */
extern uint32_t jsonParse(string jsonDoc);
