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
    ///* single thread for data base handler */
    //jsonParserHandlerThread = std::thread(&JJsonParser::handle, this);
    //jsonParserHandlerThread.join();
    /* single thread for data base handler */
    std::thread(&JJsonParser::handle, this).detach();
}

/**
 * @brief JSON parser class destructor
 */
JJsonParser::~JJsonParser(void) {
}

/**
 * @brief JSON parser main handler
 */
void JJsonParser::handle(void) {

    std::string jsonReq;
    while (true) {

        /* check if queue is empty */
        if (!jsonReqsQueueEmpty()) {
            try {
                jsonReq = pullJsonReqsQueue();
                procJsonRequest(std::move(jsonReq));
                break;
            }
            catch (std::exception& ex) {
                std::cout << ex.what() << std::endl;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(THREAD_TIMEOUT));
    }
}

/***
 * @brief Process input JSON doc
 */
void JJsonParser::procJsonRequest(std::string &&jsonDoc) {

    static err_type_jp errCode = err_type_jp::ERR_OK;

    try
    {
        /* string stream for JSON req in tree format */
        static std::stringstream req;
        /* put input json doc in string stream */
        req << jsonDoc;

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
void JJsonParser::pushJsonRespsQueue(std::string&& jsonResp) {
    try {
        const std::lock_guard<std::mutex> lock(mutex_);
        jsonParserRespsQueue.push(jsonResp);
    }
    catch (std::exception& ex) {
        std::cout << ex.what() << std::endl;
    }
}

/**
 * @brief Get first JSON response from queue and then remove it
 */
const std::string& JJsonParser::pullJsonRespsQueue(void) {
    std::string jsonResp;
    try {
        const std::lock_guard<std::mutex> lock(mutex_);
        jsonResp = jsonParserRespsQueue.front();
        jsonParserRespsQueue.pop();
    }
    catch (std::exception& ex) {
        std::cout << ex.what() << std::endl;
    }
    return jsonResp;
}

/**
 * @brief Put JSON request in queue
 */
void JJsonParser::pushJsonReqsQueue(std::string&& jsonReq) {
    try {
        const std::lock_guard<std::mutex> lock(mutex_);
        jsonParserReqsQueue.push(jsonReq);
    }
    catch (std::exception& ex) {
        std::cout << ex.what() << std::endl;
    }
}

/**
 * @brief Get first JSON request from queue and then remove it
 */
const std::string& JJsonParser::pullJsonReqsQueue(void) {
    std::string jsonReq;
    try {
        const std::lock_guard<std::mutex> lock(mutex_);
        /* get first element */
        jsonReq = jsonParserReqsQueue.front();
        jsonParserReqsQueue.pop();
    }
    catch (std::exception& ex) {
        std::cout << ex.what() << std::endl;
    }
    return jsonReq;
}

/**
 * @brief JSON request parser TODO: need to optimize this code
 */
err_type_jp JJsonParser::parseJsonRequest(std::string jsonKey, pt::ptree tree, std::string offset) {
    err_type_jp errCode = err_type_jp::ERR_OK;

    /* loop checks all values */
    for(auto &v : tree)
    {
        /* If v.second.size() == 0 it means that current branch does not have any branches.
           If size is more than 0 then cuurent branch of tree has subbranch */
        if (!v.second.data().empty()) {
            std::cout << offset << "[" << v.first.size() << "] "; /*> size is the size of defined block in structure (length of word),
            possibly size means the same of size() */
            std::cout << v.first.data() << " : ";
            std::cout << "[" << v.second.data().size() << "] "; // number of subtrees
            std::cout << v.second.get<std::string>("");
            std::cout << std::endl;
        }
        else  if (!v.first.empty()) {
            std::cout << offset << "[" << v.first.size() << "] "; /*> size is the size of defined block in structure (length of word),
            possibly size means the same of size() */
            std::cout<< v.first.data() << " : ";
            if (tree.get_child(v.first.data()).back().first.size() != 0) {
                std::cout << "\n";
                errCode = parseJsonRequest((jsonKey + v.first.data() + std::string(".")), tree.get_child(v.first.data()), offset+"\t");
                if (errCode == err_type_jp::ERR_OK) {
                    break;
                }
                std::cout << " ]";
            }
            else {
                for (auto& e : tree.get_child(v.first.data())) {
                    std::cout << "\n";
                    for (auto& kv : e.second) {
                        std::cout << offset + "\t" << "[" << kv.first.size() << "] " << kv.first << " : ";
                        std::cout << "[" << e.second.get<std::string>(kv.first).size() << "] " << e.second.get<std::string>(kv.first) << "\n";
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
