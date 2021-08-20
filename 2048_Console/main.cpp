//2048_Console v0.1.3_Release
//By Tabing010102 2018.11.16
//Rules from WikiPedia
//Thanks to these Websites or Organizations

#include <cstdio>
#include <conio.h>
#include <Windows.h>
#include <cstring>
#include <ctime>
#include <cstdlib>

static int G[4][4], free_blocks[15][2], tG[4];
static clock_t tstart;
static int key_dect_delay=-1, aftermove_pause_time=-1;
static bool hide_cursor, DEBUG;

void HideTheCursor() {
   CONSOLE_CURSOR_INFO cciCursor;
   HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
   if(GetConsoleCursorInfo(hStdOut, &cciCursor)) {
      cciCursor.bVisible = FALSE;
	  SetConsoleCursorInfo(hStdOut, &cciCursor);
   }
}

void ShowTheCursor() {
   CONSOLE_CURSOR_INFO cciCursor;
   HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
   if(GetConsoleCursorInfo(hStdOut, &cciCursor)) {
      cciCursor.bVisible = TRUE;
	  SetConsoleCursorInfo(hStdOut, &cciCursor);
   }
}

void gotoXY(int x, int y) {
   COORD coord = {x, y};
   SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void flush_block_value(int i, int j, int v) {
	int y=i*2+1, x=j*5+1;
	gotoXY(x, y);
	char buf[5] = "hhhh";
	int cnt = 4;
	while(v) {
		buf[--cnt] = v%10 + '0';
		v /= 10;
	}
	cnt = 0;
	while(buf[cnt] == 'h') buf[cnt++] = ' ';
	printf("%s", buf);
}

void draw_lines() {
	printf("---------------------\n");
	printf("|    |    |    |    |\n");
	printf("---------------------\n");
	printf("|    |    |    |    |\n");
	printf("---------------------\n");
	printf("|    |    |    |    |\n");
	printf("---------------------\n");
	printf("|    |    |    |    |\n");
	printf("---------------------\n");
}

bool can_move() {
	for(int i = 0; i < 4; i++) for(int j = 0; j < 3; j++)
		if(G[i][j]==G[i][j+1] || G[j][i]==G[j+1][i]) return true;
	return false;
}

int get_free_block() {
	int n = 0;
	for(int i = 0; i < 4; i++) for(int j = 0; j < 4; j++)
		if(!G[i][j]) {
			free_blocks[n][0] = i;
			free_blocks[n][1] = j;
			n++;
		}
	return n;
}

bool is_win() {
	for(int i = 0; i < 4; i++) for(int j = 0; j < 4; j++)
		if(G[i][j] >= 2048) return true;
	return false;
}

void Generate() {
	int new_v = (rand()%2)?2:4;
	int free_block = get_free_block();
	int new_pos = rand()%free_block;
	G[free_blocks[new_pos][0]][free_blocks[new_pos][1]] = new_v;
	flush_block_value(free_blocks[new_pos][0], free_blocks[new_pos][1], new_v);
}

void quit(const char *message) {
	ShowTheCursor();
	gotoXY(0 ,9);
	printf("%s\n", message);
	printf("You have played %.3lf secs this time.\n", (double)(clock() - tstart)/1000.0);
	system("pause");
	exit(0);
}

//0上1下2左3右 
void move(int dirt) {
	int cnt, t;
	bool moved = false;
	switch(dirt) {
	case 0://上
		for(int i = 0; i < 4; i++) {
			for(int j = 0; j < 4; j++) tG[j] = G[j][i];
			cnt = 3;
			while(1) {
				while(cnt>=0 && !tG[cnt]) cnt--;
				if(cnt < 0) break;
				t = cnt-1;
				while(t>=0 && !tG[t]) t--;
				if(t < 0) break;
				if(tG[cnt] == tG[t]) { tG[cnt] = 0; tG[t] *= 2; cnt = t-1; }
				else cnt = t;
			}
			cnt = 0;
			for(int j = 0; j < 4; j++) if(tG[j]) {
				tG[cnt] = tG[j];
				if(cnt != j) tG[j] = 0;
				cnt++;
			}
			for(int j = 0; j < 4; j++) if(tG[j] != G[j][i]) {
				flush_block_value(j, i, (G[j][i]=tG[j]));
				moved = true;
			}
		}
		break;
	case 1://下 
		for(int i = 0; i < 4; i++) {
			for(int j = 0; j < 4; j++) tG[j] = G[j][i];
			cnt = 0;
			while(1) {
				while(cnt<=3 && !tG[cnt]) cnt++;
				if(cnt > 3) break;
				t = cnt+1;
				while(t<=3 && !tG[t]) t++;
				if(t > 3) break;
				if(tG[cnt] == tG[t]) { tG[cnt] = 0; tG[t] *= 2; cnt = t+1; }
				else cnt = t;
			}
			cnt = 3;
			for(int j = 3; j >= 0; j--) if(tG[j]) {
				tG[cnt] = tG[j];
				if(cnt != j) tG[j] = 0;
				cnt--;
			}
			for(int j = 0; j < 4; j++) if(tG[j] != G[j][i]) {
				flush_block_value(j, i, (G[j][i]=tG[j]));
				moved = true;
			}
		}
		break;
	case 2://左 
		for(int i = 0; i < 4; i++) {
			for(int j = 0; j < 4; j++) tG[j] = G[i][j];
			cnt = 3;
			while(1) {
				while(cnt>=0 && !tG[cnt]) cnt--;
				if(cnt < 0) break;
				t = cnt-1;
				while(t>=0 && !tG[t]) t--;
				if(t < 0) break;
				if(tG[cnt] == tG[t]) { tG[cnt] = 0; tG[t] *= 2; cnt = t-1; }
				else cnt = t;
			}
			cnt = 0;
			for(int j = 0; j < 4; j++) if(tG[j]) {
				tG[cnt] = tG[j];
				if(cnt != j) tG[j] = 0;
				cnt++;
			}
			for(int j = 0; j < 4; j++) if(tG[j] != G[i][j]) {
				flush_block_value(i, j, (G[i][j]=tG[j]));
				moved = true;
			}
		}
		break;
	case 3://右 
		for(int i = 0; i < 4; i++) {
			for(int j = 0; j < 4; j++) tG[j] = G[i][j];
			cnt = 0;
			while(1) {
				while(cnt<=3 && !tG[cnt]) cnt++;
				if(cnt > 3) break;
				t = cnt+1;
				while(t<=3 && !tG[t]) t++;
				if(t > 3) break;
				if(tG[cnt] == tG[t]) { tG[cnt] = 0; tG[t] *= 2; cnt = t+1; }
				else cnt = t;
			}
			cnt = 3;
			for(int j = 3; j >= 0; j--) if(tG[j]) {
				tG[cnt] = tG[j];
				if(cnt != j) tG[j] = 0;
				cnt--;
			}
			for(int j = 0; j < 4; j++) if(tG[j] != G[i][j]) {
				flush_block_value(i, j, (G[i][j]=tG[j]));
				moved = true;
			}
		}
		break;
	default:
		break;
	}
	Sleep(aftermove_pause_time);
	if(moved) Generate();
}

void game() {
	Generate();
	while(1) {
		while(_kbhit()) {
			switch(_getch()) {
			case 'w':
			case 'W':
			case 0x48://上箭头
				move(0);
				break;
			case 'S':
			case 's':
			case 0x50://下箭头
				move(1);
				break;
			case 'A':
			case 'a':
			case 0x4B://左箭头
				move(2);
				break;
			case 'D':
			case 'd':
			case 0x4D://右箭头
				move(3);
				break;
			case 0x1B://Esc
				quit("User exit.");
				break;
			default:
				break;
			}
			if(!get_free_block() && !can_move()) quit("You lost!");
			if(is_win()) quit("You win!");
		}
		Sleep(key_dect_delay);
		fflush(stdin);
	}
}

void show_info() {
	printf("2048_Console v0.1.3_Release\n");
	printf("By Tabing010102, 2018.11.16\n");
	printf("Rules from WikiPedia\n");
	printf("Thanks to these Websites or Organizations\n");
	printf("===========================================\n");
	printf("Use WASD or UP,DOWN,LEFT,RIGHT to control\n");
	printf("Esc to quit.\n");
	printf("===========================================\n");
	printf("You can run the game in these two ways:\n");
	printf("#1: Run directly.\n");
	printf("#2: [AppName] [Keyboard Detect Delay(ms)] [Pause Time after Move(ms)] [ShowCursor]\n");
	printf("===========================================\n");
	printf("Press any key to start...");
}

void show_bad_usage(const char *message) {
	printf("%s\n", message);
	printf("You can run the game in these two ways:\n");
	printf("#1: Run directly.\n");
	printf("#2: [AppName] [Keyboard Detect Delay(ms)] [PauseTime after Move(ms)] [ShowCursor]\n");
	system("pause");
	exit(0);
}

int main(int argc, char *argv[]) {
	DEBUG = (argc>4);
	if(argc == 1) {
		key_dect_delay = 25;
		aftermove_pause_time = 200;
		show_info();
		_getch();
	} else if(argc >= 3) {
		key_dect_delay = atoi(argv[1]);
		aftermove_pause_time = atoi(argv[2]);
		if(DEBUG) printf("%s %s\n", argv[1], argv[2]);
		if(key_dect_delay < 0) show_bad_usage("Invaild Keyboard Detect Delay");
		if(aftermove_pause_time < 0) show_bad_usage("Invaild PauseTime after Move");
		show_info();
		_getch();
	} else {
		if(argc < 3) show_bad_usage("Wrong Usage!");
	}
	hide_cursor = !(argc>3);
	if(hide_cursor) HideTheCursor();
	system("cls");
	if(DEBUG) printf("%d %d",key_dect_delay,aftermove_pause_time);
	draw_lines();
	srand(time(NULL));
	tstart=clock();
	game();
	return 0;
}
