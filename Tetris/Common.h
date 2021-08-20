#pragma once
#include <cstdio>
#include <cstdlib>
#include <graphics.h>
#include <conio.h>
#include <Windows.h>
#include <cstring>
#include <ctime>

//0-6 IJLOSTZ
const int Structures[7][4][2] = {
	{ {0, 0}, {0, 1}, {0, 2}, {0, 3} },
	{ {0, 0}, {0, 1}, {0, 2}, {1, 2} },
	{ {0, 0}, {0, 1}, {0, 2}, {1, 0} },
	{ {0, 0}, {0, 1}, {1, 0}, {1, 1} },
	{ {0, 1}, {0, 2}, {1, 0}, {1, 1} },
	{ {0, 0}, {0, 1}, {0, 2}, {1, 1} },
	{ {0, 0}, {0, 1}, {1, 1}, {1, 2} }
};

//0-6 IJLOSTZ
const COLORREF Colors[7] = { RED, MAGENTA, YELLOW, CYAN, BLUE, BROWN, GREEN };

struct WorkPlace {
	int x1, y1;
	//int x2, y2;
	int lx, ly;
	COLORREF color;
	void swap() {
		//this->x2 = this->x1 + ly;
		//this->y2 = this->y1 + lx;
		int t = lx;
		lx = ly, ly = t;	
	}
};

const int Levels[9] = {1, 2, 3, 4, 5, 10, 15, 20, 30};
const int LevelSpeed[9] = {1000, 800, 600, 500, 400, 350, 300, 250, 200};
const int LevelScore[8] = {1000, 3000, 6000, 10000, 15000, 25000, 40000, 60000};