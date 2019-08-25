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
//#include "DBConnector.h"
#include "IocpBase.h"
//#include "Monster.h"

using namespace std;

// DB ����
#define DB_ADDRESS		"localhost"
#define	DB_PORT			3306
#define DB_ID			"root"
#define DB_PW			"anfrhrl"
#define DB_SCHEMA		"sungminworld"

class MainIocp : public IocpBase
{
public:
	MainIocp();
	virtual ~MainIocp();
	
	virtual void StartServer() override;
	// �۾� ������ ����
	virtual bool CreateWorkerThread() override;
	// �۾� ������
	virtual void WorkerThread() override;
	// Ŭ���̾�Ʈ���� �۽�
	static void Send(scSOCKETINFO * pSocket);	

private:
	static map<int, SOCKET> SessionSocket;	// ���Ǻ� ���� ����

	FuncProcess				fnProcess[100];	// ��Ŷ ó�� ����ü

	// ��ε�ĳ��Ʈ �Լ�
	static void Broadcast(stringstream & SendStream);
};
