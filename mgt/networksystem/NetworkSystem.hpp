/// @file

#pragma once

#include "networksystem/INetworkSystem.hpp"

struct ISystem;
struct ICvarRegistry;
struct IConVarController;

class CNetworkSystem final : public INetworkSystem
{
public:
	CNetworkSystem();
	~CNetworkSystem();
	
	bool Init(CreateInterfaceFn afnEngineFactory) override;
	void Shutdown() override;
	
	bool GetPacket(netsrc_t sock, netadr_t *net_from, sizebuf_t *net_message) override;
	void SendPacket(netsrc_t sock, int length, void *data, netadr_t to) override;
	
	bool StringToAdr(const char *s, netadr_t &a) override;
private:
	int UDP_OpenSocket(int port);
	void GetLocalAddress();
	
	ISystem *mpSystem{nullptr};
	ICvarRegistry *mpCvarRegistry{nullptr};
	IConVarController *mpCvarController{nullptr};
};