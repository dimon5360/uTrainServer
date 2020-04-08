/**
 * @file DDataProcess.h
 * @note Data processor class prototype
 *
 * @date 08.04.2020
 * @author Kalmykov Dmitry
 *
 * @modified 08.04.2020
 * @version 0.1
 */ 

#pragma once

#include <vector>
#include <string>
#include <cstdint>


class DDataProcess {

private:
    /* private variables */
    std::vector<std::string> queue;
    uint32_t queueReqsNumber = 0;

public:
    /* public prototypes */
    DDataProcess();
    ~DDataProcess();
};

