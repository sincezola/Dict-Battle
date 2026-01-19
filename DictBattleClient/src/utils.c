#include "trace.h"
#include "utils.h"
#include "terminal_utils.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void vFlushStdin(int iWarnUser)
{
  if (iWarnUser)
    vPrintLine("Please, press enter...", INSERT_NEW_LINE);

  int ch;
  while ((ch = getchar()) != '\n' && ch != EOF);
}

int iIsValidNumber(char *s, int iCheckLeadingZeros)
{
  if (iCheckLeadingZeros && (*s < '1' || *s > '9'))
    return 0;

  for (s++; *s && *s != '\n'; s++)
    if (*s < '0' || *s > '9')
      return 0;

  return 1;
}

int iIsValidIPV4(char *pszIpv4)
{
  char *pTok;
  int iCount;
  unsigned short usDotsCount = 0;  

  for (pTok = pszIpv4; *pTok && usDotsCount <= 3; pTok++)
    if (*pTok == '.') usDotsCount++;
  if (usDotsCount != 3)
    return 0;

  pTok = strtok(pszIpv4, ".");

  if (!iIsValidNumber(pTok, NO_LEAD_ZEROS) || atoi(pTok) > 255)
    return 0;

  for (iCount = 0; iCount < 3; pTok++, iCount++)
  { 
    pTok = strtok(NULL, ".");

    if (!iIsValidNumber(pTok, NO_LEAD_ZEROS) || atoi(pTok) > 255)
      return 0;
  }

  return 1;
}

int iIsValidUsername(char *pszBuff, size_t zuBuffLen)
{
  // TODO: Implement this

  return 1;
}