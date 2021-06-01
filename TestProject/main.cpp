/* main classes headers */
#include "main.h"

/* internal libs */
#include <iostream>
#include <memory>
/* external libs */
//#include <mysql.h>          // for work with db MYSQL
#include <boost/format.hpp> // boost library

// v.0.0.17 Build from 01.06.2021
#define MAJOR  0
#define MINOR  0
#define BUILD  17

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

    /* print application information */
    PrintInfoApp();

#if UNIT_TESTS_ENABLE
    std::thread{
        [&](){ 
            UUnitTests::UnitTestsInit();
            if (!unitTests->GetUnitTestsResult()) {
                return -1;
            }}
    }.join();
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
   server = std::make_shared<SServer>("192.168.1.64", 4059);
}

/**
 * @brief Handle for database thread 
 */
static void databaseInit(void) {
    err_type_db errCode = err_type_db::ERR_OK;
    /* create database connection */
    db = std::make_shared<DDataBase>("testdb", 3306);
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
    dataProcessor = std::make_shared<DDataProcess>();
}

/**
 * @brief Handle for JSON process thread
 */
static void jsonProcessInit(void) {
    /* create thread for data processor */
    jsonProcessor = std::make_shared<JJsonParser>();
}

/** 
 * @brief print application information 
 */
static void PrintInfoApp(void) {
    std::cout << "Hello %username%" << std::endl;
    std::cout << "Application version v." <<
            boost::format("%u.%u.%u\n") % MAJOR % MINOR % BUILD <<
            "Compilation time: " <<
            boost::format("%s %s") %
            __DATE__ % __TIME__  << std::endl;

    std::cout << "Application started." << std::endl;
    std::cout << "===============================================\n\n";
}