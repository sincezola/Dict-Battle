#include "utils.h"
#include "config.h"
#include "multiplayer.h"
#include "player.h"
#include "sockets.h"
#include "terminal_utils.h"
#include "trace.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
  #include <winsock2.h>
  #include <ws2tcpip.h>
#else
  #include <arpa/inet.h>
  #include <netinet/in.h>
  #include <sys/socket.h>
  #include <sys/un.h>
  #include <unistd.h>
#endif

/* Extern initialization */
int          gkiSocketServer;
unsigned int guiServerPort;

#ifdef _WIN32
  int iStartServer() { return 0; }
#else
int iAcceptClients() {
  char cAcceptedClients;
  int iClientFd;
  socklen_t iClientLenAddr;

  struct sockaddr_in stMyAddr;
  memset(&stMyAddr, 0x00, sizeof(stMyAddr));

  stMyAddr.sin_family = AF_INET;
  stMyAddr.sin_port = htons(guiServerPort);
  inet_pton(AF_INET, LOCALHOST, &stMyAddr.sin_addr);

  iClientLenAddr = sizeof(stMyAddr);

  for (cAcceptedClients = 0; cAcceptedClients < gucPlayersAmount; cAcceptedClients++) {
    if ((iClientFd = accept(gkiSocketServer, (struct sockaddr *)&stMyAddr, &iClientLenAddr)) == -1)
    {
      vTraceVarArgsFn("Error accepting socket");

      return 1;
    }

    iSpawnHandler(iClientFd);
  }

  return 0;
}

int iStartServer() {
  struct sockaddr_in stMyAddr;
  memset(&stMyAddr, 0x00, sizeof(stMyAddr));

  stMyAddr.sin_family = AF_INET;
  stMyAddr.sin_port = htons(guiServerPort);
  inet_pton(AF_INET, LOCALHOST, &stMyAddr.sin_addr);

  gkiSocketServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

  if (gkiSocketServer < 0) {
    vTraceVarArgsFn("Socket could not init.");

    return 1;
  }

  if (bind(gkiSocketServer, (struct sockaddr *)&stMyAddr, sizeof(stMyAddr)) <
      0) {
    vTraceVarArgsFn("Socket could not open");

    return 1;
  }

  listen(gkiSocketServer, 10);
  vTraceVarArgsFn("Server listening on port: %d\n", guiServerPort);
  printf("Server listening on port: %d\n", guiServerPort);

  iAcceptClients();

  return 0;
}
#endif

int iConnectToServer(unsigned char *pszServerAdress) {
  struct sockaddr_in stAddrIn;

  memset(&stAddrIn, 0x00, sizeof(stAddrIn));

  stAddrIn.sin_family = AF_INET;
  stAddrIn.sin_port = htons(guiServerPort);
  inet_pton(AF_INET, pszServerAdress, &stAddrIn.sin_addr);

  if ((gkiSocketServer = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
    vTraceVarArgsFn("Socket server could not be created");

    return 1;
  }

  if ((connect(gkiSocketServer, (struct sockaddr *)&stAddrIn,
                sizeof(stAddrIn))) < 0) {
    vTraceVarArgsFn("Socket client could not connect to server.");

    return 1;
  }

  vTraceVarArgs("Connected to server at: %s", pszServerAdress);

  return 0;
}

void vHandleSocket(const unsigned int kuiClientFd, const ulpid_t ulPid) {
  char szBuff[STANDART_BUF_SIZE] = { 0x00 };
  unsigned char ucAlreadyGreet;

  while ((read(kuiClientFd, szBuff, STANDART_BUF_SIZE)) > 0) {
    /* First message every client will
       send is their username
    ** */
    if (!ucAlreadyGreet)
    {
      ucAlreadyGreet = 1;

      if (!iIsValidUsername(szBuff, sizeof(szBuff)))
      {
        if (write(kuiClientFd, "INVALID USERNAME", 16) == -1)
        {
          vTraceVarArgsFn("Error writing, pid: %d", ulPid);
        }

        break;
      }

      iNewPlayer(kuiClientFd, ulPid, &szBuff[0]);
    }
    
    // printf("Client sent: %s\n", szBuff);

    memset(szBuff, 0x00, STANDART_BUF_SIZE);
    sleep((unsigned int)0.1);
  }

  vTraceVarArgsFn("Client with pid: %d disconnected.", ulPid);
}
