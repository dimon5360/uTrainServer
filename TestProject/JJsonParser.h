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
    queue<string> jsonParserReqsQueue;
    queue<string> jsonParserRespsQueue;
    /* Parse JSON string */
    err_type_jp parseJsonRequest(string jsonDoc, pt::ptree tree, std::string offset);
    /* Process JSON string */
    void procJsonRequest(string jsonDoc);
       
public:
    /* Handle ------------------------------------- */
    /* main json thread handler */
    void handle(void);

    /* constructor */
    JJsonParser(void);
    /* destructor */
    ~JJsonParser(void);

    /* interfaces for json queue ----------------- */
    /* Put JSON data to queue */
    void pushJsonRespsQueue(string);
    /* Put JSON data to queue */
    void pushJsonReqsQueue(string);
    /* Get JSON data from queue */
    string pullJsonRespsQueue(void);
    /* Get JSON data from queue */
    string pullJsonReqsQueue(void);

    bool jsonReqsQueueEmpty(void);
    bool jsonRespsQueueEmpty(void);

/* UNIT_TESTS -------------------------------------------------------------- */
private:
    bool unitTestResult = false;

public:
    /* unit test result for test queue */
    bool GetUnitTestResult(void);
       
};
