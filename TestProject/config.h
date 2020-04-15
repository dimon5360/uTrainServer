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
#define UNIT_TEST_JSON_PARSER           1
#if UNIT_TEST_JSON_PARSER
#define TEST4                           1
#endif /* UNIT_TEST_JSON_PARSER */


#endif /* UNIT_TESTS_ENABLE */


/* logger items ------------------------------------------------------------ */
#define LOG_FUNCTIONS_CALLS             0
/* log JSON processor (push, pull requests) */
#define JSON_PROC_PUSH_PULL_LOG         0
#define JSON_PROC_CONSTR_DESTR_LOG      0
#define JSON_PROC_PARSER_LOG            1

/* log unit tests handler */
#define UNIT_TESTS_LOG                  0