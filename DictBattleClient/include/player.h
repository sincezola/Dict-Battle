#ifndef PLAYER_H
#define PLAYER_H
  #include "config.h"

  typedef struct STRUCT_PLAYER {
    unsigned int         uiId;
    unsigned int         uiSckFd;
    ulpid_t              ulPid; 
    char                 szPlayerName[128];
    struct STRUCT_PLAYER *pstNext;
  } STRUCT_PLAYER, *PSTRUCT_PLAYER;

  typedef struct STRUCT_PLAYER_LIST {
    PSTRUCT_PLAYER pstFirst;
    PSTRUCT_PLAYER pstLast;
  } STRUCT_PLAYER_LIST, *PSTRUCT_PLAYER_LIST;

  extern STRUCT_PLAYER_LIST gstPlayerList;
  extern       char         *gszPlayerName;

  PSTRUCT_PLAYER pstFindPlayer(unsigned int uiId);

  int iNewPlayer(unsigned int uiSckFd, unsigned int ulPid, char *pszName);

  void vLogPlayerList();
  void vClearPlayerList();
  void vInitPlayerList();
#endif