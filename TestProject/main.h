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

/* user classes */
#include "SServer.h"
#include "DDataBase.h"
#include "DDataProcess.h"

#include "JJsonParser.h"
#include "HHttpHandler.h"
#include "UUnitTests.h"

#define THREAD_TIMEOUT 100 // ms