#include "trace.h"
#include "player.h"
#include "utils.h"
#include "config.h"
#include "multiplayer.h"
#include "terminal_utils.h"
#include <string.h>

#ifdef _WIN32
  #include <windows.h>
#else
  #include <unistd.h>
#endif

unsigned char gszMyName[NAME_BUFFER_SIZE];

int main()
{
  char cOption;
  char *lineChar;

  vClearTerminal();
  vInitLogs();
  vInitPlayerList();

  vPrintLine("Insert a name (max 18 characters): ", NO_NEW_LINE);

  fgets(gszMyName, NAME_BUFFER_SIZE, stdin);
  
  lineChar = strchr(gszMyName, '\n');
  *lineChar = 0x00;

  #ifdef _WIN32
    vTraceVarArgs("GLOBAL NAME: %s (pid: %lu)", gszMyName, GetCurrentProcessId());
  #else
    vTraceVarArgs("GLOBAL NAME: %s (pid: %lu)", gszMyName, getpid());
  #endif

  printf("Welcome, %s!\n\n", gszMyName);

  do {
    vPrintLine("Choose an option:\n  [1] Host a game\n  [2] Join a game\n",
      INSERT_NEW_LINE);
      
    cOption = getchar();
    vClearTerminal();
  } while (cOption != '1' && cOption != '2');

  vClearTerminal();
  
  if (cOption == '1')
    iHostBattle();
  else
    iJoinBattle();

  return 0;
}