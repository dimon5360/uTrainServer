/**
 * @file DDataProcess.cpp
 * @note Data processor class realization
 *
 * @date 08.04.2020
 * @author Kalmykov Dmitry
 *
 * @modified 08.04.2020
 * @version 0.1
 */

#include "config.h"
#include "main.h"

/**
 * @brief Data processor constructor
 */
DDataProcess::DDataProcess() {
    err_type_db errCode = err_type_db::ERR_OK;
    std::shared_ptr<DDataBase> dataBaseProcessor = make_shared<DDataBase>("testdb", 3306);
    /* connect to user databse */
    errCode = dataBaseProcessor->DDataBaseConnect("localhost", "adM1n34#184");
    if (errCode != err_type_db::ERR_OK) {
        std::cout << "Database has failed.\n";
    }
    else {
        std::cout << "Database has connected.\n";
        unitTestResult = true;
    }
}

/**
 * @brief Data processor constructor
 */
DDataProcess::DDataProcess(std::string sDatabaesRequest) {
    err_type_db errCode = err_type_db::ERR_OK;
    std::shared_ptr<DDataBase> dataBaseProcessor = make_shared<DDataBase>("testdb", 3306);
    /* connect to user databse */
    errCode = dataBaseProcessor->DDataBaseConnect("localhost", "adM1n34#184");
    if (errCode != err_type_db::ERR_OK) {
        std::cout << "Database has failed.\n";
        return;
    }
    std::cout << "Database has connected.\n";
    dataBaseProcessor->DDataBaseProcRequest(sDatabaesRequest, 0);
    unitTestResult = true;
}

/**
 * @brief Data processor destructor
 */
DDataProcess::~DDataProcess() {

}


#if UNIT_TEST_DATA_PROCESSOR
bool DDataProcess::GetUnitTestResult(void) {
    return unitTestResult;
}
#endif /* UNIT_TEST_DATA_PROCESSOR */