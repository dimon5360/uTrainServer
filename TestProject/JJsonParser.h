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
#include "main.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>

using namespace std;
namespace pt = boost::property_tree;

enum class err_type_jp {
    ERR_OK = 0,
    ERR_PARSE_FAILED,
    ERR_ALL_CODES_MOUNT
};

class JJsonParser
{
private:
    vector<string> inputData;
    queue<string> jsonParserQueue;
    /* Parse JSON string */
    err_type_jp parseJsonRequest(string jsonDoc, pt::ptree tree);
       
public:
    /* Handle ------------------------------------- */
    /* main json thread handler */
    void jsonHandle(void);
    /* constructor */
    JJsonParser(void);
    /* destructor */
    ~JJsonParser(void);
    /* interfaces for json queue ----------------- */
    /* Get JSON data from queue */
    string pullJsonReqQueue(void);
    /* Put JSON data to queue */
    void putJsonReqQueue(string);
    

/* UNIT_TESTS -------------------------------------------------------------- */
private:
#if UNIT_TEST_JSON_PARSER
    bool unitTestResult = false;
#endif /* UNIT_TEST_JSON_PARSER */

public:
#if UNIT_TEST_JSON_PARSER
    /* redefined constructor for unit test */
    JJsonParser(string testReq);
    /* unit test result for test queue */
    bool GetUnitTestResult(void);
#endif /* UNIT_TEST_JSON_PARSER */
       
};
