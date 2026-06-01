#include <stdio.h> 
#include <time.h>
#include "gameio.h"
#include "keytypes.h"
#include <windows.h>//system("cls")を使うため
#include <stdlib.h>
#include  <string.h>
#define setPosition(x,y) printf("\033[%d;%dH",(y)+1,(x)*2+1) 
#define cursolOn()  printf("\033[?25h") //カーソルを表示 
#define cursolOff()  printf("\033[?25l")   //カーソルを非表示 
#define BLOCK_START 1
#define WIDTH 12   //フィールドの幅 
#define HEIGHT 24  //フィールドの高さ
#define clearScreen()  printf("\033[2J") 
//***********************************************
#define setCharColor(n) printf("\033[3%dm",n) 
#define setBackColor(n) printf("\033[4%dm",n) 
#define BLACK 0 
#define RED 1 
#define GREEN 2 
#define YELLOW 3 
#define BLUE 4 
#define MAGENTA 5 
#define CYAN 6 
#define WHITE 7 
#define DEFAULT 9 
//**************************************************
#define setAttribute(n) printf("\033[%dm",(n)) //文字の属性
#define NORMAL 0//通常 
#define BLIGHT 1 //明るく 
#define DIM 2  //暗く
#define UNDERBAR 4 //下線 
#define BLINK 5 //点滅 
#define REVERSE 7 //明暗反転 
#define HIDE 8  //隠れ(見えない) 
#define STRIKE 9 //取り消し線
//*****************************************************
#define BLOCK_SIZE 4
#define BLOCK_NUM 7 //ブロックの種類数
#define NAME_MAX 16
#define NAME_LIST 10
#define RANKING_X WIDTH+8
#define RANKING_Y 4
#define MAX_DATA_SIZE 100
typedef struct cell {
	//表示文字 
	char c;
	int charcolor;  //表示色 
	int backcolor;  //背景色 
	int attribute;  //文字属性 

}Cell;
typedef struct player {
	char playerName[NAME_MAX];
	int score;
	
}Player;
Cell map[HEIGHT][WIDTH];
void initialize(void);
void reset(void);
int checkRenge(Cell a, int x, int y);
void printNextCell(Cell c, int x, int y);
int printCell(Cell c, int x, int y);
int clearCell(Cell c, int x, int y);
int printBlock(Cell block[BLOCK_SIZE][BLOCK_SIZE], int x, int y);
void printNextBlock(Cell block[BLOCK_SIZE][BLOCK_SIZE], int x, int y);
int clearBlock(Cell block[BLOCK_SIZE][BLOCK_SIZE], int x, int y);
void copyBlock(Cell src[BLOCK_SIZE][BLOCK_SIZE], Cell dst[BLOCK_SIZE][BLOCK_SIZE]);
void rotateBlock(Cell src[BLOCK_SIZE][BLOCK_SIZE], Cell dst[BLOCK_SIZE][BLOCK_SIZE]);
int checkCell(Cell a, int x, int y);
int checkMap(Cell block[BLOCK_SIZE][BLOCK_SIZE], int x, int y);
void putMap(Cell block[BLOCK_SIZE][BLOCK_SIZE], int x, int y);
void printMap(void);
int checkLine(int y);
void deleteLine(int ys);
int deleteMap(void);
void printNext(int type);
void resetChar(void);
void printGameBox();
int savePlayData(char fname[], Player player);
void sortFdata(char fname[]);
int printRanking(char fname[], int x, int y);
void printScore(int score);
Cell block_type[BLOCK_NUM][BLOCK_SIZE][BLOCK_SIZE] = {
  '\0', RED, BLACK, NORMAL,
  '\0', RED, BLACK, NORMAL,
  ' ', RED, BLACK, REVERSE,
  '\0', RED, BLACK, NORMAL,
  '\0', RED, BLACK, NORMAL,
  ' ', RED, BLACK, REVERSE,
  ' ', RED, BLACK, REVERSE,
  '\0', RED, BLACK, NORMAL,
  '\0', RED, BLACK, NORMAL,
  ' ', RED, BLACK, REVERSE,
  '\0', RED, BLACK, NORMAL,
  '\0', RED, BLACK, NORMAL,
  '\0', RED, BLACK, NORMAL,
  '\0', RED, BLACK, NORMAL,
  '\0', RED, BLACK, NORMAL,
  '\0', RED, BLACK, NORMAL,

  '\0', BLUE, BLACK, NORMAL,
  ' ', BLUE, BLACK, REVERSE,
  ' ', BLUE, BLACK, REVERSE,
  '\0', BLUE, BLACK, NORMAL,
  '\0', BLUE, BLACK, NORMAL,
  ' ', BLUE, BLACK, REVERSE,
  '\0', BLUE, BLACK, NORMAL,
  '\0', BLUE, BLACK, NORMAL,
  '\0', BLUE, BLACK, NORMAL,
  ' ', BLUE, BLACK, REVERSE,
  '\0', BLUE, BLACK, NORMAL,
  '\0', BLUE, BLACK, NORMAL,
  '\0', BLUE, BLACK, NORMAL,
  '\0', BLUE, BLACK, NORMAL,
  '\0', BLUE, BLACK, NORMAL,
  '\0', BLUE, BLACK, NORMAL,

  '\0', GREEN, BLACK, NORMAL,
  '\0', GREEN, BLACK, NORMAL,
  '\0', GREEN, BLACK, NORMAL,
  '\0', GREEN, BLACK, NORMAL,
  '\0', GREEN, BLACK, NORMAL,
  ' ', GREEN, BLACK, REVERSE,
  ' ', GREEN, BLACK, REVERSE,
  '\0', GREEN, BLACK, NORMAL,
  ' ', GREEN, BLACK, REVERSE,
  ' ', GREEN, BLACK, REVERSE,
  '\0', GREEN, BLACK, NORMAL,
  '\0', GREEN, BLACK, NORMAL,
  '\0', GREEN, BLACK, NORMAL,
  '\0', GREEN, BLACK, NORMAL,
  '\0', GREEN, BLACK, NORMAL,
  '\0', GREEN, BLACK, NORMAL,

  '\0', YELLOW, BLACK, NORMAL,
  '\0', YELLOW, BLACK, NORMAL,


  '\0', YELLOW, BLACK, NORMAL,
  '\0', YELLOW, BLACK, NORMAL,
  '\0', YELLOW, BLACK, NORMAL,
  ' ', YELLOW, BLACK, REVERSE,
  ' ', YELLOW, BLACK, REVERSE,
  '\0', YELLOW, BLACK, NORMAL,
  '\0', YELLOW, BLACK, NORMAL,
  ' ', YELLOW, BLACK, REVERSE,
  ' ', YELLOW, BLACK, REVERSE,
  '\0', YELLOW, BLACK, NORMAL,
  '\0', YELLOW, BLACK, NORMAL,
  '\0', YELLOW, BLACK, NORMAL,
  '\0', YELLOW, BLACK, NORMAL,
  '\0', YELLOW, BLACK, NORMAL,

  '\0', CYAN, BLACK, NORMAL,
  '\0', CYAN, BLACK, NORMAL,
  '\0', CYAN, BLACK, NORMAL,
  '\0', CYAN, BLACK, NORMAL,
  ' ', CYAN, BLACK, REVERSE,
  ' ', CYAN, BLACK, REVERSE,
  ' ', CYAN, BLACK, REVERSE,
  ' ', CYAN, BLACK, REVERSE,
  '\0', CYAN, BLACK, NORMAL,
  '\0', CYAN, BLACK, NORMAL,
  '\0', CYAN, BLACK, NORMAL,
  '\0', CYAN, BLACK, NORMAL,
  '\0', CYAN, BLACK, NORMAL,
  '\0', CYAN, BLACK, NORMAL,
  '\0', CYAN, BLACK, NORMAL,
  '\0', CYAN, BLACK, NORMAL,

  '\0', MAGENTA, BLACK, NORMAL,
  '\0', MAGENTA, BLACK, NORMAL,
  '\0', MAGENTA, BLACK, NORMAL,
  '\0', MAGENTA, BLACK, NORMAL,
  ' ', MAGENTA, BLACK, REVERSE,
  ' ', MAGENTA, BLACK, REVERSE,
  ' ', MAGENTA, BLACK, REVERSE,
  '\0', MAGENTA, BLACK, NORMAL,
  '\0', MAGENTA, BLACK, NORMAL,
  ' ', MAGENTA, BLACK, REVERSE,
  '\0', MAGENTA, BLACK, NORMAL,
  '\0', MAGENTA, BLACK, NORMAL,
  '\0', MAGENTA, BLACK, NORMAL,
  '\0', MAGENTA, BLACK, NORMAL,
  '\0', MAGENTA, BLACK, NORMAL,
  '\0', MAGENTA, BLACK, NORMAL,

  '\0', RED, BLACK, NORMAL,
  ' ', RED, BLACK, REVERSE,
  '\0', RED, BLACK, NORMAL,
  '\0', RED, BLACK, NORMAL,
  '\0', RED, BLACK, NORMAL,
  ' ', RED, BLACK, REVERSE,
  '\0', RED, BLACK, NORMAL,
  '\0', RED, BLACK, NORMAL,
  '\0', RED, BLACK, NORMAL,
  ' ', RED, BLACK, REVERSE,
  ' ', RED, BLACK, REVERSE,
  '\0', RED, BLACK, NORMAL,
  '\0', RED, BLACK, NORMAL,
  '\0', RED, BLACK, NORMAL,
  '\0', RED, BLACK, NORMAL,
  '\0', RED, BLACK, NORMAL,


};

int main(void)
{

	LARGE_INTEGER frequency;
	LARGE_INTEGER start_time, now_time;
	double duration, thold = 0.5;
	int x,y,prex,prey,t,next,score,highScore,preScore,rankScore;
    char c;
	char playerName[NAME_MAX];
	score = 0;
	preScore = 0;
	highScore = 0;
	char saveDatafname[NAME_MAX] = "PlayerData";//プレイヤーネームとそのスコアを昇順に管理しているファイル
	FILE* fp2 = NULL;
	fp2 = fopen(saveDatafname, "r");
	if (fp2 != NULL) {
		fclose(fp2);
	}
	else {
		printf("file error\n");
		getchar();
	}
	Cell block[BLOCK_SIZE][BLOCK_SIZE];
	Cell block_tmp[BLOCK_SIZE][BLOCK_SIZE];
	Cell block_lotate_check[BLOCK_SIZE][BLOCK_SIZE];
	Player player;
	t = rand() % BLOCK_NUM;            //最初のブロックの種類を決定 
	x = 5;
	y = BLOCK_SIZE;
	thold = 0.5;  //落下の時間間隔 
	next = rand() % BLOCK_NUM; //次のブロックの種類を決定
	copyBlock(block_type[t], block); //最初のブロックをテンプレからコピーして準備
	initialize();//画面初期化
	QueryPerformanceFrequency(&frequency);// 開始時刻の取得
	QueryPerformanceCounter(&start_time);//前回落下時刻 最初は開始時刻 
	printBlock(block, x, y); //初期表示 
	printGameBox();
	printScore(score);
	printRanking(savePlayData, RANKING_X, RANKING_Y);
	printNext(next); //次のブロックを表示
	setBackColor(BLACK);
	setCharColor(WHITE);
	setAttribute(NORMAL);
	setPosition(0, HEIGHT + 2);
	printf("あなたの　ゲーム名を　教えてください:\n");
	scanf("%s" , playerName);
	sprintf(player.playerName, playerName);
	getchar();
	clearScreen();
	printBlock(block, x, y); //初期表示 
	printGameBox();
	printScore(score);
	printRanking(savePlayData, RANKING_X, RANKING_Y);
	printNext(next); //次のブロックを表示
	for (;1 ; )  //無限ループで落下処理を行う
	{
		
		prex = x; //前の位置を保存
		prey = y;
		if (kbhit() != 0)
		{

			c = getKey();
			if (c == KEY_LEFT) {
				if (checkMap(block, x - 1, y) == 0) {
					x--;
					
				}
			}
			else if (c == KEY_RIGHT) {
				if (checkMap(block, x + 1, y) == 0) {
					x++;
					
				}
			}
		
			else if (c == KEY_UP)
			{
				copyBlock(block, block_tmp); //ブロックをコピーして回転用に保存
				copyBlock(block, block_lotate_check);//回転後にブロックがマップ外にいないかのチェック用
				rotateBlock(block, block_lotate_check);

				 if (checkMap(block_tmp, x, y + 1) == 0&&checkMap(block_lotate_check,x,y)==0) { //回転して表示できるかチェック

					 rotateBlock(block, block_tmp); //ブロック回転 
					 clearBlock(block, x, y);         //元のブロックを画面から消去 
					 printBlock(block_tmp, x, y);     //回転したブロックを表示 
					 copyBlock(block_tmp, block);    //回転したブロックを元のブロックに上書き
					 
				}
				else {

				}
				
			
			}
			else if (c == KEY_DOWN) {
				while (checkMap(block, x, y + 1) == 0) {
					y++;
					score = score + (y - prey); //スコアを更新

					
					
					break;
				}
				
			}

			else if (c == KEY_ESC) //ESCキーが押されたら終了 
			{
				reset();
				exit(0);
			}
			else //それ以外の場合 
			{
				reset();
				exit(1);
			}
		}
		QueryPerformanceCounter(&now_time);// 経過時間（秒）を計算
        duration = (double)(now_time.QuadPart - start_time.QuadPart) / frequency.QuadPart;

		if (duration > thold) //もしも落下時間間隔以上に時間経過していたら 
		{
			printScore(score);
			printRanking(saveDatafname, RANKING_X, RANKING_Y);//ランキング表示
			start_time = now_time; //前回落下時間を現在時刻に 

			if (checkMap(block,x,y+1)==0) {   //ブロックが落下し終わっていなければ1つ落下
				printNext(next); //次のブロックを表示
				y++;
				
			}
			else {
				int line;
				if (y == BLOCK_SIZE) {
					setPosition(0, HEIGHT + 2);
					setBackColor(WHITE);
					setCharColor(RED);
					printf("ゲームオーバー\n");
					
					player.score = score;
				
					savePlayData(saveDatafname, player);//プレイヤーの名前とスコアを保存
					sortFdata(saveDatafname);
					getchar();
					reset(); //ゲームオーバーなので画面をリセットして終了 
					exit(0);
				}
				putMap(block, x, y); //ブロックをマップに登録
				line = deleteMap(); //マップをチェックしてラインを消す
				switch (line) {
				case 0: //消したライン数が0ならば 
					break;
				case 1: //消したライン数が1ならば
					score += 100; //スコアを100点加算 
					break;
				case 2: //消したライン数が2ならば
					score += 300; //スコアを300点加算 
					break;
				case 3: //消したライン数が3ならば
					score += 500; //スコアを500点加算 
					break;
				case 4: //消したライン数が4ならば
					score += 800; //スコアを800点加算 
					break;
				}
			
				printRanking(saveDatafname,RANKING_X,RANKING_Y); //スコアを表示
				
				y = BLOCK_SIZE;
				x = 5; 
				prex = 5; //前の位置を保存
				prey = 0;
				t = next; //次のブロックの種類を決定
				next = rand() % BLOCK_NUM; //次のブロックの種類を決定
				printGameBox();
				
				copyBlock(block_type[t], block); //最初のブロックをテンプレからコピーして準備
				printBlock(block, x, y); //新しいブロックを表示
				printNext(next); //次のブロックを表示
			} 
		}
		if (prex != x || prey != y)            //もしもブロックが左右移動/落下していたら 
		{
			clearBlock(block, prex, prey); //前回位置のブロックを消して 
			printBlock(block, x, y);        //新しい場所に表示 
		}
			
		
	}
	
	//ここまでが落下処理
	

	
	getchar();
	reset();
	
	return 0;
}
void initialize(void)  //画面の初期化 
{
	int x, y;
	Cell a = { '\0', BLACK, BLACK, NORMAL }; //空セルの初期化
	setBackColor(BLACK);
	setCharColor(WHITE);
	setAttribute(NORMAL);
	clearScreen();
	cursolOff();
	for (y = 0;y < HEIGHT;y++)
		for (x = BLOCK_START;x < WIDTH;x++)
			map[y][x] = a;
	//マップを空に 
}
int checkCell(Cell a, int x, int y)
{
	if (checkRange(a, x, y) !=0|| map[y][x].c != '\0')
		return -1; //失敗 
	return 0;
	//成功 
}
int checkMap(Cell block[BLOCK_SIZE][BLOCK_SIZE], int x, int y)
{
	int i, j;
	for (j = 0; j < BLOCK_SIZE; j++)
		for (i = 0; i < BLOCK_SIZE; i++) {
			if (block[j][i].c != '\0')
			{
				if (checkCell(block[j][i], x + i, y + j))
					return -1; //どれか一つでも失敗なら，ブロック全体として失敗 
			}
		}
	return 0; //全部表示できそうならば成功 
}
void reset(void)
{

	setBackColor(BLACK);
	setCharColor(WHITE);
	setAttribute(NORMAL);
	clearScreen();
	cursolOn();
}

int checkRange(Cell a, int x, int y)
{
	if (a.c == '\0' || x < BLOCK_START || y < 0 || x >= WIDTH || y >= HEIGHT)
		return -1; //失敗 
	else
		return 0;  //成功 
}
void printNextCell(Cell c, int x, int y) {
	setPosition(x, y);
	setAttribute(c.attribute);
	setBackColor(c.backcolor);
	setCharColor(c.charcolor);
	printf("%c%c", c.c, c.c);
	fflush(stdout);

}
int printCell(Cell c, int x, int y)
{
	if (checkRange(c, x, y) == -1) {
		return -1;

	}
	else {
		setPosition(x, y);
		setAttribute(c.attribute);
		setBackColor(c.backcolor);
		setCharColor(c.charcolor);
		printf("%c%c", c.c, c.c);
		fflush(stdout);
		return 0;
	}
}
void putMap(Cell block[BLOCK_SIZE][BLOCK_SIZE], int x, int y)
{
	int i, j;
	for (j = 0; j < BLOCK_SIZE; j++)
		for (i = 0; i < BLOCK_SIZE; i++) {
			if (checkCell(block[j][i], x + i, y + j) == 0)
				map[y + j][x + i] = block[j][i];
		}
}

int clearCell(Cell c, int x, int y)
{
	if (checkRange(c, x, y) == -1) {
		return -1;
	}
	else {
		setPosition(x, y);
		setAttribute(NORMAL);
		setBackColor(BLACK);
		setCharColor(BLACK);
		printf("  "); //空白2文字 
		fflush(stdout);
		return 0;
	}
}
void copyBlock(Cell src[BLOCK_SIZE][BLOCK_SIZE], Cell dst[BLOCK_SIZE][BLOCK_SIZE])
{
	int i, j;
	for (j = 0; j < BLOCK_SIZE; j++)
		for (i = 0; i < BLOCK_SIZE; i++)
			dst[j][i] = src[j][i];
}
int printBlock(Cell block[BLOCK_SIZE][BLOCK_SIZE], int x, int y)
{
	int i, j;
	for (j = 0; j < BLOCK_SIZE; j++)
		for (i = 0; i < BLOCK_SIZE; i++)
			printCell(block[j][i], i + x, j + y);
	return 0;
}
void printNextBlock(Cell block[BLOCK_SIZE][BLOCK_SIZE], int x, int y) {
	int i, j;
	for (j = 0; j < BLOCK_SIZE; j++)
		for (i = 0; i < BLOCK_SIZE; i++)
			printNextCell(block[j][i], i + x, j + y);
	return 0;
}
int clearBlock(Cell block[BLOCK_SIZE][BLOCK_SIZE], int x, int y)
{
	int i, j;
	for (j = 0; j < BLOCK_SIZE; j++)
		for (i = 0; i < BLOCK_SIZE; i++)
			clearCell(block[j][i], i + x, j + y);
	return 0;
}
void rotateBlock(Cell src[BLOCK_SIZE][BLOCK_SIZE], Cell dst[BLOCK_SIZE][BLOCK_SIZE])
{
	int i, j;
	for (j = 0; j < BLOCK_SIZE; j++)
		for (i = 0; i < BLOCK_SIZE; i++)
			dst[i][BLOCK_SIZE - 1 - j] = src[j][i];
}
void printMap(void)
{
	int x, y;
	for (y = 0;y < HEIGHT;y++) {
		for (x = BLOCK_START;x < WIDTH;x++) {

			printCell(map[y][x], x, y);
		}
	}
}
int checkLine(int y)
{
	int x;
	for (x = BLOCK_START;x < WIDTH;x++) {
		if (map[y][x].c == '\0') { //どこか一か所でも空セルがあれば 
			return -1;
		}
	}      //失敗 
	return 0;                //全部揃っている 
}

void deleteLine(int ys)
{
	int x, y;
	for (y = ys;y > 0;y--) {        //対象ラインより上のラインを繰り返し 
		for (x = BLOCK_START;x < WIDTH;x++) {    //一行全部繰り返し 
			map[y][x] = map[y - 1][x]; //一つ上のセルを下に落とす
		}
	}
	setBackColor(BLACK);
	clearScreen();              //ゲーム画面をクリアして 
	printMap();                 //マップに従って全部再描画 
}

int deleteMap(void)
{
	int y, count;
	count = 0;//消したライン数をカウント
	for (y = 0;y < HEIGHT;y++) {     //ゲーム画面の全部の行を繰り返し 
		if (checkLine(y) == 0) {     //揃ったラインがあれば 
			deleteLine(y); //一行消滅 
			count++; //消したライン数をカウント
		}
	}
	return count; //消したライン数を返す
}
void printNext(int type)
{
	int i, j;
	Cell a = { ' ',BLACK,BLACK,NORMAL };
	setPosition(0, 0);
	setAttribute(NORMAL);
	setBackColor(WHITE);
	setCharColor(BLACK);
	printf("NEXT");
	for (j = 0; j < BLOCK_SIZE; j++) {
		for (i = 0; i < BLOCK_SIZE; i++) {
			printNextCell(a, 5 + i, 0 + j);
		}
	}
	printNextBlock(block_type[type], 5, 0);
}
void printScore(int score)
{
	setPosition(WIDTH+2, 5);
	setAttribute(NORMAL);
	setBackColor(WHITE);
	setCharColor(BLACK);
	printf("Score: %d", score);
}

int savePlayData(char fname[], Player player) {
	FILE* fp = NULL;
	fp = fopen(fname, "a");
	if (fp != NULL) {
		fprintf(fp, "%s %d\n", player.playerName, player.score);
		fclose(fp);
		return 0;
	}
	else {
		return -1;
	}
}
void sortFdata(char fname[]) {
	char playerNameBox[MAX_DATA_SIZE][NAME_MAX];
	int playerScoreBox[MAX_DATA_SIZE];
	
	int count = 0;
	
	FILE* fp = NULL;
	fp = fopen(fname, "r");
	//fscanfは返り値として正しく読み取れたデータの数をかえす
	while(count<MAX_DATA_SIZE&& fscanf(fp, "%s %d", playerNameBox[count], &playerScoreBox[count]) == 2) {

		
		count++;
	}
	for (int i = 0;i < count;i++) {
		
		for (int j = i+1;j < count;j++) {
			int scoretmp;
			char nametmp[NAME_MAX];
			
			if (playerScoreBox[i] < playerScoreBox[j]) {
				scoretmp = playerScoreBox[i];//スコア入れ替え
				playerScoreBox[i] = playerScoreBox[j];
				playerScoreBox[j] = scoretmp;
				strcpy(nametmp, playerNameBox[i]);//名前入れ替え
				strcpy(playerNameBox[i], playerNameBox[j]);
				strcpy(playerNameBox[j], nametmp);
				
			}
		}

	}
	fclose(fp);
	fp = fopen(fname, "w");
	for (int i = 0;i < count;i++) {
		fprintf(fp, "%s %d\n", playerNameBox[i], playerScoreBox[i]);
	}
	fclose(fp);

}
int printRanking(char fname[], int x, int y) {
	FILE* fp = NULL;
	fp = fopen(fname, "r");
	int count = 0;
	if (fp != NULL) {
		
		setPosition(x, y);
		setAttribute(NORMAL);
		setBackColor(WHITE);
		setCharColor(BLUE);
		printf("|--------------------|");
		setPosition(x, y+1);
		setCharColor(RED);
		printf("|  スコアランキング  |");
		setPosition(x, y+2);
		setCharColor(BLUE);
		printf("+--------------------+");
		for(int i=0;count<NAME_LIST;i=i+2) {
			count++;
			int score = 0;
			char playerName[NAME_MAX];
			

			if (fscanf(fp, "%s %d", playerName, &score) == 2) {
				setPosition(x, y + 3 + i);
				setCharColor(BLACK);
				printf("|%d位%10s   %4d|\n", count, playerName, score);
				setPosition(x, y + 4 + i);
				setCharColor(BLUE);
				printf("+--------------------+");
			}
			
		}
		fclose(fp);
		return 0;
	}
	else {
		return -1;
	}
}
void resetChar(void) {
	setBackColor(BLACK);
	setCharColor(WHITE);
	setAttribute(NORMAL);

}


void printGameBox() {
	setBackColor(BLACK);
	setCharColor(WHITE);
	setAttribute(REVERSE);
	for (int i = 4;i < HEIGHT;i++) {
		setPosition(0, i);
		printf("  ");
		setPosition(WIDTH, i);
		printf("  ");
	}
	for (int i = 0;i < WIDTH + 1;i++) {
		setPosition(i, HEIGHT);
		printf("  ");
	}
	resetChar();
}
