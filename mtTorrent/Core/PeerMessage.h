#pragma once
#include <vector>
#include "Interface.h"

namespace mtt
{
	enum PeerMessageId
	{
		Choke = 0,
		Unchoke,
		Interested,
		NotInterested,
		Have,
		Bitfield,
		Request,
		Piece,
		Cancel,
		Port,
		Extended = 20,
		Handshake,
		KeepAlive,
		Invalid
	};

	struct PeerMessage
	{
		PeerMessageId id = Invalid;

		uint32_t havePieceIndex;
		DataBuffer bitfield;

		struct  
		{
			uint8_t reservedBytes[8];
			uint8_t peerId[20];
			uint8_t info[20];
		}
		handshake;

		PieceBlockInfo request;
		PieceBlock piece;

		uint16_t port;
		uint16_t messageSize = 0;

		PeerMessage(DataBuffer& data);

		struct
		{
			uint8_t id;
			DataBuffer data;
		}
		extended;	
	};
}
