#include <stdio.h>
#include <unistd.h>
#include <winsock2.h>

#include <string.h>
#include <ctype.h>
#include <stdlib.h>



// display 

#define BOARD_SQUARE 15
int board[BOARD_SQUARE][BOARD_SQUARE] = {{0}};  

int displayBoard(char *str, int turn){
    

    char *token;
    token=strtok(str,",");

    if(token==NULL){
        printf("error msg=%d\n",str);
        return 0;
    }

    int x = atoi(token);
    
    if( x<1 || x>BOARD_SQUARE) {
        printf("error x=%d\n",x);
        return 0;
    }

    token=strtok(NULL,",");
 
    if(token==NULL){
        printf("error msg=%d\n",str);
        return 0;
    }

   int y = atoi(token);

    if( y<1 || y>BOARD_SQUARE) {
        printf("error y=%d\n",y);
        return 0;
    }

    if(board[x-1][y-1]!=0){
        printf("already put\n");
        return 0;
    }
    board[x-1][y-1] = turn;
    
    int i=0;
    printf("  ");
    for(i = 1; i < BOARD_SQUARE+1; i++ ){
        printf("%2d",i);
    }
    puts("");


    int j=0;
    for(i = 1; i < BOARD_SQUARE+1; i++ ){        
        printf("%2d",i);        
        for(j = 1; j < BOARD_SQUARE+1; j++ ){
            
	    if(board[j-1][i-1]==0) printf(" -");
	    if(board[j-1][i-1]==1) printf(" o");
	    if(board[j-1][i-1]==2) printf(" x");

        }
        puts("");
    }
    puts("");

    token=strtok(NULL,",");

    if(token!=NULL){
        printf("recieved judge = %s\n",token);
        return 0;
    }

    Sleep(2000);
    return 1;
    
}


int main(int argc, char* argv[]){

  SOCKET s, s1;         //�\�P�b�g
  int result;          //�߂�l

  //�ڑ���������N���C�A���g�[���̏��

  struct sockaddr_in source;
  char name1[256], name2[256];

  memset(name1, '\0', sizeof(name1));

  //���M���̒[������o�^����

  memset(&source, 0, sizeof(source));
  source.sin_family = AF_INET;

  //�|�[�g�ԍ��̓N���C�A���g�v���O�����Ƌ���

  source.sin_port = htons(12345);
  source.sin_addr.s_addr = htonl(INADDR_ANY);

  //�\�P�b�g�ʐM�̊J�n����
  WSADATA data;
  result = WSAStartup(MAKEWORD(2, 0), &data);
  if (result < 0){
    printf("%d\n", GetLastError());
    printf("�\�P�b�g�ʐM�����G���[\n");
  }



  //�\�P�b�g�̐���
  s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (s < 0){
    printf("%d\n", GetLastError());
    printf("�\�P�b�g�����G���[\n");
  }

  //�\�P�b�g�̃o�C���h
  result = bind(s, (struct sockaddr *)&source, sizeof(source));
  if (result < 0){
    printf("%d\n", GetLastError());
    printf("�o�C���h�G���[\n");
  }

  //�ڑ��̋���
  result = listen(s, 1);
  if (result < 0){
    printf("�ڑ����G���[\n");
  }


  printf("wait for the first client ... (port 12345)\n");



  //�N���C�A���g����ʐM������܂őҋ@
  s1 = accept(s, NULL, NULL);
  if (s1 < 0){
    printf("�ҋ@�G���[\n");
  }

  //�N���C�A���g�փf�[�^�𑗐M����
  result = send(s1, "player1 name =", 20, 0);

  //�N���C�A���g���瑗�M���ꂽ�f�[�^�̎�M
  result = recv(s1, name1, 20, 0);
  if (result < 0){
    printf("��M�G���[\n");
  }

  printf("%s����M���܂���\n", name1);



  SOCKET s2, s3;         //�\�P�b�g

  //�ڑ���������N���C�A���g�[���̏��

  struct sockaddr_in source2;

  memset(name2, '\0', sizeof(name2));

  //���M���̒[������o�^����

  memset(&source2, 0, sizeof(source2));
  source2.sin_family = AF_INET;

  //�|�[�g�ԍ��̓N���C�A���g�v���O�����Ƌ���

  source2.sin_port = htons(12346);
  source2.sin_addr.s_addr = htonl(INADDR_ANY);

  //�\�P�b�g�ʐM�̊J�n����
  WSADATA data2;
  result = WSAStartup(MAKEWORD(2, 0), &data2);
  if (result < 0){
    printf("%d\n", GetLastError());
    printf("�\�P�b�g�ʐM�����G���[\n");
  }

  //�\�P�b�g�̐���
  s2 = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (s2 < 0){
    printf("%d\n", GetLastError());
    printf("�\�P�b�g�����G���[\n");
  }

  //�\�P�b�g�̃o�C���h
  result = bind(s2, (struct sockaddr *)&source2, sizeof(source2));
  if (result < 0){
    printf("%d\n", GetLastError());
    printf("�o�C���h�G���[\n");
  }

  //�ڑ��̋���
  result = listen(s2, 1);
  if (result < 0){
    printf("�ڑ����G���[\n");
  }


  printf("wait for the second client ... (port 12346)\n");



  //�N���C�A���g����ʐM������܂őҋ@
  s3 = accept(s2, NULL, NULL);
  if (s3 < 0){
    printf("�ҋ@�G���[\n");
  }

  //�N���C�A���g�փf�[�^�𑗐M����
  result = send(s3, "player2 name=", 20, 0);

  //�N���C�A���g���瑗�M���ꂽ�f�[�^�̎�M
  result = recv(s3, name2, 20, 0);
  if (result < 0){
    printf("��M�G���[\n");
  }

  printf("%s����M���܂���\n", name2);


  char buffer[256] = "start";  //��M�f�[�^�̃o�b�t�@�̈�

  while(1){

    result = send(s1, buffer, 10, 0);
    //buffer[0] = '\0';
    memset(&buffer, '\0', sizeof(buffer));

    result = recv(s1, buffer, 10, 0);

    if (result < 0){
      printf("��M�G���[\n");
      break;
    }

    printf("%s����M���܂���(%s)\n", buffer, name1);


    char buffer1[256];
    strcpy(buffer1,buffer);
    if(!displayBoard(buffer1,1)){
      printf("�Q�[���Z�b�g\n");
      break;
    }
  
    result = send(s3, buffer, 10, 0);
    //buffer[0] = '\0';
    memset(&buffer, '\0', sizeof(buffer));
    result = recv(s3, buffer, 10, 0);

    if (result < 0){
      printf("��M�G���[\n");
      break;
    }

    printf("%s����M���܂���(%s)\n", buffer, name2);

    char buffer2[256];
    strcpy(buffer2,buffer);

    if(!displayBoard(buffer2,2)){
      printf("�Q�[���Z�b�g\n");
      break;
    }



  }

  result = send(s1, "end", 10, 0);
  result = send(s3, "end", 10, 0);

  printf("�ڑ��I��\n");
  closesocket(s1);
  closesocket(s3);


  //�\�P�b�g�ʐM�̏I��
  WSACleanup();

  printf("END\n");


  return 0;

}

