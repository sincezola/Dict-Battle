#ifndef SOCKETS_H
#define SOCKETS_H
  #define LOCALHOST     "127.0.0.1"
  #define DEFAULT_PORT  8602
  #define IPV4_MAX_SIZE 15

  int iStartServer();
  int iConnectToServer(unsigned char *pszServerAdress);

  void vHandleSocket(const unsigned int kuiClientFd, const ulpid_t ulPid);

  extern int gkiSocketServer;
#endif