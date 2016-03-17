#pragma once

#include "Interface.h"

namespace Torrent
{
	struct TrackerMessage
	{
		uint32_t action;
		uint32_t transaction;
	};

	struct AnnounceResponse : public TrackerMessage
	{
		uint32_t interval;

		uint32_t leechCount;
		uint32_t seedCount;

		std::vector<PeerInfo> peers;
	};

	struct ConnectResponse : public TrackerMessage
	{
		uint64_t connectionId;
	};

	enum Action
	{
		Connnect = 0,
		Announce,
		Scrape,
		Error
	};

	enum Event
	{
		None = 0,
		Completed,
		Started,
		Stopped
	};

	class UdpTrackerComm
	{
	public:

		UdpTrackerComm();
		void setInfo(ClientInfo* client, TorrentInfo* tInfo);

		AnnounceResponse announceUdpTracker(std::string host, std::string port);

	private:

		DataBuffer createConnectRequest();
		ConnectResponse getConnectResponse(DataBuffer buffer);

		DataBuffer createAnnounceRequest(ConnectResponse& response);
		AnnounceResponse getAnnounceResponse(DataBuffer buffer);

		bool validResponse(TrackerMessage& resp);
		TrackerMessage lastMessage;

		uint32_t connectionId;

		ClientInfo* client;
		TorrentInfo* torrent;
	};
}