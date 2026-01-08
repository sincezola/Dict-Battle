#include <stdio.h>  
#include <terminal_utils.h>
#include <config.h>
#include <stdlib.h>

void vPrintLine(char *pszLine, int bNewLine) {
  printf("%s", pszLine);
  if ( bNewLine )
    printf("\n");
}

void vPrintErrorLine(char *pszLine, int bNewLine) {
  if (pszLine == NULL)
    return;
  printf("\033[31m%s\033[0m", pszLine);
  if (bNewLine)
    printf("\n");
  fflush(stdout);
}

void vPrintHighlitedLine(char *pszLine, int bNewLine) {
  printf("\x1b[7m%s\x1b[0m", pszLine);
  if ( bNewLine )
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
