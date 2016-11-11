//==============================================================================
//  x_udx-socket.h
//==============================================================================
#ifndef __XP2P_UDX_SOCKET_H__
#define __XP2P_UDX_SOCKET_H__
#include "xbase\x_target.h"
#ifdef USE_PRAGMA_ONCE 
#pragma once 
#endif

#include "xp2p\libudx\x_udx-packet.h"
#include "xp2p\libudx\x_udx-packetqueue.h"

namespace xcore
{
	class udx_address;
	class udx_socket;
	struct udx_msg;

	// --------------------------------------------------------------------------------------------
	// [PUBLIC] API
	class udx_socket
	{
	public:
		virtual udx_address*	get_address() const = 0;

		virtual udx_msg			alloc_msg(u32 size) = 0;
		virtual void			free_msg(udx_msg& msg) = 0;

		virtual udx_address*	connect(const char* address) = 0;
		virtual bool			disconnect(udx_address*) = 0;

		virtual void			send(udx_msg& msg, udx_address* to) = 0;
		virtual bool			recv(udx_msg& msg, udx_address*& from) = 0;

		// Process time-outs and deal with re-transmitting, disconnecting etc..
		virtual void			process(u64 delta_time_us) = 0;
	};

	udx_socket*			gCreateUdxSocket(const char* local_address);
}

#endif