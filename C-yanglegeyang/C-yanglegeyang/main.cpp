#include<stdio.h>
#include<graphics.h>
#include<stdlib.h>
#include<math.h>
#include"rand.h"
#include"music.h"

using namespace std;

#define Type int
#define Status int
#define INIT_NUMBER 54
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
#define INSTANCE_PATH "实例.txt"


typedef struct Card {
	Type type;	//卡片类型
	Status status;	//卡片状态：覆盖/未覆盖
	int begin_x;	//卡片左上角x坐标
	int begin_y;	//卡片左上角y坐标
	int end_x;	//卡片右下角x坐标
	int end_y;	//卡片右下角y坐标
	int flag;   //卡片显示标记
}Card, * Card_BiTree;	//卡片生成二叉树(数组形式)

typedef struct Stack {
	int* base;
	int* top;
	int stacksize;
}Slot;	//使用栈实现卡槽


//定义全局变量
Card_BiTree card_bitree;
Slot slot;
int bucket[8][INIT_NUMBER] = { 0 };	//建立桶，降低被覆盖卡片的查找次数，最后一行存储其余行中的元素个数
IMAGE shapes, cardx, card[6], cardg[6];


void global_init() {	//全局变量初始化
	void ToGray_Photoshop(COLORREF & color);
	card_bitree = (Card*)calloc(INIT_NUMBER + 1, sizeof(Card));
	slot.base = (int*)calloc(SLOT_SIZE + 1, sizeof(int));
	slot.top = slot.base;
	slot.stacksize = 0;
	loadimage(&shapes, "img/ss.jpg", 370, 500, 1);//背景
	loadimage(&cardx, "img/ka.jpg", 350, 50, 1);//卡槽
	//卡片
	loadimage(&card[0], "img/yellow.jpg", 50, 50, 1);
	loadimage(&card[1], "img/grass.jpg", 50, 50, 1);
	loadimage(&card[2], "img/brush.jpg", 50, 50, 1);
	loadimage(&card[3], "img/brrown.jpg", 50, 50, 1);
	loadimage(&card[4], "img/carrot.jpg", 50, 50, 1);
	loadimage(&card[5], "img/mao.jpg", 50, 50, 1);
	loadimage(&cardg[0], "img/yellow.jpg", 50, 50, 1);
	loadimage(&cardg[1], "img/grass.jpg", 50, 50, 1);
	loadimage(&cardg[2], "img/brush.jpg", 50, 50, 1);
	loadimage(&cardg[3], "img/brrown.jpg", 50, 50, 1);
	loadimage(&cardg[4], "img/carrot.jpg", 50, 50, 1);
	loadimage(&cardg[5], "img/mao.jpg", 50, 50, 1);
	for (int i = 0;i < 6;i++)	//灰度化
	{
		DWORD* imagep = GetImageBuffer(&cardg[i]);
		for (int t = 0; t < 50; t++)
			for (int j = 0; j < 50; j++)
				ToGray_Photoshop(imagep[t * 50 + j]);
	}
}

int get_serial_number(int n) {	//获取第n张卡片的所在桶
	return	card_bitree[n].begin_x / 50;
}


void cards_init() {	//初始化卡片
	void status_covered(int n);
	FILE* fp;
	fp = fopen(INSTANCE_PATH, "r");
	int serial_number,n;
	//n = random() % 20;
	n = 0;
	fseek(fp, n * INIT_NUMBER, 0);

	//读取卡片类型
	for (int i = 1; i <= INIT_NUMBER; i++)
	{
		card_bitree[i].type = (int)fgetc(fp) - 48;
	}
	fclose(fp);
	//设置卡片位置
	card_bitree[1].begin_x = WINDOW_WIDTH / 2 - IMG_WIDTH / 2;
	card_bitree[1].begin_y = WINDOW_HEIGHT / 2 - IMG_HEIGHT / 2;
	card_bitree[1].end_x = card_bitree[1].begin_x + IMG_WIDTH;
	card_bitree[1].end_y = card_bitree[1].begin_y + IMG_HEIGHT;
	card_bitree[1].status = UNCOVERED;
	serial_number = get_serial_number(1);	//获取桶位置
	bucket[serial_number][bucket[7][serial_number]] = 1;
	bucket[7][serial_number] += 1;

	for (int i = 2; i <= INIT_NUMBER; i++)
	{
		do {
			card_bitree[i].begin_x = (card_bitree[i / 2].begin_x - IMG_WIDTH) + random() % (IMG_WIDTH * 2 + 1);
			Sleep(10);
			card_bitree[i].begin_y = (card_bitree[i / 2].begin_y - IMG_HEIGHT) + random() % (IMG_HEIGHT * 2 + 1);
			card_bitree[i].end_x = card_bitree[i].begin_x + IMG_WIDTH;
			card_bitree[i].end_y = card_bitree[i].begin_y + IMG_HEIGHT;
			Sleep(10);
		} while (!(card_bitree[i].begin_x >= 0 && card_bitree[i].end_x <= WINDOW_WIDTH && card_bitree[i].begin_y >= 0 && card_bitree[i].end_y <= SLOT_Y));
		card_bitree[i].status = UNCOVERED;
		serial_number = get_serial_number(i);
		bucket[serial_number][bucket[7][serial_number]] = i;
		bucket[7][serial_number] += 1;
		status_covered(i);	//改变被覆盖卡片的status
	}

}

//检查覆盖
int check_cover(int n1, int n2) {	
	if (card_bitree[n1].end_x > card_bitree[n2].begin_x && card_bitree[n1].end_x < (card_bitree[n2].end_x + IMG_WIDTH)) {
		if (card_bitree[n1].end_y > card_bitree[n2].begin_y && card_bitree[n1].end_y < (card_bitree[n2].end_y + IMG_HEIGHT)) {
			return 1;
		}
		else {
			return 0;
		}
	}
	else {
		return 0;
	}
}

void status_covered(int n) {	//改变被覆盖卡片的status
	int serial_number = get_serial_number(n);
	for (int i = serial_number - 1; i <= serial_number + 1; i++)	//在卡片相邻桶间检查
	{
		if (i < 0 || i>9)	continue;
		for (int j = 0;j < bucket[7][i];j++) {
			if (bucket[i][j] >= n)	break;
			if (check_cover(bucket[i][j], n))
				card_bitree[bucket[i][j]].status = COVERED;
		}
	}
}



void status_uncovered(int n) {	//改变将入槽卡片下覆盖的卡片状态
	int serial_number = get_serial_number(n);

	for (int i = serial_number - 1; i <= serial_number + 1; i++)
	{
		if (i < 0 || i>9)	continue;
		for (int j = 0;j < bucket[7][i];j++) {
			if (bucket[i][j] >= n)	break;
			if (check_cover(bucket[i][j], n)) {	//被入槽卡片覆盖的卡片
				card_bitree[bucket[i][j]].status = UNCOVERED;

				for (int q = i - 1; q <= i + 1; q++)
				{
					if (q < 0 || q>9)	continue;
					for (int t = 0;t < bucket[7][q];t++) {
						if (bucket[q][t] <= bucket[i][j] || bucket[q][t] == n)	continue;
						if (check_cover(bucket[q][t], bucket[i][j])) {	//被入槽卡片覆盖的卡片如果被其他卡片覆盖
							//printf("%d被%d盖住了 %d:%d %d %d:%d %d\n ", bucket[i][j], bucket[q][t], bucket[i][j], card_bitree[bucket[i][j]].begin_x, card_bitree[bucket[i][j]].begin_y, bucket[q][t], card_bitree[bucket[q][t]].begin_x, card_bitree[bucket[q][t]].begin_y);
							card_bitree[bucket[i][j]].status = COVERED;
							break;
						}
					}
					if (card_bitree[bucket[i][j]].status == COVERED)	break;
				}
			}
		}
	}
}

int slot_isfull() {	//判断槽满
	if (slot.stacksize == SLOT_SIZE) return true;
	else return false;
}

void ToGray_Photoshop(COLORREF& color)	//灰度运算
{
	COLORREF R;
	R = pow((pow(GetRValue(color), 2.2) * 0.2973 + pow(GetGValue(color), 2.2) * 0.6247 + pow(GetBValue(color), 2.2) * 0.0753), 1 / 2.2);
	color = R + (R << 8) + (R << 16);
}

void graph()	//绘图
{
	setbkcolor(WHITE);
	cleardevice();
	putimage(0, 0, &shapes);	//背景
	putimage(10, 450, &cardx);	//卡槽
	for (int i = 1; i <= INIT_NUMBER; i++)
	{
		if (card_bitree[i].status == UNCOVERED && card_bitree[i].flag == SHOW)
		{
			putimage(card_bitree[i].begin_x, card_bitree[i].begin_y, &card[card_bitree[i].type - 1]);	//未被覆盖卡片
		}
		else if (card_bitree[i].status == COVERED && card_bitree[i].flag == SHOW)
		{
			putimage(card_bitree[i].begin_x, card_bitree[i].begin_y, &cardg[card_bitree[i].type - 1]);	//被覆盖卡片
		}
		else
			continue;
	}
}


void slot_push(int n, int& prex, int& prey) {	//被点击卡片入槽
	prex = card_bitree[n].begin_x;
	prey = card_bitree[n].begin_y;
	status_uncovered(n);
	card_bitree[n].begin_x = SLOT_X + IMG_WIDTH * slot.stacksize;
	card_bitree[n].begin_y = SLOT_Y;
	card_bitree[n].end_x = card_bitree[n].begin_x + IMG_WIDTH;
	card_bitree[n].end_y = card_bitree[n].begin_y + IMG_HEIGHT;
	slot.stacksize += 1;
	*slot.top = n;
	slot.top++;
	Sleep(10);
	BeginBatchDraw();//绘图
	cleardevice();
	graph();
	FlushBatchDraw();
	//负责消除的部分
	if (slot.stacksize >= 3) {
		for (int* i = slot.base; i < slot.top - 1; ++i)
			if (card_bitree[*(slot.top - 1)].type == card_bitree[*i].type){
				for (int* j = i + 1; j < slot.top - 1; ++j) {
					if (card_bitree[*j].type == card_bitree[*i].type) {
							//flag[*i] = flag[*j] = flag[*(slot.top - 1)] = UNSHOW;
							card_bitree[*j].flag = card_bitree[*i].flag = card_bitree[*(slot.top - 1)].flag =  UNSHOW;
							//flag[*(slot.top - 1)] = UNSHOW;
							printf("%d %d \n", *(i),*(j));
							//card_bitree[*(slot.top - 1)] = NULL;
							//slot.top -= 1;
							for (int* k = j+1; k < slot.top-1; k++) {
								printf("OK %d %d \n", *(k-1), *(k));
								card_bitree[*(k - 1)].status = card_bitree[*(k)].status;
								card_bitree[*(k - 1)].type = card_bitree[*(k)].type;
								card_bitree[*(k - 1)].flag = card_bitree[*(k)].flag;
								//*(k - 1) = *(k);
							}
							//slot.top -= 1;
							for (int* k = i+1; k < slot.top-2; k++) {
								printf("OK %d %d \n", *(k - 1), *(k));
								card_bitree[*(k - 1)].status = card_bitree[*(k)].status;
								card_bitree[*(k - 1)].type = card_bitree[*(k)].type;
								card_bitree[*(k - 1)].flag = card_bitree[*(k)].flag;
								//*(k - 1) = *(k);
							}
							card_bitree[*(slot.top - 3)].flag = card_bitree[*(slot.top - 1)].flag = card_bitree[*(slot.top - 2)].flag = UNSHOW;
							slot.top -= 3;
							slot.stacksize -= 3;
							ELIM_MUSIC();
							break;
					}
				}
			}
	}
	for (int* i = slot.base; i <= slot.top - 1; i++) 
		printf("%d ", *i);
	printf("\n");
	for (int* i = slot.base; i <= slot.top - 1; i++)
		printf("%d ", card_bitree[*i].begin_x);
	printf("\n");
	BeginBatchDraw();//绘图
	cleardevice();
	graph();
	FlushBatchDraw();
}


int click(ExMessage msg) {	//寻找被点击的卡片
	int serial_number = msg.x / 50;
	for (int i = serial_number - 1; i <= serial_number + 1; i++)
	{
		for (int j = 0;j < bucket[7][i];j++) {
			if (card_bitree[bucket[i][j]].begin_x < msg.x && card_bitree[bucket[i][j]].begin_y < msg.y)
			{
				if (card_bitree[bucket[i][j]].end_x > msg.x && card_bitree[bucket[i][j]].end_y > msg.y)
				{
					if (card_bitree[bucket[i][j]].status == UNCOVERED)
						return	bucket[i][j];
				}
			}
		}
	}
	return -1;
}


void ad()//广告函数
{
	mciSendString((LPCSTR)"open ./resource/video.avi type MPEGVideo alias video", NULL, 0, NULL);	//打开音乐
	mciSendString((LPCSTR)"play video", NULL, 0, NULL);
	Sleep(13000);
	mciSendString((LPCSTR)"close video", 0, 0, 0);
}

int main() {
	ExMessage msg;//接收鼠标点击
	int n, prex, prey, count = INIT_NUMBER;

	//initgraph(WINDOW_WIDTH, WINDOW_HEIGHT, SHOWCONSOLE);//开窗口
	initgraph(WINDOW_WIDTH, WINDOW_HEIGHT);//开窗口
	random();//随机数生成
	global_init();
	SetWindowText(GetHWnd(), "二叉树的简单运用了个二叉树的简单运用");
	cards_init();//初始化卡片
	//for (int i = 1; i < INIT_NUMBER + 1; i++)	flag[i] = 1;
	for (int i = 1; i < INIT_NUMBER + 1; i++)	card_bitree[i].flag = 1;
	//Ground_MUSIC();
	mciSendString("open easy_disco.mp3 alias mymusic", NULL, 0, NULL);//播放音乐mciSendString("pause mymusic", NULL, 0, NULL);
	mciSendString("play mymusic repeat", NULL, 0, NULL);
	while (1)
	{
		BeginBatchDraw();//绘图
		cleardevice();
		graph();
		if (peekmessage(&msg, EM_MOUSE)) {
			if (msg.message == WM_LBUTTONDOWN&&msg.y < SLOT_Y)
			{
				n = click(msg);
				if (n != -1) {
					//slot_push(n, prex, prey);
					slot_push(n, prex, prey);
					Check_MUSIC();
					count--;
					if (slot_isfull() || (count == 0 && !slot_isfull() && slot.stacksize != 0))
					{
						int result = MessageBox(GetHWnd(), "是否选择观看广告后退一步？", "这是复活币喵", MB_OKCANCEL);
						if (result == IDOK)//如果寄掉了
						{
							//Close_Ground_MUSIC();
							mciSendString("pause mymusic", NULL, 0, NULL);
							ad();//广告函数
							//Ground_MUSIC();
							mciSendString("resume mymusic", NULL, 0, NULL);
							card_bitree[n].begin_x = prex;
							card_bitree[n].begin_y = prey;
							card_bitree[n].end_x = card_bitree[n].begin_x + IMG_WIDTH;
							card_bitree[n].end_y = card_bitree[n].begin_y + IMG_HEIGHT;
							status_covered(n);
							slot.top--;
							slot.stacksize--;
							count++;
						}
						else if (result == IDCANCEL) {
							MessageBox(GetHWnd(), "扣除50原石！这就是不玩原神的下场！", "这就是神罚喵！", MB_OK);
							break;
						}
					}
					//消除完了
					else if (count == 0 && !slot_isfull() && slot.stacksize == 0) {
						int result = MessageBox(GetHWnd(), "恭喜获得50原石和华为14promax冷锋蓝！", "You are the champion！！", MB_OK);
						if (result == IDOK)	break;
					}
				}
			}
		}

		FlushBatchDraw();
	}
	closegraph();
	return 0;
}