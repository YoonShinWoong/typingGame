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


// ��� ����ü ����
typedef struct node * nodePointer;
typedef struct node {
	nodePointer right,left;
	char str[MAX]; // ��� ����
	int row,col; // ��� �࿭
	int mode; // ��� ���
}node;

// ���� ����
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

// ������ �����ӿ� ���� ����
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

// ����Լ���
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

// ���� �Լ�
void main() {
	
	char option;
	
	// �ʱ�ȭ
	initscr();
	clear();
	
	// signal ó��
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

	// �Է� �ޱ�
	option = getch();
	
	// option �� ����
	switch(option){
		// �����ϱ�
		case '1':
			// ����
			while(hp>0){
				game();
			}
			break;

		// �����ϱ�
      		case '3':
   			break;
	}	
	// �ʱ�ȭ	
	reset();
	endwin();
	clear();
}


// ���� ���� �Լ�
void game(){
	
	
	pthread_t t1,t2;
	
	// �ʱ�ȭ
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

	// ������
	move(19, 80);
	addstr("COCOCOCOCOCOCOCOD");
	refresh();

	// enter setting
	draw(20, 20, "	| Enter | : ");

	// ������ ����
	pthread_create(&t1, NULL, thread_1, NULL);
	
	// hp > 0 �̻��� ���� �ݺ�
	while (hp > 0) {		
		
		// ������ġ �ʱ�ȭ
		enterHere = 0;

		// �ݺ���
		for (enterHere = 0; enterHere < 20;) {
			int c = getch();
			
			if(enterHere >= 19 && c !='\n'){
				move(20, 36);
				addstr(enterText);			
				continue;
			}

			// enter ������ ���ڿ� ã�Ƽ� ����
			else if (c == '\n') {
				enterText[enterHere] = '\0';
				checkWord(enterText);

				delete(20, 36, enterText);						// enter�� �Է��� ���� ����

				for (i = 0; i < 20; i++)
					enterText[i] = '\0';

				draw(20, 20, "	| Enter | : ");
				move(20, 36);

				break;
			}
			// backspace ������ ���� �ϳ� ����
			else if (c == 127) {
				// ���������� ����
				if (enterHere > 0) {
					enterText[--enterHere] = '\0';
					move(20, 36);
					addstr("		");
					move(20, 36);
					addstr(enterText);
				}
				// ������ ���ڿ� ���
				else {
					move(20, 36);
					addstr("		");
				}
			}

			// �Է��� �Ǹ� �ϳ��߰��ؼ� �����ֱ�
			else {
				enterText[enterHere++] = c;
				move(20, 36);
				addstr(enterText);
			}
			
			refresh();
		}		
	}

	// ������ ����
	pthread_join(t1, NULL);

	// �ʱ�ȭ	
	reset();
	clear();
}

// ���� ����� �Լ� ���� ó��
void function(int signum){
	reset();
	curs_set(1);
	endwin();
	exit(1);
}

// node free �Լ�
void reset() {
	nodePointer temp = NULL;
	nodePointer temp2 = NULL;

	temp = ptr;

	if (temp != NULL) {
		// ���� ���������� �����ϱ�
		while (nodeCnt > 0) {
			temp = ptr;

			while (temp->right) {
				temp2 = temp;
				temp = temp->right;
			}

			free(temp); // ����ֱ�

			if (temp2 != NULL)
				temp2->right = NULL;

			nodeCnt--; // ���� ����
			temp2 = NULL;
		}
		ptr = NULL;
	}
}

// �ܾ� ��� ������
void * thread_1()
{
	int t = sleep_time;
	nodePointer temp = NULL;
	nodePointer del = NULL;	
	int check = 0;												// cute_pos_comp�Լ��� ���� ������ ����
	char test[10] = "";
	char test2[10] = "";
	char test3[10] = "";
	while (hp > 0) {	
		sleep_cnt++;

		// 3ĭ���� ���ڿ� ����
		switch(sleep_cnt){
			case 0:
				addQ("",0);
				break;
			case 1:
				addQ("",0);
				break;
			case 2:
				addQ(makeWord(), (rand() % 40) + 12);			// score�� 10���ڸ��϶� 8��° ĭ
				sleep_cnt=0;
		}
		temp = ptr;
		

		// ���ڿ� ���� ���
		while (temp) {
			check = cute_pos_comp(temp->row, temp->col, strlen(temp->str));
			
			// �� ���ڿ� �ƴ� �ֵ鸸 ���
			if (temp->row < 19 && strcmp(temp->str, "")) {
				delete(temp->row-1, temp->col, temp->str);
				if (!check) {									// �����̶� �Ȱ�ġ�� �׸���.
					draw(temp->row, temp->col, temp->str);	
				}
			}

			else if(temp->row < 19 && !strcmp(temp->str,"")){
				delete(temp->row, temp->col, "");				// ������ �����ϸ� �װ� �����̶� ��ġ�� 
				draw(temp->row,temp->col,"");					// ������ �������� ���ڿ��� �ٲ�
			}
			if (temp->row == 19 && strcmp(temp->str, "     ")) {
				move(1, 70);
				sprintf(test, "%d", temp->row);
				addstr(test);
				sprintf(test2, "%d", temp->col);
				move(2, 70);
				addstr(test2);
				move(3, 70);
				addstr(test3);
				refresh();
			}
			// row�� 18 �̻��̸� ���ְ� �������
			// �Ǵ� �����̶� ��ġ�� ������� (�� �������� �̹� �׷��� ����)
			if(temp->row >=19 && strcmp(temp->str,"") || check){
				delete(temp->row-1, temp->col, temp->str);		// ������ ���� �� ���� �Ⱥ��̰�
				
				pthread_mutex_lock(&lock); // ����
				// ����
				del =temp;				
				temp->left->right = NULL;
				temp = temp->right;
				free(del); 

				// hp print
				hp--;
				pthread_mutex_unlock(&lock); // ���� unlock

				move(0,1);
				addstr("life  : ");
				sprintf(hp_str,"%d",hp);
				addstr(hp_str);
				refresh();
			}
			
			// row 18 �̻��̰� ���ڿ��� ���
			else if(temp->row >= 19){
				pthread_mutex_lock(&lock); // ����
				// ����
				del =temp;				
				temp->left->right = NULL;
				temp = temp->right;
				free(del); 
				pthread_mutex_unlock(&lock); // ���� unlock

			}

			else
				temp = temp->right;
		}

		move(20, 36); // Ŀ�� �̵�
		sleep(t); // ���
		
	}
}

// �ش� �ܾ� ã�� �Լ�
void checkWord(char *str) {

	nodePointer temp = NULL;
	temp = ptr;
	
	// �ǳ����� ����
	while(temp->right)
		temp=temp->right;
	// �ؿ������� Ž��
	while (temp) {
		//if(!strcmp(str,""))
		//	return;

		// ���� �� ã���� ���ڿ��� �ٲ��� �� ����
		if (!strcmp(temp->str, str)) {
			delete(temp->row, temp->col, temp->str);		// �� ���ڷ� �ٲٱ� ���� ����
			strcpy(temp->str,"");
			
			score++;

			cute(0);						 // ã�����ϱ� ������ �����̱�
			
			// score print
			move(1,1);
			addstr("score : ");
			sprintf(score_str, "%d", score);
			addstr(score_str);
			refresh();
			return;
		}
		// ���� ���
		else
			temp = temp->left;
	}

}

// ��� ����� �Լ�
nodePointer makeNode() {	
	nodePointer temp;

	temp = (nodePointer)malloc(sizeof(*temp));
	temp->right = temp->left = NULL;
	return temp;
}

// ���� �� ������ �Լ�
void nextLine() {

	nodePointer temp = ptr->right;

	// row 1 ����
	while (temp) {
		temp->row += 1;
		temp = temp->right;
	}
}

// �ܾ� �߰��ϴ� �Լ�
void addQ(char *str, int col) {

	nodePointer temp, temp2;

	// ù ��� �� ���, �ܾ� �߰�
	if (ptr == NULL) {
		ptr = makeNode();
		strcpy(ptr->str, str); // ���� ���ڿ��� �ܾ� �����
		ptr->row = 2;
		ptr->col = col;
	}

	// �ܾ� �߰�
	else {
		temp = makeNode();
		strcpy(temp->str, str); // ���� ���ڿ��� �ܾ� �����
		temp->row = 2;
		temp->col = col;
		temp->right = ptr; // ����� ����
		ptr->left = temp; // ����� ����
		ptr = temp;
		nextLine();
	}
	nodeCnt++;
}

// �������� ���ڿ� ��ȯ�ϱ�
char * makeWord() {

	char * DB[] = { "apple","jung","cocaine","hello","elite","fail","game",
				"halo","icon","jail","knight","lake","monkey","nope" };

	return DB[rand() % 14];
}

// ���ڿ� ������ġ ����� �Լ�
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

// �׸��� �Լ�
void draw(int row, int col, char* str) {
	move(row, col);
	addstr(str);

	move(20, 36);

	refresh();
}

// ������ �����̴� �Լ�
void cute(int move) {
	if (move_cute % 2 == 0) {
		move(19, 80 -  (move_cute / 2));
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

		move(20, 36);													// Ŀ�� ��ġ������ �����̰� 
																		// ������� ��쵵 �־ �׻� �� ��ġ�� �̵�
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

// �ܾ� ��ġ�� ������ ��ġ �� �Լ�
int cute_pos_comp(int row, int col, int len) {
	int cute_col_1 = 80 -  (move_cute / 2);
	int cute_col_2 = 83 -  (move_cute / 2);
	int check = 0;																// �ܾ�� �����̶� ��ġ�� 1
	int i;																		// �ܾ��� �Ϻζ� ��ġ�� �� Ȯ���ϱ� ���� 

	if (cute_mode == 0) {
		if (row >= 19) {														// �� �� ����
			for (i = 0; i < len; i++) {
				if ((col + i >= cute_col_1) && (col + i < cute_col_1 + 17)) {
					check = 1;
					break;
				}
			}
		}
	}

	if (cute_mode == 1) {
		if (row >= 16) {														// �ɱ׸� ����
			for (i = 0; i < len; i++) {
				if ((col + i >= cute_col_2) && (col + i < cute_col_2 + 4)) {	// ���κ�
					check = 1;
					break;
				}
			}

			if (row >= 18) {													// �Ʒ��κ�
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