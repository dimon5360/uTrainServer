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

 /* std C++ lib headers */
#include <mutex>

namespace pt = boost::property_tree;

enum class err_type_jp {
    ERR_OK = 0,
    ERR_PARSE_FAILED,
    ERR_ALL_CODES_MOUNT
};

class JJsonParser
{
private:
    std::queue<std::string> jsonParserReqsQueue;
    std::queue<std::string> jsonParserRespsQueue;
    /* Parse JSON string */
    err_type_jp parseJsonRequest(std::string jsonDoc, pt::ptree tree, std::string offset);
    /* Process JSON string */
    void procJsonRequest(std::string&& jsonDoc);
    
    /* mutex object to avoid data race */
    mutable std::mutex mutex_;
       
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
    void pushJsonRespsQueue(std::string&&);
    /* Put JSON data to queue */
    void pushJsonReqsQueue(std::string&&);
    /* Get JSON data from queue */
    const std::string& pullJsonRespsQueue(void);
    const std::string& pullJsonReqsQueue(void);

    bool jsonReqsQueueEmpty(void);
    bool jsonRespsQueueEmpty(void);

/* UNIT_TESTS -------------------------------------------------------------- */
private:
    bool unitTestResult = false;

public:
    /* unit test result for test queue */
    bool GetUnitTestResult(void);
       
};
