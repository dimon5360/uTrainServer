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
            err_type_jp errCode = err_type_jp::ERR_OK;
#if TEST1
            string reqForValidFactors = "validationFactors", 
                   reqForPassword = "password", 
                   reqForUsername = "username";
#elif TEST2
            string reqForValidFactors = "validationFactors",
                reqForPassword = "password",
                reqForUsername = "username";
#endif /* TEST_ITEM */

            std::stringstream req;
            req << testReq;

            try
            {
                pt::ptree tree;
                pt::read_json(req, tree);

                std::cout << "\n#1 json parser unit test.\n";
                errCode = parseJsonRequest("", tree);
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

err_type_jp JJsonParser::parseJsonRequest(string jsonKey, pt::ptree tree) {
    err_type_jp errCode = err_type_jp::ERR_OK;

#if JSON_PROC_PARSER_LOG
    cout << "\nKey of pair : Value of pair" << endl;
#endif /* JSON_PROC_PARSER_LOG */

    cout << "\n======== Req = " << jsonKey << endl;

    /* loop checks all values */
    BOOST_FOREACH(auto & v, tree)
    {
        /* If v.second.size() == 0 it means that current branch does not have any branches.
           If size is more than 0 then cuurent branch of tree has subbranch */
        if (!v.second.data().empty()) {
            cout << "[" << v.first.size() << "] "; /*> size is the size of defined block in structure (length of word),
            possibly size means the same of size() */
            std::cout << v.first.data() << " : ";
            cout << "[" << v.second.data().size() << "] "; // number of subtrees
            std::cout << v.second.get<std::string>("");
            std::cout << std::endl;    
        }
        else {
            if (!v.first.empty()) {
                cout << "\t[" << v.first.size() << "] "; /*> size is the size of defined block in structure (length of word),
                possibly size means the same of size() */
                std::cout << v.first.data() << " : <size of next data = " << tree.get_child(v.first.data()).back().first.size() << ">\n";
                if (tree.get_child(v.first.data()).back().first.size() != 0) {
                    errCode = parseJsonRequest((jsonKey + v.first.data() + string(".")), tree.get_child(v.first.data()));
                    if (errCode == err_type_jp::ERR_OK) {
                        continue;
                    }
                }
                else {
                    cout << "v.second is array\n";
                    for (auto& e : tree.get_child(v.first.data())) {
                        for (auto& kv : e.second) {
                            cout << "[" << kv.first.size()  << "] " << kv.first << " : ";
                            cout << "[" << e.second.get<std::string>(kv.first).size() << "] " << e.second.get<std::string>(kv.first) << endl;
                        }
                    }
                }
            }
            else {
                return err_type_jp::ERR_PARSE_FAILED;
            }
        }
    }
    return errCode;

}