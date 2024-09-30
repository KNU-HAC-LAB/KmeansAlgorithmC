#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <Windows.h>

#include "terminal.h"
#include "string.h"

#define NUM_TO_MAX_BUFFER 32

void gotoxy(const int x, const int y)
{
	COORD pos = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void terminalClear(void)
{
	system("cls");
}

void terminalSize(const int x, const int y)
{
	char command[NUM_TO_MAX_BUFFER];
	char cols[NUM_TO_MAX_BUFFER], lines[NUM_TO_MAX_BUFFER];

	sprintf(cols, "%d", x + 1);
	sprintf(lines, "%d", y + 1);
	sprintf(command, "mode con cols=%s lines=%s", cols, lines);

	printf("cols: %s, lines: %s, command: %s\n", cols, lines, command);
	system(command);
}

void textColoring(const int colorNum)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorNum);
}