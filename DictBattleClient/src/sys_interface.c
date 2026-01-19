#include "config.h"
#include "sockets.h"
#include "sys_interface.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
  
#ifdef _WIN32
  #include <stdint.h>
  #include <stdlib.h>
  #include <sys/stat.h>
  #include <windows.h>
#else
  #include <libgen.h>
  #include <sys/stat.h>
  #include <unistd.h>
#endif

#ifdef _WIN32
  int iDIR_MkDir(char *szDir) { 
    return CreateDirectory(szDir, NULL); 
  }
  int iDIR_SplitFilename(char *szFilename, char *szPath, char *szName, char *szExt) {
    char szDrive[_MAX_DRIVE];
    char szDir[_MAX_DIR];
    _splitpath(szFilename, szDrive, szDir, szName, szExt);
    strcpy(szPath, szDrive);
    strcat(szPath, szDir);
    return 0;
  }
  int iDIR_IsDir(char *szDir) {
    HANDLE hArquivo;
    WIN32_FIND_DATA wfdArquivo;
    hArquivo = FindFirstFile(szDir, &wfdArquivo);
    if (hArquivo == INVALID_HANDLE_VALUE)
      return 0;
    FindClose(hArquivo);
    if (wfdArquivo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
      return 1;
    return 0;
  }
#else /** LINUX */
/*
 * Is directory: 1
 * Exists, not directory: 0
 * Does not exist: -1
 * **/
int iDIR_IsDir(char *szDir) {
  struct stat stStat;
  if (stat(szDir, &stStat) != 0)
    return 0;
  if (S_ISDIR(stStat.st_mode))
    return 1;
  return 0;
}
int iDIR_SplitFilename(char *szFilename, char *szPath, char *szName,
                       char *szExt) {
  char szWrk[_MAX_PATH];
  char szBase[_MAX_PATH];
  char *pszBase;
  int ii;
  strcpy(szWrk, szFilename);
  pszBase = dirname(szWrk);
  if (pszBase == NULL) {
    szPath[0] = 0;
  } else {
    strcpy(szPath, pszBase);
  }
  strcpy(szWrk, szFilename);
  pszBase = basename(szWrk);
  strcpy(szBase, pszBase);
  for (ii = strlen(szBase) - 1; ii && szBase[ii] != '.'; ii--) {
    if (szBase[ii] == '\n' || szBase[ii] == '\r')
      szBase[ii] = 0;
  }
  if (ii > 0) { // found dot
    strcpy(szExt, &szBase[ii]);
    szBase[ii] = 0;
    strcpy(szName, szBase);
  } else {
    strcpy(szName, szBase);
    *szExt = 0;
  }

  return 0;
}
// Return 0 if error
int iDIR_MkDir(char *szDir) // linux
{
  int iRsl;
  // if (DEBUG_LEVEL)
  // vTracePid("iDIR_MkDir(%s)", strlen(szDir));
  iRsl = mkdir(szDir, S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IWGRP |
                          S_IXGRP | S_IROTH);
  return (iRsl == 0);
}
#endif

int iOpenFile(FILE **fppFile, const char *kpszFileName, const char *kpszMode) {
  if ((*fppFile = fopen(kpszFileName, kpszMode)) == NULL) {
    return FALSE;
  }
  return TRUE;
}

int iCloseFile(FILE **fppFile) {
  if (*fppFile != NULL) {
    fclose(*fppFile);
    *fppFile = NULL;

    return TRUE;
  }
  return FALSE;
}

int iFileExist(const char *kpszFileName) {
  FILE *fpFile = NULL;

  if (!iOpenFile(&fpFile, kpszFileName, "r")) {
    return FALSE;
  }

  iCloseFile(&fpFile);

  return TRUE;
}

int iRunCmd(char *pszCmd, char *pszRsl, int iRslSz) {
  FILE *pfPopen;
  char szLine[1024];
  int iCurrLen = 0;

  if ((pfPopen = popen(pszCmd, "r")) == NULL)
    return FALSE;

  while (fgets(szLine, sizeof(szLine), pfPopen)) {
    if (((int)strlen(szLine) + iCurrLen) > iRslSz) {
      pclose(pfPopen);
      return FALSE;
    }
    sprintf(&pszRsl[iCurrLen], "%s", szLine);
    iCurrLen += strlen(szLine);
  }

  pclose(pfPopen);
  if (iStrIsEmpty(pszRsl))
    return FALSE;

  return TRUE;
}

int iStrIsEmpty(const char *kpszStr) {
  if (kpszStr == NULL || !strcmp(kpszStr, "") || !strcmp(kpszStr, "\n")) {
    return TRUE;
  }

  return FALSE;
}
