
#pragma once

#ifndef __THREAD_H__
#define __THREAD_H__

#include "Common.h"
#include <vector>

constexpr int THREAD_WAITTIME = 100;		// ������ ����� ���ð�

class CThread
{
public :
	CThread();
	~CThread();
protected :	// �������� �۵����� Ȯ���� ���� volatile���� ���°��� ������ ����.
	volatile bool	m_bRun;
private :
	std::thread		m_Thread;						// Handle�� C++�� Thread�� ��ü����
	UINT			m_nThreadID;					// ID �ʿ��Ѱ�?
public:
	HANDLE			m_hStopEvent;
public :
	virtual const UINT		Run() = 0;
    const bool				StartThread();
	const bool				StopThread();
	const bool				JoinThread();
	inline const bool		IsRun()	{
		return m_bRun;	
	}
	inline const UINT		GetThreadID() {
		return m_nThreadID;	
	}
private :	// ������ Ŭ�������� �ڽ��� �����Լ��� ������ ����
	static void ThreadFunc(LPVOID lpparam);
};

// �Լ������� ������
typedef const UINT (*LPTHREADCALLBACK)(LPVOID lpparam);

class CSingleThread	: public CThread
{
private :
	LPVOID				m_lpParam{};
	LPTHREADCALLBACK	m_lpFunc{};
public :
	// ������ ������ �Լ������͸� �����Ѵ�. lpparam ���� ServerManager�� ������, lpfunc���� �Լ�������
	inline const VOID Initialize(const LPVOID lpparam, const LPTHREADCALLBACK lpfunc)
	{
   		m_lpParam	= lpparam;
		m_lpFunc	= lpfunc;
	}
	// ������ �Լ� ����
	inline const UINT Run()	{
		return (*m_lpFunc)(m_lpParam);
	}
};

typedef std::vector<CSingleThread*> VECSINGLETHREAD;

// ���� Ÿ���� �����带 ������ �����ϱ� ���� Ŭ����
class CMultiThread
{
public :
	CMultiThread();
	~CMultiThread();
private :
	VECSINGLETHREAD		m_vecThread;
	volatile BOOL		m_bRun;
public :
	VOID Initialize(const int nsize, const LPVOID lpparam, const LPTHREADCALLBACK lpfunc);
	const bool StartMultiThread();
	const bool StopMultiThread();
	const bool JoinMultiThread();
	inline const bool IsRun() {
		return m_bRun;	
	}
};

#endif	// __THREAD_H__