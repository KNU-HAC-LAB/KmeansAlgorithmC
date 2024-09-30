#ifndef __TERMINAL_H__
#define __TERMINAL_H__

void gotoxy(const int, const int);
void terminalClear(void);
void terminalSize(const int x, const int y);
void textColoring(const int colorNum);

#endif // __TERMINAL_H__
