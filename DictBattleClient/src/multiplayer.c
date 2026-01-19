#include "trace.h"
#include "config.h"
#include "multiplayer.h"
#include "sockets.h"
#include "terminal_utils.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef LINUX
  #include <unistd.h>
#endif

unsigned char gucPlayersAmount;
unsigned int  guiServerPort;

unsigned char gszMyName[NAME_BUFFER_SIZE];

int iHostBattle() {
  char szBuff[STANDART_BUF_SIZE];

  memset(szBuff, 0x00, sizeof(szBuff));
  
  vFlushStdin(WARN_USER);
  vClearTerminal();

  vPrintLine("Insert how many players will connect (max 6): ", NO_NEW_LINE);
  if (!fgets(szBuff, sizeof(szBuff), stdin))
    return 1;

  gucPlayersAmount = szBuff[0];

  if (!iIsValidNumber(szBuff, CHECK_LEADING_ZEROS) || szBuff[0] > '6') {
    vPrintLine("Please, insert a valid number, from 1 to 6", INSERT_NEW_LINE);

    return 1;
  }

  vClearTerminal();

  vFlushStdin(WARN_USER);

  vPrintLine("Choose a port to listen (default 8602): ", NO_NEW_LINE);

  if (!fgets(szBuff, sizeof(szBuff), stdin))
    return 1;

  vClearTerminal();

  if (szBuff[0] != '\n' && !iIsValidNumber(szBuff, CHECK_LEADING_ZEROS)) {
    vPrintLine("Sorry, you need to specify a valid port...", INSERT_NEW_LINE);

    return 1;
  }

  if (szBuff[0] != '\n')
    guiServerPort = atoi(szBuff);

  vTraceVarArgs("GLOBAL SERVER PORT: %d", guiServerPort);

  if (iStartServer()) {
    vPrintLine("Error while starting your server... Sorry!", INSERT_NEW_LINE);
    return 1;
  }

  return 0;
}

int iJoinBattle() {
  char szBuff[STANDART_BUF_SIZE]               = { 0x00 };
  unsigned char szServerAddress[IPV4_MAX_SIZE] = { 0x00 };

  vFlushStdin(WARN_USER);
  
  vClearTerminal();
  vPrintLine("Please, insert the server ip (only ipv4): ", NO_NEW_LINE);

  memset(szBuff, 0x00, sizeof(szBuff));
  if (!fgets(szBuff, sizeof(szBuff), stdin))
    return 1;

  vClearTerminal();

  if (!iIsValidIPV4(szBuff)) {
    vPrintLine("Sorry, you need provide a valid ip...", INSERT_NEW_LINE);

    return 1;
  }

  strncpy(szServerAddress, szBuff, IPV4_MAX_SIZE);
  szServerAddress[IPV4_MAX_SIZE - 1] = 0x00;

  vFlushStdin(WARN_USER);
  vClearTerminal();

  vPrintLine("Please, insert the server port (default 8602): ", NO_NEW_LINE);

  memset(szBuff, 0x00, sizeof(szBuff));
  if (!fgets(szBuff, sizeof(szBuff), stdin))
    return 1;

  vClearTerminal();

  if (szBuff[0] != '\n' && !iIsValidNumber(szBuff, CHECK_LEADING_ZEROS)) {
    vPrintLine("Sorry, you need to specify a valid port...", INSERT_NEW_LINE);

    return 1;
  }

  if (szBuff[0] != '\n')
    guiServerPort = atoi(szBuff);

  vTraceVarArgs("GLOBAL SERVER PORT: %d", guiServerPort);

  if (iConnectToServer(szServerAddress)) {
    vPrintLine("Error while connecting to the server... Sorry!",
               INSERT_NEW_LINE);
    return 1;
  }

  vComunicateWithServer(szBuff);

  return 0;
}

#ifdef _WIN32
void vComunicateWithServer() { return; }
void iSpawnHandler(const unsigned int kuiSocketFd) { return; }
#else
void vComunicateWithServer()
{
  if (write(gkiSocketServer, gszMyName, strlen(gszMyName)) < 0)
}

void iSpawnHandler(const unsigned int kuiSocketFd) {
  ulpid_t llChildPid;

  llChildPid = fork();
  if (llChildPid != 0)
    /* Parent Process */
    return;
  else
    vHandleSocket(kuiSocketFd, llChildPid);
}
#endif