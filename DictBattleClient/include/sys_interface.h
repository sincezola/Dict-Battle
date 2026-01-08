#ifndef SYSINTERFACE_H
#define SYSINTERFACE_H

#include <limits.h>
#include <stdio.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>

#ifndef _MAX_DRIVE
#define _MAX_DRIVE 3
#endif

#ifndef _MAX_DIR
#define _MAX_DIR 256
#endif

#ifndef _MAX_FNAME
#define _MAX_FNAME 256
#endif

#ifndef _MAX_EXT
#define _MAX_EXT 256
#endif

#ifndef _MAX_PATH
#define _MAX_PATH MAX_PATH
#endif

#else // ======= LINUX / UNIX ========

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

#define _MAX_PATH PATH_MAX

#ifndef HAVE__SPLITPATH
static inline void _splitpath(const char *pszPath, char *pszDrive, char *pszDir,
                              char *pszFname, char *pszExt) {
  const char *pszSlash = strrchr(pszPath, '/');
  const char *pszDot = strrchr(pszPath, '.');

  if (pszDrive)
    pszDrive[0] = '\0';

  if (pszSlash) {
    if (pszDir) {
      size_t iLen = pszSlash - pszPath + 1;
      strncpy(pszDir, pszPath, iLen);
      pszDir[iLen] = '\0';
    }
    pszPath = pszSlash + 1;
  } else if (pszDir) {
    pszDir[0] = '\0';
  }

  if (pszDot && pszDot > pszPath) {
    if (pszExt)
      strcpy(pszExt, pszDot);
    if (pszFname) {
      size_t iLen = pszDot - pszPath;
      strncpy(pszFname, pszPath, iLen);
      pszFname[iLen] = '\0';
    }
  } else {
    if (pszExt)
      pszExt[0] = '\0';
    if (pszFname)
      strcpy(pszFname, pszPath);
  }
}
#define HAVE__SPLITPATH
#endif
#endif

// ====== prototypes ======
int iStrIsEmpty(const char *kpszStr);
int iDIR_MkDir(char *szDir);
int iDIR_SplitFilename(char *szFilename, char *szPath, char *szName,
                       char *szExt);
int iDIR_IsDir(char *szDir);
int iOpenFile(FILE **fppFile, const char *kpszFileName, const char *kpszMode);
int iCloseFile(FILE **fppFile);
int iFileExist(const char *kpszFileName);

#endif
