#include "jsonParser.h"

/**
 * @brief JSON parser class constructor
 */
jsonParser::jsonParser(void) {
    /* start JSON handler  */
    jsonHandler();
}

/**
 * @brief JSON parser class destructor
 */
jsonParser::~jsonParser(void) {

}

/**
 * @brief Put JSON data to queue
 */
void jsonParser::putJsonReqQueue(string jsonReq) {
    jsonQueue.push(jsonReq);
}

/**
 * @brief Get JSON data from queue
 */
string jsonParser::getJsonReqQueue(void) {

    string jsonString;
    jsonString = jsonQueue.front();
    return jsonString;
}

uint32_t jsonParse(string jsonDoc) {

}

/**
 * @brief JSON parser main handler
 */
void jsonParser::jsonHandler(void) {

    string jsonReq;
    while (true) {

        /* check if queue is empty */
        if (!jsonQueue.empty()) {
            jsonReq = getJsonReqQueue();
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(3));
    }
}