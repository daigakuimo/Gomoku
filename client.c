//http://blog.livedoor.jp/akf0/archives/51585502.html
#include "parameter.h"
#include "move.h"
#include "forbiddenMove.h"

extern int board[BOARD_MAX][BOARD_MAX];
extern int tempBoard[BOARD_MAX][BOARD_MAX];
extern int battingFirstPlayer;
extern int turnCount;
extern int winFlag;

int  init(char *a[],SOCKET *s);


int main(int argc, char *argv[]) {	

	//配列初期化
	int i,j;
	for(i = 0; i < BOARD_MAX; i++){
		for(j = 0; j < BOARD_MAX; j++){
			board[i][j]     = 0;
			tempBoard[i][j] = 0;
		}
	}

	//先攻か後攻
	checkBattingFirst();

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

	//先攻だったら真ん中に手を打つ
	if(battingFirstPlayer == AI)
	{
		char buffer[1024];
		//サーバからデータを受信(startを受け取る)
		recv(s, buffer, 1024, 0);
		printf("→ %s\n\n", buffer);

		char input[256];
		sprintf(input,"%d,%d", 8, 8);
		send(s, input, strlen(input), 0);
		
		insertBoard(input,AI);
		display(1);

		turnCount;
	}

	while(1)
	{	
		int controller = 2;

		//サーバからデータを受信
		char buffer[1024];
		recv(s, buffer, 1024, 0);
		printf("→ %s\n\n", buffer);
		
		controller = insertBoard(buffer,ENEMY);
		display(1);

		turnCount++;

		if(controller == END){
			printf("you lose\n");
			break;
		}
		
		char input[256];
		move(input);
		send(s, input, strlen(input), 0);
		
		char* tempInput;

		tempInput = input;

		printf("%s\n",tempInput);

		insertBoard(input,AI);
		display(1);

		turnCount++;

		if(winFlag == 1){
			printf("you win\n");
			break;
		}
		
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

