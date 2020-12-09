#include"contact.h"

void Menu()
{
	printf("*************通讯录**************\n");
	printf("*  [1] Add            [2] Del   *\n");
	printf("*  [3] Find           [4] Modify*\n");
	printf("*  [5] Show           [6] Clear *\n");
	printf("*  [7] Sort           [0] Quit  *\n");
	printf("*********************************\n");
}

int main(int argc, char *argv[])
{
	Contact cont;
	InitContact(&cont);
	int select = 1;
	while (select)
	{
		Menu();

		printf("请选择:>");
		scanf("%d", &select);
		if (select == QUIT)
			break;
		switch (select)
		{
		case ADD:
			AddContact(&cont);
			break;
		case DEL:
			DelContact(&cont);
			break;
		case FIND:
			FindContact(&cont);
			break;
		case MODIFY:
			ModifyContact(&cont);
			break;
		case SHOW:
			ShowContact(&cont);
			break;
		case CLEAR:
			ClearContact(&cont);
			break;
		case SORT:
			SortContact(&cont);
			break;
		default:
			printf("输入有误，请重新输入...\n");
			break;
		}

	}
	SaveContact(&cont);
	return 0;
}