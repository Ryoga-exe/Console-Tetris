#include "Game.h"
#include <math.h>

Game::Game() {
	m_scene = e_TITLE;
	Init();
}

bool Game::Update() {
	m_addtionalScore = 0;
	switch (m_scene) {
	case e_TITLE:
		if (Console::Instance()->GetKeyEvent() != KEY_NOT_INPUTED) {
			StartGame();
		}
		break;
	case e_GAME:
		if (!m_isDeleting) {
			MinoOpe();
			if (MinoDown()) StartGameOver();
			m_lockDown.Update();
		}
		else {
			if (DeleteLine() == -1) StartGameOver();
		}
		break;
	case e_GAMEOVER:
		GameOverUpdate();
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
		if (!m_isDeleting) {
			DrawGhostMino();
			DrawCurrentMino();
		}
		DrawNextMinos();
		DrawHoldMino();
		break;
	case e_GAMEOVER:
		DrawStage();
		DrawField();
		GameOverDraw();
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
		m_nextMinos[i].minoAngle = 0;
		m_bagIndex++;
	}
	m_holdMino = { MINO_TYPE, 0 };
	m_hasHeld = m_isDeleting = m_isBack2Back = false;

	InitMinoPos();

	m_score = 0;
	m_currentLevel = 1;
	m_currentDeletedLineNum = 0;
	m_prevMinoDownTime = m_del = 0;
	SpeedUpdate();
	m_lockDown.Init();
	m_actionNotification = SINGLE;
	m_timeActionNotification = -1000;
	m_addtionalScore = 0;
	m_topScore = 0; //~~~~~~~
}
void Game::StartGame() {
	m_scene = e_GAME;
	m_gameTimer.Start();
	m_prevMinoDownTime = m_gameTimer.Elapse();
}
void Game::ClearField() {
	for (size_t i = 0; i < FIELD_H; i++) for (size_t j = 0; j < FIELD_W; j++) m_field[j][i] = NONE;
}
void Game::SetBag() {
	for (int i = 0; i < MINO_TYPE; i++) m_bagArr[i] = i;
	m_bagIndex = 0;
	// shuffle
	for (size_t i = MINO_TYPE - 1; i > 0; i--) {
		size_t r = GetRand(i);
		byte tmp = m_bagArr[i];
		m_bagArr[i] = m_bagArr[r];
		m_bagArr[r] = tmp;
	}
}
bool Game::InitMinoPos() {
	m_currentMinoPos.X = 3;
	bool ret = true;
	for (SHORT i = m_currentMinoPos.Y = m_currentMino.minoType == 0 ? 0 : -1; i > m_currentMinoPos.Y - 4; i--) {
		if (!IsHit({ m_currentMinoPos.X , i}, m_currentMino)) {
			m_currentMinoPos.Y = i;
			ret = false;
			break;
		}
	}
	return ret;
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

	Console::Instance()->Print(34, 20, GetColor(H_BLACK, L_BLACK), "Ryoga.exe");

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

	Console::Instance()->Print(2, 7, BLOCK_COLOR[NONE], "SCORE:");
	Console::Instance()->Printf(2, 8, BLOCK_COLOR[TEXT], "%8d", m_score);
	Console::Instance()->Print(2, 9, BLOCK_COLOR[NONE], "HI-SCORE");
	Console::Instance()->Printf(2,10, BLOCK_COLOR[TEXT], "%8d", 100);
	Console::Instance()->Print(2, 12, BLOCK_COLOR[NONE], "LEVEL:");
	Console::Instance()->Printf(2, 13, BLOCK_COLOR[TEXT], "%8d", m_currentLevel);
	Console::Instance()->Print(2, 14, BLOCK_COLOR[NONE], "LINES:");
	Console::Instance()->Printf(2, 15, BLOCK_COLOR[TEXT], "%8d", m_currentDeletedLineNum);
	Console::Instance()->Print(2, 16, BLOCK_COLOR[NONE], "TIME:");
	LONGLONG t = m_scene == e_GAME ? m_gameTimer.Elapse() : m_timeActionNotification;
	if ((int)(m_gameTimer.Elapse() / 60000) < 60)
		Console::Instance()->Printf(2, 17, BLOCK_COLOR[TEXT], "%02d:%02d.%02d", (int)(t / 60000) % 60, (int)(t / 1000) % 60, (int)(t / 10) - (int)(t / 1000) * 100);
	else 
		Console::Instance()->Printf(2, 16, BLOCK_COLOR[TEXT], "%02d:%02d:%02d", (int)(t / 3600000) % 100, (int)(t / 60000) % 60, (int)(t / 1000) % 60);
	Console::Instance()->Print(2, 19, BLOCK_COLOR[NONE], "FPS:");
	Console::Instance()->Printf(6, 19, BLOCK_COLOR[TEXT], "%.1f", Console::Instance()->GetFPSRate());

	if (m_timeActionNotification + 1000 > (signed)m_gameTimer.Elapse() && m_scene == e_GAME) {
		Console::Instance()->Print((12 - (SHORT)strlen(ACTION_NOTIFICATIONS[m_actionNotification])) / 2, 5, GetColor(H_YELLOW, L_BLUE), ACTION_NOTIFICATIONS[m_actionNotification]);
		if (m_isBack2Back)
			Console::Instance()->Print(0, 6, GetColor(H_YELLOW, L_BLUE), "Back to Back");
	}
	if (m_addtionalScore > 0)
		Console::Instance()->Printf(2, 6, BLOCK_COLOR[TEXT], "%+8d", m_addtionalScore);
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
void Game::DrawHoldMino() {
	DrawMino({ 1,1 }, m_holdMino, false);
}
void Game::DrawGhostMino() {
	SHORT i = m_currentMinoPos.Y;
	for (; i < FIELD_H_SEEN && !IsHit({ m_currentMinoPos.X, i + 1 }, m_currentMino); i++);
	DrawMino({ m_currentMinoPos.X, i }, m_currentMino, true, true);
}
void Game::MinoOpe() {
	switch (Console::Instance()->GetKeyEvent()) {
	case KEY_INPUT_LEFT:	// MOVE LEFT
		MinoMoveX(-1);
		break;
	case KEY_INPUT_RIGHT:	// MOVE RIGHT
		MinoMoveX(+1);
		break;
	case KEY_INPUT_DOWN:	// SOFT DROP
		if (!m_lockDown()) {
			if (!IsHit({ m_currentMinoPos.X, m_currentMinoPos.Y + 1 }, m_currentMino)) m_prevMinoDownTime = -m_speedWaitMs;
			m_score++;
		}
		
		break;
	case ' ':				// HARD DROP
		for (; m_currentMinoPos.Y < FIELD_H_SEEN && !IsHit({ m_currentMinoPos.X, m_currentMinoPos.Y + 1 }, m_currentMino); m_currentMinoPos.Y++, m_score+=2);
		m_prevMinoDownTime = -m_speedWaitMs;
		break;
	case KEY_INPUT_UP:		// ROTATE CLOCKWISE
	case 'X':
	case 'x':
		MinoRotate(true);
		break;
	case 'Z':				// ROTATE COUNTER-CLOCKWISE
	case 'z':
		MinoRotate(false);
		break;
	case 'C':
	case 'c':
		HoldChange();
	default:
		break;
	}
	
}
bool Game::MinoDown() {
	if (m_currentLevel > 19)
		for (; m_currentMinoPos.Y < FIELD_H_SEEN && !IsHit({ m_currentMinoPos.X, m_currentMinoPos.Y + 1 }, m_currentMino); m_currentMinoPos.Y++);

	int speedWaitMs = m_speedWaitMs;
	if (IsHit({ m_currentMinoPos.X, m_currentMinoPos.Y + 1 }, m_currentMino)) {
		if (!m_lockDown()) {
			m_lockDown.Set(m_currentMinoPos.Y);
		}
		if (m_lockDown()) {
			speedWaitMs = 500;// m_speedWaitMs > 500 ? m_speedWaitMs : 500;
			if (m_lockDown.hasChanged()) m_prevMinoDownTime = m_gameTimer.Elapse();
			if (m_lockDown.isOverStep()) speedWaitMs = 0;
		}
	}
	if (m_lockDown()) {
		m_lockDown.UpdateMaxY(m_currentMinoPos.Y);
	}

	if (speedWaitMs + m_prevMinoDownTime <= (signed)m_gameTimer.Elapse()) {
		m_prevMinoDownTime = m_gameTimer.Elapse();
		if (!IsHit({ m_currentMinoPos.X, m_currentMinoPos.Y + 1 }, m_currentMino)) m_currentMinoPos.Y++;
		else {
			FixMino();
			if (!DeleteLine()) {
				MinoUpdate();
				if (InitMinoPos()) {
					// GameOver
					return true;
				}
			}
			m_lockDown.Init();
			m_hasHeld = false;
		}
	}
	return false;
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
void Game::FixMino() {
	for (int i = 0; i < MINO_SIZE; i++)
		for (int j = 0; j < MINO_SIZE; j++)
			if (minoShapes[m_currentMino.minoType][m_currentMino.minoAngle][i][j] != NONE)
				m_field[m_currentMinoPos.X + j][m_currentMinoPos.Y + i + (FIELD_H - FIELD_H_SEEN)] = minoShapes[m_currentMino.minoType][m_currentMino.minoAngle][i][j];
}
void Game::MinoUpdate() {
	m_currentMino = m_nextMinos[0];
	for (size_t i = 0; i < 4 - 1; i++) {
		m_nextMinos[i] = m_nextMinos[i + 1];
	}
	if (m_bagIndex >= MINO_TYPE) {
		SetBag();
	}
	m_nextMinos[4 - 1].minoType = m_bagArr[m_bagIndex];
	m_bagIndex++;
}
bool Game::MinoMoveX(SHORT x) {
	if (!IsHit({ m_currentMinoPos.X + x, m_currentMinoPos.Y }, m_currentMino)) {
		m_currentMinoPos.X += x;
		if (m_lockDown()) m_lockDown++;
		if (IsHit({ m_currentMinoPos.X, m_currentMinoPos.Y + 1 }, m_currentMino)) m_prevMinoDownTime = m_gameTimer.Elapse();
		return false;
	}
	return true;
}
bool Game::MinoRotate(bool isClockWise) {
	if (m_currentMino.minoType == 1) return false;

	MinoInfo_t buf = m_currentMino;
	buf.minoAngle = isClockWise ? (buf.minoAngle + 1) % MINO_ANGLE : (buf.minoAngle + MINO_ANGLE - 1) % MINO_ANGLE;
	if (!IsHit(m_currentMinoPos, buf)) {
		m_currentMino = buf;
		if (m_lockDown()) m_lockDown++;
		return false;
	}
	else {
		COORD bufPos = m_currentMinoPos;
		if (m_currentMino.minoType == 0) {
			if (isClockWise) {
				if (m_currentMino.minoAngle == 0) {
					bufPos.X -= 2;
					if (IsHit(bufPos, buf)) {
						bufPos.X = m_currentMinoPos.X + 1;
						if (IsHit(bufPos, buf)) {
							bufPos.X = m_currentMinoPos.X - 2;
							bufPos.Y += 1;
							if (IsHit(bufPos, buf)) {
								bufPos.X = m_currentMinoPos.X + 1;
								bufPos.Y = m_currentMinoPos.Y - 2;
								if (IsHit(bufPos, buf)) {
									return true;
								}
							}
						}
					}
					m_currentMino = buf;
					m_currentMinoPos = bufPos;
					if (m_lockDown()) m_lockDown++;
					return false;
				}
				else if (m_currentMino.minoAngle == 1) {
					bufPos.X -= 1;
					if (IsHit(bufPos, buf)) {
						bufPos.X = m_currentMinoPos.X + 2;
						if (IsHit(bufPos, buf)) {
							bufPos.X = m_currentMinoPos.X - 1;
							bufPos.Y = m_currentMinoPos.Y - 2;
							if (IsHit(bufPos, buf)) {
								bufPos.X = m_currentMinoPos.X + 2;
								bufPos.Y = m_currentMinoPos.Y + 1;
								if (IsHit(bufPos, buf)) {
									return true;
								}
							}
						}
					}
					m_currentMino = buf;
					m_currentMinoPos = bufPos;
					if (m_lockDown()) m_lockDown++;
					return false;
				}
				else if (m_currentMino.minoAngle == 2) {
					bufPos.X += 2;
					if (IsHit(bufPos, buf)) {
						bufPos.X = m_currentMinoPos.X - 1;
						if (IsHit(bufPos, buf)) {
							bufPos.X = m_currentMinoPos.X + 2;
							bufPos.Y = m_currentMinoPos.Y - 1;
							if (IsHit(bufPos, buf)) {
								bufPos.X = m_currentMinoPos.X - 1;
								bufPos.Y = m_currentMinoPos.Y + 2;
								if (IsHit(bufPos, buf)) {
									return true;
								}
							}
						}
					}
					m_currentMino = buf;
					m_currentMinoPos = bufPos;
					if (m_lockDown()) m_lockDown++;
					return false;
				}
				else if (m_currentMino.minoAngle == 3) {
					bufPos.X -= 2;
					if (IsHit(bufPos, buf)) {
						bufPos.X = m_currentMinoPos.X + 1;
						if (IsHit(bufPos, buf)) {
							bufPos.X = m_currentMinoPos.X + 1;
							bufPos.Y = m_currentMinoPos.Y + 2;
							if (IsHit(bufPos, buf)) {
								bufPos.X = m_currentMinoPos.X - 2;
								bufPos.Y = m_currentMinoPos.Y + 1;
								if (IsHit(bufPos, buf)) {
									return true;
								}
							}
						}
					}
					m_currentMino = buf;
					m_currentMinoPos = bufPos;
					if (m_lockDown()) m_lockDown++;
					return false;
				}
			}
			else {
				if (m_currentMino.minoAngle == 0) {
					bufPos.X -= 1;
					if (IsHit(bufPos, buf)) {
						bufPos.X = m_currentMinoPos.X + 2;
						if (IsHit(bufPos, buf)) {
							bufPos.X = m_currentMinoPos.X - 1;
							bufPos.Y -= 2;
							if (IsHit(bufPos, buf)) {
								bufPos.X = m_currentMinoPos.X + 2;
								bufPos.Y = m_currentMinoPos.Y + 1;
								if (IsHit(bufPos, buf)) {
									return true;
								}
							}
						}
					}
					m_currentMino = buf;
					m_currentMinoPos = bufPos;
					if (m_lockDown()) m_lockDown++;
					return false;
				}
				else if (m_currentMino.minoAngle == 1) {
					bufPos.X += 2;
					if (IsHit(bufPos, buf)) {
						bufPos.X = m_currentMinoPos.X - 1;
						if (IsHit(bufPos, buf)) {
							bufPos.X = m_currentMinoPos.X + 2;
							bufPos.Y -= 1;
							if (IsHit(bufPos, buf)) {
								bufPos.X = m_currentMinoPos.X - 1;
								bufPos.Y = m_currentMinoPos.Y + 2;
								if (IsHit(bufPos, buf)) {
									return true;
								}
							}
						}
					}
					m_currentMino = buf;
					m_currentMinoPos = bufPos;
					if (m_lockDown()) m_lockDown++;
					return false;
				}
				else if (m_currentMino.minoAngle == 2) {
					bufPos.X += 1;
					if (IsHit(bufPos, buf)) {
						bufPos.X = m_currentMinoPos.X - 2;
						if (IsHit(bufPos, buf)) {
							bufPos.X = m_currentMinoPos.X + 1;
							bufPos.Y += 2;
							if (IsHit(bufPos, buf)) {
								bufPos.X = m_currentMinoPos.X - 2;
								bufPos.Y = m_currentMinoPos.Y - 1;
								if (IsHit(bufPos, buf)) {
									return true;
								}
							}
						}
					}
					m_currentMino = buf;
					m_currentMinoPos = bufPos;
					if (m_lockDown()) m_lockDown++;
					return false;
				}
				else if (m_currentMino.minoAngle == 3) {
					bufPos.X += 1;
					if (IsHit(bufPos, buf)) {
						bufPos.X = m_currentMinoPos.X - 2;
						if (IsHit(bufPos, buf)) {
							bufPos.X = m_currentMinoPos.X - 2;
							bufPos.Y += 1;
							if (IsHit(bufPos, buf)) {
								bufPos.X = m_currentMinoPos.X + 1;
								bufPos.Y = m_currentMinoPos.Y - 2;
								if (IsHit(bufPos, buf)) {
									return true;
								}
							}
						}
					}
					m_currentMino = buf;
					m_currentMinoPos = bufPos;
					if (m_lockDown()) m_lockDown++;
					return false;
				}
			}
		}
	}
	return true;
}
void Game::HoldChange() {
	if (m_hasHeld) return;
	// first (hold is empty)
	if (m_holdMino.minoType < 0 || m_holdMino.minoType >= MINO_TYPE) {
		m_holdMino.minoType = m_currentMino.minoType;
		MinoUpdate();
		m_lockDown.Init();
	}
	else {
		MinoInfo_t tmp = m_holdMino;
		m_holdMino.minoType = m_currentMino.minoType;
		m_currentMino = tmp;
	}
	if (InitMinoPos()) {
		// GameOver
	}
	m_hasHeld = true;
}
char Game::DeleteLine() {
	char deletedlineNum = 0;
	bool hasCleared = false;
	for (int i = 0; i < FIELD_H; i++) {
		bool lineCheck = true;
		for (int j = 0; j < FIELD_W; j++) {
			if (m_field[j][i] == NONE) {
				lineCheck = false;
				break;
			}
		}
		if (lineCheck) {
			deletedlineNum++;
			
			if (!m_isDeleting) {
				m_del = m_gameTimer.Elapse();
			}

			if (m_del + 300 > (signed)m_gameTimer.Elapse()) {
				for (int j = 0; j < FIELD_W; j++) m_field[j][i] = CLRD;
				m_isDeleting = true;
			}
			else {
				for (; i > 0; i--) for (int j = 0; j < FIELD_W; j++) m_field[j][i] = m_field[j][i - 1];
				for (int j = 0; j < FIELD_W; j++) m_field[j][0] = NONE;
				hasCleared = true;
			}
		}
	}
	switch (deletedlineNum) {
	case 1: m_addtionalScore += 100 * m_currentLevel; break;
	case 2: m_addtionalScore += 300 * m_currentLevel; break;
	case 3: m_addtionalScore += 500 * m_currentLevel; break;
	case 4: m_addtionalScore += 800 * m_currentLevel; break;
	default:break;
	}
	if (m_actionNotification == TETRIS && deletedlineNum == 4) {
		m_addtionalScore += (int)m_addtionalScore / 2;
	}

	if (hasCleared) {

		// if É~ÉmÇ™TÇ© && íºëOÇÃëÄçÏÇ™âÒì]Ç© && TÉXÉsÉìèÛë‘Ç…äYìñÇµÇƒÇ¢ÇÈÇ©

		// else
		
		m_score += m_addtionalScore;

		m_currentDeletedLineNum += deletedlineNum;

		m_isBack2Back = false;
		if (m_actionNotification == TETRIS && deletedlineNum == 4) {
			m_isBack2Back = true;
		}

		m_actionNotification = (Actions)(deletedlineNum - 1);
		m_timeActionNotification = m_gameTimer.Elapse();

		m_currentLevel = 1 + (int)m_currentDeletedLineNum / 15;
		SpeedUpdate();

		MinoUpdate();
		m_isDeleting = false;
		if (InitMinoPos()) {
			// GameOver
			return -1;
		}
	}

	return deletedlineNum;
}
void Game::StartGameOver() {
	m_scene = e_GAMEOVER;
	m_timeActionNotification = m_gameTimer.Elapse();
	m_gameTimer.Start();
	m_prevMinoDownTime = m_gameTimer.Elapse();
	m_del = 0;
}
void Game::GameOverUpdate() {
	if (m_del < FIELD_H) {
		if (20 + m_prevMinoDownTime <= (signed)m_gameTimer.Elapse()) {
			m_prevMinoDownTime = m_gameTimer.Elapse();
			for (int i = FIELD_H - 1; i > 0; i--)
				for (int j = 0; j < FIELD_W; j++)
					m_field[j][i] = m_field[j][i - 1];

			for (int j = 0; j < FIELD_W; j++)
				m_field[j][0] = NONE;
			m_del++;
		}
	}
	else {
		if (Console::Instance()->GetKeyEvent() == KEY_INPUT_RETURN) {
			Init();
			StartGame();
		}

	}
}
void Game::GameOverDraw() {
	if (!(m_del < FIELD_H)) {
		Console::Instance()->Print(17, 5, GetColor(L_RED, GetBackgroundColor(NONE)), "GAME  OVER");

		Console::Instance()->Print(13, 12, BLOCK_COLOR[NONE], "ENTER >> TRY AGAIN");
		Console::Instance()->Print(13, 14, BLOCK_COLOR[NONE], "ESC   >> EXIT");
	}
}