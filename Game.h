#pragma once
#include "Console.h"
#define STAGE_W	22
#define STAGE_H	21
#define FIELD_W	10
#define FIELD_H	25
#define FIELD_H_SEEN	20

#define MINO_SIZE		4
#define MINO_TYPE		7
#define MINO_ANGLE		4

enum Scenes {
	e_TITLE,
	e_GAME
};

typedef struct {
	int minoType = 0, minoAngle = 0;
}MinoInfo_t;

struct LockDown_t{
	void  Init() {
		m_isLockDown = false;
		m_count = m_prevCount = 0;
		m_maxY = 0;
	}
	void Set(SHORT maxY) {
		m_isLockDown = true;
		m_count = m_prevCount = 0;
		m_maxY = maxY;
	}
	void Update() {
		m_prevCount = m_count;
	}
	bool hasChanged() {
		return m_count != m_prevCount;
	}
	bool isOverStep() {
		return m_count > 14;
	}
	bool UpdateMaxY(int y){
		bool ret = m_maxY < y;
		if (ret) Init();
		return ret;
	}

	bool operator()() {
		return m_isLockDown;
	}
	LockDown_t operator++() {
		++m_count;
		return *this;
	}
	const LockDown_t operator++(int) {
		LockDown_t tmp = *this;
		++(*this);
		return tmp;
	}

private:
	int   m_count, m_prevCount;
	SHORT m_maxY;
	bool  m_isLockDown;
};

class Game {
public:
	Game();
	~Game() {

	}

	bool Update();
	void Draw();
private:
	void Init();
	void ClearField();
	void SetBag();
	void SpeedUpdate();
	void DrawTitle();
	void DrawStage();
	void DrawMino(COORD minoPos, MinoInfo_t minoInfo, bool isFix = true, bool isGhost = false);
	void DrawField();
	void DrawCurrentMino();
	void DrawNextMinos();
	void DrawGhostMino();
	void MinoOpe();
	void MinoDown();
	bool IsHit(COORD minoPos, MinoInfo_t minoInfo);
	void FixMino();

private:
	enum Blocks {
		NONE,
		BLCK,
		TEXT,
		MN_I,
		MN_O,
		MN_S,
		MN_Z,
		MN_J,
		MN_L,
		MN_T,
		BLOCK_NUM
	};

	const WORD BLOCK_COLOR[BLOCK_NUM] = {
		GetColor(H_WHITE, L_BLACK),		// NONE
		GetColor(L_BLACK, H_WHITE),		// BLOCK
		GetColor(H_YELLOW, L_BLACK),	// TEXT
		GetColor(H_CYAN, H_CYAN ),		// I
		GetColor(H_YELLOW, H_YELLOW),	// O
		GetColor(H_GREEN, H_GREEN),		// S
		GetColor(L_RED, L_RED),			// Z
		GetColor(L_BLUE, L_BLUE),		// J
		GetColor(L_YELLOW, L_YELLOW),	// L
		GetColor(L_PURPLE, L_PURPLE)	// T
	};

	Scenes m_scene = e_TITLE;
	Timer  m_gameTimer;
	
	Blocks m_field[FIELD_W][FIELD_H];
	Blocks minoShapes[MINO_TYPE][MINO_ANGLE][MINO_SIZE][MINO_SIZE] {
		{		// I
			{	NONE,NONE,NONE,NONE,
				MN_I,MN_I,MN_I,MN_I,
				NONE,NONE,NONE,NONE,
				NONE,NONE,NONE,NONE
			},
			{	NONE,NONE,MN_I,NONE,
				NONE,NONE,MN_I,NONE,
				NONE,NONE,MN_I,NONE,
				NONE,NONE,MN_I,NONE
			},
			{	NONE,NONE,NONE,NONE,
				NONE,NONE,NONE,NONE,
				MN_I,MN_I,MN_I,MN_I,
				NONE,NONE,NONE,NONE
			},
			{	NONE,MN_I,NONE,NONE,
				NONE,MN_I,NONE,NONE,
				NONE,MN_I,NONE,NONE,
				NONE,MN_I,NONE,NONE
			},
		},{		// O
			{	NONE,NONE,NONE,NONE,
				NONE,MN_O,MN_O,NONE,
				NONE,MN_O,MN_O,NONE,
				NONE,NONE,NONE,NONE
			},
			{	NONE,NONE,NONE,NONE,
				NONE,MN_O,MN_O,NONE,
				NONE,MN_O,MN_O,NONE,
				NONE,NONE,NONE,NONE
			},
			{	NONE,NONE,NONE,NONE,
				NONE,MN_O,MN_O,NONE,
				NONE,MN_O,MN_O,NONE,
				NONE,NONE,NONE,NONE
			},
			{	NONE,NONE,NONE,NONE,
				NONE,MN_O,MN_O,NONE,
				NONE,MN_O,MN_O,NONE,
				NONE,NONE,NONE,NONE
			},
		},{		// S
			{	NONE,NONE,NONE,NONE,
				NONE,MN_S,MN_S,NONE,
				MN_S,MN_S,NONE,NONE,
				NONE,NONE,NONE,NONE
			},
			{	NONE,NONE,NONE,NONE,
				NONE,MN_S,NONE,NONE,
				NONE,MN_S,MN_S,NONE,
				NONE,NONE,MN_S,NONE
			},
			{	NONE,NONE,NONE,NONE,
				NONE,NONE,NONE,NONE,
				NONE,MN_S,MN_S,NONE,
				MN_S,MN_S,NONE,NONE
			},
			{	NONE,NONE,NONE,NONE,
				MN_S,NONE,NONE,NONE,
				MN_S,MN_S,NONE,NONE,
				NONE,MN_S,NONE,NONE
			},
		},{		// Z
			{	NONE,NONE,NONE,NONE,
				MN_Z,MN_Z,NONE,NONE,
				NONE,MN_Z,MN_Z,NONE,
				NONE,NONE,NONE,NONE
			},
			{	NONE,NONE,NONE,NONE,
				NONE,NONE,MN_Z,NONE,
				NONE,MN_Z,MN_Z,NONE,
				NONE,MN_Z,NONE,NONE
			},
			{	NONE,NONE,NONE,NONE,
				NONE,NONE,NONE,NONE,
				MN_Z,MN_Z,NONE,NONE,
				NONE,MN_Z,MN_Z,NONE
			},
			{	NONE,NONE,NONE,NONE,
				NONE,MN_Z,NONE,NONE,
				MN_Z,MN_Z,NONE,NONE,
				MN_Z,NONE,NONE,NONE
			},
		},{		// L
			{	NONE,NONE,NONE,NONE,
				NONE,NONE,MN_L,NONE,
				MN_L,MN_L,MN_L,NONE,
				NONE,NONE,NONE,NONE
			},
			{	NONE,NONE,NONE,NONE,
				NONE,MN_L,NONE,NONE,
				NONE,MN_L,NONE,NONE,
				NONE,MN_L,MN_L,NONE
			},
			{	NONE,NONE,NONE,NONE,
				NONE,NONE,NONE,NONE,
				MN_L,MN_L,MN_L,NONE,
				MN_L,NONE,NONE,NONE
			},
			{	NONE,NONE,NONE,NONE,
				MN_L,MN_L,NONE,NONE,
				NONE,MN_L,NONE,NONE,
				NONE,MN_L,NONE,NONE
			},
		},{		// J
			{	NONE,NONE,NONE,NONE,
				MN_J,NONE,NONE,NONE,
				MN_J,MN_J,MN_J,NONE,
				NONE,NONE,NONE,NONE
			},
			{	NONE,NONE,NONE,NONE,
				NONE,MN_J,MN_J,NONE,
				NONE,MN_J,NONE,NONE,
				NONE,MN_J,NONE,NONE
			},
			{	NONE,NONE,NONE,NONE,
				NONE,NONE,NONE,NONE,
				MN_J,MN_J,MN_J,NONE,
				NONE,NONE,MN_J,NONE
			},
			{	NONE,NONE,NONE,NONE,
				NONE,MN_J,NONE,NONE,
				NONE,MN_J,NONE,NONE,
				MN_J,MN_J,NONE,NONE
			},
		},{		// T
			{	NONE,NONE,NONE,NONE,
				NONE,MN_T,NONE,NONE,
				MN_T,MN_T,MN_T,NONE,
				NONE,NONE,NONE,NONE
			},
			{	NONE,NONE,NONE,NONE,
				NONE,MN_T,NONE,NONE,
				NONE,MN_T,MN_T,NONE,
				NONE,MN_T,NONE,NONE
			},
			{	NONE,NONE,NONE,NONE,
				NONE,NONE,NONE,NONE,
				MN_T,MN_T,MN_T,NONE,
				NONE,MN_T,NONE,NONE
			},
			{	NONE,NONE,NONE,NONE,
				NONE,MN_T,NONE,NONE,
				MN_T,MN_T,NONE,NONE,
				NONE,MN_T,NONE,NONE
			},
		}
	};

	MinoInfo_t m_currentMino, m_holdMino, m_nextMinos[4];
	COORD m_currentMinoPos = { 0, 0 };


	byte m_bagArr[MINO_TYPE];
	size_t m_bagIndex;
	int m_score, m_topScore, m_currentLevel, m_currentDeletedLineNum;
	int m_speedWaitMs;
	LONGLONG m_prevMinoDownTime;

	LockDown_t m_lockDown;
	
};