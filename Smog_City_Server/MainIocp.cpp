#include "stdafx.h"
#include "MainIocp.h"
#include <process.h>
#include <sstream>
#include <algorithm>
#include <string>

// static ���� �ʱ�ȭ
map<int, SOCKET>	MainIocp::SessionSocket;

unsigned int WINAPI CallWorkerThread(LPVOID p)
{
	MainIocp* pOverlappedEvent = (MainIocp*)p;
	pOverlappedEvent->WorkerThread();
	return 0;
}

unsigned int WINAPI CallMonsterThread(LPVOID p)
{
	MainIocp* pOverlappedEvent = (MainIocp*)p;
	//pOverlappedEvent->MonsterManagementThread();
	return 0;
}

MainIocp::MainIocp()
{

	// ��Ŷ �Լ� �����Ϳ� �Լ� ����
	/*fnProcess[EPacketType::SIGNUP].funcProcessPacket = SignUp;
	fnProcess[EPacketType::LOGIN].funcProcessPacket = Login;
	fnProcess[EPacketType::ENROLL_PLAYER].funcProcessPacket = EnrollCharacter;
	fnProcess[EPacketType::SEND_PLAYER].funcProcessPacket = SyncCharacters;
	fnProcess[EPacketType::HIT_PLAYER].funcProcessPacket = HitCharacter;
	fnProcess[EPacketType::CHAT].funcProcessPacket = BroadcastChat;
	fnProcess[EPacketType::LOGOUT_PLAYER].funcProcessPacket = LogoutCharacter;
	fnProcess[EPacketType::HIT_MONSTER].funcProcessPacket = HitMonster;*/
}


MainIocp::~MainIocp()
{
	// winsock �� ����� ������
	WSACleanup();
	// �� ����� ��ü�� ����
	if (SocketInfo)
	{
		delete[] SocketInfo;
		SocketInfo = NULL;
	}

	if (hWorkerHandle)
	{
		delete[] hWorkerHandle;
		hWorkerHandle = NULL;
	}
}

void MainIocp::StartServer()
{
	IocpBase::StartServer();
}

bool MainIocp::CreateWorkerThread()
{
	unsigned int threadId;
	// �ý��� ���� ������
	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);
	printf_s("[INFO] CPU ���� : %d\n", sysInfo.dwNumberOfProcessors);
	// ������ �۾� �������� ������ (CPU * 2) + 1
	nThreadCnt = sysInfo.dwNumberOfProcessors * 2;

	// thread handler ����
	hWorkerHandle = new HANDLE[nThreadCnt];
	// thread ����
	for (int i = 0; i < nThreadCnt; i++)
	{
		hWorkerHandle[i] = (HANDLE *)_beginthreadex(
			NULL, 0, &CallWorkerThread, this, CREATE_SUSPENDED, &threadId
		);
		if (hWorkerHandle[i] == NULL)
		{
			printf_s("[ERROR] Worker Thread ���� ����\n");
			return false;
		}
		ResumeThread(hWorkerHandle[i]);
	}
	printf_s("[INFO] Worker Thread ����...\n");
	return true;
}

void MainIocp::Send(scSOCKETINFO * pSocket)
{
	int nResult;
	DWORD	sendBytes;
	DWORD	dwFlags = 0;

	nResult = WSASend(
		pSocket->socket,
		&(pSocket->wsabuf),
		1,
		&sendBytes,
		dwFlags,
		NULL,
		NULL
	);

	if (nResult == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING)
	{
		printf_s("[ERROR] WSASend ���� : ", WSAGetLastError());
	}


}

void MainIocp::WorkerThread()
{
	// �Լ� ȣ�� ���� ����
	BOOL	bResult;
	int		nResult;
	// Overlapped I/O �۾����� ���۵� ������ ũ��
	DWORD	recvBytes;
	DWORD	sendBytes;
	// Completion Key�� ���� ������ ����
	scSOCKETINFO *	pCompletionKey;
	// I/O �۾��� ���� ��û�� Overlapped ����ü�� ���� ������	
	scSOCKETINFO *	pSocketInfo;
	DWORD	dwFlags = 0;


	while (bWorkerThread)
	{
		/**
		 * �� �Լ��� ���� ��������� WaitingThread Queue �� �����·� ���� ��
		 * �Ϸ�� Overlapped I/O �۾��� �߻��ϸ� IOCP Queue ���� �Ϸ�� �۾��� ������
		 * ��ó���� ��
		 */
		bResult = GetQueuedCompletionStatus(hIOCP,
			&recvBytes,				// ������ ���۵� ����Ʈ
			(PULONG_PTR)&pCompletionKey,	// completion key
			(LPOVERLAPPED *)&pSocketInfo,			// overlapped I/O ��ü
			INFINITE				// ����� �ð�
		);

		if (!bResult && recvBytes == 0)
		{
			printf_s("[INFO] socket(%d) ���� ����\n", pSocketInfo->socket);
			closesocket(pSocketInfo->socket);
			free(pSocketInfo);
			continue;
		}

		pSocketInfo->wsabuf.len = recvBytes;

		if (recvBytes == 0)
		{
			closesocket(pSocketInfo->socket);
			free(pSocketInfo);
			continue;
		}

		try
		{
			// ��Ŷ ����
			int PacketType;
			// Ŭ���̾�Ʈ ���� ������ȭ
			stringstream RecvStream;

			RecvStream << pSocketInfo->wsabuf.buf;
			RecvStream >> PacketType;

			// ��Ŷ ó��
			if (fnProcess[PacketType].funcProcessPacket != nullptr)
			{
				fnProcess[PacketType].funcProcessPacket(RecvStream, pSocketInfo);
			}
			else
			{
				printf_s("[ERROR] ���� ���� ���� ��Ŷ : %d\n", PacketType);
			}
		}
		catch (const std::exception& e)
		{
			printf_s("[ERROR] �� �� ���� ���� �߻� : %s\n", e.what());
		}

		// Ŭ���̾�Ʈ ���
		Recv(pSocketInfo);
	}
}

void MainIocp::Broadcast(stringstream & SendStream)
{
	scSOCKETINFO* client = new scSOCKETINFO;
	for (const auto& kvp : SessionSocket)
	{
		client->socket = kvp.second;
		CopyMemory(client->buf, (CHAR*)SendStream.str().c_str(), SendStream.str().length());
		client->wsabuf.buf = client->buf;
		client->wsabuf.len = SendStream.str().length();

		Send(client);
	}
}