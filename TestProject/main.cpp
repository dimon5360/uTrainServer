#include "main.h"

/* user classes */
#include "SServer.h"
#include "DDataBase.h"
#include "DDataProcess.h"

/* internal libs */
#include <iostream>
#include <memory>
/* external libs */
#include <mysql.h>          // for work with db MYSQL
#include <boost/format.hpp> // boost library

// v.0.0.2 Build from 20.02.2020
static const uint32_t appVersion = 0x00000002;
// database password "adM1n34#184"

using namespace std;

/* private prototypes */
static void PrintInfoApp(void);
/* Handle for server thread */
static void serverInit(void);
/* Handle for database thread */
static void databaseInit(void);
/* Handle for data process thread */
static void dataProcessInit(void);

std::shared_ptr<SServer> server;
std::shared_ptr<DDataBase> db;
std::shared_ptr<DDataProcess> dataProcessor;

/* entry point */
int main() {

    PrintInfoApp();
    ///* thread for server connect */
    //std::thread t1{ serverInit };
    ///* thread for database connect */
    //std::thread t2{ databaseInit };
    ///* thread for data process */
    //std::thread t3{ dataProcessInit };

    /*t1.join();
    t2.join();
    t3.join();*/

    databaseInit();

    return 0;
}

/** 
 * @brief Handle for server thread
 */
static void serverInit(void) {
    /* create multythread TCP server */
   server = make_shared<SServer>("192.168.1.64", 4059);
}

/**
 * @brief Handle for database thread 
 */
static void databaseInit(void) {
    err_type errCode = err_type::ERR_OK;
    /* create database connection */
    db = make_shared<DDataBase>("testdb", 3306);
    /* connect to user databse */
    errCode = db->DDataBaseConnect("localhost", "adM1n34#184");
    if (errCode != err_type::ERR_OK) {
        std::cout << "Database has failed.\n";
    }
    else {
        std::cout << "Database has connected.\n";
    }
}

/**
 * @brief Handle for data process thread
 */
static void dataProcessInit(void) {
    /* create thread for data processor */
    dataProcessor = make_shared<DDataProcess>();
}

/** 
 * @brief print application information 
 */
static void PrintInfoApp(void) {
    cout << "Server started. Hello %username% " << endl;
    cout << "Application version v." <<  boost::format("%u.%u.%u") % 
        ((appVersion>>16) & 0xFF) %
        ((appVersion>>8) & 0xFF) %
        ((appVersion) & 0xFF) << endl;

}