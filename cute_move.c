#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <curses.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#define MAX 100


// 노드 구조체 선언
typedef struct node * nodePointer;
typedef struct node {
	nodePointer right,left;
	char str[MAX]; // 출력 문자
	int row,col; // 출력 행열
	int mode; // 출력 모드
}node;

// 전역 변수
int hp = 5;
char hp_str[2]="";
int score = 0;
char score_str[MAX]="";
int i = 0; 
int nodeCnt = 0;
nodePointer ptr = NULL;
char enterText[20] = { 0 };
int enterHere = 0;
int sleep_time = 1;
int sleep_cnt=0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

// 지렁이 움직임에 쓰는 변수
int move_cute = 0;
int cute_mode = 0;
char cute1[] = "COCOCOCOCOCOCOCOD";
char cute2_1[] = "   COCO   ";
char cute2_2[] = "   O  O   ";
char cute2_3[] = "   O  O   ";
char cute2_4[] = "COCO  OCOCD";

char blank1[] = "                 ";
char blank2_1[] = "    ";
char blank2_2[] = "    ";
char blank2_3[] = "    ";
char blank2_4[] = "           ";

// 사용함수들
void reset();
void *thread_1();
void checkWord(char *str);
nodePointer makeNode();
void nextLine();
void addQ(char *str, int col);
char * makeWord();
void delete(int row, int col, char *str);
void draw(int row, int col, char* str);
void function(int);
void game();
void cute(int move);
int cute_pos_comp(int row, int col, int len);

// 메인 함수
void main() {
	
	char option;
	
	// 초기화
	initscr();
	clear();
	
	// signal 처리
	signal(SIGQUIT,function);

	move(3,10);
	addstr("******************************************************");
	move(4,10);
	addstr("*                                                    *");
	move(5,10);	
	addstr("*                   TYPING GAME                      *");
	move(6,10);	
	addstr("*                                                    *");
	move(7,10);	
	addstr("*                                                    *");
	move(8,10);	
	addstr("******************************************************");
	move(11,10);	
	addstr("1. Start Game					      ");
	move(12,10);	
	addstr("2. User score					      ");
	move(13,10);	
	addstr("3. EXIT  					      ");
	
	move(16,20);
	refresh();

	// 입력 받기
	option = getch();
	
	// option 값 따라
	switch(option){
		// 게임하기
		case '1':
			// 게임
			while(hp>0){
				game();
			}
			break;

		// 종료하기
      		case '3':
   			break;
	}	
	// 초기화	
	reset();
	endwin();
	clear();
}


// 게임 동작 함수
void game(){
	
	
	pthread_t t1,t2;
	
	// 초기화
	initscr();
	clear();


	// default setting
	move(0,70);
	addstr("*username");

	// score print
	move(1,1);
	addstr("score : ");
	sprintf(score_str,"%d",score);
	addstr(score_str);
	refresh();

	// hp print
	move(0,1);
	addstr("life  : ");
	sprintf(hp_str,"%d",hp);
	addstr(hp_str);
	refresh();

	// 지렁이
	move(19, 80);
	addstr("COCOCOCOCOCOCOCOD");
	refresh();

	// enter setting
	draw(20, 20, "	| Enter | : ");

	// 쓰레드 생성
	pthread_create(&t1, NULL, thread_1, NULL);
	
	// hp > 0 이상일 동안 반복
	while (hp > 0) {		
		
		// 시작위치 초기화
		enterHere = 0;

		// 반복문
		for (enterHere = 0; enterHere < 20;) {
			int c = getch();
			
			if(enterHere >= 19 && c !='\n'){
				move(20, 36);
				addstr(enterText);			
				continue;
			}

			// enter 들어오면 문자열 찾아서 삭제
			else if (c == '\n') {
				enterText[enterHere] = '\0';
				checkWord(enterText);

				delete(20, 36, enterText);						// enter로 입력한 내용 삭제

				for (i = 0; i < 20; i++)
					enterText[i] = '\0';

				draw(20, 20, "	| Enter | : ");
				move(20, 36);

				break;
			}
			// backspace 들어오면 문자 하나 삭제
			else if (c == 127) {
				// 남아있으면 삭제
				if (enterHere > 0) {
					enterText[--enterHere] = '\0';
					move(20, 36);
					addstr("		");
					move(20, 36);
					addstr(enterText);
				}
				// 없으면 빈문자열 출력
				else {
					move(20, 36);
					addstr("		");
				}
			}

			// 입력이 되면 하나추가해서 보여주기
			else {
				enterText[enterHere++] = c;
				move(20, 36);
				addstr(enterText);
			}
			
			refresh();
		}		
	}

	// 쓰레드 조인
	pthread_join(t1, NULL);

	// 초기화	
	reset();
	clear();
}

// 강제 종료시 함수 종료 처리
void function(int signum){
	reset();
	curs_set(1);
	endwin();
	exit(1);
}

// node free 함수
void reset() {
	nodePointer temp = NULL;
	nodePointer temp2 = NULL;

	temp = ptr;

	if (temp != NULL) {
		// 개수 남아있으면 삭제하기
		while (nodeCnt > 0) {
			temp = ptr;

			while (temp->right) {
				temp2 = temp;
				temp = temp->right;
			}

			free(temp); // 비워주기

			if (temp2 != NULL)
				temp2->right = NULL;

			nodeCnt--; // 개수 감소
			temp2 = NULL;
		}
		ptr = NULL;
	}
}

// 단어 출력 쓰레드
void * thread_1()
{
	int t = sleep_time;
	nodePointer temp = NULL;
	nodePointer del = NULL;	
	int check = 0;									// cute_pos_comp함수의 값을 저장할 변수
	
	while (hp > 0) {	
		sleep_cnt++;

		// 3칸마다 문자열 생성
		switch(sleep_cnt){
			case 0:
				addQ("",0);
				break;
			case 1:
				addQ("",0);
				break;
			case 2:
				addQ(makeWord(), (rand() % 40) + 12);			// score가 10의자리일때 8번째 칸
				sleep_cnt=0;
		}
		temp = ptr;
		

		// 문자열 전부 출력
		while (temp) {
			check = cute_pos_comp(temp->row, temp->col, strlen(temp->str));
			
			// 빈 문자열 아닌 애들만 출력
			if (temp->row < 19 && strcmp(temp->str, "")) {
				delete(temp->row-1, temp->col, temp->str);
				if (!check) {									// 지렁이랑 안겹치면 그린다.
					draw(temp->row, temp->col, temp->str);	
				}
			}

			else if(temp->row < 19 && !strcmp(temp->str,"")){
				delete(temp->row, temp->col, "");				// 공백이 존재하면 그게 지렁이랑 겹치면 
				draw(temp->row,temp->col,"");					// 지렁이 지워져서 빈문자열로 바꿈
			}
			
			// row가 18 이상이면 없애고 점수까기
			// 또는 지렁이랑 겹치면 점수까기 (위 과정에서 이미 그러지 않음)
			if(temp->row >=19 && strcmp(temp->str,"") || check){
				delete(temp->row-1, temp->col, temp->str);		// 기준점 도달 시 문자 안보이게
				
				pthread_mutex_lock(&lock); // 제어
				// 삭제
				del =temp;				
				temp->left->right = NULL;
				temp = temp->right;
				free(del); 

				// hp print
				hp--;
				pthread_mutex_unlock(&lock); // 제어 unlock

				move(0,1);
				addstr("life  : ");
				sprintf(hp_str,"%d",hp);
				addstr(hp_str);
				refresh();
			}
			
			// row 18 이상이고 빈문자열일 경우
			else if(temp->row >= 19){
				pthread_mutex_lock(&lock); // 제어
				// 삭제
				del =temp;				
				temp->left->right = NULL;
				temp = temp->right;
				free(del); 
				pthread_mutex_unlock(&lock); // 제어 unlock

			}

			else
				temp = temp->right;
		}

		move(20, 36); // 커서 이동
		sleep(t); // 대기
		
	}
}

// 해당 단어 찾기 함수
void checkWord(char *str) {

	nodePointer temp = NULL;
	temp = ptr;
	
	// 맨끝으로 가기
	while(temp->right)
		temp=temp->right;
	// 밑에서부터 탐색
	while (temp) {
		if(!strcmp(str,""))
			return;

		// 같은 걸 찾으면 빈문자열로 바꿔준 후 종료
		if (!strcmp(temp->str, str)) {
			delete(temp->row, temp->col, temp->str);		// 빈 문자로 바꾸기 전에 삭제
			strcpy(temp->str,"");
			
			score++;

			cute(0);						 // 찾았으니깐 지렁이 움직이기
			
			// score print
			move(1,1);
			addstr("score : ");
			sprintf(score_str, "%d", score);
			addstr(score_str);
			refresh();
			return;
		}
		// 다음 노드
		else
			temp = temp->left;
	}

}

// 노드 만들기 함수
nodePointer makeNode() {	
	nodePointer temp;

	temp = (nodePointer)malloc(sizeof(*temp));
	temp->right = temp->left = NULL;
	return temp;
}

// 한줄 씩 내리는 함수
void nextLine() {

	nodePointer temp = ptr->right;

	// row 1 증가
	while (temp) {
		temp->row += 1;
		temp = temp->right;
	}
}

// 단어 추가하는 함수
void addQ(char *str, int col) {

	nodePointer temp, temp2;

	// 첫 노드 일 경우, 단어 추가
	if (ptr == NULL) {
		ptr = makeNode();
		strcpy(ptr->str, str); // 받은 문자열로 단어 만들기
		ptr->row = 2;
		ptr->col = col;
	}

	// 단어 추가
	else {
		temp = makeNode();
		strcpy(temp->str, str); // 받은 문자열로 단어 만들기
		temp->row = 2;
		temp->col = col;
		temp->right = ptr; // 양방향 연결
		ptr->left = temp; // 양방향 연결
		ptr = temp;
		nextLine();
	}
	nodeCnt++;
}

// 랜덤으로 문자열 반환하기
char * makeWord() {

	char * DB[] = { "apple","jung","cocaine","hello","elite","fail","game",
				"halo","icon","jail","knight","lake","monkey","nope" };

	return DB[rand() % 14];
}

// 문자열 이전위치 지우기 함수
void delete(int row, int col, char *str) {
	char *del;
	int i;

	del = (char *)malloc(sizeof(char) * strlen(str));

	for (i = 0; i < strlen(str); i++)
		del[i] = ' ';

	move(row, col);
	addstr(del);

	move(20, 36);

	refresh();
}

// 그리기 함수
void draw(int row, int col, char* str) {
	move(row, col);
	addstr(str);

	move(20, 36);

	refresh();
}

// 지렁이 움직이는 함수
void cute(int move) {
	if (move_cute % 2 == 0) {
		move(19, 80 -  (move_cute / 2));				// (move_cute / 2) 앞에 숫자 곱해서 원하는 만큼 이동 가능		
		addstr(blank1);
		refresh();

		if(move == 0)
			move_cute++;

		move(19, 80 -  (move_cute / 2));
		addstr(cute2_4);
		move(18, 80 -  (move_cute / 2));
		addstr(cute2_3);
		move(17, 80 -  (move_cute / 2));
		addstr(cute2_2);
		move(16, 80 -  (move_cute / 2));
		addstr(cute2_1);

		move(20, 36);								// 커서 위치때문에 지렁이가 
																		// 사라지는 경우도 있어서 항상 이 위치로 이동
		cute_mode = 1;
	}

	else {
		move(19, 80 -  (move_cute / 2));
		addstr(blank2_4);
		move(18, 83 -  (move_cute / 2));
		addstr(blank2_3);
		move(17, 83 -  (move_cute / 2));
		addstr(blank2_2);
		move(16, 83 -  (move_cute / 2));
		addstr(blank2_1);
		refresh();

		if(move == 0)
			move_cute++;

		move(19, 80 -  (move_cute / 2));
		addstr(cute1);

		move(20, 36);

		cute_mode = 0;
	}

	refresh();
}

// 단어 위치와 지렁이 위치 비교 함수
int cute_pos_comp(int row, int col, int len) {
	int cute_col_1 = 80 -  (move_cute / 2);
	int cute_col_2 = 83 -  (move_cute / 2);
	int check = 0;																// 단어랑 지렁이랑 겹치면 1
	int i;																		// 단어의 일부라도 겹치는 걸 확인하기 위해 

	if (cute_mode == 0) {
		if (row >= 19) {														// 쭉 편 상태
			for (i = 0; i < len; i++) {
				if ((col + i >= cute_col_1) && (col + i < cute_col_1 + 17)) {
					check = 1;
					break;
				}
			}
		}
	}

	if (cute_mode == 1) {
		if (row >= 16) {														// 쪼그린 상태
			for (i = 0; i < len; i++) {
				if ((col + i >= cute_col_2) && (col + i < cute_col_2 + 4)) {	// 윗부분
					check = 1;
					break;
				}
			}

			if (row >= 18) {													// 아랫부분
				for (i = 0; i < len; i++) {
					if ((col >= cute_col_1) && (col < (cute_col_1 + 11))) {
						check = 1;
						break;
					}
				}
			}
		}
	}

	return check;
}
