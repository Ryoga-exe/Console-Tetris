#include "Console.h"
#include "Game.h"
#include <stdio.h>

int main(void) {
	Console::Instance()->SetWindowAndBufferSize(STAGE_W, STAGE_H);
	Console::Instance()->SetCursorInvisible();
	SetConsoleTitle("Console Tetris");
	Game game;
	if (game.LoadFile()) return -1;
	while (Console::Instance()->ProcessLoop()){
		Console::Instance()->SetWindowAndBufferSize(STAGE_W, STAGE_H);
		Console::Instance()->SetCursorInvisible();
		game.Update();
		game.Draw();
	}
	game.SaveFile();
	return 0;
}