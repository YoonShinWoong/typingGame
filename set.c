#include "set.h"

void reverse(nodePointer *node)//노드의 데이터를 반대로 표현.
{
	if ((*node)->modeset == 0)//모드에 맞게 설정되지 않았다면
	{
		char k;
		//(*node)->modeset = 1;//설정
		//for (int i = 0; i < strlen((*node)->str) - 1; i++)
		//{
		//k = (*node)->str[i];
		//(*node)->str[i] = (*node)->str[strlen((*node)->str) - 1 - i];
		//	(*node)->str[strlen((*node)->str) - 1 - i] = k;
		//}//데이터 값 변환
	}

}

void diagonal(nodePointer *node)//대각선 이동
{
	int n = rand() % 2;// -1(왼), 0(아래) , 1(오른)
	setupterm(NULL, fileno(stdout), (int *)0);
	int rows = tigetnum("lines");
	int cols = tigetnum("cols");
	//해상도

	if (n == 0)
	{
		if ((*node)->col - 2 > 0)
			(*node)->col -= 3;
	}

	if (n == 1) {
		if (cols > (*node)->col + strlen((*node)->str) - 3)
			(*node)->col += 3;
	}
}

char *getWord()
{
	const int size = 25;
	char *wordDB[] = { "anxiety","boredom","conpuse","dragon","excitement","frustrate","greed","happiness","impatience","jealousy","kindess","loyalty","master","necessary","overwhelming","pessimism","quietus","relief","satisfaction","thrill","union","view","worriation","yell","zealot" };
	int set = rand() % size;

	return wordDB[set];
}
