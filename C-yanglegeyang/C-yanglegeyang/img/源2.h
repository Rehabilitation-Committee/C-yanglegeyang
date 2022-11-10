#pragma once
#include<stdio.h>
#include<graphics.h>
#define Type int
#define Status int
#define INIT_NUMBER 18
#define SLOT_SIZE 7
#define WINDOW_WIDTH 370
#define WINDOW_HEIGHT 500
#define SLOT_WIDTH 350
#define SLOT_HEIGHT 50
#define SLOT_X 10
#define SLOT_Y 450
#define IMG_WIDTH 50
#define IMG_HEIGHT 50
#define COVERED 0
#define UNCOVERED 1
#define SHOW 1
#define UNSHOW 0
#define INSTANCE_PATH "test.txt"
void graph(Card_BiTree card_bitree,Slot Slot,int flag[INIT_NUMBER+1])
{
	IMAGE shapes, cardx, card[6];
	initgraph(370, 500, SHOWCONSOLE);
	loadimage(&shapes, _T("C:/Users/86134/Desktop/picture/ss.jpg"), 370, 500, 1);
	loadimage(&cardx, _T("C:/Users/86134/Desktop/picture/ka.jpg"), 350, 50, 1);//卡槽大小
	loadimage(&card[0], _T("C:/Users/86134/Desktop/picture/yellow.jpg"), 50, 50, 1);//卡片大小
	loadimage(&card[1], _T("C:/Users/86134/Desktop/picture/grass.jpg"), 50, 50, 1);
	loadimage(&card[2], _T("C:/Users/86134/Desktop/picture/brush.jpg"), 50, 50, 1);
	loadimage(&card[3], _T("C:/Users/86134/Desktop/picture/brrown.jpg"), 50, 50, 1);
	loadimage(&card[4], _T("C:/Users/86134/Desktop/picture/carrot.jpg"), 50, 50, 1);
	loadimage(&card[5], _T("C:/Users/86134/Desktop/picture/mao.jpg"), 50, 50, 1);
	setbkcolor(WHITE);
	cleardevice();
	putimage(0, 0, &shapes);
	putimage(10, 450, &cardx);//卡槽位置
	/*putimage(0, 0, &card1);
	putimage(50, 0, &card2 );
	putimage(100, 0, &card3);
	putimage(150, 0, &card4);
	putimage(200, 0, &card5);
	putimage(250, 0, &card6);*/
	getchar();
	closegraph();
	for (int i = 1; i <= INIT_NUMBER; i++)
	{
		if (card_bitree[i].status == UNCOVERED && flag[i] == SHOW)
			putimage(card_bitree[i].begin_x, card_bitree[i].begin_y, &card_bitree[i].card[card_bitree[i].type]);
		if (card_bitree[i].status == COVERED && flag[i] == SHOW)
			putimage(card_bitree[i].begin_x, card_bitree[i].begin_y, &card_bitree[i].card[card_bitree[i].type], RGB(150, 150, 50));
		else
			continue;
	}
}

void calculate(int x)
{
	double deta1= (card_bitree[i].begin_x - 10) / 10.0;
	if (x > 185)x -= deta1;
	else x += deta1;
}

void movexy(int i)
{
	int x, y;
	double deta1, deta2;
	int temp=0
	deta1 = (card_bitree[i].begin_x-10)/10.0;
	deta2 = (card_bitree[i].begin_y - 450) / 10.0;
	for (x = card_bitree[i].begin_x, y = card_bitree[i].begin_y; x = temp + 10, y = 450; calculate(x), y -= deta2)
	{
		temp = 50;
		i = 1;
		deta1= (card_bitree[i].begin_x - temp*i-10) / 10.0;
		i++;
		putimage(card_bitree[i].begin_x, card_bitree[i].begin_y, &card_bitree[i].card[card_bitree[i].type]);
	}
}