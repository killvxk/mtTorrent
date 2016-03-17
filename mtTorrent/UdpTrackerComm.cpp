#include "UdpTrackerComm.h"
#include "PacketHelper.h"
#include "Network.h"
#include <iostream>

using namespace Torrent;

ConnectResponse UdpTrackerComm::getConnectResponse(DataBuffer buffer)
{
	ConnectResponse out;

	if (buffer.size() >= sizeof ConnectResponse)
	{
		PacketReader packet(buffer);

		out.action = packet.pop32();
		out.transaction = packet.pop32();
		out.connectionId = packet.pop64();
	}

	return out;
}

AnnounceResponse UdpTrackerComm::getAnnounceResponse(DataBuffer buffer)
{
	PacketReader packet(buffer);

	AnnounceResponse resp;
	resp.action = packet.pop32();
	resp.transaction = packet.pop32();
	resp.interval = packet.pop32();

	resp.leechCount = packet.pop32();
	resp.seedCount = packet.pop32();

	size_t count = static_cast<size_t>(packet.getRemainingSize() / 6.0f);

	for (size_t i = 0; i < count; i++)
	{
		PeerInfo p;
		p.setIp(packet.pop32());
		p.port = packet.pop16();

		resp.peers.push_back(p);
	}

	return resp;
}

AnnounceResponse UdpTrackerComm::announceUdpTracker(std::string host, std::string port)
{
	std::cout << "Announcing to tracker " << host << "\n";

	AnnounceResponse announceMsg;

	boost::asio::io_service io_service;
	udp::resolver resolver(io_service);

	udp::socket sock(io_service);
	sock.open(udp::v4());

	/*boost::asio::ip::udp::endpoint local(
	boost::asio::ip::address_v4::any(),
	client->listenPort);
	sock.bind(local);*/

	auto messageData = createConnectRequest();

	try
	{
		auto message = sendUdpRequest(sock, resolver, messageData, host.data(), port.data(), 5000);
		auto response = getConnectResponse(message);

		if (validResponse(response))
		{
			auto announce = createAnnounceRequest(response);
			auto announceMsgBuffer = sendUdpRequest(sock, resolver, announce, host.data(), port.data());
			announceMsg = getAnnounceResponse(announceMsgBuffer);
		}

		std::cout << "Tracker " << host << " returned peers:" << std::to_string(announceMsg.peers.size()) << ", p: " << std::to_string(announceMsg.seedCount) << ", l: " << std::to_string(announceMsg.leechCount) << "\n";
	}
	catch (const std::exception&e)
	{
		std::cout << "Udp " << host << " exception: " << e.what() << "\n";
	}

	return announceMsg;
}

DataBuffer UdpTrackerComm::createAnnounceRequest(ConnectResponse& response)
{
	auto transaction = generateTransaction();

	lastMessage = { Announce, transaction };

	PacketBuilder packet;
	packet.add64(response.connectionId);
	packet.add32(Announce);
	packet.add32(transaction);

	auto& iHash = torrent->infoHash;
	packet.add(iHash.data(), iHash.size());

	packet.add(client->hashId, 20);

	packet.add64(0);
	packet.add64(0);
	packet.add64(0);

	packet.add32(Started);
	packet.add32(0);

	packet.add32(client->key);
	packet.add32(client->maxPeersPerRequest);
	packet.add32(client->listenPort);
	packet.add16(0);

	return packet.getBuffer();
}

DataBuffer UdpTrackerComm::createConnectRequest()
{
	auto transaction = generateTransaction();
	uint64_t connectId = 0x41727101980;

	lastMessage = { Connnect, transaction };

	PacketBuilder packet;
	packet.add64(connectId);
	packet.add32(Connnect);
	packet.add32(transaction);

	return packet.getBuffer();
}

void Torrent::UdpTrackerComm::setInfo(ClientInfo* c, TorrentInfo* t)
{
	client = c;
	torrent = t;
}

bool Torrent::UdpTrackerComm::validResponse(TrackerMessage& resp)
{
	return resp.action == lastMessage.action && resp.transaction == lastMessage.transaction;
}

Torrent::UdpTrackerComm::UdpTrackerComm()
{
}