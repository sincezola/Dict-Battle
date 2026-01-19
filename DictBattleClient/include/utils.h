#ifndef UTILS_H
#define UTILS_H
  #include <stddef.h>

  #define CHECK_LEADING_ZEROS 1
  #define NO_LEAD_ZEROS       0

  #define NO_WARN_USER        0
  #define WARN_USER           1

  int iIsValidNumber(char*pszString, int iCheckLeadingZeros);
  int iIsValidIPV4(char *pszIpv4);
  int iIsValidUsername(char *pszBuff, size_t zuBuffLen);
  
  void vFlushStdin(int iWarnUser);
#endif