#include "config.h"
#include "terminal_utils.h"
#include <stdio.h>  
#include <stdlib.h>

void vPrintLine(char *pszLine, int iNewLine) {
  printf("%s", pszLine);
  if ( iNewLine )
    printf("\n");
}

void vPrintErrorLine(char *pszLine, int iNewLine) {
  if (pszLine == NULL)
    return;
  printf("\033[31m%s\033[0m", pszLine);
  if (iNewLine)
    printf("\n");
  fflush(stdout);
}

void vPrintHighlitedLine(char *pszLine, int iNewLine) {
  printf("\x1b[7m%s\x1b[0m", pszLine);
  if ( iNewLine )
    printf("\n");
}

void vClearTerminal() {
  #ifdef _WIN32
    system("cls");
  #else
    system("clear");
  #endif
  
  fflush(stdout);
}
