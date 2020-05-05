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
}MinoInfo;

class Game {
public:
	Game();
	~Game() {

	}

	bool Update();
	void Draw();
private:
	void ClearField();
	void SetBag();
	void DrawTitle();
	void DrawStage();
	void DrawMino(COORD minoPos, MinoInfo minoInfo, bool isFix = true, bool isGhost = false);
	void DrawField();
	void DrawCurrentMino();
	void DrawNextMinos();
	void DrawGhostMino();
	void MinoOpe();
	bool IsHit(COORD minoPos, MinoInfo minoInfo);

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


	Scenes m_scene = e_TITLE;
	Timer  m_gameTimer;

	const WORD BLOCK_COLOR[BLOCK_NUM] = {
		GetColor(H_WHITE, L_BLACK),		// NONE
		GetColor(L_BLACK, H_WHITE),		// BLOCK
		GetColor(H_YELLOW, L_BLACK),	// TEXT
		GetColor(H_CYAN, H_CYAN ),		// I
		GetColor(H_YELLOW, H_YELLOW),	// O
		GetColor(H_GREEN, H_GREEN),		// S
		GetColor(L_RED, L_RED),		// Z
		GetColor(L_BLUE, L_BLUE),		// J
		GetColor(L_YELLOW, L_YELLOW),	// L
		GetColor(L_PURPLE, L_PURPLE)		// T
	};
	
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

	MinoInfo m_currentMino, m_holdMino, m_nextMinos[4];
	COORD m_currentMinoPos = { 0, 0 };


	byte m_bagArr[MINO_TYPE];
	size_t m_bagIndex;
	
};