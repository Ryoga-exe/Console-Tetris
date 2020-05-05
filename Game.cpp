#include "Game.h"
#include <math.h>

Game::Game() {
	m_scene = e_TITLE;
	Init();
}

bool Game::Update() {
	switch (m_scene) {
	case e_TITLE:
		if (Console::Instance()->GetKeyEvent() != KEY_NOT_INPUTED) {
			m_scene = e_GAME;
			m_gameTimer.Start();
			m_prevMinoDownTime = m_gameTimer.Elapse();
		}
		break;
	case e_GAME:
		MinoOpe();
		MinoDown();
		m_lockDown.Update();
		break;
	default:
		break;
	}

	return false;
}
void Game::Draw() {

	switch (m_scene) {
	case e_TITLE:
		DrawTitle();
		break;
	case e_GAME:
		DrawStage();
		DrawField();
		DrawGhostMino();
		DrawCurrentMino();
		DrawNextMinos();
		break;
	default:
		break;
	}
}
void Game::Init() {
	ClearField();
	SetBag();

	m_currentMino.minoType = m_bagArr[m_bagIndex];
	m_bagIndex++;
	for (int i = 0; i < 4; i++) {
		m_nextMinos[i].minoType = m_bagArr[m_bagIndex];
		m_bagIndex++;
	}

	m_currentMinoPos = { 3, -1 }; // Ç±Ç±
	m_score = 0;
	m_currentLevel = 1;
	m_currentDeletedLineNum = 0;
	m_prevMinoDownTime = 0;
	SpeedUpdate();
	m_lockDown.Init();

	m_topScore = 0; //~~~~~~~
}
void Game::ClearField() {
	for (size_t i = 0; i < FIELD_H; i++) for (size_t j = 0; j < FIELD_W; j++) m_field[j][i] = NONE;
}
void Game::SetBag() {
	for (byte i = 0; i < MINO_TYPE; i++) m_bagArr[i] = i;
	m_bagIndex = 0;
	// shuffle
	for (size_t i = MINO_TYPE - 1; i > 0; i--) {
		size_t r = GetRand(i + 1);
		byte tmp = m_bagArr[i];
		m_bagArr[i] = m_bagArr[r];
		m_bagArr[r] = tmp;
	}
}
void Game::SpeedUpdate() {
	m_speedWaitMs = (int)(pow((0.8 - ((m_currentLevel - 1) * 0.007)), (m_currentLevel - 1)) * 1000);
}
void Game::DrawTitle() {
	Console::Instance()->DrawBox(2, 8, 3, 1, BLOCK_COLOR[MN_Z]);
	Console::Instance()->DrawBox(3, 8, 1, 5, BLOCK_COLOR[MN_Z]);

	Console::Instance()->DrawBox(5, 8, 1, 5, BLOCK_COLOR[MN_L]);
	Console::Instance()->DrawBox(5, 8, 3, 1, BLOCK_COLOR[MN_L]);
	Console::Instance()->DrawBox(5, 10, 3, 1, BLOCK_COLOR[MN_L]);
	Console::Instance()->DrawBox(5, 12, 3, 1, BLOCK_COLOR[MN_L]);

	Console::Instance()->DrawBox(8, 8, 3, 1, BLOCK_COLOR[MN_O]);
	Console::Instance()->DrawBox(9, 8, 1, 5, BLOCK_COLOR[MN_O]);

	Console::Instance()->DrawBox(11, 8, 1, 5, BLOCK_COLOR[MN_S]);
	Console::Instance()->DrawPixel(12, 8, BLOCK_COLOR[MN_S]);
	Console::Instance()->DrawPixel(12, 10, BLOCK_COLOR[MN_S]);
	Console::Instance()->DrawPixel(13, 9, BLOCK_COLOR[MN_S]);
	Console::Instance()->DrawBox(13, 11, 1, 2, BLOCK_COLOR[MN_S]);

	Console::Instance()->DrawBox(14, 8, 3, 1, BLOCK_COLOR[MN_I]);
	Console::Instance()->DrawBox(14, 12, 3, 1, BLOCK_COLOR[MN_I]);
	Console::Instance()->DrawBox(15, 8, 1, 5, BLOCK_COLOR[MN_I]);

	Console::Instance()->DrawBox(17, 8, 3, 1, BLOCK_COLOR[MN_T]);
	Console::Instance()->DrawBox(17, 10, 3, 1, BLOCK_COLOR[MN_T]);
	Console::Instance()->DrawBox(17, 12, 3, 1, BLOCK_COLOR[MN_T]);
	Console::Instance()->DrawPixel(17, 9, BLOCK_COLOR[MN_T]);
	Console::Instance()->DrawPixel(19, 11, BLOCK_COLOR[MN_T]);


	Console::Instance()->Print(9, 16, GetColor((m_gameTimer.Elapse() & (1 << 9)) > 256 ? H_WHITE : H_BLACK, L_BLACK), "- PRESS ANY KEY TO START -");

}
void Game::DrawStage() {
	Console::Instance()->DrawBox(0, 0, 1, STAGE_H, BLOCK_COLOR[BLCK]);
	Console::Instance()->DrawBox(0, STAGE_H - 1, STAGE_W, 1, BLOCK_COLOR[BLCK]);
	Console::Instance()->DrawBox(0, 0, 1 + MINO_SIZE, 1, BLOCK_COLOR[BLCK]);
	Console::Instance()->DrawBox(0, 1 + MINO_SIZE, 1 + MINO_SIZE, 1, BLOCK_COLOR[BLCK]);
	Console::Instance()->DrawBox(1 + MINO_SIZE, 0, 1, STAGE_H, BLOCK_COLOR[BLCK]);
	Console::Instance()->DrawBox(2 + MINO_SIZE + FIELD_W, 0, 1, STAGE_H, BLOCK_COLOR[BLCK]);
	Console::Instance()->DrawBox(STAGE_W - 1, 0, 1, STAGE_H, BLOCK_COLOR[BLCK]);
	Console::Instance()->DrawBox(2 + MINO_SIZE + FIELD_W, 0, 5, 1, BLOCK_COLOR[BLCK]);
	Console::Instance()->DrawBox(2 + MINO_SIZE + FIELD_W, 5, 5, 1, BLOCK_COLOR[BLCK]);
	Console::Instance()->DrawBox(2 + MINO_SIZE + FIELD_W, 10, 5, 1, BLOCK_COLOR[BLCK]);
	Console::Instance()->DrawBox(2 + MINO_SIZE + FIELD_W, 15, 5, 1, BLOCK_COLOR[BLCK]);

	Console::Instance()->Print(3, 0, BLOCK_COLOR[NONE], " HOLD ");
	Console::Instance()->Print(35, 0, BLOCK_COLOR[NONE], " NEXT ");

	Console::Instance()->Print(2, 6, BLOCK_COLOR[NONE], "SCORE:");
	Console::Instance()->Printf(2, 7, BLOCK_COLOR[TEXT], "%8d", m_score);
	Console::Instance()->Print(2, 8, BLOCK_COLOR[NONE], "HI-SCORE");
	Console::Instance()->Printf(2, 9, BLOCK_COLOR[TEXT], "%8d", 100);
	Console::Instance()->Print(2, 11, BLOCK_COLOR[NONE], "LEVEL:");
	Console::Instance()->Printf(2, 12, BLOCK_COLOR[TEXT], "%8d", m_currentLevel);
	Console::Instance()->Print(2, 13, BLOCK_COLOR[NONE], "LINES:");
	Console::Instance()->Printf(2, 14, BLOCK_COLOR[TEXT], "%8d", m_currentDeletedLineNum);
	Console::Instance()->Print(2, 15, BLOCK_COLOR[NONE], "TIME:");
	if ((int)(m_gameTimer.Elapse() / 60000) < 60)
		Console::Instance()->Printf(2, 16, BLOCK_COLOR[TEXT], "%02d:%02d.%02d", (int)(m_gameTimer.Elapse() / 60000) % 60, (int)(m_gameTimer.Elapse() / 1000) % 60, (int)(m_gameTimer.Elapse() / 10) - (int)(m_gameTimer.Elapse() / 1000) * 100);
	else 
		Console::Instance()->Printf(2, 16, BLOCK_COLOR[TEXT], "%02d:%02d:%02d", (int)(m_gameTimer.Elapse() / 3600000) % 100, (int)(m_gameTimer.Elapse() / 60000) % 60, (int)(m_gameTimer.Elapse() / 1000) % 60);
	Console::Instance()->Print(2, 19, BLOCK_COLOR[NONE], "FPS:");
	Console::Instance()->Printf(6, 19, BLOCK_COLOR[TEXT], "%.1f", Console::Instance()->GetFPSRate());
}
void Game::DrawMino(COORD minoPos, MinoInfo_t minoInfo, bool isFix, bool isGhost) {
	SHORT fixVal = isFix ? 6 : 0;
	if (minoInfo.minoType >= MINO_TYPE || minoInfo.minoAngle >= MINO_ANGLE) return;
	for (int i = 0; i < MINO_SIZE; i++)
		for (int j = 0; j < MINO_SIZE; j++)
			if (minoShapes[minoInfo.minoType][minoInfo.minoAngle][i][j] != NONE &&
				((fixVal + minoPos.X + j >= 0 && fixVal + minoPos.X + j < STAGE_W) && (minoPos.Y + i  >= 0 && minoPos.Y + i < STAGE_H)))
				if (!isGhost) Console::Instance()->DrawPixel(fixVal + minoPos.X + j, minoPos.Y + i, BLOCK_COLOR[minoShapes[minoInfo.minoType][minoInfo.minoAngle][i][j]]);
				else Console::Instance()->Print((fixVal + minoPos.X + j) * 2, minoPos.Y + i, GetColor(GetBackgroundColor(BLOCK_COLOR[minoShapes[minoInfo.minoType][minoInfo.minoAngle][i][j]]), GetBackgroundColor(BLOCK_COLOR[NONE])), "Å†");
}
void Game::DrawField() {
	for (size_t i = FIELD_H - FIELD_H_SEEN; i < FIELD_H; i++)
		for (size_t j = 0; j < FIELD_W; j++)
			Console::Instance()->DrawPixel(6 + (SHORT)j, (SHORT)i - (FIELD_H - FIELD_H_SEEN), BLOCK_COLOR[m_field[j][i]]);
}
void Game::DrawCurrentMino() {
	DrawMino(m_currentMinoPos, m_currentMino);
}
void Game::DrawNextMinos() {
	for (SHORT i = 0; i < 4; i++)
		DrawMino({ 17, 1 + i * 5 }, m_nextMinos[i], false);
}
void Game::DrawGhostMino() {
	SHORT i = 0;
	for (i = m_currentMinoPos.Y; i < FIELD_H_SEEN && !IsHit({ m_currentMinoPos.X, i }, m_currentMino); i++);
	DrawMino({ m_currentMinoPos.X, i - 1 }, m_currentMino, true, true);
}
void Game::MinoOpe() {
	switch (Console::Instance()->GetKeyEvent()) {
	case KEY_INPUT_LEFT:	// MOVE LEFT
		if (!IsHit({ m_currentMinoPos.X - 1, m_currentMinoPos.Y }, m_currentMino)) {
			m_currentMinoPos.X--;
			if (m_lockDown()) m_lockDown++;
		}
		break;
	case KEY_INPUT_RIGHT:	// MOVE RIGHT
		if (!IsHit({ m_currentMinoPos.X + 1, m_currentMinoPos.Y }, m_currentMino)) {
			m_currentMinoPos.X++;
			if (m_lockDown()) m_lockDown++;
		}
		break;
	case KEY_INPUT_DOWN:	// SOFT DROP
		if (!m_lockDown()) m_prevMinoDownTime = -m_speedWaitMs;
		break;
	case ' ':				// HARD DROP
		for (; m_currentMinoPos.Y < FIELD_H_SEEN && !IsHit(m_currentMinoPos, m_currentMino); m_currentMinoPos.Y++);
		--m_currentMinoPos.Y;
		m_prevMinoDownTime = -m_speedWaitMs;
		break;
	case KEY_INPUT_UP:
		break;
	case 'X':
	case 'x':
		break;
	default:
		break;
	}
	
}
void Game::MinoDown() {
	int speedWaitMs = m_speedWaitMs;
	if (IsHit({ m_currentMinoPos.X, m_currentMinoPos.Y + 1 }, m_currentMino)) {
		if (!m_lockDown()) {
			m_lockDown.Set(m_currentMinoPos.Y);
		}
		if (m_lockDown()) {
			speedWaitMs = m_speedWaitMs > 500 ? m_speedWaitMs : 500;
			if (m_lockDown.hasChanged()) m_prevMinoDownTime = m_gameTimer.Elapse();
			if (m_lockDown.isOverStep()) speedWaitMs = 0;
			m_lockDown.UpdateMaxY(m_currentMinoPos.Y);
		}
	}

	if (speedWaitMs + m_prevMinoDownTime <= (signed)m_gameTimer.Elapse()) {
		m_prevMinoDownTime = m_gameTimer.Elapse();
		if (!IsHit({ m_currentMinoPos.X, m_currentMinoPos.Y + 1 }, m_currentMino)) m_currentMinoPos.Y++;
		else {
			m_currentMinoPos = { 3, -1 }; // Replace 
			m_lockDown.Init();
		}
	}
}
bool Game::IsHit(COORD minoPos, MinoInfo_t minoInfo) {
	if (minoInfo.minoType >= MINO_TYPE || minoInfo.minoAngle >= MINO_ANGLE) return true;
	for (int i = 0; i < MINO_SIZE; i++)
		for (int j = 0; j < MINO_SIZE; j++)
			if (minoShapes[minoInfo.minoType][minoInfo.minoAngle][i][j] != NONE)
				if ((minoPos.X + j >= 0 && minoPos.X + j < FIELD_W) && (minoPos.Y + i >= 0 - (FIELD_H - FIELD_H_SEEN) && minoPos.Y + i < FIELD_H_SEEN))
					if (m_field[minoPos.X + j][minoPos.Y + i + (FIELD_H - FIELD_H_SEEN)] != NONE) return true;
					else continue;
				else return true;
	return false;
}