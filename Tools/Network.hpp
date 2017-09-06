//
//  Network.hpp
//  SDLlib
//
//  Created by Nathan Touroux on 04/03/2017.
//  Copyright © 2017 Nathan Touroux. All rights reserved.
//

#ifndef Network_hpp
#define Network_hpp

#include <iostream>

#ifdef WIN32
#include <winsock2.h>
typedef int socklen_t;
#else
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket(s) close (s)

typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;

#endif // WIN32
#include <iostream>

struct Sock
{
    SOCKADDR_IN sin;
    SOCKET sock;
    socklen_t recsize;
};

class Network{
private:
#ifdef WIN32
    WSADATA m_WSAData;
#endif
    
    Sock csock;
    
    std::string IP;
    int port;
    int erreur;
    
public:
    Network();
    ~Network();
    
    //bool host(int port);
    bool connect(std::string IP, int port);
    
    /*void send(void* data, int sizeByte);
    void send(std::string str);
    void receive(void* data, int sizeByte);
    std::string receive();*/
    
    
    
    static bool sendTo(std::string IP, int port, void* data, int sizeByte);
    static bool sendTo(std::string IP, int port, std::string str);
    static bool receiveFrom(std::string IP, int port, void* data, int sizeByte);
    static std::string receiveFrom(std::string IP, int port);
};

#endif /* Network_hpp */
