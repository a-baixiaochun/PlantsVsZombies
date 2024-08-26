#include <cstdio>
#include <unistd.h>
#include <graphics.h>
#include "tools.h"

#define WIN_WIDTH 900
#define WIN_HEIGHT 600

enum {WAN_DOU,XIANG_RI_KUI,MUSHROOM,ZHI_WU_COUNT};//!!!不采用宏定义，因为每次修改也麻烦 enum本质为整型常量

IMAGE imgBg; //背景图片
IMAGE imagBar;
IMAGE imgCards[ZHI_WU_COUNT];
IMAGE* imgPlants[ZHI_WU_COUNT][20]; //一行代表一种植物，及多少种状态

int curX,curY; //当前选中的植物，在移动过程中的位置
int  curPlant;

struct  plant{
    int type;       //0:么有植物， 1:第一种植物
    int frameIndex; //序列帧的序号
};
struct plant map[3][9];

bool fileExist(const char* name)
{
    FILE *fp = fopen(name,"r");
//    if (fp)
//    {
//        fclose(fp);
//        return false;
//    }
//    return true;
//更优雅的写法 but 不简洁
    if (fp)
        {
            fclose(fp);
    }
    return fp != nullptr;
}

void gameInit()
{
    loadimage(&imgBg,"res/bg.jpg");//加载背景图片 目前放在内存变量中
    loadimage(&imagBar,"res/bar5.png");

    memset(imgPlants,0,sizeof(imgPlants));
    memset(map,0, sizeof(map));
    //初始化植物卡牌
    char name[64];
    for (int i = 0; i < ZHI_WU_COUNT; ++i) {
        sprintf_s(name,sizeof(name),"res/Cards/card_%d.png",i + 1);
        loadimage(&imgCards[i],name);//对数组的操作忘记了

        for (int j = 0; j < 20; ++j) {
            sprintf_s(name,sizeof(name),"res/plant/%d/%d.png",i,j + 1);
            //先判断文件是否存在
            if (fileExist(name))
            {
                imgPlants[i][j] = new IMAGE;//底层是c++
                loadimage(imgPlants[i][j],name);
            } else
            {
                break;
            }
        }
    }


    curPlant = 0;
    initgraph(WIN_WIDTH, WIN_HEIGHT);//绘制窗口

}

void updateWindow()
{
    BeginBatchDraw(); //开始缓冲

    putimage(0,0,&imgBg);
    putimagePNG(250,0,&imagBar);

    //卡牌
    for (int i = 0; i < ZHI_WU_COUNT; ++i) {
        int x = 338 + i * 65;
        int y = 6;
        putimage(x,y,&imgCards[i]);
    }

    //种植
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 9; ++j) {
            if (map[i][j].type > 0)
            {
                int x = 256 + j * 81;
                int y = 179 + i * 102;
                int plantType = map[i][j].type -1;
                int index = map[i][j].frameIndex;
                putimagePNG(x,y,imgPlants[plantType][index]);
            }
        }
    }

    // 渲染 拖动过程中的植物
    if (curPlant > 0)
    {
        IMAGE* img = imgPlants[curPlant - 1][0];
        putimagePNG(curX - img->getwidth() / 2,curY - img->getheight() / 2,imgPlants[curPlant - 1][0]);
    }
    EndBatchDraw(); //结束双缓冲
}

void userClick()
{
    ExMessage msg{};
    static int  status = 0;

    if (peekmessage(&msg))
    {
        if (msg.message == WM_LBUTTONDOWN)
        {
            if (msg.x > 338 && msg.x < 338 + 65 * ZHI_WU_COUNT && msg.y < 96)
            {
                int index = (msg.x - 338) / 65;
                status = 1;
                curPlant = index + 1;
            }
        }
        else if (msg.message == WM_MOUSEMOVE && status == 1)
        {
            curX = msg.x;
            curY = msg.y;
        }
        else if (msg.message == WM_LBUTTONUP)
        {
            int row = (msg.y - 179) / 102;
            int col = (msg.x - 256) / 81;

            if (map[row][col].type == 0)
            {
                map[row][col].type = curPlant;
                map[row][col].frameIndex = 0;
            }
            curPlant = 0;
            status = 0;
        }
    }
}

void updateGame()
{
    for (auto & i : map) {
        for (auto & j : i) {
            if (j.type > 0){
                j.frameIndex++;
                int plantType = j.type -1;
                int plantIndex = j.frameIndex;
                if (imgPlants[plantType][plantIndex] == nullptr){
                    j.frameIndex = 0;
                }
            }
        }
    }
}

int main()
{
    gameInit();
    int timer = 0;
    bool flag = true;

    while(1)
    {
        userClick();
        timer += getDelay();
        if (timer > 20)
        {
            flag = true;
            timer = 0;
        }
        if (flag)
        {
            flag = false;
            updateWindow();
            updateGame();
        }
    }

    system("pause");
    return 0;
}
