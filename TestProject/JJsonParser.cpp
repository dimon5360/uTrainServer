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
    cout << " ================================== \n";
    cout << " Json Parser class object created \n";
    cout << " ================================== \n\n";
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
    cout << " ================================== \n";
    cout << " Json Parser class object removed. \n";
    cout << " ================================== \n\n";
#endif /* JSON_PROC_CONSTR_DESTR_LOG */
    jsonParserHandlerThread.~thread();
}

/**
 * @brief JSON parser main handler
 */
void JJsonParser::handle(void) {

    std::cout << "JSON parser handler thread started.\n";

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

    static err_type_jp errCode = err_type_jp::ERR_OK;
    /* string stream for JSON req in tree format */
    static std::stringstream req;
    /* put input json doc in string stream */
    req << jsonDoc;

    try
    {
        pt::ptree tree;
        pt::read_json(req, tree);

        errCode = parseJsonRequest("", tree);
        if (errCode != err_type_jp::ERR_OK) {
#if JSON_PROC_PARSER_LOG
            std::cout << "JSON request parsing failed.\n";
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
    jsonParserRespsQueue.push(jsonResp);
}

/**
 * @brief Get first JSON response from queue and then remove it
 */
string JJsonParser::pullJsonRespsQueue(void) {
    static string jsonResp = "";
    jsonResp = jsonParserRespsQueue.front();
    jsonParserRespsQueue.pop();
    return jsonResp;
}

/**
 * @brief Put JSON request in queue
 */
void JJsonParser::pushJsonReqsQueue(string jsonReq) {
    jsonParserReqsQueue.push(jsonReq);
}

/**
 * @brief Get first JSON request from queue and then remove it
 */
string JJsonParser::pullJsonReqsQueue(void) {
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


/***
 * @brief TODO
 */
bool JJsonParser::GetUnitTestResult(void) {
    return unitTestResult;
}
