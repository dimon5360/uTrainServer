/**
 *  @file LLogger.h
 *  @note Logger class based on SService parent class
 *
 *  @date 30.05.2020
 *  @author Kalmykov Dmitry
 *
 *  @modified 30.05.2020
 *  @version 0.1
 */
#pragma once

#include "SService.h"

#include <iostream>


class LLogger : SService 
{
private:

public:
    LLogger() {
        std::cout << "Logger class constructing." << std::endl;
    }
    ~LLogger() {
        std::cout << "Logger class destructing." << std::endl;
    }
};

