
#include <stdio.h>
#include <iostream>
#include <conio.h>
#include <time.h>
#include <windows.h>
#include <fstream>
#include <sstream>


using namespace std;
int  gameState = 1;
const int height = 20;
const int width = 50;
int x = width / 2;
int y = height / 2;
int scoreTimes = 120;
int still = 1;
int tailx[100], taily[100], ntail;
int foodxx, foodyy;
int foodx, foody;
int score,L;
enum eDirection { STOP = 0, UP, DOWN, LEFT, RIGHT };

eDirection dir;

struct Score {
	string playerName;
	int score;
};

const int maxTopScores = 5;
Score topScores[maxTopScores];



const string scoreFileName = "top_scores.txt";


void InitializeTopScores() {
	for (int i = 0; i < maxTopScores; ++i) {
		topScores[i].playerName = "No Name";
		topScores[i].score = 0;
	}
}

void SaveTopScores() {
	ofstream file(scoreFileName);  
	if (file.is_open()) {
		for (int i = 0; i < maxTopScores; ++i) {
			file << topScores[i].playerName << endl;
			file << topScores[i].score << endl;
		}
		file.close();
	}
}
void LoadTopScores() {
	ifstream file(scoreFileName);
	if (file.is_open()) {
		for (int i = 0; i < maxTopScores; ++i) {
			if (file.eof()) break;
			getline(file, topScores[i].playerName);
			file >> topScores[i].score;
			file.ignore(INT_MAX, '\n');  
		}
		file.close();
	}
	bool hasValidScores = false;
	for (int i = 0; i < maxTopScores; ++i) {
		if (topScores[i].score > 0) {
			hasValidScores = true;
			break;
		}
	}

	if (!hasValidScores) {
		InitializeTopScores();
	}
}



void UpdateTopScores(string playerName, int score) {
	for (int i = 0; i < maxTopScores; ++i) {
		if (score > topScores[i].score) {
			for (int j = maxTopScores - 1; j > i; --j) {
				topScores[j] = topScores[j - 1];
			}
			topScores[i].playerName = playerName;
			topScores[i].score = score;
			break;
		}
	}
}

void DisplayTopScores() {
	cout << "\n\nTop 5 Scores:\n";
	for (int i = 0; i < maxTopScores; ++i) {
		cout << i + 1 << ". " << topScores[i].playerName << " - " << topScores[i].score << "\n";
	}
}

void unshowTopScores() {
	cout << "\n\n                   \n";
	for (int i = 0; i < maxTopScores; ++i) {
		cout <<"                " << "\n";
	}
}



void setcursor(bool visible)
{
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO lpCursor;
	lpCursor.bVisible = visible;
	lpCursor.dwSize = 20;
	SetConsoleCursorInfo(console, &lpCursor);
}

void gotoxy(int x, int y)
{
	COORD c = { x, y };
	SetConsoleCursorPosition(
		GetStdHandle(STD_OUTPUT_HANDLE), c);
}
void Food() {
	L = rand() % 100;
	if (L < 80) {
		foodx = 1 + rand() % (width - 4);
		foody = 1 + rand() % (height - 4);
		while (foodx == 'o' || foodxx == '0'||foody=='o') {
			foodx = 1 + rand() % (width - 4);
			foody = 1 + rand() % (height - 4);
		}gotoxy(foodx, foody);
		printf("F");
		foodyy = 0;
	}
	else {
		foodxx = 1 + rand() % (width - 4);
		foodyy = 1 + rand() % (height - 4);
		while (foodxx == 'o'|| foodxx == '0' ||foodyy =='o') {
			foodxx = 1 + rand() % (width - 4);
			foodyy = 1 + rand() % (height - 4);
		}
		gotoxy(foodxx, foodyy);
		printf("M");
		foody = 0;
	}


}
void drawfood() {
	if (L < 80) {
		gotoxy(foodx, foody);
		printf("F");
	}
	else {
		gotoxy(foodxx, foodyy);
		printf("M");
	}
}

void Setup() {
	gameState = 0;
	dir = STOP;
	score = 0;
	Food();
}




void input() {
	if (_kbhit()) {
		switch (_getch()) {
		case'w':
			dir = UP;
			break;
		case'a':
			dir = LEFT;
			break;
		case's':
			dir = DOWN;
			break;
		case'd':
			dir = RIGHT;
			break;
		case'p':
			gameState = 3;
		default:
			break;
		}
	}
}




void Logic() {
	int prex = tailx[0];
	int prey = taily[0];
	tailx[0] = x;
	taily[0] = y;
	int pre2x, pre2y;

	for (int i = 1;i < ntail;i++) {
		pre2x = tailx[i];
		pre2y = taily[i];
		tailx[i] = prex;
		taily[i] = prey;
		prex = pre2x;
		prey = pre2y;

	}

	switch (dir) {
	case UP:
		y--;
		break;
	case DOWN:
		y++;
		break;
	case LEFT:
		x--;
		break;
	case RIGHT:
		x++;
	default:
		break;
	}
	if (x<=0 || x>width-2 || y<=0 || y>height-1) {
		gameState = 3;
	}
	for (int i = 0;i < ntail;i++) {
		if (x == tailx[i] && y == taily[i]) {
			gameState = 3;
		}
	}
	if (x == foodx && y == foody) {
		score += 100;
		ntail++;
		Food();
		Beep(700, 100);
		still = 1;
	}
	if (x == foodxx && y == foodyy) {
		score += 200;
		if (ntail > 0){
			ntail--;
	}
	    Food();
		Beep(600, 100);
		still = 1;
	}
}
void clearState() {
	int x = 25, y = 5;
	gotoxy(x, y);
	printf("   ");
	gotoxy(18, 10); printf("   ");
	}

void draw_snake(int x,int y) {
	gotoxy(x, y);
	printf("0");
	for (int k = 0;k < ntail;k++) {
		y = taily[k];
		x = tailx[k];
			gotoxy(x, y);
			printf("o");
		}
		}

void erase_snake(int x, int y) {
	gotoxy(x, y);
	printf(" ");
	for (int k = 0;k < ntail;k++) {
		y = taily[k];
		x = tailx[k];
		gotoxy(x, y);
		printf(" ");
	}
}
	void draw_map() {
		gotoxy(0, 0);
	for (int i = 1;i < width;i++) {
		cout << "¯";
	}cout << endl;

	for (int i = 0;i < height;i++) {
		for (int j = 0;j < width;j++) {
			if (j == 0 || j == width - 1) {
				cout << "|";
			}
			else {
				cout << " ";
			}
		}cout << endl;
	}

	for (int i = 1;i < width;i++) {
		cout << "¯";
	}cout << endl;
}
	void delete_map() {
			gotoxy(0, 0);
			for (int i = 1;i <= width;i++) {
				cout << " ";
			}cout << endl;

			for (int i = 0;i < height;i++) {
				for (int j = 0;j <= width;j++) {
					if (j == 0 || j == width - 1) {
						cout << " ";
					}
					else {
						cout << " ";
					}
				}cout << endl;
			}
			for (int i = 1;i < width;i++) {
				cout << " ";
			}cout << endl;
	}





int main()
{
	InitializeTopScores();
	LoadTopScores();
	int j = 60, k = 1;
	gotoxy(j, k);
	printf("Peerawit :66010593");
	while (gameState != 2) {
		while (gameState == 1) {
			
			setcursor(0);
			int x = 25, y = 5;
			gotoxy(x, y);
			printf("Snake Games");
			gotoxy(18, 10); printf("Press Spacebar to play games");
			if (_kbhit()) {
				switch (_getch()) {
				case' ':
					gameState = 0;

					break;
				default:
					break;
				}
			}
		}

		srand(time(NULL));
		draw_map();
		Setup();
		while (gameState == 0) {
			gotoxy(0, 0);
			printf("¯");
			gotoxy(66, 10);
			printf("Score = %d", score);
			erase_snake(x, y);
			input();
			Logic();
			draw_snake(x, y);
			drawfood();
			if ((score > 1 ) && (score % 500 == 0)&& (still ==1) ){
				still = 0;
				if (scoreTimes > 40) {
					scoreTimes = scoreTimes - 5;
				}
			}
			Sleep(scoreTimes);
			
		}
		while (gameState == 3) {
			erase_snake(x, y);
			x = width / 2;
			y = height / 2;
			ntail = 0;
			delete_map();
			setcursor(1);
			string name;
			gotoxy(10, 10);
			cout << "Enter Your Name (No Spaces): ";
			cin >> name;
			printf("Your Score is: %d", score);
			UpdateTopScores(name, score);
			DisplayTopScores();
			SaveTopScores();  // Save the updated scores to the file
			gameState = 4;
			break;
		}
		while (gameState == 4) {
			setcursor(0);
			gotoxy(25, 7);
			printf("Press M to go to main menu");
			gotoxy(25, 8);
			printf("Press ESC to close game");
			if (_kbhit()) {
				int ch = _getch();
				
				if (ch == 'm') {
					gotoxy(66, 10);
					printf("                    ");
					score = 0;
					scoreTimes = 125;
					delete_map();
					gotoxy(10, 10);
					printf("                  ");
					printf("\n                     ");
					unshowTopScores();
					gameState = 1;
					break;
				}
				else if (ch == 27) {
					gameState = 2;
				}
				
			}
		}
	}
	exit (0);

}
