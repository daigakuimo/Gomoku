//http://blog.livedoor.jp/akf0/archives/51585502.html
#include "parameter.h"
#include "move.h"
#include "forbiddenMove.h"

#define END      100
#define CONTINUE 110

extern int board[BOARD_MAX][BOARD_MAX];
extern int tempBoard[BOARD_MAX][BOARD_MAX];

int init(char *a[],SOCKET *s);

int main(int argc, char *argv[]) {

    // SOCKET s;

    // //init(argv,&s);	

	// char buffer[1024];
	// //サーバからデータを受信(名前入力を促される)
	// recv(s, buffer, 1024, 0);
	// printf("→ %s\n\n", buffer);

	// //名前入力
	// char input[256];
	// scanf("%s",input);	
	// send(s, input, strlen(input), 0);

	// // Windows でのソケットの終了
	// closesocket(s);
	// WSACleanup();

	//配列初期化
	int i,j;
	for(i = 0; i < BOARD_MAX; i++){
		for(j = 0; j < BOARD_MAX; j++){
			board[i][j]     = 0;
			tempBoard[i][j] = 0;
			
		}
	}

	checkBattingFirst();

	int b[BOARD_MAX][BOARD_MAX] = 
		{
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,2,0,0,0,1,0,0,1,0,0,0,0},
			{0,0,0,0,0,1,0,0,0,1,0,0,0,0,0},
			{0,0,0,0,1,0,0,2,0,2,1,0,0,0,0},
			{0,0,0,0,1,0,1,2,1,0,0,0,0,0,0},
			{0,0,0,0,2,1,2,2,0,0,2,2,0,0,0},
			{0,0,0,0,1,0,2,2,2,1,0,0,0,0,0},
			{0,0,0,0,0,1,0,0,0,2,0,0,0,0,0},
			{0,0,0,0,0,0,0,2,0,0,2,0,0,0,0},
			{0,0,0,0,0,0,0,1,0,0,0,2,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,1,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
			{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		}; 

	
	for(i = 0; i < BOARD_MAX; i++){
		for(j = 0; j < BOARD_MAX; j++){
			board[i][j]     = b[i][j];
			tempBoard[i][j] = b[i][j];
			
		}
	}

	display(0);


	char input[256];
	move(input);



	return 0;

}

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