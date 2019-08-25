#pragma once

#ifndef __TIMER_H__
#define __TIMER_H__

#include "Common.h"
#include "Protocol.h"
#include "SessionData.h"
#include "Lock.h"

#define NOW GetTickCount64() 

constexpr UINT ONE_SECOND					= 1000;
constexpr UINT TIMER_UPDATE_GAME			= 100;
constexpr UINT TIMER_RESPAWN_PLAYER			= 5000;
constexpr UINT TIMER_RESPAWN_HEALPACK		= 10000;
constexpr UINT TIMER_ROUND_START			= 3000;
constexpr UINT TIMER_SWAP_WEAPON			= 500;
constexpr UINT TIMER_RELOAD_MACHINEGUN		= 1000;
constexpr UINT TIMER_RELOAD_ROCKETGUN		= 2000;
constexpr FLOAT REALTIME_UPDATE_CYCLE		= 0.1f;

// e_CallType�� ���ļ�����
//enum e_Timer_Type {
//	e_Timer_Start,
//	e_Timer_Update
//};

typedef struct stTimerEvent {
	ULONGLONG		s_time;
	e_Callbacktype	type;
	CSessionData*	session;
	WORD			targetID;
	WORD			subtargetID;
} TimerEvent, TIMEREVENT;

struct TimeComparison
{
	//bool reverse = false;

	bool operator() (const TimerEvent& lhs, const TimerEvent& rhs) const
	{
		return (lhs.s_time > rhs.s_time);
	}
};

typedef std::priority_queue <TimerEvent, std::vector<TimerEvent>, TimeComparison> TimerQueue;

// Ÿ�̸� Ŭ���������� �ð������� �̺�Ʈ������ ���ְ� ó���� ��Ŀ�����忡�� ����
class CTimer
{
public:
	CTimer();
	~CTimer();
private:
	HANDLE			m_hIocp;
	// Ÿ�̸��̺�Ʈ���� �����ϴ� �켱���� ť
	TimerQueue		m_TimerQueue;
	CMutex			m_Lock;
public:
	const bool		Initialize(HANDLE hiocp);

	// �켱����ť�� �ִ� �Լ�
	const bool		AddTimer(ULONGLONG s_time, e_Callbacktype etimerType, CSessionData* psession, WORD wtargetID = NOTREGISTED, WORD wsubtargetID = NOTREGISTED);
	VOID			ProcessTimer();
};

#endif // !__TIMER_H__
