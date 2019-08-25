#pragma once

#ifndef __STRUCT_H__
#define __STRUCT_H__

//#include "Protocol.h"
enum e_OBJECT_TYPE;

// ���ӿ� �ʿ��� ���� ���� ������
typedef struct stGameInfo {
	float m_fElsapedTime;

	// �� ����
	int m_nRound;

	// �� �� ���� �̰����
	int m_nUpWinCount;
	int m_nDownWinCount;

	// Main Base Info
	int m_nUpPlayerCount;
	int m_nDownPlayerCount;
	//bool m_bIsOccupied = false;
	float m_fUpTeamPercent;
	bool m_bIsMBUpOccupying = false;
	bool m_bIsMBUpOccupied = false;
	float m_fMBUpTime;
	float m_fDownTeamPercent;
	bool m_bIsMBDownOccupying = false;
	bool m_bIsMBDownOccupied = false;
	float m_fMBDownTime;
	bool m_bIsEngaging = false;

	bool m_IsSmog[SMOG_MAX_X_INDEX][SMOG_MAX_Y_INDEX][SMOG_MAX_Z_INDEX];
} GameInfo, GAMEINFO;

typedef struct stCharacterInfo {
	int		m_nGameAmount;
	int		m_nGameVictory;
	int		m_nGameDefeat;
}CharacterInfo, CHARACTERINFO;

// ���ӿ� �ʿ��� �÷��̾� ������
typedef struct stGamePlayerInfo {
	// ���Ǻ� ���̵� ����
	WORD				m_wID = NOTREGISTED;
	// ü�� ����
	FLOAT				m_fLife;
	// ��ġ ����
	FLOAT				m_fX = 0.f;
	FLOAT				m_fY = 0.f;
	FLOAT				m_fZ = 0.f;
	// ȸ�� ����
	FLOAT				m_fYaw = 0.f;
	FLOAT				m_fPitch = 0.f;
	FLOAT				m_fRoll = 0.f;
	// �ӵ� ����
	FLOAT				m_fVX = 0.f;
	FLOAT				m_fVY = 0.f;
	FLOAT				m_fVZ = 0.f;
	// �� ����
	bool				m_bIsDown = true;
	// ���� ����
	bool				m_bIsLive = true;
	// �ִϸ��̼��� ���� ����
	bool				m_bIsUp;
	bool				m_bIsFly;
	bool				m_bIsShoot;
	bool				m_bIsReloading;
	bool				m_bIsGround;
	bool				m_bIsRun;
	bool				m_bIsFalling;
	bool				m_bIsChangingWeapon;
	float				m_fSpeed;
	float				m_fDirection;
	float				m_fRotatorYaw;
	float				m_fRotatorPitch;
	float				m_fRotatorRoll;

	bool				m_bIsInMainBase;

	BYTE				m_cInGameID;

	BYTE				m_cWeaponNum;

	BYTE				m_cMachinegunAmmo;
	BYTE				m_cRocketgunAmmo;
} GamePlayerInfo, GAMEPLAYERINFO;
typedef std::map<WORD, GamePlayerInfo> mapPlayersInfo;

// ���ӿ� �ʿ��� ������Ʈ ������
typedef struct stGameObjectInfo {
	WORD				m_wID = NOTREGISTED;
	// ���� ��������
	WORD				m_wWhoID = NOTREGISTED;
	// �� ����
	bool				m_bIsDown = true;
	// Ÿ�� ����
	e_OBJECT_TYPE		m_eType = e_OBJECT_TYPE::e_Rocketgun_Bullet;
	// ��ġ ����
	FLOAT				m_fX = 0.f;
	FLOAT				m_fY = 0.f;
	FLOAT				m_fZ = 0.f;
	// ȸ�� ����
	FLOAT				m_fYaw = 0.f;
	FLOAT				m_fPitch = 0.f;
	FLOAT				m_fRoll = 0.f;
	// �ӵ� ����
	FLOAT				m_fVX = 0.f;
	FLOAT				m_fVY = 0.f;
	FLOAT				m_fVZ = 0.f;
} GameObjectInfo, GAMEOBJECTINFO;

typedef std::map<WORD, GameObjectInfo> mapObjectsInfo;

#endif // !__STRUCT_H__