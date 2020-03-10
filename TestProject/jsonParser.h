#pragma once

#include <cstring>
#include <cstdlib>
#include <vector>
#include <string>
#include <cstdint>
#include <queue>
#include <map>
#include <thread>

using namespace std;

class jsonParser
{
private:
    vector<string> inputData;
    queue<string> jsonQueue;

    /* Handle ------------------------------------- */
    /* main json thread handler */
    void jsonHandler(void);

public:
    /* constructor */
    jsonParser();
    /* destructor */
    ~jsonParser();
    
    /* interfaces for json queue ----------------- */
    /* Get JSON data from queue */
    string getJsonReqQueue(void);
    /* Put JSON data to queue */
    void putJsonReqQueue(string);


};

/* test JSON parser */
extern uint32_t jsonParse(string jsonDoc);
