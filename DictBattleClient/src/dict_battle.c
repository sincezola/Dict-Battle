#include <trace.h>
#include <terminal_utils.h>

int main()
{
  char cOption;
  vInitLogs();
  
  do {
    vPrintLine("Choose an option:\n  [1] Host a game\n  [2] Join a game",
      INSERT_NEW_LINE);
      
    cOption = getchar();
    vClearTerminal();
  } while (cOption != '1' && cOption != '2');

  if (cOption == '1')
    iHostGame();
  else
    iJoinGame();

  return 0;
}