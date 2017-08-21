//
//  Network.cpp
//  SDLlib
//
//  Created by Nathan Touroux on 04/03/2017.
//  Copyright © 2017 Nathan Touroux. All rights reserved.
//

#include "Network.hpp"

Network::Network(){
    csock.recsize = sizeof(csock.sin);
#ifdef WIN32
    erreur = WSAStartup(MAKEWORD(2,2), &m_WSAData);
#else
    erreur = 0;
#endif // WIN32
}

Network::~Network(){
    closesocket(csock.sock);
#ifdef WIN32
    WSACleanup();
#endif // WIN32
}

bool Network::connect(std::string IP, int port){
    this->IP = IP;
    this->port = port;
    
    if(!erreur)
    {
        csock.sock = socket(AF_INET,SOCK_STREAM, 0);
        if(csock.sock != INVALID_SOCKET)
        {
            //std::cout << "La socket " << csock.sock << " est maintenant ouverte en mode TCP/IP" << std::endl;
            csock.sin.sin_addr.s_addr = inet_addr(IP.c_str());
            csock.sin.sin_family = AF_INET;
            csock.sin.sin_port = htons(port);
            
            if(::connect(csock.sock, (SOCKADDR*)&csock.sin, csock.recsize) != SOCKET_ERROR)
            {
                //std::cout << "Connection a " << inet_ntoa(csock.sin.sin_addr) << " sur le port " << htons(csock.sin.sin_port) << " reussie" << std::endl;
            }
            else
                return false;
        }
        else
            return false;
    }
    else
        return false;
    return true;
}

bool Network::sendTo(std::string IP, int port, void* data, int sizeByte){
    SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock == INVALID_SOCKET)
    {
        return false;
    }
    
    SOCKADDR_IN to = { 0 };
    int tosize = sizeof to;
    to.sin_addr.s_addr = inet_addr(IP.c_str());
    to.sin_port = htons(port);
    to.sin_family = AF_INET;
    
    if(sendto(sock, data, sizeByte, 0, (SOCKADDR *)&to, tosize) < 0)
    {
        return false;
    }
    
    return true;
}
bool Network::sendTo(std::string IP, int port, std::string str){
    SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock == INVALID_SOCKET)
    {
        return false;
    }
    
    SOCKADDR_IN to = { 0 };
    int tosize = sizeof to;
    to.sin_addr.s_addr = inet_addr(IP.c_str());
    to.sin_port = htons(port);
    to.sin_family = AF_INET;
    
    if(sendto(sock, str.c_str(), str.size(), 0, (SOCKADDR *)&to, tosize) < 0)
    {
        return false;
    }
    
    return true;
}
bool Network::receiveFrom(std::string IP, int port, void* data, int sizeByte){
    SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock == INVALID_SOCKET)
    {
        return false;
    }
    
    SOCKADDR_IN to = { 0 };
    int tosize = sizeof to;
    to.sin_addr.s_addr = inet_addr(IP.c_str());
    to.sin_port = htons(port);
    to.sin_family = AF_INET;
    
    if((recvfrom(sock, data, sizeByte, 0, (SOCKADDR *)&to, (socklen_t*)&tosize)) < 0)
    {
        return false;
    }
    
    return true;
}
std::string Network::receiveFrom(std::string IP, int port){
    std::string str;
    //TODO
    return str;
}
