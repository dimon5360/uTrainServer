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
    jsonParserQueue.push(jsonReq);
}

/**
 * @brief Get JSON data from queue
 */
string JJsonParser::pullJsonReqQueue(void) {
    string jsonString;
    jsonString = jsonParserQueue.front();
    jsonParserQueue.pop();
#if JSON_PROC_PUSH_PULL_LOG
    cout << "Msg got from queue: " << jsonString << endl;
#endif /* JSON_PROC_PUSH_PULL_LOG */
    return jsonString;
}

/**
 * @brief JSON request parser
 */
err_type_jp JJsonParser::parseJsonRequest(string jsonKey, pt::ptree tree) {
    err_type_jp errCode = err_type_jp::ERR_OK;

#if JSON_PROC_PARSER_LOG
    cout << "\n======== Req = " << jsonKey << endl;
#endif /* JSON_PROC_PARSER_LOG */
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
        else  if (!v.first.empty()) {
            cout << "\t[" << v.first.size() << "] "; /*> size is the size of defined block in structure (length of word),
            possibly size means the same of size() */
            std::cout << v.first.data() << " : <size of next data = " << tree.get_child(v.first.data()).back().first.size() << ">\n";
            if (tree.get_child(v.first.data()).back().first.size() != 0) {
                errCode = parseJsonRequest((jsonKey + v.first.data() + string(".")), tree.get_child(v.first.data()));
                if (errCode == err_type_jp::ERR_OK) {
                    break;
                }
            }
            else {
                for (auto& e : tree.get_child(v.first.data())) {
                    for (auto& kv : e.second) {
                        cout << "[" << kv.first.size() << "] " << kv.first << " : ";
                        cout << "[" << e.second.get<std::string>(kv.first).size() << "] " << e.second.get<std::string>(kv.first) << endl;
                    }
                }
            }
        }
        else {
            return err_type_jp::ERR_PARSE_FAILED;
        }
    }
    return errCode;
}

/**
 * @brief JSON parser main handler
 */
void JJsonParser::jsonHandle(void) {

    err_type_jp errCode = err_type_jp::ERR_OK;

    string jsonReq;
    while (true) {

        /* check if queue is empty */
        if (!jsonParserQueue.empty()) {
            jsonReq = pullJsonReqQueue();

            std::stringstream req;
            req << jsonReq;

            try
            {
                pt::ptree tree;
                pt::read_json(req, tree);

                errCode = parseJsonRequest("", tree);
                if (errCode != err_type_jp::ERR_OK) {
                    std::cout << "JSON request parsing failed.\n";
                    return;
                }
                else {
                    // TODO: need top transfer this data to data processor class
                    std::cout << "JSON request parsing succeed.\n";
#if UNIT_TEST_JSON_PARSER
                    unitTestResult = true;
                    break;
#endif /* UNIT_TEST_JSON_PARSER */
                }
            }
            catch (std::exception const& e)
            {
                std::cerr << e.what() << std::endl;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(3));
    }
}

#if UNIT_TEST_JSON_PARSER
/***
 * @brief TODO
 */
bool JJsonParser::GetUnitTestResult(void) {
    return unitTestResult;
}
#endif /* UNIT_TEST_JSON_PARSER */
