//http://blog.livedoor.jp/akf0/archives/51585502.html
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <winsock2.h>

#define BOARD_MAX 15
int board[BOARD_MAX][BOARD_MAX] = {{0}};


int main(int argc, char *argv[]) {	

	//接続するサーバの情報の構造体を用意

	struct sockaddr_in dest;

	memset(&dest, 0, sizeof(dest));
	
	printf("%s\n",argv[1]);
	printf("%s\n",argv[2]);
	printf("%s\n",argv[3]);
	

	//サーバの情報を入力

	char destination[256];;  //アドレス
	
	strcpy(destination,argv[1]);
	
	printf("%s\n",destination);

	dest.sin_port = htons(atof(argv[2]));  //ポート番号

	dest.sin_family = AF_INET;

	dest.sin_addr.s_addr = inet_addr(destination);



	//ソケット通信の準備・生成

	WSADATA data;

	WSAStartup(MAKEWORD(2, 0), &data);

	SOCKET s = socket(AF_INET, SOCK_STREAM, 0);



	//サーバへの接続

	if (connect(s, (struct sockaddr *) &dest, sizeof(dest))) {

		printf("%sに接続できませんでした\n", destination);

		return -1;

	}

	printf("%sに接続しました\n", destination);

/*
	char msg[256];
	strcpy(msg,argv[3]);
	char buffer[1024];

*/

	//サーバにデータを送信

//	send(s, msg, strlen(msg), 0);

	
	while(1)
	{	
		char buffer[1024];
		//サーバからデータを受信
		
		int x = 99;
		int y = 99;

		recv(s, buffer, 1024, 0);

		printf("→ %s\n\n", buffer);
		
		char *token;
		
		token = strtok(buffer," ");
		
		printf("%s\n",token);
		
		while(token != NULL)
		{
			token = strtok(NULL,",");	
			printf("%s\n",token);
			if(isdigit(token))
			{
				if(x == 99)
				{
					x = atoi(token);	
				}
				else
				{
					y = atoi(token);	
				}
			}
		}
		
		board[x][y] = 1;
		
		char input[256];
		
		scanf("%s",input);
		
		send(s, input, strlen(input), 0);
		
		
		char *token2;
		
		token2 = strtok(input,",");
		printf("%s\n",token2);
		
		if(isdigit(token2))
		{
			x = atoi(token2);	
		
		}
		
		while(token2 != NULL)
		{
			token2 = strtok(NULL, ",");
			printf("%s\n",token2);
		
			if(isdigit(token2))
			{
				y = atoi(token2);	
			
			}
		}
		
		board[x][y] = 2;
		int i,j = 0;
		for(i = 0; i < BOARD_MAX; i++)
		{
			for(j = 0; j<BOARD_MAX; j++)
			{
				printf("%d",board[j][i]);
			}
			printf("\n");	
		}

	}

	// Windows でのソケットの終了

	closesocket(s);

	WSACleanup();



	return 0;

}