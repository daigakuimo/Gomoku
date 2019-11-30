//http://blog.livedoor.jp/akf0/archives/51585502.html
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <time.h>
#include <winsock2.h>
#include "move.h"
#include "forbiddenMove.h"
#include "parameter.h"

#define END 1
#define CONTINUE 2

extern int board[BOARD_MAX][BOARD_MAX];


int  init(char *a[],SOCKET *s);
void display();
void decisionMyPut(char *input);
int  insertBoard(char *data, int player);


int main(int argc, char *argv[]) {	

	//配列初期化
	int i,j;
	for(i = 0; i < BOARD_MAX; i++){
		for(j = 0; j < BOARD_MAX; j++){
			board[i][j] = 0;
		}
	}

	//サーバーに接続
	SOCKET s;
	init(argv, &s);

	char buffer[1024];
	//サーバからデータを受信(名前入力を促される)
	recv(s, buffer, 1024, 0);
	printf("→ %s\n\n", buffer);

	//名前入力
	char input[256];
	scanf("%s",input);	
	send(s, input, strlen(input), 0);
	
	while(1)
	{	
		int controller = 2;

		//サーバからデータを受信
		char buffer[1024];
		recv(s, buffer, 1024, 0);
		printf("→ %s\n\n", buffer);
		
		controller = insertBoard(buffer,1);
		display();

		if(controller == END){
			printf("you lose\n");
			break;
		}
		
		char input[256];
		decisionMyPut(input);
		send(s, input, strlen(input), 0);
		
		insertBoard(input,2);
		display();

	}

	// Windows でのソケットの終了
	closesocket(s);
	WSACleanup();
	return 0;

}

//サーバ接続
int init(char *a[], SOCKET *s)
{
    //接続するサーバの情報の構造体を用意
	struct sockaddr_in dest;
	memset(&dest, 0, sizeof(dest));
	
	//サーバの情報を入力
	char destination[256];;  //アドレス
	strcpy(destination,a[1]);
	dest.sin_port = htons(atof(a[2]));  //ポート番号
	dest.sin_family = AF_INET;
	dest.sin_addr.s_addr = inet_addr(destination);

	//ソケット通信の準備・生成
	WSADATA data;
	WSAStartup(MAKEWORD(2, 0), &data);
	*s = socket(AF_INET, SOCK_STREAM, 0);

	//サーバへの接続
	if (connect(*s, (struct sockaddr *) &dest, sizeof(dest))) {

		printf("%sに接続できませんでした\n", destination);
		return -1;
	}

	printf("%sに接続しました\n", destination);
	
}

//盤面を表示
void display()
{
	int i,j = 0;
	for(i = 0; i < BOARD_MAX; i++)
	{
		for(j = 0; j<BOARD_MAX; j++)
		{
			if(board[j][i] == 0) printf("- ",board[j][i]);
			if(board[j][i] == 1) printf("o ",board[j][i]);
			if(board[j][i] == 2) printf("x ",board[j][i]);
		}
		printf("\n");	
	}
	printf("\n\n");
}

//自分の手を決める
void decisionMyPut(char *input)
{
	int my_x, my_y = 99;
	while(1)
	{
		srand((unsigned int)time(NULL));
		my_x = rand() % 15 + 1;

		srand((unsigned int)time(NULL) + 1);
		my_y = rand() % 15 + 1;

		if(board[my_y - 1][my_x - 1] == 0){
			break;
		}
	}

	sprintf(input,"%d,%d",my_x,my_y);
}

//手をboard[][]に入力
int insertBoard(char *data, int player)
{
	int x = 99;
	int y = 99;
	char *token;

	//"end"が入力されたら終了
	if(strcmp(data,"end") == 0){
		return END;
	}
		
	token = strtok(data,",");	
	x = atoi(token);
		
	while(token != NULL)
	{
		token = strtok(NULL,",");	
		y = atoi(token);
		break;
	}
	
	if(x != 99) board[x-1][y-1] = player;

	return CONTINUE;
}