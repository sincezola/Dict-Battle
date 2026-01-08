#ifndef TERMINAL_UTILS_H
#define TERMINAL_UTILS_H

#define INSERT_NEW_LINE 1

void vPrintLine(char *pszLine, int bNewLine);
void vPrintHighlitedLine(char *pszLine, int bNewLine);
void vClearTerminal();

#define vPrintArgs(FORMAT, ...) printf(FORMAT, __VA_ARGS__)
#endif