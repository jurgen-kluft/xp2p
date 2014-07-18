//==============================================================================
//  x_msg.h
//==============================================================================
#ifndef __XPEER_2_PEER_MSG_READER_WRITER_H__
#define __XPEER_2_PEER_MSG_READER_WRITER_H__
#include "xbase\x_target.h"
#ifdef USE_PRAGMA_ONCE 
#pragma once 
#endif

#include "xbase\x_allocator.h"
#include "xp2p\private\x_lqueue.h"

namespace xcore
{
	// ==============================================================================================================================
	// ==============================================================================================================================
	// ==============================================================================================================================
	namespace xp2p
	{
		class ipeer;
		class imessage_allocator;

		class message_block : public lqueue<message_block>
		{
		public:
			inline				message_block() : lqueue(this), flags_(0), size_(0), data_(NULL), const_data_(NULL) { }
			inline				message_block(void* _data, u32 _size) : lqueue(this), flags_(0), size_(_size), data_(_data), const_data_(NULL) {}
			inline				message_block(void const* _data, u32 _size) : lqueue(this), flags_(0), size_(_size), data_(NULL), const_data_(_data) {}

			u32					get_flags() const;
			void				set_flags(u32 _flags);

			u32					get_size() const;
			void*				get_data();
			const void*			get_data() const;

			XCORE_CLASS_PLACEMENT_NEW_DELETE

		protected:
			u32					flags_;
			u32					size_;
			void*				data_;
			void const*			const_data_;
		};

		class message_reader
		{
		public:
			inline				message_reader(message_block* _block) : cursor_(0), block_(_block) {}

			void				set_cursor(u32);
			u32					get_cursor() const;

			bool				can_read(u32 number_of_bytes) const;		// check if we still can read n number of bytes

			u32					read(bool&);
			u32					read(u8  &);
			u32					read(s8  &);
			u32					read(u16 &);
			u32					read(s16 &);
			u32					read(u32 &);
			u32					read(s32 &);
			u32					read(u64 &);
			u32					read(s64 &);
			u32					read(f32 &);
			u32					read(f64 &);
			
			bool				view_data(xbyte const*& _data, u32 _size);
			bool				view_string(char const*& _str, u32& _out_len);

			void				next_block();

		protected:
			u32					cursor_;
			message_block*		block_;
		};

		class message_writer
		{
		public:
			inline				message_writer(message_block* _block) : cursor_(0), block_(_block) {}

			void				set_cursor(u32);
			u32					get_cursor() const;

			bool				can_write(u32 num_bytes = 0) const;

			void				write(bool);
			void				write(u8  );
			void				write(s8  );
			void				write(u16 );
			void				write(s16 );
			void				write(u32 );
			void				write(s32 );
			void				write(u64 );
			void				write(s64 );
			void				write(f32 );
			void				write(f64 );

			void				write_data(const xbyte*, u32);
			void				write_string(const char*, u32 _len=0);

			void				next_block();

		protected:
			u32					cursor_;
			xbyte*				data_;
			message_block*		block_;
		};


		class message : public lqueue<message>
		{
		public:
			inline				message(ipeer* _from, ipeer* _to, u32 _flags) : lqueue(this), from_(_from), to_(_to), flags_(_flags), nblocks_(0), pblocks_(NULL) {}

			enum eflags
			{
				MESSAGE_FLAG_EVENT = 0x80000000,
				MESSAGE_FLAG_DATA = 0x40000000,

				MESSAGE_FLAG_EVENT_MASK = 0x0000000f,
				MESSAGE_FLAG_EVENT_CANNOT_CONNECT = 0x00000000,
				MESSAGE_FLAG_EVENT_CONNECTED = 0x00000001,
				MESSAGE_FLAG_EVENT_DISCONNECTED = 0x00000003,
			};

			ipeer*				get_from() const;
			ipeer*				get_to() const;
			u32					get_flags() const;

			bool				is_from(ipeer*) const;

			bool				has_event() const;
			bool				has_data() const;

			bool				event_is_connected() const;
			bool				event_disconnected() const;
			bool				event_cannot_connect() const;

			void				add_block(message_block*);

			message_reader		get_reader() const;
			message_writer		get_writer() const;

			void				release(imessage_allocator*);

			XCORE_CLASS_PLACEMENT_NEW_DELETE

		protected:
			ipeer*				from_;
			ipeer*				to_;
			u32					flags_;
			u32					nblocks_;
			message_block*		pblocks_;
		};

		class outgoing_messages
		{
		public:
			inline				 outgoing_messages() : message_(NULL) {}
			inline				 outgoing_messages(message* _message) : message_(_message) {}
			inline				~outgoing_messages() {}

			ipeer*				get_from() const;
			ipeer*				get_to() const;
			u32					get_flags() const;

			message_reader		get_reader() const;
			message_writer		get_writer() const;
			
			bool				has_message() const;
			void				enqueue(message*);
			message*			dequeue();

		protected:
			inline				outgoing_messages(const outgoing_messages&) {}

			message*			message_;
		};


		class incoming_messages
		{
		public:
			inline				 incoming_messages() : message_(NULL) {}
			inline				 incoming_messages(message* _message) : message_(_message) {}
			inline				~incoming_messages() {}

			bool				is_from(ipeer*) const;
			ipeer*				get_from();
			u32					get_flags() const;

			bool				has_event() const;
			bool				has_data() const;

			bool				event_is_connected() const;
			bool				event_disconnected() const;
			bool				event_cannot_connect() const;

			message_reader		get_reader() const;

			bool				has_message() const;
			void				enqueue(message*);
			message*			dequeue();

		protected:
			inline				incoming_messages(const incoming_messages&) {}

			message*			message_;
		};

		class imessage_allocator
		{
		public:
			virtual message*		allocate(ipeer* _from, ipeer* _to, u32 _flags) = 0;
			virtual void			deallocate(message*) = 0;

			virtual message_block*	allocate(u32 _flags, u32 _size) = 0;
			virtual void			deallocate(message_block*) = 0;
		};


	}
}

#endif	///< __XPEER_2_PEER_MSG_READER_WRITER_H__
