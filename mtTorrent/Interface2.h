#pragma once
#include <string>
#include <vector>
#include "Network.h"
#include <iostream>

extern int gcount;

#define MT_NAME "mtTorrent 0.2"
#define MT_HASH_NAME "MT-0-2-"

#define DHT_LOG(x) {std::cout << x;}
//#define NETWORK_LOG(x) {std::cout << x;}
//#define PEER_LOG(x) {std::cout << x;}
#define TRACKER_LOG(x) {std::cout << x;}
//#define PARSER_LOG(x) {std::cout << x;}
#define GENERAL_INFO_LOG(x) {std::cout << x;}

#ifndef DHT_LOG
#define DHT_LOG(x){}
#endif
#ifndef NETWORK_LOG
#define NETWORK_LOG(x){}
#endif
#ifndef PEER_LOG
#define PEER_LOG(x){}
#endif
#ifndef TRACKER_LOG
#define TRACKER_LOG(x){}
#endif
#ifndef PARSER_LOG
#define PARSER_LOG(x){}
#endif
#ifndef GENERAL_INFO_LOG
#define GENERAL_INFO_LOG(x){}
#endif

namespace mtt
{
	struct File
	{
		int id;
		std::vector<std::string> path;
		size_t size;
		uint32_t startPieceIndex;
		size_t startPiecePos;
		uint32_t endPieceIndex;
		size_t endPiecePos;
	};

	struct PieceInfo
	{
		char hash[20];
	};

	struct TorrentFileInfo
	{
		std::string announce;
		std::vector<std::string> announceList;

		DataBuffer infoHash;

		std::vector<PieceInfo> pieces;
		size_t pieceSize;
		size_t expectedBitfieldSize;

		std::vector<File> files;
		std::string directory;
		size_t fullSize;
	};

	struct SelectedFile
	{
		File file;
	};

	struct DownloadSelection
	{
		std::vector<SelectedFile> files;
	};

	struct PieceBlockInfo
	{
		uint32_t index;
		uint32_t begin;
		uint32_t length;
	};

	struct PieceBlock
	{
		PieceBlockInfo info;
		DataBuffer data;
	};

	struct PieceDownloadInfo
	{
		std::vector<PieceBlockInfo> blocksLeft;
		size_t blocksCount = 0;
	};

	struct DownloadedPiece
	{
		DataBuffer data;
		uint32_t index;
		size_t dataSize;

		void reset(size_t maxPieceSize);
		void addBlock(PieceBlock& block);
		size_t receivedBlocks = 0;
	};

	struct Addr
	{
		Addr();
		Addr(char* buffer, bool v6);

		std::string str;
		std::vector<uint8_t> addrBytes;
		uint16_t port;

		bool isIpv6();
		size_t parse(char* buffer, bool v6);
	};
}