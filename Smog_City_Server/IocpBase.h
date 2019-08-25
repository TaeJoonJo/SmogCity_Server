#pragma once

// ��Ƽ����Ʈ ���� ���� define
#define _WINSOCK_DEPRECATED_NO_WARNINGS

// winsock2 ����� ���� �Ʒ� �ڸ�Ʈ �߰�
#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>
#include <map>
#include <vector>
#include <iostream>
//#include "CommonClass.h"

using namespace std;

#define	MAX_BUFFER		4096
#define SERVER_PORT		8000
#define MAX_CLIENTS		1000

// IOCP ���� ����ü
struct scSOCKETINFO
{
	WSAOVERLAPPED	overlapped;
	WSABUF			wsabuf;
	SOCKET			socket;
	char			buf[MAX_BUFFER];
	int				recvbytes;
	int				sendbytes;
};

// ��Ŷ ó�� �Լ� ������
struct FuncProcess
{
	void(*funcProcessPacket)(stringstream & RecvStream, scSOCKETINFO * pSocket);
	FuncProcess()
	{
		funcProcessPacket = nullptr;
	}
};

class IocpBase
{
public:
	IocpBase();
	virtual ~IocpBase();

	// ���� ��� �� ���� ���� ����
	bool Initialize();
	// ���� ����
	virtual void StartServer();
	// �۾� ������ ����
	virtual bool CreateWorkerThread();	
	// �۾� ������
	virtual void WorkerThread();
	// Ŭ���̾�Ʈ���� �۽�
	virtual void Send(scSOCKETINFO * pSocket);
	// Ŭ���̾�Ʈ ���� ���
	virtual void Recv(scSOCKETINFO * pSocket);		

protected:
	scSOCKETINFO	*SocketInfo;	// ���� ����
	SOCKET			ListenSocket;	// ���� ���� ����
	HANDLE			hIOCP;			// IOCP ��ü �ڵ�
	bool			bAccept;		// ��û ���� �÷���
	bool			bWorkerThread;	// �۾� ������ ���� �÷���
	HANDLE *		hWorkerHandle;	// �۾� ������ �ڵ�		
	int				nThreadCnt;	
};
