/* main classes headers */
#include "main.h"

/* internal libs */
#include <iostream>
#include <memory>
/* external libs */
#include <mysql.h>          // for work with db MYSQL
#include <boost/format.hpp> // boost library

using namespace std;

// v.0.0.13 Build from 29.04.2020
#define MAJOR  0
#define MINOR  0
#define BUILD  13

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

/**
 * @brief Main method in application
 */
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
    cout << "Hello %username% " << endl;
    cout << "Application version v." <<  boost::format("%u.%u.%u") % 
        MAJOR % MINOR % BUILD << endl;

    cout << "Compilation time: " << boost::format("%s %s\n") %
        __DATE__ % __TIME__ << endl;
    cout << "Application started.";
    cout << "==================================\n" << endl;

}