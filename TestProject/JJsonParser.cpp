/**
 *  @file JJsonParser.cpp
 *  @note Json parser class realization
 *
 *  @date 08.04.2020
 *  @author Kalmykov Dmitry
 *
 *  @modified 10.04.2020
 *  @version 0.3
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

uint32_t parseJsonRequest(string jsonDoc) {

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
#if UNIT_TEST_JSON_PARSER

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

#if TEST1
            string dataForReq = "menu", dataForReq2 = "menu.popup", dataForReq3 = "menu.foo";
#elif TEST2
            string dataForReq = "root.values";
#elif TEST3
            string dataForReq = "foo";
#elif TEST4
            string dataForReq = "validation-factors.validationFactors", dataForReq2 = "password", dataForReq3 = "username";
#endif /* TEST_ITEM */

            std::stringstream req;
            req << testReq;

            try
            {
                pt::ptree tree;
                pt::read_json(req, tree);

#if JSON_PROC_PARSER_LOG
                cout << "Parser started. Request is valid." << endl;
#endif /* JSON_PROC_PARSER_LOG */
                BOOST_FOREACH(auto &v, tree)
                {
                    /* If v.second.size() == 0 it means that current branch does not have any branches.
                       If size is more than 0 then cuurent branch of tree has subbranch */
                    //if (v.first == "username" || v.first == "validation-factors")
                    //{
                        cout << v.first.size() << " "; /*> size is the size of defined block in structure (length of word), 
                        possibly size means the same of size() */
                        std::cout << v.first.data() << " - ";
                        cout << v.second.size() << " "; // number of subtrees
                        std::cout << v.second.get<std::string>("") << std::endl;
                    //}
#if JSON_PROC_PARSER_LOG
                    /*std::cout << "[ " << v.first.size() << " ] \"";
                    std::cout << v.first.data() << "\" : ";
                    std::cout << "[ " << v.second.size() << " ] \"";
                    std::cout << v.second.data() << "\"" << std::endl;*/
#endif /* JSON_PROC_PARSER_LOG */
                }
#if JSON_PROC_PARSER_LOG
                std::cout << std::endl;
#endif /* JSON_PROC_PARSER_LOG */

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
#endif /* UNIT_TEST_JSON_PARSER */
