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
    JJsonParser(void);
    /* destructor */
    ~JJsonParser(void);
    

/* UNIT_TESTS -------------------------------------------------------------- */
private:
#if UNIT_TEST_QUEUE_EXCHANGE || UNIT_TEST_JSON_PARSE
    bool unitTestResult = false;
#endif /* UNIT_TEST_QUEUE_EXCHANGE || UNIT_TEST_JSON_PARSE */

public:
#if UNIT_TEST_QUEUE_EXCHANGE || UNIT_TEST_JSON_PARSE
    /* redefined constructor for unit test */
    JJsonParser(string testReq, bool parsedNeed = false);
    /* unit test result for test queue */
    bool GetUnitTestResult(void);
#endif /* UNIT_TEST_QUEUE_EXCHANGE || UNIT_TEST_JSON_PARSE */
       
};

/* test JSON parser */
extern uint32_t jsonParse(string jsonDoc);
