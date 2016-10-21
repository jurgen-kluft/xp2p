#ifndef __XP2P_PRACTICALSOCKET_INCLUDED__
#define __XP2P_PRACTICALSOCKET_INCLUDED__
#include "xbase\x_target.h"
#ifdef USE_PRAGMA_ONCE 
#pragma once 
#endif

#include "xp2p\x_types.h"

namespace xcore
{
	namespace xnet
	{

		/**
		 *   Base class representing basic communication endpoint
		 */
		class socket_base
		{
		public:
			enum
			{
				INVALID_SOCKET_DESCRIPTOR = -1,
			};

			/**
			 *   Close and deallocate this socket
			 */
			~socket_base();

			/**
			*   Get the local address
			*   @return true if socket has valid descriptor
			*/
			bool valid() const;

			/**
			 *   Get the local address
			 *   @return local address of socket
			 */
			s32 getLocalAddress(char* address, s32 len);

			/**
			 *   Get the local port
			 *   @return local port of socket
			 */
			u16 getLocalPort();

			/**
			 *   Set the local port to the specified port and the local address
			 *   to any interface
			 *   @param localPort local port
			 */
			void setLocalPort(u16 localPort);

			/**
			 *   Set the local port to the specified port and the local address
			 *   to the specified address.  If you omit the port, a random port
			 *   will be selected.
			 *   @param localAddress local address
			 *   @param localPort local port
			 */
			void setLocalAddressAndPort(const char* localAddress, u16 localPort = 0);

			/**
			 *   If WinSock, unload the WinSock DLLs; otherwise do nothing.  We ignore
			 *   this in our sample client code but include it in the library for
			 *   completeness.  If you are running on Windows and you are concerned
			 *   about DLL resource consumption, call this after you are done with all
			 *   Socket instances.  If you execute this on Windows while some instance of
			 *   Socket exists, you are toast.  For portability of client code, this is
			 *   an empty function on non-Windows platforms so you can always include it.
			 *   @param buffer buffer to receive the data
			 *   @param bufferLen maximum number of bytes to read into buffer
			 *   @return number of bytes read, 0 for EOF, and -1 for error
			 */
			static void cleanUp();

			/**
			 *   Resolve the specified service for the specified protocol to the
			 *   corresponding port number in host byte order
			 *   @param service service to resolve (e.g., "http")
			 *   @param protocol protocol of service to resolve.  Default is "tcp".
			 */
			static u16 resolveService(const char* service, const char* protocol = "tcp");

		private:
			// Prevent the user from trying to use value semantics on this object
			socket_base(const socket_base &sock);
			void operator=(const socket_base &sock);

		protected:
			s32 mSocketDescriptor;
			socket_base(s32 descriptor = INVALID_SOCKET_DESCRIPTOR);

			/**
			*   Open the socket
			*   @return true if socket was opened succesfully
			*/
			bool open_socket(s32 type, s32 protocol);

		};

		/**
		 *   Socket which is able to connect, send, and receive
		 */
		class socket : public socket_base
		{
		public:
			/**
			 *   Establish a socket connection with the given foreign
			 *   address and port
			 *   @param foreignAddress foreign address (IP address or name)
			 *   @param foreignPort foreign port
			 */
			void connect(const char* foreignAddress, u16 foreignPort);

			/**
			 *   Write the given buffer to this socket.  Call connect() before
			 *   calling send()
			 *   @param buffer buffer to be written
			 *   @param bufferLen number of bytes from buffer to be written
			 */
			s32 send(const void *buffer, s32 bufferLen);

			/**
			 *   Read into the given buffer up to bufferLen bytes data from this
			 *   socket.  Call connect() before calling recv()
			 *   @param buffer buffer to receive the data
			 *   @param bufferLen maximum number of bytes to read into buffer
			 *   @return number of bytes read, 0 for EOF, and -1 for error
			 */
			s32 recv(void *buffer, s32 bufferLen);

			/**
			 *   Get the foreign address. Call connect() before calling recv()
			 *   @return length of address string
			 */
			s32 getForeignAddress(char* address, s32 len);

			/**
			 *   Get the foreign port.  Call connect() before calling recv()
			 *   @return foreign port
			 */
			u16 getForeignPort();

		protected:
			/**
			*   Construct a valid socket
			*/
			socket(s32 descriptor = INVALID_SOCKET_DESCRIPTOR);
		};

		/**
		 *   TCP socket for communication with other TCP sockets
		 */
		class tcpsocket : public socket
		{
		public:
			/**
			*   Construct an invalid/empty TCP socket
			*/
			tcpsocket();

			/**
			*   Open the TCP socket
			*/
			void open();

			/**
			 *   Construct a TCP socket with a connection to the given foreign address
			 *   and port
			 *   @param foreignAddress foreign address (IP address or name)
			 *   @param foreignPort foreign port
			 */
			void connect(const char* foreignAddress, u16 foreignPort);

		private:
			// Access for TCPServerSocket::accept() connection creation
			friend class tcpserversocket;
			void	use(s32 descriptor);
		};

		/**
		 *   TCP socket class for servers
		 */
		class tcpserversocket : public socket_base
		{
		public:
			/**
			*   Construct an invalid/empty TCP server socket
			*/
			tcpserversocket();

			/**
			 *   Construct a TCP socket for use with a server, accepting connections
			 *   on the specified port on any interface
			 *   @param localPort local port of server socket, a value of zero will
			 *                   give a system-assigned unused port
			 *   @param queueLen maximum queue length for outstanding
			 *                   connection requests (default 5)
			 */
			void open(u16 localPort, s32 queueLen = 5);

			/**
			 *   Construct a TCP socket for use with a server, accepting connections
			 *   on the specified port on the interface specified by the given address
			 *   @param localAddress local interface (address) of server socket
			 *   @param localPort local port of server socket
			 *   @param queueLen maximum queue length for outstanding
			 *                   connection requests (default 5)
			 */
			void open(const char* localAddress, u16 localPort, s32 queueLen = 5);

			/**
			 *   Blocks until a new connection is established on this socket or error
			 *   s = initialized connection socket
			 *   @return 0=Ok, -1=Error
			 */
			s32	accept(tcpsocket * s);

		private:
			void setListen(s32 queueLen);
		};

		/**
		  *   UDP socket class
		  */
		class udpsocket : public socket
		{
		public:
			/**
			 *   Construct an invalid/empty UDP socket
			 */
			udpsocket();

			/**
			 *   Construct a UDP socket with the given local port
			 *   @param localPort local port
			 */
			void open(u16 localPort);

			/**
			 *   Construct a UDP socket with the given local port and address
			 *   @param localAddress local address
			 *   @param localPort local port
			 */
			void open(const char* localAddress, u16 localPort);

			/**
			 *   Unset foreign address and port
			 *   @return true if disassociation is successful
			 */
			void disconnect();

			/**
			 *   Send the given buffer as a UDP datagram to the specified address/port
			 *   @param buffer buffer to be written
			 *   @param bufferLen number of bytes to write
			 *   @param foreignAddress address (IP address or name) to send to
			 *   @param foreignPort port number to send to
			 *   @return true if send is successful
			 */
			void sendTo(const void *buffer, s32 bufferLen, const char* foreignAddress, u16 foreignPort);

			/**
			 *   Read read up to bufferLen bytes data from this socket.
			     The given buffer is where the data will be placed.
			 *   @param buffer buffer to receive data
			 *   @param bufferLen maximum number of bytes to receive
			 *   @param sourceAddress address of datagram source
			 *   @param sourcePort port of data source
			 *   @return number of bytes received and -1 for error
			 */
			int recvFrom(void *buffer, s32 bufferLen, char* sourceAddress, u16 &sourcePort);

			/**
			 *   Set the multicast TTL
			 *   @param multicastTTL multicast TTL
			 */
			void setMulticastTTL(u8 multicastTTL);

			/**
			 *   Join the specified multicast group
			 *   @param multicastGroup multicast group address to join
			 */
			void joinGroup(const char* multicastGroup);

			/**
			 *   Leave the specified multicast group
			 *   @param multicastGroup multicast group address to leave
			 */
			void leaveGroup(const char* multicastGroup);

		private:
			void setBroadcast();
		};

	}
}

#endif