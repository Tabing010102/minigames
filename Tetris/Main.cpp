//Tetris v0.2.5_Pre_Release1
//By Tabing010102
//Rules from WikiPedia
//Scoring from tetris.com
//Color from Microsoft Tetris
//Thanks to these Websites or Organization

#include "Common.h"

const int BLOCK_SIZE = 35;
const COLORREF BACKGROUND_COLOR = LIGHTGRAY;
bool G[20][10];
bool tG[4][4];
bool flag_refresh = false;
bool flag_clear = false;
int score = 0, total_cleared_lines = 0;
int level = 0;
int cnt_level = Levels[level];
double cnt_speed = LevelSpeed[level];
int gsx = 150;//游戏图形起始点

void init_text() {
	//20-40, 50-75, 100-120, 130-155, 180-200, 210-235, 260-280, 290-315
	solidroundrect(10, 50, 140, 75, 5, 5);
	solidroundrect(10, 130, 140, 155, 5, 5);
	solidroundrect(10, 210, 140, 235, 5, 5);
	solidroundrect(10, 290, 140, 315, 5, 5);
	LOGFONT f;
	gettextstyle(&f);
	f.lfHeight = 20;
	_tcscpy_s(f.lfFaceName, _T("Consolas"));
	f.lfQuality = ANTIALIASED_QUALITY;//字体抗锯齿
	settextstyle(&f);
	settextcolor(BLACK);
	RECT p = { 20, 20, 130, 40 };
	drawtext(_T("Level"), &p, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	p = { 20, 100, 130, 120 };
	drawtext(_T("Score"), &p, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	p = { 20, 180, 130, 200 };
	drawtext(_T("Lines"), &p, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	p = { 20, 260, 130, 280 };
	drawtext(_T("Status"), &p, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	//680-660, 650-630, 620-600, 590-570, 560-540, 530-510, 500-480, 470-450
	f.lfHeight = 16;
	settextstyle(&f);
	p = { 0, 450, 150, 470 };
	drawtext(_T("Pause/Remuse: R, r"), &p, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	p = { 20, 480, 130, 500 };
	drawtext(_T("Left: A, a, ←"), &p, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	p = { 20, 510, 130, 530 };
	drawtext(_T("Right: D, d, →"), &p, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	p = { 20, 540, 130, 560 };
	drawtext(_T("Down: S, s, ↓"), &p, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	p = { 20, 570, 130, 590 };
	drawtext(_T("Bottom: Space"), &p, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	p = { 0, 600, 150, 620 };
	drawtext(_T("Rotate CW: Q, q, ↑"), &p, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	p = { 0, 630, 150, 650 };
	drawtext(_T("Rotate CCW: E, e"), &p, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	p = { 20, 660, 130, 680 };
	drawtext(_T("Exit: Esc"), &p, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	f.lfHeight = 20;
	settextstyle(&f);
}

void draw_status() {
	setbkcolor(WHITE);
	TCHAR s[20];
	//20-40, 50-75, 100-120, 130-155, 180-200, 210-235, 260-280, 290-315
	RECT p = { 20, 50, 130, 75 };
	_stprintf_s(s, _T("%d"), cnt_level);
	drawtext(s, &p, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	p = { 20, 130, 130, 155 };
	_stprintf_s(s, _T("%d"), score);
	drawtext(s, &p, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	p = { 20, 210, 130, 235 };
	_stprintf_s(s, _T("%d"), total_cleared_lines);
	drawtext(s, &p, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	p = { 20, 290, 130, 315 };
	drawtext(_T("Running"), &p, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	setbkcolor(BACKGROUND_COLOR);
}

void draw_lines() {
	setlinecolor(DARKGRAY);
	for (int i = 0; i < 700; i += BLOCK_SIZE) {
		line(gsx, i, gsx + 350, i);
		line(gsx, i+BLOCK_SIZE-1, gsx + 350, i+BLOCK_SIZE-1);
	}
	for (int i = gsx; i < gsx + 350; i += BLOCK_SIZE) {
		line(i, 0, i, 700);
		line(i+BLOCK_SIZE-1, 0, i+BLOCK_SIZE-1, 700);
	}
}

void set_block_color(int ny, int nx, COLORREF color) {//从左向右X，从上到下Y，从0开始
	if (nx < 0 || ny < 0 || nx > 9 || ny > 19) return;
	int ox1 = nx * BLOCK_SIZE + 1 + gsx, oy1 = ny * BLOCK_SIZE + 1;
	int ox2 = ox1 + BLOCK_SIZE - 3, oy2 = oy1 + BLOCK_SIZE - 3;
	setfillcolor(color);
	solidrectangle(ox1, oy1, ox2, oy2);
}

COLORREF get_block_color(int ny, int nx) {
	if (nx < 0 || ny < 0 || nx > 9 || ny > 19) return BACKGROUND_COLOR;
	int x = nx * BLOCK_SIZE + 1 + gsx, y = ny * BLOCK_SIZE + 1;
	return getpixel(x, y);
}

int move(WorkPlace *cntw, int mode=0) {
	//0下1左2右
	switch (mode) {
	case 0://下
		for (int i = 0; i < cntw->ly; i++) {
			int cnt = cntw->lx - 1;
			while (!tG[cnt][i]) cnt--;
			if (cntw->x1 + cnt >= 19 || G[cntw->x1 + cnt + 1][cntw->y1 + i]) {
				flag_refresh = true;
				return -1;
			}
		}
		for (int i = cntw->lx - 1; i >= 0; i--) {
			for (int j = 0; j < cntw->ly; j++) if (tG[i][j]) {
				set_block_color(cntw->x1 + i + 1, cntw->y1 + j, cntw->color);
				G[cntw->x1 + i + 1][cntw->y1 + j] = true;
				set_block_color(cntw->x1 + i, cntw->y1 + j, BACKGROUND_COLOR);
				G[cntw->x1 + i][cntw->y1 + j] = false;
			}
		}
		cntw->x1++;
		//cntw->x2++;
		return 1;
		break;
	case 1://左
		for (int i = 0; i < cntw->lx; i++) {
			int cnt = 0;
			while (!tG[i][cnt]) cnt++;
			if (cntw->y1 + cnt <= 0 || G[cntw->x1 + i][cntw->y1 + cnt - 1])
				return 0;
		}
		for (int i = 0; i < cntw->ly; i++) {
			for (int j = 0; j < cntw->lx; j++) if (tG[j][i]) {
				set_block_color(cntw->x1 + j, cntw->y1 + i - 1, cntw->color);
				G[cntw->x1 + j][cntw->y1 + i - 1] = true;
				set_block_color(cntw->x1 + j, cntw->y1 + i, BACKGROUND_COLOR);
				G[cntw->x1 + j][cntw->y1 + i] = false;
			}
		}
		cntw->y1--;
		//cntw->y2--;
		return 1;
		break;
	case 2://右
		for (int i = 0; i < cntw->lx; i++) {
			int cnt = cntw->ly - 1;
			while (!tG[i][cnt]) cnt--;
			if (cntw->y1 + cnt >= 9 || G[cntw->x1 + i][cntw->y1 + cnt + 1])
				return 0;
		}
		for (int i = cntw->ly - 1; i >= 0; i--) {
			for (int j = 0; j < cntw->lx; j++) if (tG[j][i]) {
				set_block_color(cntw->x1 + j, cntw->y1 + i + 1, cntw->color);
				G[cntw->x1 + j][cntw->y1 + i + 1] = true;
				set_block_color(cntw->x1 + j, cntw->y1 + i, BACKGROUND_COLOR);
				G[cntw->x1 + j][cntw->y1 + i] = false;
			}
		}
		cntw->y1++;
		//cntw->y2++;
		return 1;
		break;
	default:
		return 0;
		break;
	}
}

void rotate(WorkPlace *cntw, int mode) {
	if (mode == 0 || mode == 1) {//右边界不能转
		if (cntw->lx > cntw->ly && cntw->y1 + cntw->lx > 10) {
			return;
		}
	}
	bool ttG[4][4];
	switch (mode) {
	case 0://顺时针
		memcpy(ttG, tG, sizeof(tG));
		memset(tG, 0, sizeof(tG));
		for (int i = 0; i < cntw->lx; i++)
			for (int j = 0; j < cntw->ly; j++)
				tG[j][cntw->lx - i - 1] = ttG[i][j];
		break;
	case 1://逆时针
		memcpy(ttG, tG, sizeof(tG));
		memset(tG, 0, sizeof(tG));
		for (int i = 0; i < cntw->lx; i++)
			for (int j = 0; j < cntw->ly; j++)
				tG[cntw->ly - j - 1][i] = ttG[i][j];
		break;
	default:
		return;
	}
	//转不动
	for(int i = 0; i < cntw->ly; i++)
		for(int j = 0; j < cntw->lx; j++) 
			if (tG[i][j] && !ttG[i][j] && G[cntw->x1 + i][cntw->y1 + j]) {
				memcpy(tG, ttG, sizeof(tG));
				return;
			}
	//转
	for (int i = 0; i < cntw->lx; i++)
		for (int j = 0; j < cntw->ly; j++)
			if (ttG[i][j]) {
				set_block_color(cntw->x1 + i, cntw->y1 + j, BACKGROUND_COLOR);
				G[cntw->x1 + i][cntw->y1 + j] = false;
			}
	cntw->swap();
	for (int i = 0; i < cntw->lx; i++)
		for (int j = 0; j < cntw->ly; j++)
			if (tG[i][j]) {
				set_block_color(cntw->x1 + i, cntw->y1 + j, cntw->color);
				G[cntw->x1 + i][cntw->y1 + j] = true;
			}
	/*system("cls");
	printf("tG:\n");
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			printf("%d ", tG[i][j]);
		}
		printf("\n");
	}
	printf("ttG:\n");
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			printf("%d ", ttG[i][j]);
		}
		printf("\n");
	}
	_getch();
	_getch();
	*/
}

bool new_block(WorkPlace *cntw) {
	int type = rand() % 7;
	//int type = 2;
	cntw->color = Colors[type];
	switch (type) {
	case 0:
		cntw->x1 = 0, cntw->y1 = 3;
		//cntw->x2 = 0, cntw->y2 = 6;
		cntw->lx = 1, cntw->ly = 4;
		break;
	case 3:
		cntw->x1 = 0, cntw->y1 = 4;
		//cntw->x2 = 1, cntw->y2 = 5;
		cntw->lx = 2, cntw->ly = 2;
		break;
	default:
		cntw->x1 = 0, cntw->y1 = 4;
		//cntw->x2 = 1, cntw->y2 = 6;
		cntw->lx = 2, cntw->ly = 3;
		break;
	}
	memset(tG, 0, sizeof(tG));
	for (int i = 0; i < 4; i++) {
		if (G[cntw->x1 + Structures[type][i][0]][cntw->y1 + Structures[type][i][1]] == true) {
			return false;
		}
	}
	for (int i = 0; i < 4; i++) {
		set_block_color(cntw->x1 + Structures[type][i][0],
			cntw->y1 + Structures[type][i][1], cntw->color);//画图
		tG[Structures[type][i][0]][Structures[type][i][1]] = true;
		G[cntw->x1 + Structures[type][i][0]][cntw->y1 + Structures[type][i][1]] = true;
	}
	return true;
}

void print_G(WorkPlace *cntw) {
	system("cls");
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 10; j++) {
			printf("%d ", G[i][j]);
		}
		printf("\n");
	}
	/*printf("\n");
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			printf("%d ", tG[i][j]);
		}
		printf("\n");
	}*/
	printf("x1=%d, y1=%d, lx=%d, ly=%d\n", cntw->x1, cntw->y1, cntw->lx, cntw->ly);
}

void clear_full_lines() {//处理最横排满的情况
	Sleep((int)(cnt_speed / 5));
	flag_clear = false;
	int cleared_lines = 0;
	for (int i = 19; i >= 0; i--) {
		bool flag = true;
		for (int j = 0; j < 10; j++) if (!G[i][j]) { flag = false; break; }
		if (flag) {
			cleared_lines++;
			total_cleared_lines++;
			for (int j = 0; j < 10; j++) {
				G[i][j] = false;
				set_block_color(i, j, BACKGROUND_COLOR);
			}
			for(int j = i-1; j >= 0; j--)
				for (int k = 0; k < 10; k++) if (G[j][k]) {
					G[j + 1][k] = true;
					set_block_color(j + 1, k, get_block_color(j, k));
					G[j][k] = false;
					set_block_color(j, k, BACKGROUND_COLOR);
				}
			i++;
		}
	}
	if (cleared_lines == 1) score += 100 * cnt_level;
	if (cleared_lines == 2) score += 300 * cnt_level;
	if (cleared_lines == 3) score += 500 * cnt_level;
	if (cleared_lines == 4) score += 800 * cnt_level;
	if (cleared_lines != 0) draw_status();
	Sleep((int)(cnt_speed / 5));
}

void check_level() {
	int ori_level = level;
	if (score >= LevelScore[0] && score < LevelScore[1]) level = 1;
	if (score >= LevelScore[1] && score < LevelScore[2]) level = 2;
	if (score >= LevelScore[2] && score < LevelScore[3]) level = 3;
	if (score >= LevelScore[3] && score < LevelScore[4]) level = 4;
	if (score >= LevelScore[4] && score < LevelScore[5]) level = 5;
	if (score >= LevelScore[5] && score < LevelScore[6]) level = 6;
	if (score >= LevelScore[6] && score < LevelScore[7]) level = 7;
	if (score >= LevelScore[7]) level = 8;
	if (level != ori_level) {
		cnt_level = Levels[level];
		cnt_speed = LevelSpeed[level];
	}
}

bool pause() {//true继续false结束
	setbkcolor(WHITE);
	RECT p = { 20, 290, 130, 315 };
	drawtext(_T(" Paused "), &p, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	setbkcolor(BACKGROUND_COLOR);
	while (1) {
		while (_kbhit()) {
			switch (_getch()) {
			case 'R':
			case 'r':
				setbkcolor(WHITE);
				p = { 20, 290, 130, 315 };
				drawtext(_T("Running"), &p, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				setbkcolor(BACKGROUND_COLOR);
				return true;
				break;
			case 0x1B://Esc
				return false;
				break;
			default:
				break;
			}
		}
		Sleep(25);
	}
}

void game() {
	WorkPlace *cntw = new WorkPlace();
	new_block(cntw);
	clock_t told = clock(), tnow;
	draw_status();
	while (1) {
		if (flag_clear) clear_full_lines();
		if (flag_refresh) {
			bool success = new_block(cntw);
			if (!success) {
				printf("Game Over.\n");
				printf("Current level: %d\n", cnt_level);
				printf("Score: %d\n", score);
				printf("Total cleared %d lines\n", total_cleared_lines);
				return;
			}
			flag_refresh = false;
			told = clock();
			continue;
		}
		tnow = clock();
		if (tnow - told >= cnt_speed) {
			int t = move(cntw, 0);
			if (t == -1) flag_clear = true;//移到最下面再清行
			draw_status();
			told = tnow;
			continue;
		}
		while (_kbhit()) {
			switch (_getch()) {
			case 'Q':
			case 'q':
			case 0x48://上箭头
				rotate(cntw, 0);
				break;
			case 'E':
			case 'e':
				rotate(cntw, 1);
				break;
			case 'S':
			case 's':
			case 0x50://下箭头
				int t;
				t = move(cntw, 0);
				if (t == -1) flag_clear = true;//移到最下面再清行
				score += 1 * cnt_level;
				draw_status();
				told = clock();
				break;
			case 'A':
			case 'a':
			case 0x4B://左箭头
				move(cntw, 1);
				break;
			case 'D':
			case 'd':
			case 0x4D://右箭头
				move(cntw, 2);
				break;
			case 0x20://空格
				int retr, dn;
				dn = 0;
				do {
					retr = move(cntw, 0);
					dn++;
				} while (retr != -1);
				score += 2 * (dn-1) *cnt_level;
				draw_status();
				told = clock();
				flag_clear = true;//移到最下面再清行
				break;
			case 0x1B://Esc
				printf("User Exit.\n");
				printf("Current level: %d\n", cnt_level);
				printf("Score: %d\n", score);
				printf("Total cleared %d lines\n", total_cleared_lines);
				return;
				break;
			case 'R':
			case 'r':
				bool c;
				c = pause();
				if (!c) {
					printf("User Exit.\n");
					printf("Current level: %d\n", cnt_level);
					printf("Score: %d\n", score);
					printf("Total cleared %d lines\n", total_cleared_lines);
					return;
				}
				told = clock();
				break;
			default:
				break;
			}
		}
		//print_G(cntw);
		if(score <= 60000 || level < 8) check_level();
		Sleep((int)(cnt_speed/20));
	}
}

int main() {
	printf("Tetris_v0.2.5_PR1 By Tabing010102, 2018.10.18\n");
	printf("Using EasyX %s\n", GetEasyXVer());
	printf("Rules from WikiPedia\n");
	printf("Scoring from tetris.com\n");
	printf("Color from Microsoft Tetris\n");
	printf("Thanks to these Websites or Organization\n");
	printf("Press any key to start...");
	_getch();
	clock_t tstart = clock();
	system("cls");
	initgraph(500, 700);
	//setbkmode(TRANSPARENT);
	setbkcolor(BACKGROUND_COLOR);
	cleardevice();//设置背景色
	draw_lines();//画框架
	init_text();
	memset(G, 0, sizeof(G));
	srand(time(NULL));
	//score = 60001;
	game();
	closegraph();
	printf("You have played %.3lf secs this time.\n", (double)(clock() - tstart)/1000.0);
	system("pause");
	return 0;
}