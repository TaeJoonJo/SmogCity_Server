
#include "IOCPModule.h"

CIOCPModule::CIOCPModule()
{
	m_CompletionPort = nullptr;
}

CIOCPModule::~CIOCPModule()
{
	CloseHandle(m_CompletionPort);
	m_CompletionPort = INVALID_HANDLE_VALUE;
}

// iocp ����
const bool CIOCPModule::StartIOCP()
{
	m_CompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);		// ������ ���ڿ� 0�� ������ �ڵ����� cpu������ ���� ������ ����

	if (m_CompletionPort == nullptr) {
		std::cout << "[CreateIoCompletionPort() Error!] in StartIOCP\n";
		return false;
	}

	return true;
}

// accept�� Ŭ���̾�Ʈ�� iocp�� ����
const bool CIOCPModule::RegisterClient(SOCKET clientSocket, CSessionData* pplayer)
{
	// pplayer�� �Ѱ��ش�. ���̵� �Ѱ��ִ°� ������ �����غ���
	 HANDLE hresult = CreateIoCompletionPort(reinterpret_cast<HANDLE>(clientSocket), m_CompletionPort, reinterpret_cast<ULONG_PTR>(pplayer), 0);

	if (hresult == INVALID_HANDLE_VALUE) {
		std::cout << "[CreateIoCompletionPort() Error!] in RegisterClient\n";
		return false;
	}

	return true;
}