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

