#include <iostream>
#include <curses.h>
#include <unistd.h>
#include <termios.h>
#include <thread>

char _getch() {
        char buf = 0;
        struct termios old = {0};
        if (tcgetattr(0, &old) < 0)
                perror("tcsetattr()");
        old.c_lflag &= ~ICANON;
        old.c_lflag &= ~ECHO;
        old.c_cc[VMIN] = 1;
        old.c_cc[VTIME] = 0;
        if (tcsetattr(0, TCSANOW, &old) < 0)
                perror("tcsetattr ICANON");
        if (read(0, &buf, 1) < 0)
                perror ("read()");
        old.c_lflag |= ICANON;
        old.c_lflag |= ECHO;
        if (tcsetattr(0, TCSADRAIN, &old) < 0)
                perror ("tcsetattr ~ICANON");
        return (buf);
}

using namespace std;
bool gameOver;
const int width = 20;
const int height = 10;
int x1, y1, starX, starY, score;
int tailX[100], tailY[100];
int nTail;
enum eDirection{ STOP = 0, LEFT, RIGHT, UP, DOWN };
eDirection dir;
void Setup(){
	gameOver = false;
	x1 = (width/2);
	y1 = height/2;
	starX = rand() % width;
	starY = rand() % height;
	score = 0;
}
void Draw(){
	system("clear");
	for(int i = 0; i < width; i++){
		cout << "#";
	}
	cout<<endl;
	for(int i = 0; i < height; i++){
		for(int j = 0; j < width; j++){
			if(j == 0 || j == width-1)
				cout << "#";
			else if(i == y1 && j == x1)
				cout << "0";
			else if(j == starX && i == starY)
				cout << "*";
			else{
				bool print = false;
				for(int k = 0; k < nTail; k++){
					if(tailX[k] == j && tailY[k] == i){
						cout << "L";
						print = true;
					}
				}
				if(!print)
				cout << " ";
			}
		}
		cout << endl;
	}
	for(int i = 0; i < width; i++){
            cout << "#";
    }
	cout<<endl;
	cout << score << endl;
}
void Input(){
	if(true){
		switch(_getch()){
			case 'a':
			dir = LEFT;
			break;

			case 'd':
			dir = RIGHT;
			break;

			case 's':
			dir = DOWN;
			break;

			case 'w':
			dir = UP;
			break;
			
			case 'x':
			gameOver = true;
			break;

			default:
			dir = STOP;
			break;
		}
	}
}
void Logic(){
	int prevX = x1;
	int prevY = y1;
	int prev2X, prev2Y;
	for(int i = 1; i < nTail; i++){
		prev2X = tailX[i];
		prev2Y = tailY[i];
		tailX[i] = prevX;
		tailY[i] = prevY;
		prevX = prev2X;
		prevY = prev2Y;
	}
	switch(dir){
		case LEFT:
		x1--;
		break;
		case RIGHT:
		x1++;
		break;
		case UP:
		y1--;
		break;
		case DOWN:
		y1++;
		break;
		default:
		break;
	}
	/*===================================
	|	if the player touches wall		|
	====================================*/
	if(x1 > width-2 || x1 < 1 || y1 > height-1 || y1 < 0)
		gameOver = true;
	for(int i = 0; i < nTail; i++)
		if(tailX[i] == x1 && tailY[i] == y1)
			gameOver = true;
	/*===============================
	|	if the player eats star		|
	===============================*/
	if(x1 == starX && y1 == starY){
		nTail++;
		score += 10;
		starX = rand() % (width-2) + 1;
		starY = rand() % (height-2) + 1;
	}
}
int main(){
	srand(time(0));
	Setup();
	while(!gameOver){
		Draw();
		Input();
		Logic();
	}
	return 0;
}