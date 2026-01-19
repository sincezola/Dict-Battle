#include "player.h"
#include "trace.h"
#include <stdlib.h>
#include <stdio.h>

STRUCT_PLAYER_LIST gstPlayerList;
PSTRUCT_PLAYER     gpstCurrentPlayer;

void vInitPlayerList()
{
  gstPlayerList.pstFirst = NULL;
  gstPlayerList.pstLast  = NULL;
}

static PSTRUCT_PLAYER pstCreatePlayer(PSTRUCT_PLAYER pstPlayer)
{
  PSTRUCT_PLAYER pstNewPlayer = (PSTRUCT_PLAYER)malloc(sizeof(STRUCT_PLAYER));

  if (pstNewPlayer == NULL)
    return NULL;

  memset(pstNewPlayer, 0x00, sizeof(STRUCT_PLAYER));
  memcpy(pstNewPlayer, pstPlayer, sizeof(STRUCT_PLAYER));

  pstNewPlayer->pstNext = NULL;

  return pstNewPlayer;
}

void vAddPlayer2List(PSTRUCT_PLAYER pstPlayer)
{ 
  PSTRUCT_PLAYER *pstLast;

  if (pstPlayer == NULL)
    return;

  if (gstPlayerList.pstFirst == NULL)
  {
    gstPlayerList.pstFirst = pstPlayer;
    gstPlayerList.pstLast = pstPlayer;
    pstLast = &gstPlayerList.pstLast;
  } else {
    pstLast = &gstPlayerList.pstLast;
    (*pstLast)->pstNext = pstPlayer;
    (*pstLast) = pstPlayer;
  }

  (*pstLast)->pstNext = NULL;

  vTraceVarArgsFn("Player with pid: %d created inserted into list!", pstPlayer->ulPid);
}

int iNewPlayer(unsigned int uiSckFd, unsigned int ulPid, char *pszName) {
  STRUCT_PLAYER stPlayer;
  PSTRUCT_PLAYER pstPlayer;

  strcpy(stPlayer.szPlayerName, pszName);
  stPlayer.uiSckFd = uiSckFd;
  stPlayer.ulPid   = ulPid;

  pstPlayer = pstCreatePlayer(&stPlayer);

  if (pstPlayer == NULL)
    return -1;

  if (!gstPlayerList.pstFirst) pstPlayer->uiId = 1;
  else pstPlayer->uiId = gstPlayerList.pstLast->uiId + 1;

  vAddPlayer2List(pstPlayer);

  return 0;
}

void vLogPlayerList()
{
  PSTRUCT_PLAYER pWrk;

  for (pWrk = gstPlayerList.pstFirst; pWrk != NULL; pWrk = pWrk->pstNext)
  {
    vTraceVarArgsFn("%d", pWrk->uiId);
    vTraceVarArgsFn("%s", pWrk->szPlayerName);
  }
}

void vClearPlayerList()
{
  PSTRUCT_PLAYER pstCurrent;
  PSTRUCT_PLAYER pstNext;

  pstCurrent = gstPlayerList.pstFirst;

  while (pstCurrent != NULL)
  {
    pstNext = pstCurrent->pstNext;
    free(pstCurrent);
    pstCurrent = pstNext;
  }

  gstPlayerList.pstFirst = NULL;
  gstPlayerList.pstLast  = NULL;

  vTraceVarArgsFn("List cleared!!");
}

PSTRUCT_PLAYER pstFindPlayer(unsigned int uiId)
{
  PSTRUCT_PLAYER pstPlayer;

  for (pstPlayer = gstPlayerList.pstFirst; pstPlayer != NULL; pstPlayer = pstPlayer->pstNext)
    if (pstPlayer->uiId == uiId) return pstPlayer;

  return NULL;
}