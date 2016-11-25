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
	struct udx_msg;

	class udx_msg_handler
	{
	public:
		virtual void			handle_msg(udx_msg& msg) = 0;
	};

	// --------------------------------------------------------------------------------------------
	// [PUBLIC] API
	class udx_peer
	{
	public:
		virtual udx_address*	get_address() const = 0;

		virtual bool 			connect() = 0;
		virtual bool 			disconnect() = 0;
		virtual bool 			is_connected() const = 0;

		virtual void 			push_incoming(udx_packet*) = 0;
		virtual void 			push_outgoing(udx_packet*) = 0;

		virtual bool			pop_incoming(udx_packet*&) = 0;
		virtual bool 			pop_outgoing(udx_packet*&) = 0;

		virtual void			process(u64 delta_time_us) = 0;
	};

	udx_peer*	gCreateUdxPeer(udx_address* address, udx_alloc* allocator);
}

#endif
