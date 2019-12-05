//http://blog.livedoor.jp/akf0/archives/51585502.html
#include "parameter.h"
#include "move.h"
#include "forbiddenMove.h"

#define END      100
#define CONTINUE 110

extern int board[BOARD_MAX][BOARD_MAX];
extern int tempBoard[BOARD_MAX][BOARD_MAX];
extern int battingFirstPlayer;

void checkBattingFirst();
int  init(char *a[],SOCKET *s);
void display();
void decisionMyPut(char *input);
int  insertBoard(char *data, int player);


int main(int argc, char *argv[]) {	

	//�z�񏉊���
	int i,j;
	for(i = 0; i < BOARD_MAX; i++){
		for(j = 0; j < BOARD_MAX; j++){
			board[i][j]     = 0;
			tempBoard[i][j] = 0;
		}
	}

	//��U����U
	checkBattingFirst();

	//�T�[�o�[�ɐڑ�
	SOCKET s;
	init(argv, &s);

	char buffer[1024];
	//�T�[�o����f�[�^����M(���O���͂𑣂����)
	recv(s, buffer, 1024, 0);
	printf("�� %s\n\n", buffer);

	//���O����
	char input[256];
	scanf("%s",input);	
	send(s, input, strlen(input), 0);

	//��U��������^�񒆂Ɏ��ł�
	if(battingFirstPlayer == AI)
	{
		char buffer[1024];
		//�T�[�o����f�[�^����M(start���󂯎��)
		recv(s, buffer, 1024, 0);
		printf("�� %s\n\n", buffer);

		char input[256];
		sprintf(input,"%d,%d", 8, 8);
		send(s, input, strlen(input), 0);
		
		insertBoard(input,AI);
		display();
	}

	while(1)
	{	
		int controller = 2;

		//�T�[�o����f�[�^����M
		char buffer[1024];
		recv(s, buffer, 1024, 0);
		printf("�� %s\n\n", buffer);
		
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

	// Windows �ł̃\�P�b�g�̏I��
	closesocket(s);
	WSACleanup();
	return 0;

}

/* ====================================================================== */
/**
 * @brief  ��U���������G�����߂�
 */
/* ====================================================================== */
void checkBattingFirst()
{
	int errorFlag = 1;

	while(errorFlag == 1)
	{
		char checkTurn[256];
		printf("black or white\n");
		scanf("%s",checkTurn);

		if(strcmp(checkTurn,"black") == 0)
		{
			battingFirstPlayer = AI;
			errorFlag = 0;
		}
		else if(strcmp(checkTurn,"white") == 0)
		{
			battingFirstPlayer = ENEMY;
			errorFlag = 0;
		}

		if(errorFlag == 1)
		{
			printf("one more input\n");
		}
	}
	
}

/* ====================================================================== */
/**
 * @brief  �T�[�o�[���ɐڑ�(TCP�ʐM)
 *
 * @param[in]     a  IP�A�h���X�E�|�[�g�ԍ�
 * @param[in,out] s  �\�P�b�g
 *
 */
/* ====================================================================== */
int init(char *a[], SOCKET *s)
{
    //�ڑ�����T�[�o�̏��̍\���̂�p��
	struct sockaddr_in dest;
	memset(&dest, 0, sizeof(dest));
	
	//�T�[�o�̏������
	char destination[256];;  //�A�h���X
	strcpy(destination,a[1]);
	dest.sin_port = htons(atof(a[2]));  //�|�[�g�ԍ�
	dest.sin_family = AF_INET;
	dest.sin_addr.s_addr = inet_addr(destination);

	//�\�P�b�g�ʐM�̏����E����
	WSADATA data;
	WSAStartup(MAKEWORD(2, 0), &data);
	*s = socket(AF_INET, SOCK_STREAM, 0);

	//�T�[�o�ւ̐ڑ�
	if (connect(*s, (struct sockaddr *) &dest, sizeof(dest))) {

		printf("%s�ɐڑ��ł��܂���ł���\n", destination);
		return -1;
	}

	printf("%s�ɐڑ����܂���\n", destination);
	
}

/* ====================================================================== */
/**
 * @brief  �Ֆʂ�\��
 */
/* ====================================================================== */
void display()
{
	int i,j = 0;
	for(i = 0; i < BOARD_MAX; i++)
	{
		for(j = 0; j<BOARD_MAX; j++)
		{
			if(board[i][j] == 0) printf("- ",board[i][j]);
			if(board[i][j] == 1) printf("o ",board[i][j]);
			if(board[i][j] == 2) printf("x ",board[i][j]);
		}
		printf("\n");	
	}
	printf("\n\n");
}

//�����̎�����߂� ��ŏ���
void decisionMyPut(char *input)
{
	int my_x, my_y = 99;
	int put_x,put_y = 99;
	while(1)
	{
		srand((unsigned int)time(NULL));
		my_x = rand() % 15 + 1;

		srand((unsigned int)time(NULL) + 1);
		my_y = rand() % 15 + 1;

		put_x = my_x -1;
		put_y = my_y -1;
		if(board[put_y][put_x] == 0){
			break;
		}
	}

	sprintf(input,"%d,%d",my_x,my_y);
}

/* ====================================================================== */
/**
 * @brief  �ł��ꂽ���board�Ɋi�[
 *
 * @param[in] data   �ł��ꂽ��
 * @param[in] player �ǂ���̃v���C���[���ł�����
 *
 * @return  �Q�[���̏I�����茋��(����)
 */
/* ====================================================================== */
int insertBoard(char *data, int player)
{
	int x = 99;
	int y = 99;
	char *token;

	//"end"�����͂��ꂽ��I��
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
		board[y-1][x-1] = player;
		tempBoard[y-1][x-1] = player;
	}

	return CONTINUE;
}

