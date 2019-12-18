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
		display(1);

		turnCount;
	}

	while(1)
	{	
		int controller = 2;

		//�T�[�o����f�[�^����M
		char buffer[1024];
		recv(s, buffer, 1024, 0);
		printf("�� %s\n\n", buffer);
		
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

	// Windows �ł̃\�P�b�g�̏I��
	closesocket(s);
	WSACleanup();
	return 0;

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

