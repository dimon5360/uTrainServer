/**
 * @file config.h
 * @note Preprocessor settings
 *
 * @date 08.04.2020
 * @author Kalmykov Dmitry
 *
 * @modified 08.04.2020
 * @version 0.1
 */

#pragma once

#define NEW_REALIZATION                 0

/* unit tests items--------------------------------------------------------- */
#define UNIT_TESTS_ENABLE               1
#if UNIT_TESTS_ENABLE
/* unit tests for class DDataProcessor */
#define UNIT_TEST_DATA_PROCESSOR        0
#define UNIT_TEST_SERVER_DATA_PROC      0
#define UNIT_TEST_LOGEER_CONSTRUCTOR    1


#endif /* UNIT_TESTS_ENABLE */


/* logger items ------------------------------------------------------------ */
#define TCP_SERVER_CALLED_FUNCTION      1

/* log JSON processor (push, pull requests) */
#define JSON_PROC_PUSH_PULL_LOG         0
#define JSON_PROC_CONSTR_DESTR_LOG      0
#define JSON_PROC_PARSER_LOG            0
#define JSON_PARSER_HANDLER_LOG         0

/* log unit tests handler */
#define UNIT_TESTS_LOG                  0

/* log data processor */
#define DATA_PROC_CONSTR_DESTR_LOG      0
#define DATA_PROC_CALLED_FUNCTION       0
#define DATA_PROC_HANDLER_LOG           1

/* log data base */
#define DATA_BASE_CONSTR_DESTR_LOG      0
#define DATA_BASE_PUSH_PULL_LOG         0
#define DATA_BASE_HANDLER_LOG           0

/* log http handler */
#define HTTP_PROC_CONSTR_DESTR_LOG      0
#define HTTP_PROC_PARSER_LOG            1
#define HTTP_HANDLER_LOG                1
#define HTTP_HANDLER_CALLED_FUNCTION    0

/* logger class */
#define LOGGER_ENABLE                   1