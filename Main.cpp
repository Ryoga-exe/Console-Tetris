#include "Console.h"
#include "Game.h"
#include <stdio.h>

int main(void) {
	Console::Instance()->SetWindowAndBufferSize(STAGE_W, STAGE_H);
	Console::Instance()->SetCursorInvisible();
	SetConsoleTitle("Console Tetris");
	Game game;
	while (Console::Instance()->ProcessLoop()){
		Console::Instance()->SetWindowAndBufferSize(STAGE_W, STAGE_H);
		Console::Instance()->SetCursorInvisible();
		game.Update();
		game.Draw();
	}
	
	return 0;
}