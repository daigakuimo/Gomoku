//http://blog.livedoor.jp/akf0/archives/51585502.html
#include "move.h"
#include "forbiddenMove.h"
#include "parameter.h"

#define END      100
#define CONTINUE 110

extern int board[BOARD_MAX][BOARD_MAX];
extern int tempBoard[BOARD_MAX][BOARD_MAX];


int  init(char *a[],SOCKET *s);
void display();
void decisionMyPut(char *input);
int  insertBoard(char *data, int player);


int main(int argc, char *argv[]) {	

	//配列初期化
	int i,j;
	for(i = 0; i < BOARD_MAX; i++){
		for(j = 0; j < BOARD_MAX; j++){
			board[i][j]     = 0;
			tempBoard[i][j] = 0;
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
		
		controller = insertBoard(buffer,ENEMY);
		display();

		if(controller == END){
			printf("you lose\n");
			break;
		}
		
		char input[256];
		decisionMyPut(input);
		send(s, input, strlen(input), 0);
		
		insertBoard(input,AI);
		display();

	}

	// Windows でのソケットの終了
	closesocket(s);
	WSACleanup();
	return 0;

}

/* ====================================================================== */
/**
 * @brief  サーバー側に接続(TCP通信)
 *
 * @param[in]     a  IPアドレス・ポート番号
 * @param[in,out] s  ソケット
 *
 */
/* ====================================================================== */
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

/* ====================================================================== */
/**
 * @brief  盤面を表示
 */
/* ====================================================================== */
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

//自分の手を決める 後で消す
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

/* ====================================================================== */
/**
 * @brief  打たれた手をboardに格納
 *
 * @param[in] data   打たれた手
 * @param[in] player どちらのプレイヤーが打ったか
 *
 * @return  ゲームの終了判定結果(整数)
 */
/* ====================================================================== */
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
	
	if(x != 99)
	{
		board[x-1][y-1] = player;
		tempBoard[x-1][y-1] = player;
	}

	return CONTINUE;
}