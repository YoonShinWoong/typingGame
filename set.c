#include "set.h"

void reverse(nodePointer *node)//����� �����͸� �ݴ�� ǥ��.
{
	if ((*node)->modeset == 0)//��忡 �°� �������� �ʾҴٸ�
	{
		char k;
		//(*node)->modeset = 1;//����
		//for (int i = 0; i < strlen((*node)->str) - 1; i++)
		//{
		//k = (*node)->str[i];
		//(*node)->str[i] = (*node)->str[strlen((*node)->str) - 1 - i];
		//	(*node)->str[strlen((*node)->str) - 1 - i] = k;
		//}//������ �� ��ȯ
	}

}

void diagonal(nodePointer *node)//�밢�� �̵�
{
	int n = rand() % 2;// -1(��), 0(�Ʒ�) , 1(����)
	int rows = tigetnum("lines");
	int cols = tigetnum("cols");
	//�ػ�

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