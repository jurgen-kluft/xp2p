#include "xbase\x_target.h"
#include "xp2p\libudx\x_udx-packet.h"

namespace xcore
{
	// User message data block
	void*		udx_packet::get_msg(u32& size)
	{
		// The message data block exists after the header
		udx_packet_inf*	inf = get_inf();
		size = inf->m_size_in_bytes - sizeof(udx_packet_inf) - sizeof(udx_packet_hdr);
		u8* msgblock = (u8*)((u8*)this + sizeof(udx_packet_inf) + 2 * sizeof(udx_packet_hdr));
		return msgblock;
	}

	// Pointer and size of the packet to transfer
	void const*	udx_packet::get_pkt(u32& size) const
	{
		// Figure out the size of the header, this is dynamic since the ACK data is not
		// constant size
		udx_packet_hdr const* hdr = get_hdr();
		u32 const real_ack_bytes = (hdr->m_hdr_ack_size + 7) / 8;
		u32 const max_ack_bytes = sizeof(hdr->m_hdr_acks);

		// We still should compact the udx_packet_hdr 
		u32 const sizeof_hdr = sizeof(udx_packet_hdr);
		u32 const offset_hdr = max_ack_bytes - real_ack_bytes;
		u32 const sizeof_com = sizeof_hdr - offset_hdr;

		u8 const* compacthdr = (u8 const*)((u8 const*)this + sizeof(udx_packet_inf) + sizeof(udx_packet_hdr) + offset_hdr);

		udx_packet_inf const* inf = get_inf();
		size = inf->m_body_in_bytes + sizeof_com;

		return compacthdr;
	}

	// Copy the 'accessible' packet header into the 'compact' header
	void		udx_packet::a2c_hdr()
	{
		// Figure out the size of the header, this is dynamic since the ACK data is not
		// constant size
		udx_packet_hdr const* hdr = get_hdr();
		u32 const real_ack_bytes = (hdr->m_hdr_ack_size + 7) / 8;
		u32 const max_ack_bytes = sizeof(hdr->m_hdr_acks);

		// We still should compact the udx_packet_hdr 
		u32 const sizeof_hdr = sizeof(udx_packet_hdr);
		u32 const offset_hdr = max_ack_bytes - real_ack_bytes;
		u32 const sizeof_com = sizeof_hdr - offset_hdr;

		u8 const* generichdr = (u8*)((u8*)this + sizeof(udx_packet_inf));
		u8* compacthdr = (u8*)((u8*)this + sizeof(udx_packet_inf) + sizeof(udx_packet_hdr));
		for (s32 i = 0; i < sizeof_com; ++i)
		{
			compacthdr[offset_hdr + i] = generichdr[i];
		}
	}
}