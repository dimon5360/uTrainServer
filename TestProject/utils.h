#pragma once

#include <string>
#include <iostream>

/* Log string in console with green color */
void ConsoleComment(std::string s_log);
/* Print important information with white color */
void ConsoleInfo(std::string s_log);
/* Print error information with red color */
void ConsoleError(std::string s_log);
/* Log called function in console with green color */
void ConsoleFunctionNameLog(std::string s_log);