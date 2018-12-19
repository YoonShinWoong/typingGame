#include "JinYong.h"

void reverse(nodePointer *node)//����� �����͸� �ݴ�� ǥ��.
{
	int size = strlen((*node)->str);
	char temp;
	if ((*node)->modeset == 0) {
		(*node)->modeset = 1;
		for (int i = 0; i < size / 2; i++) {
			temp = (*node)->str[i];
			(*node)->str[i] = (*node)->str[(size - 1) - i];
			(*node)->str[(size - 1) - i] = temp;
		}
	}
}

void diagonal(nodePointer *node)//�밢�� �̵�
{
	int n = rand() % 2;// -1(��), 0(�Ʒ�) , 1(����)

	if (n == 0)
	{
		if ((*node)->col - 2 > 0)
			(*node)->col -= 3;
	}

	if (n == 1) {
		if ((*node)->col + strlen((*node)->str) - 3 < 80)
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