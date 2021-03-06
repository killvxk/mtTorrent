#pragma once
#include "utils/ServiceThreadpool.h"
#include "utils/TcpAsyncStream.h"

namespace mtt
{
	class IncomingPeersListener
	{
	public:

		IncomingPeersListener(std::function<void(std::shared_ptr<TcpAsyncStream>, const uint8_t* hash)> onNewPeer);

		void stop();

	private:

		std::function<void(std::shared_ptr<TcpAsyncStream>, const uint8_t* hash)> onNewPeer;

		std::mutex peersMutex;
		std::vector<std::shared_ptr<TcpAsyncStream>> pendingPeers;

		std::shared_ptr<TcpAsyncServer> listener;
		ServiceThreadpool pool;

		void removePeer(TcpAsyncStream* s);
		void addPeer(TcpAsyncStream* s, const uint8_t* hash);
	};
}