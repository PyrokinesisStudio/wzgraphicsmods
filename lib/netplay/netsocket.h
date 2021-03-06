/*
	This file is part of Warzone 2100.
	Copyright (C) 1999-2004  Eidos Interactive
	Copyright (C) 2005-2010  Warzone Resurrection Project

	Warzone 2100 is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	Warzone 2100 is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with Warzone 2100; if not, write to the Free Software
	Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
*/

#ifndef _net_socket_h
#define _net_socket_h

#include "lib/framework/types.h"

#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus

typedef struct Socket Socket;
typedef struct SocketSet SocketSet;
typedef struct addrinfo SocketAddress;

#ifndef WZ_OS_WIN
static const int SOCKET_ERROR = -1;
#endif


// Init/shutdown.
void SOCKETinit(void);
void SOCKETshutdown(void);

// General.
int getSockErr(void);                                                   ///< Gets last socket error. (May be overwritten by functions that set errno.)
void setSockErr(int error);                                             ///< Sets last socket error.
const char *strSockError(int error);                                    ///< Converts a socket error to a string.

// Socket addresses.
SocketAddress *resolveHost(const char *host, unsigned port);            ///< Looks up a socket address.
void deleteSocketAddress(SocketAddress *addr);                          ///< Destroys the socket address.

// Sockets.
Socket *socketOpen(const SocketAddress *addr, unsigned timeout);        ///< Opens a Socket, using the first address in addr.
Socket *socketListen(unsigned int port);                                ///< Creates a listen-only Socket, which listens for incoming connections.
Socket *socketAccept(Socket *sock);                                     ///< Accepts an incoming Socket connection from a listening Socket.
void socketClose(Socket *sock);                                         ///< Destroys the Socket.
Socket *socketOpenAny(const SocketAddress *addr, unsigned timeout);     ///< Opens a Socket, using the first address that works in addr.
size_t socketArrayOpen(Socket **sockets, size_t maxSockets, const SocketAddress *addr, unsigned timeout);  ///< Opens up to maxSockets Sockets, of the types listed in addr.
void socketArrayClose(Socket **sockets, size_t maxSockets);             ///< Closes all Sockets in the array.

char const *getSocketTextAddress(Socket const *sock);                   ///< Gets a string with the socket address.
bool socketReadReady(Socket const *sock);                               ///< Returns if checkSockets found data to read from this Socket.
ssize_t readNoInt(Socket *sock, void *buf, size_t max_size);            ///< Reads up to max_size bytes from the Socket.
ssize_t readAll(Socket* sock, void *buf, size_t size, unsigned timeout);///< Reads exactly size bytes from the Socket, or blocks until the timeout expires.
ssize_t writeAll(Socket *sock, const void* buf, size_t size);           ///< Nonblocking write of size bytes to the Socket. All bytes will be written asynchronously, by a separate thread.

// Socket sets.
SocketSet *allocSocketSet(void);                                        ///< Constructs a SocketSet.
void deleteSocketSet(SocketSet *set);                                   ///< Destroys the SocketSet.

void SocketSet_AddSocket(SocketSet *set, Socket *socket);               ///< Adds a Socket to a SocketSet.
void SocketSet_DelSocket(SocketSet *set, Socket *socket);               ///< Removes a Socket from a SocketSet.
int checkSockets(const SocketSet *set, unsigned int timeout);           ///< Checks which Sockets are ready for reading. Returns the number of ready Sockets, or returns SOCKET_ERROR on error.

#ifdef __cplusplus
}
#endif //__cplusplus

#endif //_net_socket_h
