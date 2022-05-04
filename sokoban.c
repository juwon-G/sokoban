#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termio.h>

#define M 22
#define N 30
#define K 6
#define S 3
#define SIZE 15
#define XnY 2

void printeq(void);
void Start(void);
int Namefind(int namecnt, char name[S]);
void moniter(char name[S], int lv, int move, char command, char lev[K][N][N]);
int getch(void);
void Exit(void);
void maptest(void);
int lv1(char lev[K][N][N]);
int lv2(char lev[K][N][N]);
int lv3(char lev[K][N][N]);
void ListClear(char lev[K][N][N]);
int Undo(char lev[K][N][N]);
void Stack(char lev[K][N][N]);
void Replay(int lv, char lev[K][N][N]);
int New(int lv, char lev[K][N][N]);
int CompleteCheck(int size, int place[SIZE][XnY], char lev[K][N][N]);
int Complete(int lv, int move, char name[S], char lev[K][N][N]);
void rankingsave(int lv, int move, char name[S]);
void ListPrint(char lev[K][N][N]);
int coordinates_sero(char lev[K][N][N]);
int coordinates_garo(char lev[K][N][N]);
int coordinates_place(int place[SIZE][XnY], char lev[K][N][N]);
int Move(int size, int x, int y, int i, int j, int place[SIZE][XnY], char lev[K][N][N]);
void save(char name[S], int lv, int move, char lev[K][N][N]);
void call_map(char name[S], char lev[K][N][N]);
int call_lv(char name[S]);
int call_move(char name[S]);
void help(int move);
void ranking(int move);

int main(void) {

	maptest();

	char opt;
	int eng, size, move = 0, lv = 0, namecnt = 0, place[SIZE][XnY] = { 0 };
	char name[S] = { 0 }, lev[K][N][N] = { 0 };

	do {
		Start();
		printf("Input option : ");
		scanf("%c", &opt);

	} while (opt != 'n' && opt != 'f' && opt != 'N' && opt != 'F' && opt != '1' && opt != '2' && opt != '3');

	do {
		eng = 0;
		Start();
		printf("Input option : %c\n", opt);
		printf("Input  your name : ");
		scanf("%s", name);

		if (strlen(name) > 3)
			continue;

		for (int i = 0;i < 3;i++) {

			if (name[i] > 64 && name[i] < 91);

			else if (name[i] > 96 && name[i] < 123);

			else if (name[i] == 0);

			else
				eng = 1;
		}
	} while (eng);
	
	namecnt = Namefind(namecnt, name);
	
	switch (opt) {
	case 'n':
	case 'N':
	case '1':
		lv = lv1(lev);
		size = coordinates_place(place, lev);
		break;

	case 'f':
	case 'F':
		if (namecnt == strlen(name)) {
			
			call_map(name, lev);
			lv=call_lv(name);
			move=call_move(name);
		}

		else {

			lv=lv1(lev);
		}

		size = coordinates_place(place, lev);
		break;

	case '2':
		lv = lv2(lev);
		size = coordinates_place(place, lev);
		break;

	case '3':
		lv = lv3(lev);
		size = coordinates_place(place, lev);
		break;

	}

	while (1) {
		int command, sero = 0, garo = 0;
		moniter(name, lv, move, command, lev);
		sero = coordinates_sero(lev);
		garo = coordinates_garo(lev);

		if (CompleteCheck(size, place, lev)) {

			lv=Complete(lv, move, name, lev);
			size = coordinates_place(place, lev);
			move = 0;
			continue;

		}

		command = getch();

		switch (command) {

		case 'h':
		case 'H':
			move += Move(size, sero, garo, 0, -1, place, lev);
			break;

		case 'j':
		case 'J':
			move += Move(size, sero, garo, 1, 0, place, lev);
			break;

		case 'k':
		case 'K':
			move += Move(size, sero, garo, -1, 0, place, lev);
			break;

		case 'l':
		case 'L':
			move += Move(size, sero, garo, 0, 1, place, lev);
			break;

		case 'u':
		case 'U':
			move += Undo(lev);
			break;

		case 'r':
		case 'R':
			Replay(lv, lev);
			break;

		case 'n':
		case 'N':
			lv=New(lv, lev);
			size = coordinates_place(place, lev);
			move = 0;
			break;

		case 'e':
		case 'E':
			moniter(name, lv, move, command, lev);
			printf("\nGood Bye\n");
			Exit();
			break;

		case 's':
		case 'S':
			save(name, lv, move, lev);
			break;

		case 'f':
		case 'F':
			ListClear(lev);
			lv=call_lv(name);

			switch (lv) {
			case 1:
				lv1(lev);
				size = coordinates_place(place, lev);
				break;
			case 2:
				lv2(lev);
				size = coordinates_place(place, lev);
				break;
			case 3:
				lv3(lev);
				size = coordinates_place(place, lev);
				break;
			}
			ListClear(lev);
			call_map(name, lev);
			move=call_move(name);
			break;

		case 'd':
		case 'D':
			help(move);
			break;

		case 't':
		case 'T':
			ranking(move);
			break;

		}
	}
	return 0;
}

void printeq(void) {

	for (int i = 0; i < M; i++)
		printf("=");
	printf("\n");
}

void Start(void) {

	system("clear");
	printeq();
	printf(" S  O  K  O  B  A  N\n");
	printeq();
	for (int i = 0; i < 2; i++)
		printf("\n");

	printf("n : New Game\n");
	printf("f : File load\n");
	printf("1~3 : Level Number\n");

	for (int i = 0; i < 2; i++)
		printf("\n");
}

int Namefind(int namecnt, char name[S]) {

	int c;
	FILE* in;
	in = fopen("soko.txt", "r");

	while ((c = getc(in)) != EOF) {

		if (namecnt == strlen(name))
			break;

		if (namecnt > 0) {

			if (c == name[namecnt]) {
				namecnt++;
			}

			else {
				namecnt = 0;
			}

			continue;
		}

		if (c == name[namecnt])
			namecnt++;
	}

	fclose(in);
	return namecnt;
}

void moniter(char name[S], int lv, int move, char command, char lev[K][N][N]) {

	system("clear");
	printeq();
	printf("   %s in level %d \n", name, lv);
	printeq();
	for (int i = 0; i < 2; i++)
		printf("\n");

	ListPrint(lev);
	
	for (int i = 0; i < 2; i++)
		printf("\n");
	printf("moves : %04d\n", move);
	printf("command : %c", command);



}

int getch(void) {

	int ch;
	struct termios buf;
	struct termios save;
	tcgetattr(0, &save);
	buf = save;
	buf.c_lflag &= ~(ICANON | ECHO);
	buf.c_cc[VMIN] = 1;
	buf.c_cc[VTIME] = 0;
	tcsetattr(0, TCSAFLUSH, &buf);
	ch = getchar();
	tcsetattr(0, TCSAFLUSH, &save);
	return ch;
}

void Exit(void) {
	exit(0);

}

void maptest(void) {

	int box = 0, place = 0, cnt = 0, lv = 0;
	char c;
	FILE* in;
	in = fopen("map.txt", "r");

	while ((c = getc(in)) != EOF) {

		switch (c) {

		case 's':
			cnt++;

		case 'e':
			if (box != place)
				lv = cnt;

			break;

		case '$':
			box++;
			break;

		case 'O':
			place++;
			break;
		}

		if (lv > 0) {
			printf("Wrong level %d map\n", lv);
			Exit();
			return;
		}
	}
}

int lv1(char lev[K][N][N]) {

	int cnt = 0;
	char c;
	FILE* in;
	in = fopen("map.txt", "r");


	for (int i = 0;i < N;i++) {

		for (int j = 0; j < N; j++) {

			if (cnt > 1)

				break;

			fscanf(in, "%c", &c);

			if (c == 0)
				continue;
			if (c == '\n') {
				j = N;
				continue;
			}
			lev[0][i][j] = c;

			if (c == 's') {

				cnt += 1;
				continue;
			}
		}
	}

	fclose(in);
	return 1;
}

int lv2(char lev[K][N][N]) {

	int cnt = 0;
	char c;
	FILE* in;
	in = fopen("map.txt", "r");

	int i = 0, j = 0;
	while (1) {
		if (j == N) {
			j = 0;
			i++;
		}
		if (i == N)
			break;
		if (cnt > 2)
			break;
		fscanf(in, "%c", &c);
		if (c == 's') {
			cnt += 1;
		}

		if (cnt > 1) {

			if (c == 0)
				continue;

			if (c == '\n') {
				j = N;
				continue;
			}
			lev[0][i][j] = c;
			j++;
		}
	}

	fclose(in);
	return 2;
}

int lv3(char lev[K][N][N]) {

	int cnt = 0;
	char c;
	FILE* in;
	in = fopen("map.txt", "r");

	int i = 0, j = 0;
	while (1) {
		if (j == N) {
			j = 0;
			i++;
		}
		if (i == N)
			break;
		if (cnt > 3)
			break;
		fscanf(in, "%c", &c);
		if (c == 's'||c=='e') {
			cnt += 1;
		}

		if (cnt > 2) {

			if (c == 0)
				continue;

			if (c == '\n') {
				j = N;
				continue;
			}
			lev[0][i][j] = c;
			j++;
		}
	}

	fclose(in);
	return 3;
}

void ListClear(char lev[K][N][N]) {

	for (int l = 0;l < K;l++) {

		for (int i = 0;i < N;i++) {

			for (int j = 0;j < N;j++) {

				lev[l][i][j] = 0;

			}
		}
	}

}

int Undo(char lev[K][N][N]) {
	int sum = 0, point = K - 1;

	for (int l = 0;l < K - 1;l++) {

		for (int i = 0;i < N;i++) {

			for (int j = 0;j < N;j++) {

				sum += lev[K - 1 - l][i][j];

			}
		}

		if (sum == 0) {
			point = K - 2 - l;
			continue;
		}

		else {
			break;
		}
	}

	if (point == 0)
		return 0;

	for (int l = 0;l < K - 1;l++) {

		for (int i = 0;i < N;i++) {

			for (int j = 0;j < N;j++) {

				lev[l][i][j] = lev[l + 1][i][j];

			}
		}
	}

	for (int i = 0;i < N;i++) {

		for (int j = 0;j < N;j++) {

			lev[point][i][j] = 0 ;
		}
	}

	return 1;

}

void Stack(char lev[K][N][N]) {

	for (int l = K-1; l > 0;l--) {

		for (int i = 0;i < N;i++) {

			for (int j = 0;j < N;j++) {

				lev[l][i][j] = lev[l-1][i][j];

			}
		}
	}
}



void Replay(int lv, char lev[K][N][N]) {

	ListClear(lev);

	switch (lv) {

	case 1:
		lv1(lev);
		break;
	
	case 2:
		lv2(lev);
		break;
	
	case 3:
		lv3(lev);
		break;
	}

	return;
}

int New(int lv, char lev[K][N][N]) {

	ListClear(lev);

	lv=lv1(lev);

	return lv;

}
int CompleteCheck(int size, int place[SIZE][XnY], char lev[K][N][N]) {
	int x, y;
	for (int i = 0;i < size;i++) {
		x = place[i][0];
		y = place[i][1];
		if (lev[0][x][y] == '$')
			continue;
		else
			return 0;
		
	}
	return 1;
}

int Complete(int lv, int move, char name[S], char lev[K][N][N]) {
	rankingsave(lv, move, name);
	char choice;
	printf("\nComplete!\n");

	if (lv == 3) {

		printf("It's final stage! Goodbye!\n");
		Exit();
	}

	printf("Continue? y/n\n");
	
	while (1) {
		choice = getch();

		if (choice == 'y' || choice == 'Y') {

			ListClear(lev);

			switch (lv) {

			case 1:
				lv = lv2(lev);
				return lv;

			case 2:
				lv = lv3(lev);
				return lv;

			}
		}
		else if (choice == 'n' || choice == 'N')
			Exit();

	}
	
}

void rankingsave(int lv, int move, char name[S]) {

	int d, tmpmove, type = 0, namecount = -1, movecount = -1, tmp[3] = { 0 }, rankingmove[6] = { 0 };
	char c, tmpname[3] = { 0 }, rankingname[6][4] = { 0 };	

	for (int i = 0;i < 4;i++) {
		tmp[i] = name[i];
		rankingname[5][i] = tmp[i];
	}
	rankingmove[5] = move;

	FILE* in;

	switch (lv) {
	case 1:
		in = fopen("ranking1.txt", "r");
		break;

	case 2:
		in = fopen("ranking2.txt", "r");
		break;

	case 3:
		in = fopen("ranking3.txt", "r");
		break;
	}

	while ((c = getc(in)) != EOF) {

		if (c == '/') {
			type = 1;
			namecount++;
			continue;
		}
		if (c == '|') {
			type = 2;
			movecount++;
			continue;
		}
		switch (type) {
		case 1:
			for (int i = 0;i < 3;i++) {
				fscanf(in, "%c", &c);
				rankingname[namecount][i] = c;
			}
			type = 0;
			break;
		case 2:
			fscanf(in, "%d", &d);
			rankingmove[movecount] = d;
			type = 0;
			break;
		}
	}
	fclose(in);

	for (int i = 0;i < 6;i++) {
		for (int j = 5;j > i; j--) {
			if (rankingmove[j] == 0)
				continue;
			if (rankingmove[j - 1] == 0 || rankingmove[j] < rankingmove[j - 1]) {
				tmpmove = rankingmove[j];
				for (int k = 0;k < 4;k++) {
					tmpname[k] = rankingname[j][k];
				}
				rankingmove[j] = rankingmove[j - 1];
				rankingmove[j - 1] = tmpmove;
				for (int k = 0;k < 4;k++) {
					rankingname[j][k] = rankingname[j - 1][k];
					rankingname[j - 1][k] = tmpname[k];
				}
			}
		}
	}

	FILE* out;
	switch (lv) {
	case 1:
		out = fopen("ranking1.txt", "w");
		break;

	case 2:
		out = fopen("ranking2.txt", "w");
		break;
	case 3:
		out = fopen("ranking3.txt", "w");
		break;
	}

	for (int i = 0;i < 5;i++) {
		fprintf(out, "/ ");

		for (int j = 0;j < 4;j++) {
			fprintf(out, "%c", rankingname[i][j]);
		}
		fprintf(out, " ");
		fprintf(out, "| %d\n", rankingmove[i]);
	}
	fclose(out);
}



void ListPrint(char lev[K][N][N]) {
	int count = 0;
	for (int i = 0;i < N;i++) {
		for (int j = 0;j < N;j++) {
			if (lev[0][i][j] == 's' || lev[0][i][j] == 'e') {
				count++;
				continue;
			}
			if (count == 1)
				printf("%c", lev[0][i][j]);
		}
		if (count == 1)
			printf("\n");
	}
}

int coordinates_sero(char lev[K][N][N]) {
	for (int n=0; n < N; n++) {
		for (int m=0; m < N; m++) {
			if (lev[0][n][m] == '@') {
				return n;
			}
		}
	}
}

int coordinates_garo(char lev[K][N][N]) {
	for (int n=0; n < N; n++) {
		for (int m=0; m < N; m++) {
			if (lev[0][n][m] == '@') {
				return m;
			}
		}
	}
}

int coordinates_place(int place[SIZE][XnY], char lev[K][N][N]) {
		int x = 0, size = 0;
		for (int n = 0; n < N; n++) {
			for (int m = 0; m < N; m++) {
				if (lev[0][n][m] == 'O') {
					place[x][0] = n;
					place[x][1] = m;
					x++;
					size++;
				}
			}
		}
		return size;
}

int Move(int size, int x, int y, int i, int j, int place[SIZE][XnY], char lev[K][N][N]) {
	int cnt=0;
	for (int m = 0; m < size;m++) {
		if (x == place[m][0] && y == place[m][1]) {
			cnt = 1;
			break;
		}
		else
			cnt = 2;
	}

	if (lev[0][x + i][y + j] != '#') {
		if (lev[0][x + i][y + j] == '.' || lev[0][x + i][y + j] == 'O') {
			Stack(lev);
			lev[0][x + i][y + j] = '@';
		}
		else if (lev[0][x + i][y + j] == '$') {
			if (lev[0][x + i * 2][y + j * 2] == '#' || lev[0][x + i * 2][y + j * 2] == '$')
				return 0;
			Stack(lev);
			lev[0][x + i][y + j] = '@';
			lev[0][x + i * 2][y + j * 2] = '$';

		}
	}
	else
		return 0;

	switch (cnt) {
	case 1:
		lev[0][x][y] = 'O';
		return 1;
	case 2:
		lev[0][x][y] = '.';
		return 1;
	}

	return 0;
}


void save(char name[S], int lv, int move, char lev[K][N][N]) {

	char c;
	FILE* in;
	in = fopen("soko.txt", "a");
	fprintf(in, "^");
	fprintf(in, "%s", name);
	fprintf(in, "%%%%");
	fprintf(in, "%d", lv);
	fprintf(in, " ");
	fprintf(in, "%d", move);
	fprintf(in, "/");
	for (int i = 0;i < N;i++) {
		for (int j = 0; j < N; j++) {
			c = lev[0][i][j];
			fprintf(in, "%c", c);
		}
	}
	fprintf(in, "|");
	fclose(in);

}
void call_map(char name[S], char lev[K][N][N]) {
	int c, i;
	char a, arng, nam[S] = { 0 };
	FILE* in;
	in = fopen("soko.txt", "r");
	while ((a = getc(in)) != EOF) {

		if ((a == '^')) {
			int e = 0;

			while ((a = getc(in)) != EOF) {

				if ((a >= 'a' && a <= 'z') || (a > 'A' && a <= 'Z')) {
					nam[e] = a;
				}

				else
					break;

				e++;
			}
		}

		i = strcmp(name, nam);
		if (i == 0) {
			while ((a = getc(in)) != EOF) {
				if (a == '/') {
					for (int i = 0;i < N;i++) {
						for (int j = 0;j < N;j++) {
							fscanf(in, "%c", &arng);
							if (arng == '|') {
								fclose(in);
								return;
								continue;
							}
							lev[0][i][j] = arng;
						}
					}
				}
			}
		}
	}
	fclose(in);
	return;
}

int call_lv(char name[S]) {
	int c, i;
	char a, arng, nam[S] = { 0 };
	FILE* in;
	in = fopen("soko.txt", "r");
	while ((a = getc(in)) != EOF) {
		if ((a == '^')) {
			int e = 0;

			while ((a = getc(in)) != EOF) {

				if ((a >= 'a' && a <= 'z') || (a > 'A' && a <= 'Z')) {
					nam[e] = a;
				}

				else
					break;

				e++;
			}
		}
		i = strcmp(name, nam);
		if (i == 0) {
			while ((a = getc(in)) != EOF) {
				if (a == '%') {
					fscanf(in, "%d", &c);
				}
			}

		}
	}
	fclose(in);
	return c;
}

int call_move(char name[S]) {
	int c, i;
	char a, arng, nam[S] = { 0 };
	FILE* in;
	in = fopen("soko.txt", "r");
	while ((a = getc(in)) != EOF) {
		if ((a == '^')) {
			int e = 0;

			while ((a = getc(in)) != EOF) {

				if ((a >= 'a' && a <= 'z') || (a > 'A' && a <= 'Z')) {
					nam[e] = a;
				}

				else
					break;

				e++;
			}
		}
		i = strcmp(name, nam);
		if (i == 0) {
			while ((a = getc(in)) != EOF) {
				if(a == ' '){
					fscanf(in, "%d", &c);
				}	
			}
		}
	}
	fclose(in);
	return c;
}

void help(int move) {
	int command;
	do {
		system("clear");
		printeq();
		printf("    SOKOBAN HELP   \n");
		printeq();
		printf("h(왼쪽), j(아래),k(위), l(오른쪽)\n");
		printf("u(undo)\n");
		printf("r(replay)\n");
		printf("n(new)\n");
		printf("e(exit)\n");
		printf("s(save)\n");
		printf("f(file load)\n");
		printf("d(display help)\n");
		printf("t(top)\n");
		printf("enter(redraw map)\n");
		printf("\n\nmoves : %04d\n", move);
		printf("command : %c", command);
	} while ((command = getch()) != 10 && command != 13);
}

void ranking(int move) {
	int command;
	do {
		system("clear");
		printeq();
		printf("     RANKING    \n");
		printeq();
		for (int j = 1;j < 4;j++) {
			int d, type = 0, namecount = -1, movecount = -1, rankingmove[5] = { 0 };
			char c, rankingname[5][4] = { 0 };
			FILE* in;

			printf("\n*****level %d*****", j);
			switch (j) {
			case 1:
				in = fopen("ranking1.txt", "r");
				break;

			case 2:
				in = fopen("ranking2.txt", "r");
				break;

			case 3:
				in = fopen("ranking3.txt", "r");
				break;
			}

			while ((c = getc(in)) != EOF) {

				if (c == '/') {
					type = 1;
					namecount++;
					continue;
				}
				if (c == '|') {
					type = 2;
					movecount++;
					continue;
				}
				switch (type) {
				case 1:
					for (int i = 0;i < 3;i++) {
						fscanf(in, "%c", &c);
						rankingname[namecount][i] = c;
					}
					type = 0;
					break;
				case 2:
					fscanf(in, "%d", &d);
					rankingmove[movecount] = d;
					type = 0;
					break;
				}
			}
			fclose(in);

			for (int i = 0;i < 5;i++) {
				printf("\n");
				for (int j = 0;j < 4;j++) {
					printf("%c", rankingname[i][j]);
				}
				printf(" ");
				printf("%d", rankingmove[i]);
			}
		}
		printf("\n\nenter(redraw map)\n");
		printf("\nmoves : %04d\n", move);
		printf("command : %c", command);
	} while ((command = getch()) != 10 && command != 13);
}


