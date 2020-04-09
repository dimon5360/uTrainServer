/**
 *  @file JJsonParser.cpp
 *  @note Json parser class realization
 *
 *  @date 08.04.2020
 *  @author Kalmykov Dmitry
 *
 *  @modified 09.04.2020
 *  @version 0.2
 */

#include "config.h"
#include "main.h"

/**
 * @brief JSON parser class constructor
 */
JJsonParser::JJsonParser(void) {
#if JSON_PROC_CONSTR_DESTR_LOG
    cout << "Json Parser class object removed.\n";
#endif /* JSON_PROC_CONSTR_DESTR_LOG */
    /* start JSON handler  */
    jsonHandler();
}

/**
 * @brief JSON parser class destructor
 */
JJsonParser::~JJsonParser(void) {
#if JSON_PROC_CONSTR_DESTR_LOG
    cout << "Json Parser class object removed.\n";
#endif /* JSON_PROC_CONSTR_DESTR_LOG */
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
#if UNIT_TEST_QUEUE_EXCHANGE || UNIT_TEST_JSON_PARSE

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/foreach.hpp>
#include <string>
#include <set>
#include <exception>
#include <iostream>

namespace pt = boost::property_tree;

/**
 * @brief JSON parser class constructor
 */
JJsonParser::JJsonParser(string testReq, bool parsedNeed) {
    /* put in queue test request */
    putJsonReqQueue(testReq);
    /* check if queue is empty */
    if (!jsonQueue.empty()) {
        string jsonReq = getJsonReqQueue();
        if (!parsedNeed && jsonReq == testReq) {
            inputData.clear();
            unitTestResult = true;
        }
        else if (parsedNeed) {

            std::stringstream req; 

#if TEST1
            req << "{ \"menu\": { \"foo\": [ \"ad \", 4, 3, 4, 5 ], \"bar\": \"true\", \"value\": 102.3E+06,\
                     \"popup\": { \"value\": \"New\", \"onclick\": \"CreateNewDoc()\" } } }";
            string dataForReq = "menu", dataForReq2 = "menu.popup", dataForReq3 = "menu.foo";
#elif TEST2
            req << "{ \"root\": { \"values\": [1, 2, 3, 4, 5 ] } }";
            string dataForReq = "root.values";
#elif TEST3
            req << "{ \"foo\" : \"bar\" }";
            string dataForReq = "foo";
#endif /* TEST_ITEM */

            try
            {
                pt::ptree tree;
                pt::read_json(req, tree);

                BOOST_FOREACH(boost::property_tree::ptree::value_type &v, tree.get_child(dataForReq))
                {
                    if (v.first.empty() || v.second.size()) { // TODO: need to check 'what does this mean'
#if JSON_PROC_PARSER_LOG
                        std::cout << v.first.data() << " ";
#endif /* JSON_PROC_PARSER_LOG */
                        continue;
                    }
#if JSON_PROC_PARSER_LOG
                    std::cout << v.first.data() << " : ";
                    std::cout << v.second.data() << std::endl;
#endif /* JSON_PROC_PARSER_LOG */
                }
#if JSON_PROC_PARSER_LOG
                std::cout << std::endl;
#endif /* JSON_PROC_PARSER_LOG */

#if TEST1
                BOOST_FOREACH(boost::property_tree::ptree::value_type& v, tree.get_child(dataForReq2))
                {
                    if (v.first.empty() || v.second.size()) {
#if JSON_PROC_PARSER_LOG
                        std::cout << v.second.data() << " ";
#endif /* JSON_PROC_PARSER_LOG */
                        continue;
                    }
#if JSON_PROC_PARSER_LOG
                    std::cout << v.first.data() << " : ";
                    std::cout << v.second.data() << std::endl;
#endif /* JSON_PROC_PARSER_LOG */
                }
#if JSON_PROC_PARSER_LOG
                std::cout << std::endl;
#endif /* JSON_PROC_PARSER_LOG */

                BOOST_FOREACH(boost::property_tree::ptree::value_type& v, tree.get_child(dataForReq3))
                {
                    if (v.first.empty() || v.second.size()) {
#if JSON_PROC_PARSER_LOG
                        std::cout << v.second.data() << " ";
#endif /* JSON_PROC_PARSER_LOG */
                        continue;
                    }
#if JSON_PROC_PARSER_LOG
                    std::cout << v.first.data() << " : ";
                    std::cout << v.second.data() << std::endl;
#endif /* JSON_PROC_PARSER_LOG */
                }
#if JSON_PROC_PARSER_LOG
                std::cout << std::endl;
#endif /* JSON_PROC_PARSER_LOG */
#endif /* TEST1 */

                unitTestResult = true;
                return;

            }
            catch (std::exception const& e)
            {
                std::cerr << e.what() << std::endl;
            }
        }
    }
}


bool JJsonParser::GetUnitTestResult(void) {
    return unitTestResult;
}
#endif /* UNIT_TEST_QUEUE_EXCHANGE || UNIT_TEST_JSON_PARSE */
