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

/* import thread */
#include <thread>

/* JSON parser handler thread */
std::thread jsonParserHandlerThread;

/**
 * @brief JSON parser class constructor
 */
JJsonParser::JJsonParser(void) {
#if JSON_PROC_CONSTR_DESTR_LOG
    ConsoleInfo("Json Parser class object created.");
#endif /* JSON_PROC_CONSTR_DESTR_LOG */

    /* single thread for data base handler */
    jsonParserHandlerThread = std::thread(&JJsonParser::handle, this);
    jsonParserHandlerThread.detach();
}

/**
 * @brief JSON parser class destructor
 */
JJsonParser::~JJsonParser(void) {
#if JSON_PROC_CONSTR_DESTR_LOG
    ConsoleInfo("Json Parser class object removed.");
#endif /* JSON_PROC_CONSTR_DESTR_LOG */
    jsonParserHandlerThread.~thread();
}

/**
 * @brief JSON parser main handler
 */
void JJsonParser::handle(void) {
#if DATA_PROC_CALLED_FUNCTION
    ConsoleFunctionNameLog(__FUNCTION__);
#endif /* DATA_PROC_CALLED_FUNCTION */

#if JSON_PARSER_HANDLER_LOG
    ConsoleInfo("JSON parser handler thread started.");
#endif /* JSON_PARSER_HANDLER_LOG */

    string jsonReq;
    while (true) {

        /* check if queue is empty */
        if (!jsonReqsQueueEmpty()) {
            jsonReq = pullJsonReqsQueue();
            procJsonRequest(jsonReq);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(THREAD_TIMEOUT));
    }
}

/***
 * @brief Process input JSON doc
 */
void JJsonParser::procJsonRequest(string jsonDoc) {
#if DATA_PROC_CALLED_FUNCTION
    ConsoleFunctionNameLog(__FUNCTION__);
#endif /* DATA_PROC_CALLED_FUNCTION */

    static err_type_jp errCode = err_type_jp::ERR_OK;
    /* string stream for JSON req in tree format */
    static std::stringstream req;
    /* put input json doc in string stream */
    req << jsonDoc;

    try
    {
        pt::ptree tree;
        pt::read_json(req, tree);

        errCode = parseJsonRequest("", tree, "");
        if (errCode != err_type_jp::ERR_OK) {
#if JSON_PROC_PARSER_LOG
            ConsoleError("JSON request parsing failed.");
#endif /* JSON_PROC_PARSER_LOG */
        }
        else {
            pushJsonRespsQueue("JSON request parsing succeed.");
        }
    }
    catch (std::exception const& e)
    {
        std::cerr << e.what() << std::endl;
    }
}

/***
 * @brief Returns true if queue with requests is empty
 */
bool JJsonParser::jsonReqsQueueEmpty(void) {
    return jsonParserReqsQueue.empty();
}

/***
 * @brief Returns true if queue with responses is empty
 */
bool JJsonParser::jsonRespsQueueEmpty(void) {
    return jsonParserRespsQueue.empty();
}

/**
 * @brief Put JSON response in queue
 */
void JJsonParser::pushJsonRespsQueue(string jsonResp) {
#if DATA_PROC_CALLED_FUNCTION
    ConsoleFunctionNameLog(__FUNCTION__);
#endif /* DATA_PROC_CALLED_FUNCTION */
    jsonParserRespsQueue.push(jsonResp);
}

/**
 * @brief Get first JSON response from queue and then remove it
 */
string JJsonParser::pullJsonRespsQueue(void) {
#if DATA_PROC_CALLED_FUNCTION
    ConsoleFunctionNameLog(__FUNCTION__);
#endif /* DATA_PROC_CALLED_FUNCTION */
    static string jsonResp = "";
    jsonResp = jsonParserRespsQueue.front();
    jsonParserRespsQueue.pop();
    return jsonResp;
}

/**
 * @brief Put JSON request in queue
 */
void JJsonParser::pushJsonReqsQueue(string jsonReq) {
#if DATA_PROC_CALLED_FUNCTION
    ConsoleFunctionNameLog(__FUNCTION__ );
#endif /* DATA_PROC_CALLED_FUNCTION */
    jsonParserReqsQueue.push(jsonReq);
}

/**
 * @brief Get first JSON request from queue and then remove it
 */
string JJsonParser::pullJsonReqsQueue(void) {
#if DATA_PROC_CALLED_FUNCTION
    ConsoleFunctionNameLog(__FUNCTION__);
#endif /* DATA_PROC_CALLED_FUNCTION */
    static string jsonReq = "";
    /* get first element */
    jsonReq = jsonParserReqsQueue.front();
    /* remove first element */
    jsonParserReqsQueue.pop();
    return jsonReq;
}

/**
 * @brief JSON request parser TODO: need to optimize this code
 */
err_type_jp JJsonParser::parseJsonRequest(string jsonKey, pt::ptree tree, std::string offset) {
#if DATA_PROC_CALLED_FUNCTION
    ConsoleFunctionNameLog(__FUNCTION__);
#endif /* DATA_PROC_CALLED_FUNCTION */
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
            cout << offset << "[" << v.first.size() << "] "; /*> size is the size of defined block in structure (length of word),
            possibly size means the same of size() */
            std::cout << v.first.data() << " : ";
            cout << "[" << v.second.data().size() << "] "; // number of subtrees
            std::cout << v.second.get<std::string>("");
            std::cout << std::endl;
        }
        else  if (!v.first.empty()) {
            cout << offset << "[" << v.first.size() << "] "; /*> size is the size of defined block in structure (length of word),
            possibly size means the same of size() */
            std::cout<< v.first.data() << " : ";
            if (tree.get_child(v.first.data()).back().first.size() != 0) {
                std::cout << "\n";
                errCode = parseJsonRequest((jsonKey + v.first.data() + string(".")), tree.get_child(v.first.data()), offset+"\t");
                if (errCode == err_type_jp::ERR_OK) {
                    break;
                }
                cout << " ]";
            }
            else {
                for (auto& e : tree.get_child(v.first.data())) {
                    cout << "\n";
                    for (auto& kv : e.second) {
                        cout << offset + "\t" << "[" << kv.first.size() << "] " << kv.first << " : ";
                        cout << "[" << e.second.get<std::string>(kv.first).size() << "] " << e.second.get<std::string>(kv.first) << "\n";
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


/***
 * @brief TODO
 */
bool JJsonParser::GetUnitTestResult(void) {
    return unitTestResult;
}
