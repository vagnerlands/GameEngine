#include "CWinSocket.h"
#include "SocketUtils.h"

// general conversion constants
// socket type conversion table
const Int32 SocketType[SocketType_Total] =
{
    (Int32)SOCK_STREAM,
    (Int32)SOCK_DGRAM
};

// socket type conversion table
const Int32 Protocol[SocketProtocol_Total] =
{
    (Int32)IPPROTO_UDP,
    (Int32)IPPROTO_TCP
};

CWinSocket::CWinSocket()
{
    m_socket = INVALID_SOCKET;
    m_recycle = false;
    // clear this variable
    memset(&m_resolvedAddress, 0, sizeof(sockaddr_in));
}

CWinSocket::~CWinSocket()
{
    // temporary variables
    int iResult;
    // shutdown the connection since we're done
    iResult = shutdown(m_socket, SD_SEND);
    if (iResult == SOCKET_ERROR)
    {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(m_socket);
        m_socket = INVALID_SOCKET;
    }
    else
    {
        // cleanup
        closesocket(m_socket);
        m_socket = INVALID_SOCKET;
    }
}

ESocketCreationStatus CWinSocket::Create(const SSocketAttributes& socketAttributes)
{
    // makes a copy of the given socket attributes
    m_socketAttributes = socketAttributes;

    // actually creates the socket and gets the socket descriptor
    m_socket = (SOCKET)socket(
        AF_INET, // let's be practical - supports only AF_INET
        SocketType[socketAttributes.m_type], // SOCK_DGRAM or SOCK_STREAM
        Protocol[socketAttributes.m_protocol] // IPPROTO_UDP or IPPROTO_TCP
    );

    // socket was created successfully?
    if (m_socket != InvalidSocket)
    {
        // resolves the IP address with the given parameters
        translateAddress(socketAttributes.m_port, socketAttributes.m_ip.c_str(), m_resolvedAddress);

        // set if socket is blocking type
        (void)ioctlsocket(
            (SOCKET)m_socket,
            FIONBIO,
            (u_long*)&(socketAttributes.m_isNonBlocking));

        // set Time Out
        (void)setsockopt(
            (SOCKET)m_socket,
            SOL_SOCKET,
            SO_SNDTIMEO,
            (Byte*)&socketAttributes.m_timeout,
            sizeof(socketAttributes.m_timeout)
        );

        // set Send Buffer Size (if applicable)
        (void)setsockopt(
            (SOCKET)m_socket,
            SOL_SOCKET,
            SO_SNDBUF,
            (Byte*)&socketAttributes.m_sendingBufferSize,
            sizeof(socketAttributes.m_sendingBufferSize)
        );

        // set Receive Buffer Size (if applicable)
        (void)setsockopt(
            (SOCKET)m_socket,
            SOL_SOCKET,
            SO_RCVBUF,
            (Byte*)&socketAttributes.m_receiveBufferSize,
            sizeof(socketAttributes.m_receiveBufferSize)
        );

        UInt32 ttl = 60; // max = 255 
        (void)setsockopt((SOCKET)m_socket, 
            IPPROTO_IP, 
            IP_MULTICAST_TTL,
            (Byte*)&ttl, 
            sizeof(ttl)
        );

        // set Receive Buffer Size (if applicable)
        (void)setsockopt(
            (SOCKET)m_socket,
            SOL_SOCKET,
            SO_RCVBUF,
            (Byte*)&socketAttributes.m_receiveBufferSize,
            sizeof(socketAttributes.m_receiveBufferSize)
        );

        if (socketAttributes.m_isBroadcast == 1)
        {
            UInt32 broadcastOption = (UInt32)socketAttributes.m_isBroadcast;
            (void)setsockopt((SOCKET)m_socket, SOL_SOCKET, SO_BROADCAST, (Byte*)&broadcastOption, sizeof(broadcastOption));
        }


    }

    return ESOCKETCREATION_OK;
}

ESocketConnectionStatus CWinSocket::startListenSocket()
{
    return ESocketConnectionStatus();
}

Int32
CWinSocket::translateAddress(UInt16 port, const Byte* pIp, sockaddr_in& sockaddress) const
{
    // may this IP be parsed using inet_pton()?
    bool isIpValid = false;
    // temporary place to store the inet result
    UByte inetAddress[4];
    // convert IP from text to numeric form (4 bytes)
    Int32 result = inet_pton(AF_INET, pIp, inetAddress);
    // result == 1 is no errors
    if (result == 1)
    {
        isIpValid = true;
    }
    sockaddress.sin_family = AF_INET;
    // probably a sending socket
    if ((port != 0) && (isIpValid))
    {
        sockaddress.sin_port = htons(port);
        sockaddress.sin_addr.S_un.S_un_b.s_b1 = inetAddress[0];
        sockaddress.sin_addr.S_un.S_un_b.s_b2 = inetAddress[1];
        sockaddress.sin_addr.S_un.S_un_b.s_b3 = inetAddress[2];
        sockaddress.sin_addr.S_un.S_un_b.s_b4 = inetAddress[3];
    }
    else
    {
        // probably a receiving socket
        sockaddress.sin_port = 0;
        sockaddress.sin_addr.S_un.S_addr = htonl(ADDR_ANY);
        sockaddress.sin_addr.S_un.S_un_b.s_b1 = 0;
        sockaddress.sin_addr.S_un.S_un_b.s_b2 = 0;
        sockaddress.sin_addr.S_un.S_un_b.s_b3 = 0;
        sockaddress.sin_addr.S_un.S_un_b.s_b4 = 0;
    }
    // no utility for this - for now
    return 0;
}

Int32 CWinSocket::Bind()
{
    // operation status (return value)
    Int32 operationStatus = 0;

    if (SocketUtils::IsMulticastIp(m_socketAttributes.m_ip.c_str()))
    {
        // set reuse
        UInt32 reuse = (UInt32)m_socketAttributes.m_mc.m_reuse;
        operationStatus = setsockopt((SOCKET)m_socket, SOL_SOCKET, SO_REUSEADDR, (Byte*)&reuse, sizeof(reuse));

        // join multicast
        struct ip_mreq mreqv4 = { 0 };
        struct sockaddr_in socketAddress = { 0 };
        translateAddress(m_socketAttributes.m_port, m_socketAttributes.m_interface.c_str(), socketAddress);
        mreqv4.imr_interface = socketAddress.sin_addr;
        mreqv4.imr_multiaddr = m_resolvedAddress.sin_addr;
        operationStatus = setsockopt(m_socket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (Byte *)& mreqv4, sizeof(mreqv4));

        // set interface
        //operationStatus = setsockopt(m_socket, IPPROTO_IP, IP_MULTICAST_IF, (Byte *)& mreqv4.imr_interface, sizeof(mreqv4.imr_interface));

        // since this is multicast and we're trying to bind, we no longer need to bind to a specific IP
        // alls optional setups related to multicast were set before
        // let's clear the IP address
        // this will make this socket listen from any address
        m_resolvedAddress.sin_addr = IN_ADDR();

        // disable LOOP back (Win95 and NT doesn't have this feature)
        //operationStatus = setsockopt(m_socket,
        //    IPPROTO_IP,
        //    IP_MULTICAST_LOOP,
        //    (Byte*)&m_socketAttributes.m_mc.m_loopback,
        //    sizeof(UInt32));
        if (operationStatus == SOCKET_ERROR)
        {
            cout << "SetMulticastLoopBack: setsockopt() failed with error code " << WSAGetLastError();
        }
    }
    // finally - binds for listening
    operationStatus = (bind((SOCKET)m_socket, (struct sockaddr*)&m_resolvedAddress, sizeof(m_resolvedAddress)));

    return operationStatus;
}

Int32
CWinSocket::Bind(UInt16 port, const Byte* pIp)
{
    struct sockaddr_in raw = { 0 };
    //
    translateAddress(port, pIp, raw);

    if (SocketUtils::IsMulticastIp(m_socketAttributes.m_ip.c_str()))
    {
        // set reuse
        UInt32 reuse = (UInt32)m_socketAttributes.m_mc.m_reuse;
        (void)setsockopt((SOCKET)m_socket, SOL_SOCKET, SO_REUSEADDR, (Byte*)&reuse, sizeof(reuse));

        // join multicast
        struct ip_mreq mreqv4 = { 0 };
        struct sockaddr_in socketAddress = { 0 };
        translateAddress(0, m_socketAttributes.m_interface.c_str(), socketAddress);
        mreqv4.imr_interface = socketAddress.sin_addr;
        mreqv4.imr_multiaddr = raw.sin_addr;
        setsockopt(m_socket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (Byte *)& mreqv4, sizeof(mreqv4));
    }

    Int32 bindCode = (bind((SOCKET)m_socket, (struct sockaddr*)&raw, sizeof(raw)));
    return bindCode;
}

Int32 CWinSocket::Write(const Byte* pIp, UInt32 port, const Byte * buffer, Int32 const size)
{
    // method result - how many bytes were send
    int iSendResult = -1;

    struct sockaddr_in toRaw = { 0 };
    translateAddress(port, pIp, toRaw);

    // Echo the buffer back to the sender
    iSendResult = sendto(m_socket, buffer, size, 0, (struct sockaddr*)&toRaw, sizeof(sockaddr));
    if (iSendResult == SOCKET_ERROR)
    {
        cout << "Connection was lost - code: " << WSAGetLastError() << endl;
        closesocket(m_socket);
        // error
        return -1;
    }
    // message sent
    return iSendResult;
}

Int32
CWinSocket::Write(const Byte* buffer, Int32 const size)
{
    // method result - how many bytes were send
    int iSendResult = -1;

    struct sockaddr_in toRaw = { 0 };
    translateAddress(m_socketAttributes.m_port, m_socketAttributes.m_ip.c_str(), toRaw);

    // Echo the buffer back to the sender
    iSendResult = sendto(m_socket, buffer, size, 0, (struct sockaddr*)&toRaw, sizeof(sockaddr));
    if (iSendResult == SOCKET_ERROR)
    {
        cout << "Connection was lost - code: " << WSAGetLastError() << endl;
        closesocket(m_socket);
        // error
        return -1;
    }
    // message sent
    return iSendResult;
}


// incoming buffer
// size in bytes
ESocketReadStatus
CWinSocket::Read(Byte* buffer, Int32 *size)
{
    // Read the socket and place content in buffer*
    // *size shall hold the amount of bytes Read
    struct sockaddr_in saFrom = { 0 };
    Int32 saFromLength = (Int32)sizeof(saFrom);
    *size = recv(m_socket, buffer, s_MAX_BUFFER_SIZE, 0/*, (struct sockaddr*)&saFrom, &saFromLength*/);
    if (*size > 0)
    {
        return ESOCKETREAD_OK;
    }
    else if (*size == -1)
    {
        // no message available for now
        return ESOCKETREAD_FAIL;
    }
    else if ((*size == 0) || (m_socket == INVALID_SOCKET))
    {
        cout << "Connection was closed\n";
        return ESOCKETREAD_CONNECTION_LOST;
    }

    // message sent
    return ESOCKETREAD_OK;
}

