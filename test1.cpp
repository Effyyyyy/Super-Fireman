#include <stdio.h>
#include <easyx.h> // easyX
#include <windows.h> // (GetAsyncKeyState)异步输入
#include <malloc.h>
#include <mmsystem.h>
#pragma comment (lib, "winmm.lib")

#define WINDOW_WIDTH   1320
#define WINDOW_HEIGHT  720
#define ROLE_WIDTH_1   32
#define ROLE_HEIGHT_1  50
#define ROLE_WIDTH_2   32
#define ROLE_HEIGHT_2  50
#define ITEM_WIDTH_1   96
#define ITEM_HEIGHT_1  32
#define ITEM_WIDTH_2   100
#define ITEM_HEIGHT_2  100
#define ITEM_WIDTH_3   96
#define ITEM_HEIGHT_3  40
#define y_up           118
#define y_down         520
#define y_mid          360
#define y_mid_up       268
#define y_mid_down     418
#define item_1_left    132
#define item_1_right   420
#define item_2_left    900
#define item_2_right   1188
#define item_3_left    132
#define item_3_right   324
#define item_4_left    468
#define item_4_right   852
#define item_5_left    996
#define item_5_right   1096
#define item_6_left    516
#define item_6_right   804
#define item_7_left    228
#define item_7_right   1000
#define item_8_left    371
#define item_8_right   421
#define item_9_left    899
#define item_9_right   949
#define FIRE_WIDTH_1   64
#define FIRE_HEIGHT_1  64
#define SHIELD_WIDTH   50
#define SHIELD_HEIGHT  50
#define TP_WIDTH       50
#define TP_HEIGHT      95
#define HP_WIDTH       32
#define HP_HEIGHT      32
#define ICON_WIDTH     32
#define ICON_HEIGHT    50
#define GAMEOVER_WIDTH 192
#define GAMEOVER_HEIGHT 42
#define GAMEOVER_LEFT  564
#define GAMEOVER_RIGHT 756

void mouseclick();
void button(int x, int y, int w, int h, const char* text);
void Gamemenu();
void initGame();
void showGame();
void ctolGame();
void keyevent();
int  getdelay();
void judge_edge();
void jump();
void judge_fall();
void gravity_ctol();
void drawfire(struct ROLE* boom);
void judge_tp();
void judge_attack_damage1();
void judge_attack_damage2();
void GameOver();
void Ctol_GameOver();
void checkhp();

// 变量初始化
// 图片变量
IMAGE icon1;
IMAGE icon2;
IMAGE map1;
IMAGE map2;
IMAGE map3;
IMAGE map4;
IMAGE ROLE_1;
IMAGE ROLE_2;
IMAGE menu1;
IMAGE menu2;
IMAGE gameover;
IMAGE shield;
IMAGE item1;
IMAGE item2;
IMAGE item3;
IMAGE explore1;
IMAGE numbers;
IMAGE tp;
IMAGE hp;
IMAGE tp_door[20];
IMAGE gamestart;
IMAGE png1;
IMAGE id[5];
IMAGE _id;
IMAGE _id2;
IMAGE regraph;
IMAGE gameover1;
IMAGE intro1;
IMAGE intro2;


//暂停检测
int if_pause = 0;
//人物方向
enum direct_1 { p_left_1, p_right_1 };
enum direct_2 { p_left_2, p_right_2 };
// 人物控制
//人物是否移动判断
int speed_left_1, speed_left_2;
int speed_right_1, speed_right_2;
//人物朝向判断
int if_running_left_1, if_running_left_2;
int if_running_right_1, if_running_right_2;
//人物是否起跳判断
int if_jump_1, if_jump_2;
int if_down_1, if_down_2;
//人物目前跳跃量
int jump_down_1, jump_down_2;
int jump_high_1, jump_high_2;
//人物是否可以跳跃
int active_jump_1, active_jump_2;
int active_down;
//判断人物方向
int judge_dir_1, judge_dir_2;
double down_1, down_2;
//设定人物加速度
int g_1, g_2;
//重力场开关
int active_gravity_1;
int active_gravity_2;
//菜单按钮坐标
int bx = 585;
int by1 = 453, by2 = 553;
int bw = 250;
int bh = 50;
// 护盾开关
int active_shield_1;
int active_shield_2;
// 人物坐标
int res = 1;
int role, count, count_fire, count_tp, dir_1, dir_2;
int x_1, x_2;
int y_1, y_2;
// 检测人物是否处于开火状态
int firing1 = 0;
int firing2 = 0;
// 人物血量
double hp1;
double hp2;
// 判断子弹
int if_role_1;
int if_role_2;
// 游戏结束判断
int active_gameover;
int GAMEOVER_Y;
// 开始id动态效果
int count_id = 0;
// 结束后的菜单显示
int Regraph;

// 子弹坐标
struct ROLE
{
	int x;
	int y;
	int speed;
	int count_boom1 = 0;
	int count_boom2 = 0;
};

//设置子弹所用链表
struct Node
{
	struct ROLE data;
	struct Node* next;
};

struct Node* list1 = NULL;
struct Node* list2 = NULL;

//创建list
struct Node* createHead()
{
	struct Node* headNode = (struct Node*)malloc(sizeof(struct Node));
	if (!headNode) return NULL;
	headNode->next = NULL;

	return headNode;
}

//创建节点
struct Node* createNode(struct ROLE data)
{
	struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
	newNode->data = data;
	newNode->next = NULL;
	return newNode;
}

// 插入链表
void insertByTail(struct Node* headNode, struct ROLE data) {
	struct Node* newNode = createNode(data);
	struct Node* p = headNode;
	while (p->next) {
		p = p->next;
	}
	p->next = newNode;
	newNode->next = NULL;
}

// 整点bgm
// 攻击音效
DWORD WINAPI playmusic1(LPVOID lpvoid) {
	mciSendString("open attack2.wav", 0, 0, 0);
	mciSendString("play attack2.wav", 0, 0, 0);
	mciSendString("close attack2.wav", 0, 0, 0);

	return 0;
}
// 主菜单bgm
DWORD WINAPI playmusic2(LPVOID lpvoid) {
	mciSendString("open dungeon_track_1.mp3 alias bkmusic", 0, 0, 0);
	mciSendString("play bkmusic repeat", 0, 0, 0);
	mciSendString("close dungeon_track_1.mp3", 0, 0, 0);

	return 0;
}

//主界面菜单按钮
void button(int x, int y, int w, int h, const char* text) {
	setbkmode(TRANSPARENT);
	setfillcolor(BROWN);
	fillroundrect(x, y, x + w, y + h, 10, 10);
	settextstyle(33, 0, "Consolas");
	int tx = x + (w - textwidth(text)) / 2;
	int ty = y + (h - textheight(text)) / 2;
	outtextxy(tx, ty, text);
}

// 鼠标点击控制（菜单选项）
void mouseclick() {
	int temp = 0;
	ExMessage msg;
	while (1) {
		if (peekmessage(&msg, EM_MOUSE)) {
			switch (msg.message) {
			case WM_LBUTTONDOWN:
				if (msg.x >= bx && msg.x <= bx + bw && msg.y >= by1 && msg.y <= by1 + bh) {
					cleardevice();
					initGame();
					showGame();
					temp = 1;
				}
				else if (msg.x >= bx && msg.x <= bx + bw && msg.y >= by2 && msg.y <= by2 + bh) {
					closegraph();
					exit;
				}
				break;
			}
			if (temp == 1) break;
		}
	}
}

// 游戏菜单初始化
void Gamemenu() {
	initgraph(WINDOW_WIDTH, WINDOW_HEIGHT); //初始化窗口
	//主菜单界面贴图
	loadimage(&menu1, "maps\\menu1.png");
	loadimage(&menu2, "maps\\menu2.png");
	loadimage(&gamestart, "maps\\jump-across.png");
	loadimage(&png1, "maps\\burning-meteor.png");
	loadimage(&id[0], "maps\\id0.png");
	loadimage(&id[1], "maps\\id1.png");
	loadimage(&id[2], "maps\\id2.png");
	loadimage(&id[3], "maps\\id3.png");
	loadimage(&id[4], "maps\\id4.png");
	loadimage(&_id, "maps\\id.png");
	loadimage(&_id2, "maps\\_id2.png");
	putimage(0, 0, &menu2);
	putimage(200, 50, &id[2], SRCAND);

	//设置按钮选项
	button(bx - 30, by1, bw, bh, "Start");
	button(bx - 30, by2, bw, bh, "Exit");
	// BGM
	CreateThread(NULL, NULL, playmusic2, NULL, NULL, NULL);
	
	// 鼠标点击检测
	mouseclick();
}

// 游戏初始化
void initGame() {
	loadimage(&item1, "maps_extra\\item2min.png");
	loadimage(&explore1, "maps_extra\\explore1.png");
	loadimage(&shield, "maps_extra\\shield2.png", 100, 50);
	loadimage(&map1, "maps\\map1.png");
	loadimage(&map2, "maps\\map2.png");
	loadimage(&map3, "maps\\map3.png");
	loadimage(&map4, "maps\\map4.png");
	loadimage(&ROLE_1, "人物贴图素材\\001.png");
	loadimage(&ROLE_2, "人物贴图素材\\022.png");
	loadimage(&item2, "maps\\tiles_001.png");
	loadimage(&item3, "maps_extra\\item3.png");
	loadimage(&tp, "maps_extra\\tp.png", 50, 95);
	loadimage(&hp, "maps_extra\\hearts.png");
	loadimage(&icon1, "人物贴图素材\\icon1.png");
	loadimage(&icon2, "人物贴图素材\\icon2.png");
	loadimage(&gameover, "maps_extra\\gameover.png");
	loadimage(&regraph, "maps\\forest-background.png");
	loadimage(&gameover1, "maps\\gameover1.png");
	loadimage(&intro1, "maps\\intro1.png");
	loadimage(&intro2, "maps\\intro2.png");

	char buff[256];
	for (int i = 0; i < 20; i++) {
		sprintf_s(buff, "maps_extra\\传送门正_%d.png", i);
		loadimage(&tp_door[i], buff, 50, 95);
	}

	//初始化人物坐标
		//人物1
	x_1 = 500;
	y_1 = y_down - ROLE_HEIGHT_1;
	//人物2
	x_2 = 250;
	y_2 = y_down - ROLE_HEIGHT_1;
	//计数
	role = 0;
	count = 0;
	count_tp = 0;
	// 初始化人物血量
	hp1 = 10;
	hp2 = 10;
	// 初始化人物朝向
	dir_1 = p_right_1;
	dir_2 = p_left_2;
	//人物跳动/下落的高度
	jump_high_1 = 40;
	jump_down_1 = 0;
	jump_high_2 = 40;
	jump_down_2 = 0;
	// 人物是否在向左/向右运动
	judge_dir_1 = 1;
	if_running_right_1 = 0;
	if_running_left_1 = 0;
	judge_dir_2 = 1;
	if_running_right_2 = 0;
	if_running_left_2 = 0;
	//人物是否起跳/下落
	down_1 = 0;
	active_jump_1 = 1;
	if_jump_1 = 0;
	if_down_1 = 0;
	down_2 = 0;
	active_jump_2 = 1;
	if_jump_2 = 0;
	if_down_2 = 0;
	// 人物是否站立
	speed_left_1 = 0;
	speed_right_1 = 0;
	speed_left_2 = 0;
	speed_right_2 = 0;
	// 初始化人物重力
	g_1 = 15;
	g_2 = 15;
	active_gravity_1 = 1;
	active_gravity_2 = 1;
	// 初始化护盾状态
	active_shield_1 = 0;
	active_shield_2 = 0;
	// 
	if_role_1 = 0;
	if_role_2 = 0;
	// 游戏结束控制
	active_gameover = 0;
	GAMEOVER_Y = 600;
	// 结束后的菜单显示
	Regraph = 0;
}

/*void drawshield(struct ROLE temp) {

	BeginBatchDraw();
	putimage(temp.x, temp.y, SHIELD_WIDTH, SHIELD_HEIGHT, &shield, 0, 0);
	EndBatchDraw();
}*/

// 画出fire
void drawfire(struct ROLE* boom) {
	BeginBatchDraw();

	// 子弹变化6次后释放该地址内存
	if (if_role_1 == 1) {
		if (boom->speed > 0) {
			putimage(boom->x - ROLE_WIDTH_1, boom->y, FIRE_WIDTH_1, FIRE_HEIGHT_1, &explore1, boom->count_boom1 * FIRE_WIDTH_1, 0, SRCPAINT);
		}
		else if (boom->speed < 0) {
			putimage((boom->x) - ROLE_WIDTH_1 * 2, boom->y, FIRE_WIDTH_1, FIRE_HEIGHT_1, &explore1, boom->count_boom1 * FIRE_WIDTH_1, 0, SRCPAINT);
		}

		boom->x += boom->speed;
		if (boom->count_boom1 == 18) {
			if (list1->next) {
				struct Node* p1 = list1->next;
				list1->next = p1->next;
				free(p1);
			}
		}
		else {
			boom->count_boom1++;
			if (!(list1->next)) {
				if_role_1 = 0;
			}
		}
	}
	if (if_role_2 == 1) {
		if (boom->speed > 0) {
			putimage(boom->x - ROLE_WIDTH_2, boom->y, FIRE_WIDTH_1, FIRE_HEIGHT_1, &explore1, boom->count_boom2 * FIRE_WIDTH_1, 0, SRCPAINT);
		}
		else if (boom->speed < 0) {
			putimage((boom->x) - ROLE_WIDTH_2 * 2, boom->y, FIRE_WIDTH_1, FIRE_HEIGHT_1, &explore1, boom->count_boom2 * FIRE_WIDTH_1, 0, SRCPAINT);
		}
		boom->x += boom->speed;
		if (boom->count_boom2 == 18) {
			if (list2->next) {
				struct Node* p2 = list2->next;
				list2->next = p2->next;
				free(p2);
			}
		}
		else {
			boom->count_boom2++;
			if (!(list2->next)) {
				if_role_2 = 0;
			}
		}
	}


	EndBatchDraw();
}

void printlist(struct Node* list) {
	while (list->next != NULL) {
		drawfire(&list->next->data);
	}
}

// 游戏显示
void showGame() {
	BeginBatchDraw();
	// 贴地图
	putimage(0, 0, &map1);
	// 贴空中台阶 /左上
	for (int i = item_1_left; i < item_1_right; i += 96) {
		putimage(i, y_up, ITEM_WIDTH_1, ITEM_HEIGHT_1, &item2, 96, 0);
	}
	// 右上
	for (int j = item_2_left; j < item_2_right; j += 96) {
		putimage(j, y_up, ITEM_WIDTH_1, ITEM_HEIGHT_1, &item2, 96, 0);
	}
	// 左中
	for (int i = item_3_left; i < item_3_right; i += 96) {
		putimage(i, y_mid_up, ITEM_WIDTH_1, ITEM_HEIGHT_1, &item2, 96, 0);
	}
	// 右中
	for (int i = item_5_left; i < item_5_right; i += 96) {
		putimage(i, y_mid_up, ITEM_WIDTH_1, ITEM_HEIGHT_1, &item2, 96, 0);
	}
	// 中上
	for (int i = item_4_left; i < item_4_right; i += 96) {
		putimage(i, y_mid_up, ITEM_WIDTH_1, ITEM_HEIGHT_1, &item2, 96, 0);
	}
	// 中下 // 正常
	/*for (int i = item_6_left; i < item_6_right; i += 96) {
		putimage(i, y_mid_down, ITEM_WIDTH_1, ITEM_HEIGHT_1, &item2, 96, 0);
	}*/
	// 中下 // 特殊
	for (int i = item_6_left; i < item_6_right; i += 96) {
		putimage(i, y_mid_down, ITEM_WIDTH_3, ITEM_HEIGHT_3, &item3, 0, 0);
	}
	putimage(200, 720 - ITEM_HEIGHT_3 - bh / 2, &item3);
	button(270 + ITEM_WIDTH_3, 720 - bh, 0, 0, ":强化跳跃");

	// 下
	for (int i = item_7_left; i < item_7_right; i += 96) {
		putimage(i, y_down, ITEM_WIDTH_1, ITEM_HEIGHT_1, &item2, 96, 0);
	}

	// 特殊块 1
	for (int i = item_8_left; i < item_8_right; i += 50) {
		putimage(i, y_mid_up, ITEM_WIDTH_2, ITEM_HEIGHT_2, &item1, 0, 0);
	}
	// 特殊块 2
	for (int i = item_9_left; i < item_9_right; i += 50) {
		putimage(i, y_mid_up, ITEM_WIDTH_2, ITEM_HEIGHT_2, &item1, 0, 0);
	}

	// 传送门
	//putimage(item_8_left, y_mid_up - TP_HEIGHT, TP_WIDTH, TP_HEIGHT, &tp, 0, 0, SRCPAINT);
	//putimage(item_9_left, y_mid_up - TP_HEIGHT, TP_WIDTH, TP_HEIGHT, &tp, 0, 0, SRCPAINT);
	putimage(item_8_left, y_mid_up - TP_HEIGHT, TP_WIDTH, TP_HEIGHT, &tp_door[count_tp], 0, 0, SRCPAINT);
	putimage(item_9_left, y_mid_up - TP_HEIGHT, TP_WIDTH, TP_HEIGHT, &tp_door[count_tp], 0, 0, SRCPAINT);

	putimage((1320 - TP_WIDTH) / 2 - TP_WIDTH, 720 - TP_HEIGHT, &tp_door[count_tp], SRCPAINT);
	button((1320 - TP_WIDTH) / 2 + 50, 720 - bh, 0, 0, ":传送门");

	// INTRO
//	putimage(10, 591, &intro1, SRCPAINT);
	button(60, 720 - 3 * bh, 0, 0, "W:JUMP");
	button(60, 720 - 2 * bh, 0, 0, "A:LEFT");
	button(60, 720 - bh, 0, 0, "D:RIGHT");
	button(200, 720 - 3 * bh, 0, 0, "Q:SHIELD");
	button(200, 720 - 2 * bh, 0, 0, "E:FIRE");

	button(1100, 720 - 3 * bh, 0, 0, "↑:JUMP");
	button(1100, 720 - 2 * bh, 0, 0, "←:LEFT");
	button(1100, 720 - bh, 0, 0, "→:RIGHT");
	button(1250, 720 - 3 * bh, 0, 0, "1:SHIELD");
	button(1250, 720 - 2 * bh, 0, 0, "3:FIRE");


	// 人物 1 血量条
	int hp1_temp = hp1;
	if (hp1_temp >= 5) {
		for (int i = 20; i <= 148; i += 32) {
			putimage(i, ICON_HEIGHT, HP_WIDTH, HP_HEIGHT, &hp, 0, 0, SRCPAINT);
		}
		if (hp1_temp >= 5 && hp1_temp <= 10) {
			for (int i = 20; i <= (hp1_temp - 5) * 32; i += 32) {
				putimage(i, ICON_HEIGHT + HP_HEIGHT + 2, HP_WIDTH, HP_HEIGHT, &hp, 0, 0, SRCPAINT);
			}
		}
	}
	else if (hp1_temp <= 5) {
		for (int i = 20; i <= (hp1_temp) * 32; i += 32) {
			putimage(i, ICON_HEIGHT, HP_WIDTH, HP_HEIGHT, &hp, 0, 0, SRCPAINT);
		}
	}


	// 人物 2 血量条
	int hp2_temp = hp2;
	if (hp2_temp >= 5) {
		for (int i = 1300 - HP_WIDTH * 2; i >= 1140 - HP_WIDTH; i -= 32) {
			putimage(i, ICON_HEIGHT, HP_WIDTH, HP_HEIGHT, &hp, 0, 0, SRCPAINT);
		}
		if (hp2_temp >= 5 && hp2_temp <= 10) {
			for (int i = 1300 - HP_WIDTH * 2; i >= (1300 - (hp2_temp - 5) * 32) - HP_WIDTH; i -= 32) {
				putimage(i, ICON_HEIGHT + HP_HEIGHT + 2, HP_WIDTH, HP_HEIGHT, &hp, 0, 0, SRCPAINT);
			}
		}
	}
	else if (hp2_temp <= 5) {
		for (int i = 1300 - HP_WIDTH * 2; i >= (1300 - hp2_temp * 32) - HP_WIDTH; i -= 32) {
			putimage(i, ICON_HEIGHT, HP_WIDTH, HP_HEIGHT, &hp, 0, 0, SRCPAINT);
		}
	}


	// 人物 1 头像
	putimage(20, 0, ICON_WIDTH, ICON_HEIGHT, &icon1, 0, 0, SRCPAINT);
	// 人物 2 头像
	putimage(1300 - ICON_WIDTH, 0, ICON_WIDTH, ICON_HEIGHT, &icon2, 0, 0, SRCPAINT);

	// 贴人物1
	switch (dir_1) {
	case p_left_1:
		if (speed_left_1 == 1) {
			putimage(x_1, y_1, ROLE_WIDTH_1, ROLE_HEIGHT_1, &ROLE_1, role * ROLE_WIDTH_1, 50, SRCPAINT);
			speed_left_1 = 0;
		}
		break;
	case p_right_1:
		if (speed_right_1 == 1) {
			putimage(x_1, y_1, ROLE_WIDTH_1, ROLE_HEIGHT_1, &ROLE_1, role * ROLE_WIDTH_1, 100, SRCPAINT);
			speed_right_1 = 0;
		}
		break;
	}
	if (speed_left_1 == 0 || speed_right_1 == 0) {
		int choose;
		if (judge_dir_1 == 0) choose = 50;
		else if (judge_dir_1 == 1) choose = 100;
		putimage(x_1, y_1, ROLE_WIDTH_1, ROLE_HEIGHT_1, &ROLE_1, ROLE_WIDTH_1, choose, SRCPAINT);
	}
	// 贴人物 1 的护盾
	if (active_shield_1 == 1) {
		putimage(x_1 - 9, y_1, SHIELD_WIDTH, SHIELD_HEIGHT, &shield, 0 * SHIELD_WIDTH, 0, SRCPAINT);
	}

	// 贴人物 2
	switch (dir_2) {
	case p_left_2:
		if (speed_left_2 == 1) {
			putimage(x_2, y_2, ROLE_WIDTH_2, ROLE_HEIGHT_2, &ROLE_2, role * ROLE_WIDTH_2, 50, SRCPAINT);
			speed_left_2 = 0;
		}
		break;
	case p_right_2:
		if (speed_right_2 == 1) {
			putimage(x_2, y_2, ROLE_WIDTH_2, ROLE_HEIGHT_2, &ROLE_2, role * ROLE_WIDTH_2, 100, SRCPAINT);
			speed_right_2 = 0;
		}
		break;
	}
	if (speed_left_2 == 0 || speed_right_2 == 0) {
		int choose;
		if (judge_dir_2 == 0) choose = 50;
		else if (judge_dir_2 == 1) choose = 100;
		putimage(x_2, y_2, ROLE_WIDTH_2, ROLE_HEIGHT_2, &ROLE_2, ROLE_WIDTH_2, choose, SRCPAINT);
	}
	// 人物2 的护盾
	if (active_shield_2 == 1) {
		putimage(x_2 - 9, y_2, SHIELD_WIDTH, SHIELD_HEIGHT, &shield, 0 * SHIELD_WIDTH, 0, SRCPAINT);
	}

	// 游戏结束:缓慢上升效果
	if (active_gameover == 1) {
		putimage(GAMEOVER_LEFT, GAMEOVER_Y, GAMEOVER_WIDTH, GAMEOVER_HEIGHT, &gameover, 0, 0, SRCPAINT);
	}

	EndBatchDraw();
}

// 按键事件
void keyevent() {
	// 判断按键输入
	// P(暂停)
	if (GetAsyncKeyState('P') & 0x8000) {
		if (if_pause == 1) {
			if_pause = 0;
		}
		if_pause = 1;
	}
	// a
	if (GetAsyncKeyState('A') & 0x8000) {
		dir_1 = p_left_1;
		x_1 -= 5;
		judge_dir_1 = 0;
		speed_left_1 = 1;
	}
	// d
	if (GetAsyncKeyState('D') & 0x8000) {
		dir_1 = p_right_1;
		x_1 += 5;
		judge_dir_1 = 1;
		speed_right_1 = 1;
	}
	// w
	if (GetAsyncKeyState('W') & 0x8000 && active_jump_1 == 1) {
		if_jump_1 = 1;
		active_jump_1 = 0;
		active_gravity_1 = 0;
	}
	// OPEN FIRE
	if (GetAsyncKeyState('E') & 0x8000) {
		struct ROLE boom1;
		boom1.x = x_1 + ROLE_WIDTH_1;
		boom1.y = y_1;
		if (judge_dir_1) {
			boom1.speed = 15;
		}
		else {
			boom1.speed = -15;
		}
		if_role_1 = 1;
		insertByTail(list1, boom1);
		judge_attack_damage2();
		CreateThread(NULL, NULL, playmusic1, NULL, NULL, NULL);
	}
	// OPEN SHIELD
	if (GetAsyncKeyState('Q') & 0x8000) {
		if (active_shield_1 == 1) active_shield_1 = 0;
		else active_shield_1 = 1;
	}



	//人物 2
	if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
		dir_2 = p_left_2;
		x_2 -= 5;
		judge_dir_2 = 0;
		speed_left_2 = 1;
	}
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
		dir_2 = p_right_2;
		x_2 += 5;
		judge_dir_2 = 1;
		speed_right_2 = 1;
	}

	if (GetAsyncKeyState(VK_UP) & 0x8000 && active_jump_2 == 1) {
		if_jump_2 = 1;
		active_jump_2 = 0;
		active_gravity_2 = 0;
	}
	// 人物2 护盾
	if (GetAsyncKeyState(VK_NUMPAD1) & 0x8000) {
		if (active_shield_2 == 1) active_shield_2 = 0;
		else active_shield_2 = 1;
	}
	// 开火
	if (GetAsyncKeyState(VK_NUMPAD3) & 0x8000) {
		struct ROLE boom2;
		boom2.x = x_2 + ROLE_WIDTH_2;
		boom2.y = y_2;
		if (judge_dir_2) {
			boom2.speed = 15;
		}
		else {
			boom2.speed = -15;
		}
		if_role_2 = 1;
		insertByTail(list2, boom2);
		judge_attack_damage1();
		CreateThread(NULL, NULL, playmusic1, NULL, NULL, NULL);
	}
}

void judge_fall() {
	if (y_1 >= 710) {
		hp1 = 0;
	}
	if (y_2 >= 710) {
		hp2 = 0;
	}
}

// 边缘判断
void judge_edge() {
	// 人物1边缘判断
	if (x_1 <= 0 || x_1 >= (WINDOW_WIDTH - ROLE_WIDTH_1)) {
		if (x_1 <= 0) x_1 = 0;
		else if (x_1 >= (WINDOW_WIDTH - ROLE_WIDTH_1)) x_1 = WINDOW_WIDTH - ROLE_WIDTH_1;
	}
	if (y_1 >= WINDOW_HEIGHT) {
		y_1 = WINDOW_HEIGHT;
		active_jump_1 = 0;
	}
	// 人物2边缘判断
	if (x_2 <= 0 || x_2 >= (WINDOW_WIDTH - ROLE_WIDTH_2)) {
		if (x_2 <= 0) x_2 = 0;
		else if (x_2 >= (WINDOW_WIDTH - ROLE_WIDTH_2)) x_2 = WINDOW_WIDTH - ROLE_WIDTH_2;
	}
	if (y_2 >= WINDOW_HEIGHT) {
		y_2 = WINDOW_HEIGHT;
		active_jump_2 = 0;
	}
}

// 传送门判断
void judge_tp() {
	// 左边的传送门
	if (x_1 <= item_8_right && x_1 + ROLE_WIDTH_1 >= item_8_left && y_1 >= y_mid_up - TP_HEIGHT && y_1 <= y_mid_up) {
		x_1 = item_7_right - ROLE_WIDTH_1 * 3;
		y_1 = y_down - ROLE_HEIGHT_1;
		g_1 = 0;
		if_down_1 = 0;
		jump_high_1 = 40;
		active_jump_1 = 1;
	}
	if (x_2 <= item_8_right && x_2 + ROLE_WIDTH_1 >= item_8_left && y_2 >= y_mid_up - TP_HEIGHT && y_2 <= y_mid_up) {
		x_2 = item_7_right - ROLE_WIDTH_1 * 3;
		y_2 = y_down - ROLE_HEIGHT_1;
		g_2 = 0;
		if_down_2 = 0;
		jump_high_2 = 40;
		active_jump_2 = 1;
	}
	// 右边的传送门
	if (x_1 <= item_9_right && x_1 + ROLE_WIDTH_1 >= item_9_left && y_1 >= y_mid_up - TP_HEIGHT && y_1 <= y_mid_up) {
		x_1 = item_1_left + ROLE_WIDTH_1;
		y_1 = 118 - ROLE_HEIGHT_1 - 2;
		g_1 = 0;
		if_down_1 = 0;
		jump_high_1 = 40;
		active_jump_1 = 1;
	}
	if (x_2 <= item_9_right && x_2 + ROLE_WIDTH_1 >= item_9_left && y_2 >= y_mid_up - TP_HEIGHT && y_2 <= y_mid_up) {
		x_2 = item_1_left + ROLE_WIDTH_1;
		y_2 = 118 - ROLE_HEIGHT_1 - 2;
		g_2 = 0;
		if_down_2 = 0;
		jump_high_2 = 40;
		active_jump_2 = 1;
	}
}

// 角色受伤害判断 (受到攻击 / 展开防御 / 坠落伤害)
void judge_attack_damage1() {
	if (judge_dir_2 == 1) {
		if (x_1 <= 5 * 15 + x_2 + FIRE_WIDTH_1 && x_1 + ROLE_WIDTH_1 >= x_2 && y_1 + ROLE_HEIGHT_1 >= y_2 + FIRE_HEIGHT_1 - ROLE_HEIGHT_1 && y_1 <= y_2 + ROLE_HEIGHT_1) {
			if (active_shield_1 == 1) {
				x_1 += 15;
				active_shield_1 = 0;
			}
			else {
				hp1 -= 0.2;
				x_1 += 15;
			}
			// 播放音效？
		}
	}
	else if (judge_dir_2 == 0) {
		if (x_1 + ROLE_WIDTH_1 >= x_2 - 6 * 15 && x_1 <= x_2 && y_1 + ROLE_HEIGHT_1 >= y_2 + FIRE_HEIGHT_1 - ROLE_HEIGHT_1 && y_1 <= y_2 + ROLE_HEIGHT_1) {
			if (active_shield_1 == 1) {
				x_1 -= 15;
				active_shield_1 = 0;
			}
			else {
				hp1 -= 0.2;
				x_1 -= 15;
			}
			// 播放音效？
		}
	}
	if (y_1 >= 700) {
		hp1 = 0;
	}
}

void judge_attack_damage2() {
	if (judge_dir_1 == 1) {
		if (x_2 <= 5 * 15 + x_1 + FIRE_WIDTH_1 && x_2 + ROLE_WIDTH_1 >= x_1 && y_2 + ROLE_HEIGHT_1 >= y_1 + FIRE_HEIGHT_1 - ROLE_HEIGHT_1 && y_2 <= y_1 + ROLE_HEIGHT_1) {
			if (active_shield_2 == 1) {
				x_2 += 15;
				active_shield_2 = 0;
			}
			else {
				hp2 -= 0.2;
				x_2 += 15;
			}
			// 播放音效？
		}
	}
	else if (judge_dir_1 == 0) {
		if (x_2 + ROLE_WIDTH_1 >= x_1 - 6 * 15 && x_2 <= x_1 && y_2 + ROLE_HEIGHT_1 >= y_1 + FIRE_HEIGHT_1 - ROLE_HEIGHT_1 && y_2 <= y_1 + ROLE_HEIGHT_1) {
			if (active_shield_2 == 1) {
				x_2 -= 15;
				active_shield_2 = 0;
			}
			else {
				hp2 -= 0.2;
				x_2 -= 15;
			}
			// 播放音效？
		}
	}
	if (y_2 >= 700) {
		hp2 = 0;
	}
}

// 重力场控制
void gravity_ctol() {
	// 人物1 在空中受重力下降
	// 左上
	if (x_1 >= item_1_left - ROLE_WIDTH_1 && x_1 <= item_1_right && y_1 == 118 - ROLE_HEIGHT_1 - 2) {
		g_1 = 0;
		if_down_1 = 0;
		jump_high_1 = 40;
		active_jump_1 = 1;
		active_gravity_1 = 1;
	}
	// 右上
	if (x_1 >= item_2_left - ROLE_WIDTH_1 && x_1 <= item_2_right && y_1 == 118 - ROLE_HEIGHT_1 - 2) {
		g_1 = 0;
		if_down_1 = 0;
		jump_high_1 = 40;
		active_jump_1 = 1;
		active_gravity_1 = 1;
	}
	// 左2 
	if (x_1 >= item_3_left - ROLE_WIDTH_1 && x_1 <= item_3_right && (y_1 == y_mid_up - ROLE_HEIGHT_1 || y_1 == 216)) {
		g_1 = 0;
		if_down_1 = 0;
		jump_high_1 = 40;
		active_jump_1 = 1;
		active_gravity_1 = 1;
		y_1 = y_mid_up - ROLE_HEIGHT_1;
	}
	// 中上
	if (x_1 >= item_4_left - ROLE_WIDTH_1 && x_1 <= item_4_right && (y_1 == y_mid_up - ROLE_HEIGHT_1 || y_1 == 216)) {
		if_down_1 = 0;
		jump_high_1 = 40;
		active_jump_1 = 1;
		active_gravity_1 = 1;
		y_1 = y_mid_up - ROLE_HEIGHT_1;
		g_1 = 0;
	}
	// 右2
	if (x_1 >= item_5_left - ROLE_WIDTH_1 && x_1 <= item_5_right + ITEM_WIDTH_1 && (y_1 == y_mid_up - ROLE_HEIGHT_1 || y_1 == 216)) {
		g_1 = 0;
		if_down_1 = 0;
		jump_high_1 = 40;
		active_jump_1 = 1;
		y_1 = y_mid_up - ROLE_HEIGHT_1;
	}
	// 中下
	if ((x_1 >= item_6_left - ROLE_WIDTH_1 && x_1 <= item_6_right) && (y_1 == 375 || y_1 == y_mid_down - ROLE_HEIGHT_1 || y_1 == 366)) {
		if_down_1 = 0;
		jump_high_1 = 40;
		active_jump_1 = 1;
		active_gravity_1 = 1;
		y_1 = 368;
		g_1 = 0;
	}
	// 最下
	if (x_1 >= item_7_left - ROLE_WIDTH_1 && x_1 <= item_7_right + ITEM_WIDTH_1 && (y_1 == y_down - ROLE_HEIGHT_1 || y_1 == 473 || y_1 == 471)) {
		if_down_1 = 0;
		jump_high_1 = 40;
		active_jump_1 = 1;
		y_1 = y_down - ROLE_HEIGHT_1;
		g_1 = 0;
	}
	// 特殊块 1 
	if (x_1 >= item_8_left - ROLE_WIDTH_1 && x_1 <= item_8_right && (y_1 == y_mid_up - ROLE_HEIGHT_1 || y_1 == 216)) {
		if_down_1 = 0;
		jump_high_1 = 40;
		active_jump_1 = 1;
		y_1 = y_mid_up - ROLE_HEIGHT_1;
		g_1 = 0;
	}
	// 特殊块 2
	if (x_1 >= item_9_left - ROLE_WIDTH_1 && x_1 <= item_9_right && (y_1 == y_mid_up - ROLE_HEIGHT_1 || y_1 == 216)) {
		if_down_1 = 0;
		jump_high_1 = 40;
		active_jump_1 = 1;
		y_1 = y_mid_up - ROLE_HEIGHT_1;
		g_1 = 0;
	}

	if (active_gravity_1 == 1) {
		y_1 += g_1;
	}
	g_1 = 15;

	// 人物2 在空中受重力下降
	// 左上
	if (x_2 >= item_1_left && x_2 <= item_1_right && y_2 == 118 - ROLE_HEIGHT_1 - 2) {
		g_2 = 0;
		if_down_2 = 0;
		jump_high_2 = 40;
		active_jump_2 = 1;
		active_gravity_2 = 1;
	}
	// 右上
	if (x_2 >= item_2_left - ROLE_WIDTH_1 && x_2 <= item_2_right && y_2 == 118 - ROLE_HEIGHT_1 - 2) {
		g_2 = 0;
		if_down_2 = 0;
		jump_high_2 = 40;
		active_jump_2 = 1;
		active_gravity_2 = 1;
	}
	// 左2 
	if (x_2 >= item_3_left - ROLE_WIDTH_1 && x_2 <= item_3_right && (y_2 == y_mid_up - ROLE_HEIGHT_1 || y_2 == 216)) {
		g_2 = 0;
		if_down_2 = 0;
		jump_high_2 = 40;
		active_jump_2 = 1;
		active_gravity_2 = 1;
		y_2 = y_mid_up - ROLE_HEIGHT_1;
	}
	// 中上
	if (x_2 >= item_4_left - ROLE_WIDTH_1 && x_2 <= item_4_right && (y_2 == y_mid_up - ROLE_HEIGHT_1 || y_2 == 216)) {
		if_down_2 = 0;
		jump_high_2 = 40;
		active_jump_2 = 1;
		active_gravity_2 = 1;
		y_2 = y_mid_up - ROLE_HEIGHT_1;
		g_2 = 0;
	}
	// 右2
	if (x_2 >= item_5_left - ROLE_WIDTH_1 && x_2 <= item_5_right + ITEM_WIDTH_1 && (y_2 == y_mid_up - ROLE_HEIGHT_1 || y_2 == 216)) {
		g_2 = 0;
		if_down_2 = 0;
		jump_high_2 = 40;
		active_jump_2 = 1;
		y_2 = y_mid_up - ROLE_HEIGHT_1;
	}
	// 中下
	if ((x_2 >= item_6_left - ROLE_WIDTH_1 && x_2 <= item_6_right) && (y_2 == 375 || y_2 == y_mid_down - ROLE_HEIGHT_1 || y_2 == 366)) {
		if_down_2 = 0;
		jump_high_2 = 40;
		active_jump_2 = 1;
		active_gravity_2 = 1;
		y_2 = 368;
		g_2 = 0;
	}
	// 最下
	if (x_2 >= item_7_left - ROLE_WIDTH_1 && x_2 <= item_7_right + ITEM_WIDTH_1 && (y_2 == y_down - ROLE_HEIGHT_1 || y_2 == 473 || y_2 == 471)) {
		if_down_2 = 0;
		jump_high_2 = 40;
		active_jump_2 = 1;
		y_2 = y_down - ROLE_HEIGHT_1;
		g_2 = 0;
	}
	// 特殊块 1 
	if (x_2 >= item_8_left - ROLE_WIDTH_1 && x_2 <= item_8_right && (y_2 == y_mid_up - ROLE_HEIGHT_1 || y_2 == 216)) {
		if_down_2 = 0;
		jump_high_2 = 40;
		active_jump_2 = 1;
		y_2 = y_mid_up - ROLE_HEIGHT_1;
		g_2 = 0;
	}
	// 特殊块 2
	if (x_2 >= item_9_left - ROLE_WIDTH_1 && x_2 <= item_9_right && (y_2 == y_mid_up - ROLE_HEIGHT_1 || y_2 == 216)) {
		if_down_2 = 0;
		jump_high_2 = 40;
		active_jump_2 = 1;
		y_2 = y_mid_up - ROLE_HEIGHT_1;
		g_2 = 0;
	}

	if (active_gravity_2 == 1) {
		y_2 += g_2;
	}
	g_2 = 15;

}

// 跳跃控制
void jump() {
	// 控制人物1起跳
	if (if_jump_1 == 1) {
		y_1 -= 19;
		jump_high_1 -= 5;
		if (jump_high_1 <= 0) {
			if_jump_1 = 0;
			if_down_1 = 1;
			jump_high_1 = 40;
		}
	}
	else if (if_down_1 == 1 && active_gravity_1 == 0) {
		y_1 += 19;
		jump_high_1 -= 5;
		if (jump_high_1 <= 0) {
			if_down_1 = 0;
			jump_high_1 = 40;
			active_jump_1 = 1;
			active_gravity_1 = 1;
		}
	}
	if (y_1 != 216 && y_1 != 218 && y_1 != 470 && y_1 != 473 && y_1 != 471 && y_1 != 375 && y_1 != 366 && y_1 != 368) {
		active_jump_1 = 0;
	}
	// 控制人物2起跳
	if (if_jump_2 == 1) {
		y_2 -= 19;
		jump_high_2 -= 5;
		if (jump_high_2 <= 0) {
			if_jump_2 = 0;
			if_down_2 = 1;
			jump_high_2 = 40;
		}
	}
	else if (if_down_2 == 1 && active_gravity_2 == 0) {
		y_2 += 19;
		jump_high_2 -= 5;
		if (jump_high_2 <= 0) {
			if_down_2 = 0;
			jump_high_2 = 40;
			active_jump_2 = 1;
			active_gravity_2 = 1;
		}
	}
	if (y_2 != 216 && y_2 != 218 && y_2 != 470 && y_2 != 473 && y_2 != 471 && y_2 != 375 && y_2 != 366 && y_2 != 368) {
		active_jump_2 = 0;
	}
}

// 控制角色
void ctolGame() {
	//printf("x_1 = %d, y_1 = %d\n", x_1, y_1);
	jump();
	judge_edge();
	gravity_ctol();
	judge_tp();
	judge_fall();
}

int getdelay() {
	static unsigned long long lastTime = 0;
	unsigned long long currentTime = GetTickCount();
	if (lastTime == 0) {
		lastTime = currentTime;
		return 0;
	}
	else {
		int ret = currentTime - lastTime;
		lastTime = currentTime;
		return ret;
	}
}

// 游戏暂停控制
void pause() {
	if (if_pause == 1) {
		system("pause");
	}
}

// 游戏结束控制
void GameOver() {
	if (hp1 <= 0 || hp2 <= 0) {
		active_gameover = 1;
	}
}

// 游戏结束时动态贴图
void Ctol_GameOver() {
	// 游戏结束动态效果
	if (active_gameover == 1) {
		GAMEOVER_Y--;
		if (GAMEOVER_Y == 200) {
			Sleep(4000);
			cleardevice();
			Regraph = 1;
		}
	}
}

// 结束后的菜单
void REGRAPH() {
	if (Regraph == 1) {
		putimage(0, 0, &regraph);
		putimage(454, 200, &gameover1, SRCPAINT);
	}
}

int judge = 0;
// 结束后的菜单选项
void after_gameover() {
	if (Regraph == 1) {
		BeginBatchDraw();
		button(bx, by1, bw, bh, "按空格Remake！");
		button(bx, by2, bw, bh, "按Esc润咯润咯");
		EndBatchDraw();
		if (GetAsyncKeyState(' ') & 0x8000) {
			cleardevice();
			initGame();
			showGame();
		}
		if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
			closegraph();
			exit;
		}
	}
}

// 主函数
int main()
{
	list1 = createHead();
	list2 = createHead();
	Gamemenu();

	int res = 0;
	while (1) {
		res++;
		if (res > 30) {
			role++;
			count++;
			if (role >= 4) role = 0;
			if (count >= 6) count = 0;
			res = 0;
		}
		// 传送门动态效果
		count_tp++;
		if (count_tp >= 20) count_tp = 0;
		if (Regraph == 0) {
			keyevent();
			showGame();
			printlist(list1);
			printlist(list2);
			ctolGame();
			GameOver();
			Ctol_GameOver();
		}
		REGRAPH();
		after_gameover();

		Sleep(12);

	}

	return 0;
}
