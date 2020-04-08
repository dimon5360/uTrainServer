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
#define UNIT_TESTS_ENABLE               0
#if UNIT_TESTS_ENABLE
#define UNIT_TEST_QUEUE_EXCHANGE        1


#endif /* UNIT_TESTS_ENABLE */


/* logger items ------------------------------------------------------------ */
#define LOG_FUNCTIONS_CALLS             0
/* log JSON processor (push, pull requests) */
#define JSON_PROC_PUSH_PULL_LOG         1