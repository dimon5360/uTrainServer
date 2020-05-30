#pragma once

#include <string>
#include <Windows.h>
#include <iostream>

#include <boost/format.hpp> // boost library

extern HANDLE hConsole;

/* Console settings */
#define CONSOLE_INACTIVE() SetConsoleTextAttribute(hConsole, 8)
#define CONSOLE_RED() SetConsoleTextAttribute(hConsole, 12)
#define CONSOLE_WHITE() SetConsoleTextAttribute(hConsole, 15)
#define CONSOLE_GREEN() SetConsoleTextAttribute(hConsole, 10)
#define CONSOLE_RESET() SetConsoleTextAttribute(hConsole, 7)

/* Log string in console with green color */
void ConsoleComment(std::string s_log);
/* Print important information with white color */
void ConsoleInfo(std::string s_log);
/* Print error information with red color */
void ConsoleError(std::string s_log);
/* Log called function in console with green color */
void ConsoleFunctionNameLog(std::string s_log);