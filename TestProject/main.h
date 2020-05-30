/**
 * @file main.h
 * @note Keep includes
 *
 * @date 08.04.2020
 * @author Kalmykov Dmitry
 */

#pragma once

 /* presprocessor configuration */
#include "config.h"

#ifndef BOOST_ASIO_HPP
#include <boost/asio.hpp>
#endif /* BOOST_ASIO_HPP */


/* user classes */
#include "SService.h"

#include "LLogger.h"

#include "DDataBase.h"
#include "DDataProcess.h"

#include "JJsonParser.h"
#include "HHttpHandler.h"
#include "UUnitTests.h"

/* for formatted output */
#include "utils.h"

extern void StartTcpServer(std::string s_ip, uint16_t s_port);

#define THREAD_TIMEOUT 100 // ms