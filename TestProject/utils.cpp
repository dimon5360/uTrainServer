#include "utils.h"

HANDLE hConsole;

/***
 * @brief Log string in console with white color
 * 
 * @param s_log String of log which must be written in console
 */
void ConsoleComment(std::string s_log) {
    CONSOLE_WHITE();
    std::cout << std::endl << "/* " << s_log << " */" << std::endl;
    CONSOLE_RESET();
}

/***
 * @brief Print important information with white color
 *
 * @param s_log String of log which must be written in console
 */
void ConsoleInfo(std::string s_log) {
    CONSOLE_WHITE();
    std::cout << s_log << std::endl;
    CONSOLE_RESET();
}

/***
 * @brief Print error information with red color
 *
 * @param s_log String of log which must be written in console
 */
void ConsoleError(std::string s_log) {
    CONSOLE_RED();
    std::cout << s_log << std::endl;
    CONSOLE_RESET();
}

/***
 * @brief Log called function in console with white color
 *
 * @param s_log Name of called function
 */
void ConsoleFunctionNameLog(std::string s_log) {
    CONSOLE_WHITE();
    std::cout << std::endl << s_log << "()" << std::endl;
    CONSOLE_RESET();
}