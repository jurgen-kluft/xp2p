#include "xbase\x_target.h"
#include "xp2p\x_sha1.h"
#include "xp2p\libudx\x_udx.h"
#include "xp2p\libudx\x_udx-alloc.h"
#include "xp2p\libudx\x_udx-address.h"
#include "xp2p\libudx\x_udx-packet.h"
#include "xp2p\libudx\x_udx-message.h"
#include "xp2p\libudx\x_udx-registry.h"
#include "xp2p\libudx\x_udx-peer.h"
#include "xp2p\libudx\x_udx-udp.h"
#include "xp2p\libudx\x_udx-rtt.h"
#include "xp2p\libudx\x_udx-time.h"

#include "xp2p\private\x_sockets.h"

namespace xcore
{
	// --------------------------------------------------------------------------------------------
	// [PRIVATE] IMPLEMENTATION OF PEER (P2P CONNECTION)
	// 
	// Functionality of a peer:
	//   - Receive and Send packets according to the Congestion-Control module
	//   - Process ACK data and apply it to our in-flight queue
	//   - Make and send ACK data back to remote peer for received packets
	//   - Release incoming (in-order) messages to user
	//   - Deallocate in-flight packets that have been ACK-ed
	//   - Compute RTT and RTO in real-time
	//   - Re-send any in-flight packets that are not ACK-ed and older than RTO 
	// 
	// When DISCONNECTED:
	//   - When the receiving a SYN packet change state to CONNECTED
	//     Note: After verifying the credentials (public key, private key)
	// 
	// When CONNECTED and asked to disconnect or receiving a FIN packet:
	//   - Change state to DISCONNECTING
	//   - Do not accept any more outgoing packets from user 
	//   - Create FIN packet and add to outgoing queue
	//   - Keep sending remaining packets in outgoing queue
	//   - Accept incoming packets
	//   - Wait for in-flight queue to be empty (ACK-ed)
	//   - When outgoing and in-flight queues are empty change state to DISCONNECTED
	//
	// Future Features:
	//   - Encrypted header
	//   - 
	 

	class udx_socket_peer : public udx_peer
	{
	public:
		udx_socket_peer(udx_alloc* _allocator, udx_alloc* _msg_allocator, udx_address* _address);

		virtual udx_address*	get_address() const;

		virtual bool 			connect();
		virtual bool 			disconnect();
		virtual bool 			is_connected() const;

		virtual void			push_incoming(udx_packet* packet);
		virtual bool			pop_incoming(udx_packet*& packet);

		virtual void			push_outgoing(udx_packet* packet);
		virtual bool			pop_outgoing(udx_packet*& packet);

		// Process time-outs and deal with re-transmitting, disconnecting etc..
		virtual void			process(u64 delta_time_us);

	protected:
		udx_alloc*				m_sys_alloc;
		udx_alloc*				m_msg_alloc;

		udx_rtt*				m_rtt;

		udx_address*			m_address;

		udx_packetqueue			m_incoming;
		udx_packetqueue			m_outgoing;
		udx_packetqueue			m_inflight;
	};

	udx_socket_peer::udx_socket_peer(udx_alloc* _allocator, udx_alloc* _msg_allocator, udx_address* _address)
		: m_sys_alloc(_allocator)
		, m_msg_alloc(_msg_allocator)
		, m_address(_address)
	{

	}

	udx_address*	udx_socket_peer::get_address() const
	{
		return m_address;
	}

	bool 	udx_socket_peer::connect()
	{
		return false;
	}

	bool	udx_socket_peer::disconnect()
	{
		return false;
	}

	bool	udx_socket_peer::is_connected() const
	{
		return false;
	}
	
	void	udx_socket_peer::push_incoming(udx_packet * packet)
	{
		udx_packet_hdr* packet_hdr = packet->get_hdr();

		// Switch type:
		// - DATA
		// - ACK
		// - SYN

		// Process ACK data immediately and see if we can reduce the
		// 'inflight' queue.

		m_incoming.enqueue(packet_hdr->m_hdr_pkt_seqnr, (void*)packet);
	}

	bool	udx_socket_peer::pop_incoming(udx_packet *& packet)
	{
		return false;
	}

	void	udx_socket_peer::push_outgoing(udx_packet * packet)
	{
		udx_packet_hdr* packet_hdr = packet->get_hdr();
		m_outgoing.enqueue(packet_hdr->m_hdr_pkt_seqnr, (void*)packet);
	}

	bool	udx_socket_peer::pop_outgoing(udx_packet *& packet)
	{
		return false;
	}

	void	udx_socket_peer::process(u64 delta_time_us)
	{
		// For our inflight queue check for any packets that
		// have not received an ACK and are older than RTO.
		// For those we need to mark them as resend

		// Do we have new incoming packets, if so prepare an
		// ack packet to be send back.

	}

}
