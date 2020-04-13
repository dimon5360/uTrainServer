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

                err_type_jp errCode = parseJsonRequest(dataForReq, tree);
                if (errCode != err_type_jp::ERR_OK) {
                    std::cout << "Database has failed.";
                    return;
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

err_type_jp JJsonParser::parseJsonRequest(string jsonDoc, pt::ptree tree) {
    err_type_jp errCode = err_type_jp::ERR_OK;

#if JSON_PROC_PARSER_LOG
    //cout << "\nKey of pair : Value of pair" << endl;
#endif /* JSON_PROC_PARSER_LOG */

    /* loop checks all values */
    BOOST_FOREACH(auto & v, tree)
    {
        /* If v.second.size() == 0 it means that current branch does not have any branches.
           If size is more than 0 then cuurent branch of tree has subbranch */
        if (v.second.size() == 0) {
            cout << "[" << v.first.size() << "] "; /*> size is the size of defined block in structure (length of word),
            possibly size means the same of size() */
            std::cout << v.first.data() << " : ";
            cout << "[" << v.second.size() << "] "; // number of subtrees
            std::cout << v.second.get<std::string>("");
            std::cout << std::endl;            
        }
        else {
            if (tree.get_child(jsonDoc).count(v.first.data()) == 0) {
                cout << "[" << tree.get_child(jsonDoc).count(v.first.data()) << "] ";
                cout << v.first.data() << " : " << endl;
                cout << "[" << v.second.size() << "] "; // number of subtrees
                std::cout << v.second.get<std::string>("");
            }
            else {
                string firstData = v.first.data();
                cout << "[" << v.first.size() << "] "; /*> size is the size of defined block in structure (length of word),
                possibly size means the same of size() */
                std::cout << firstData << " : ";
                errCode = parseJsonRequest(firstData, tree.get_child((firstData)));
                if (errCode != err_type_jp::ERR_OK) {
                    std::cout << "Database has failed.";
                    return err_type_jp::ERR_PARSE_FAILED;
                }
            }
        }
    }
    std::cout << std::endl;
    return err_type_jp::ERR_OK;

}