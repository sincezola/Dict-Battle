#ifndef MULTIPLAYER_H
#define MULTIPLAYER_H
  #include "sockets.h"

  int iHostBattle();
  int iJoinBattle();
  
  void iSpawnHandler(const unsigned int kuiSocketFd);

  extern unsigned char gucPlayersAmount;
  extern unsigned int  guiServerPort = DEFAULT_PORT;
  
  extern unsigned char gszMyName[20];
#endif