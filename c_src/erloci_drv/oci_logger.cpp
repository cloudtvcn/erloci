#ifdef __WIN32__
#include <Winsock2.h>
#else
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#endif

#ifdef __WIN32__
SOCKET s; //Socket handle
#else
int s;
#endif

//CONNECTTOHOST � Connects to a remote host
char * connect_tcp(int PortNo)
{
#ifdef __WIN32__
    //Start up Winsock�
    WSADATA wsadata;

    int error = WSAStartup(0x0202, &wsadata);

    //Did something happen?
    if (error)
        return "unknown error";

    //Did we get the right Winsock version?
    if (wsadata.wVersion != 0x0202)
    {
        WSACleanup(); //Clean up Winsock
        return "bad Winsock version";
    }

    //Fill out the information needed to initialize a socket�
    SOCKADDR_IN target; //Socket address information

    target.sin_family = AF_INET; // address family Internet
    target.sin_port = htons (PortNo); //Port to connect on
    target.sin_addr.s_addr = inet_addr ("127.0.0.1"); //Target IP

    s = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP); //Create socket
    if (s == INVALID_SOCKET)
    {
        return "Winsock sock create failed"; //Couldn't create the socket
    }  

    //Try connecting...

    if (connect(s, (SOCKADDR *)&target, sizeof(target)) == SOCKET_ERROR)
    {
        return "Winsock sock connect failed"; //Couldn't connect
    }
    else
        return NULL; //Success
#else
    struct sockaddr_in serv_addr;
	if((s = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        return (char*)"sock create failed";

	memset(&serv_addr, 0, sizeof(serv_addr)); 

	serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PortNo); 

    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
        return (char*)"inet_pton error occured";

	if(connect(s, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	    return (char*)"sock connect failed";

    return NULL; //Success
#endif
}

//CLOSECONNECTION � shuts down the socket and closes any connection on it
void close_tcp()
{
#ifdef __WIN32__
    //Close the socket if it exists
    if (s)
        closesocket(s);

    WSACleanup(); //Clean up Winsock
#else
	close(s);
#endif
}

#include <iostream>
#define MAX_FORMATTED_STR_LEN 1024
void log_remote(const char *fmt, ...)
{
    char log_str[MAX_FORMATTED_STR_LEN];
    va_list arguments;
    va_start(arguments, fmt);

#ifdef __WIN32__
    vsprintf_s
#else
    vsnprintf
#endif
    (log_str, MAX_FORMATTED_STR_LEN, fmt, arguments);

	send(s, log_str, (int)strlen(log_str), 0);

    va_end(arguments);

#ifdef __WIN32__
	Sleep(2);
#else
	usleep(2000);
#endif
}
