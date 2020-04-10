#include "main.h"
#include "config.h"

/* internal libs */
#include <iostream>
#include <memory>
/* external libs */
#include <mysql.h>          // for work with db MYSQL
#include <boost/format.hpp> // boost library

using namespace std;

// v.0.0.6 Build from 10.04.2020
static const uint32_t appVersion = 0x00000006;

/* private prototypes */
static void PrintInfoApp(void);
/* Handle for server thread */
static void serverInit(void);
/* Handle for database thread */
static void databaseInit(void);
/* Handle for data process thread */
static void dataProcessInit(void);
/* Handle for JSON process thread */
static void jsonProcessInit(void);

std::shared_ptr<SServer> server;
std::shared_ptr<DDataBase> db;
std::shared_ptr<DDataProcess> dataProcessor;
std::shared_ptr<JJsonParser> jsonProcessor;
#if UNIT_TESTS_ENABLE
std::shared_ptr<UUnitTests> unitTests;
#endif /* UNIT_TESTS_ENABLE */

/* entry point */
int main() {

    PrintInfoApp();

#if UNIT_TESTS_ENABLE
    unitTests = make_shared<UUnitTests>();
    if (!unitTests->GetUnitTestsResult()) {
        return -1;
    }
#else 
    /* thread for server connect */
    std::thread t1{ serverInit };
    /* thread for database connect */
    std::thread t2{ databaseInit };
    /* thread for data process */
    std::thread t3{ dataProcessInit };
    /* thread for data process */
    std::thread t4{ jsonProcessInit };

    t1.join();
    t2.join();
    t3.join();
    t4.join();
#endif /* UNIT_TESTS_ENABLE */

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
    err_type_db errCode = err_type_db::ERR_OK;
    /* create database connection */
    db = make_shared<DDataBase>("testdb", 3306);
    /* connect to user databse */
    errCode = db->DDataBaseConnect("localhost", "adM1n34#184");
    if (errCode != err_type_db::ERR_OK) {
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
 * @brief Handle for JSON process thread
 */
static void jsonProcessInit(void) {
    /* create thread for data processor */
    jsonProcessor = make_shared<JJsonParser>();
}

/** 
 * @brief print application information 
 */
static void PrintInfoApp(void) {
    cout << "Application started. Hello %username% " << endl;
    cout << "Application version v." <<  boost::format("%u.%u.%u") % 
        ((appVersion>>16) & 0xFF) %
        ((appVersion>>8) & 0xFF) %
        ((appVersion) & 0xFF) << endl;

    cout << "Compilation time: " << boost::format("%s %s") %
        __DATE__ % __TIME__ << endl;

}