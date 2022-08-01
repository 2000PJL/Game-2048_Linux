/*
 * @Author: PengJL 
 * @Date: 2022-07-15 20:37:56
 * @LastEditors: PengJL 
 * @LastEditTime: 2022-07-23 17:01:16
 * @Description: 游戏主体逻辑
 * 
 * Copyright (c) by PengJL, All Rights Reserved. 
 */
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#include<time.h>
#include<string.h>
#include <sys/wait.h>
#include <math.h>

#include"lcd.h"
#include"displaytext.h"
#include"filelist.h"
#include"jpeg.h"
#include"lcdtouch.h"
#include"game_logic.h"




int touch_event = 0;
Touch_point my_touch_point;
int mode = 0;
int game_score = 0;

int matrix_change_flag = 0; //方块产生判断标志
int Invalid_operation_num = 0;

int matrix[4][4] = {0};
int matrix_view[4][4] = {0};

void mainpage_init(Filelist *image_flist)
{
    display_jpg(flist_findfile(image_flist,"backgroud.jpg"),0,0);
    display_jpg(flist_findfile(image_flist,"mainpage.jpg"),250,30); //300*300
    display_jpg(flist_findfile(image_flist,"copyright.jpg"),200,420); //400*40
    display_jpg(flist_findfile(image_flist,"start_game.jpg"),70,350); //300*60
    display_jpg(flist_findfile(image_flist,"stop_game.jpg"),430,350); //300*60
}

void gamepage_init(Filelist *flist)
{
    game_score = 0;
    display_jpg(flist_findfile(flist,"backgroud.jpg"),0,0);
    lcd_draw_rectangle(0, 0, 460, 480, 0xBBADA2);
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            display_jpg(flist_findfile(flist,"num_0.jpg"),10 + j*110, 10 + i*110);
        }
    }
    display_score(480,100);
    lcd_draw_rectangle(480,160,300,100,0xffffff);
    display_int(game_score,550,180);
    display_jpg(flist_findfile(flist,"start_game.jpg"),480,320); //300*60
    display_jpg(flist_findfile(flist,"stop_game.jpg"),480,400); //300*60

    time_t t;
    srand((unsigned)time(&t));
    int number = 4; //随机产生四张牌
    do
    {
        int x = rand()%4; //随机牌的坐标
        int y = rand()%4;
        if(matrix[y][x] == 0)
        {
            matrix[y][x]  = 2;
        }else{
            number++;
        }
    }while(--number>0);
    refresh_view(flist);
}



/**
 * @Author: PengJL
 * @Description: 
 * @param {void} *arg
 * @return {*}
 */
void *listen_touch_thread(void *arg)
{

    for(;;)
    {
        touch_event = get_touch(&my_touch_point);
        usleep(1000*20);
        printf("touch_event = %d,x,y %d,%d\n",touch_event,my_touch_point.x,my_touch_point.y);   
    }

}



/**
 * @Author: PengJL
 * @Description: 游戏主处理逻辑线程
 * @param {void} *arg: 传入image_flist的地址，
 * 函数内部能直接调用主进程中创建的jpg图片的文件链表
 * 
 * @return {*}
 */
void *game_logic_thread(void *arg)
{
    Filelist *image_flsit = arg;
    
    for(;;)
    {
        usleep(1000*200);
        switch(touch_event)
        {
            case 5:
                touch_program(image_flsit);
                break;

            case 0:
                break;

            default:
                move_program(image_flsit);
                matrixIsChange();
                if(matrix_change_flag == 1)
                {
                    random_program();
                }else{
                    Invalid_operation_num++;
                }
                
                refresh_view(image_flsit);
                gameIsWin(image_flsit);
                break;
        }
        touch_event = 0;

    }
}




/**
 * @Author: PengJL
 * @Description: 后台播放指定背景音乐
 * @param {void} *argv: 文件链表指针
 * @return {*}
 */
void *mp3_play_thread(void *arg)
{
    Filelist *mp3_flist = arg;
    char *mp3_name =NULL;
    mp3_name = flist_findfile(mp3_flist,"Glorious-days_Beyond.mp3");

    while(1)
    {
        char cmd[512];
        sprintf(cmd,"madplay -Q -a -30 -S %s",mp3_name);
        printf("mp3_play_thread: %s\n",cmd);
        system(cmd);
        sleep(1);
    }
}







/**
 * @Author: PengJL
 * @Description: 获取按键触摸响应值
 * @return {int}
 *              1: 开始游戏
 *              0: 退出游戏
 */
int get_button()
{
    int x = my_touch_point.x;
    int y = my_touch_point.y;
    if(mode == 0)
    {
        if(x>70 && x<370 && y>350 && y<410)
        {
            return 1;
        }
        if(x>430 && x<730 && y>350 && y<410)
        {
            return 0;
        }
    }else{
        if(x>480 && x<780 && y>320 && y<380)
        {
            return 1;
        }
        if(x>480 && x<780 && y>400 && y<460)
        {
            return 0;
        }
    }
    
    
    return -1;
}

/**
 * @Author: PengJL
 * @Description: 触摸事件响应
 * @param {Filelist} *image_flist
 * @return {*}
 */
void touch_program(Filelist *image_flist)
{

    int key = get_button();
    if(mode == 0)
    {
        if(key == 1)
        {
            gamepage_init(image_flist);



            mode = 1;
        }else if(key == 0)
        {
            display_jpg(flist_findfile(image_flist,"backgroud.jpg"),0,0);
            pthread_exit(NULL);
        }
    }else{
        if(key == 1)
        {
            gamepage_init(image_flist);
            for(int i = 0; i < 4; i++)
            {
                for(int j = 0; j < 4; j++)
                {
                    matrix[i][j] = matrix_view[i][j] = 0;
                }
            }
        }else if(key == 0)
        {
            display_jpg(flist_findfile(image_flist,"backgroud.jpg"),0,0);
            pthread_exit(NULL);
        }
    }
}

/**
 * @Author: PengJL
 * @Description: 屏幕滑动事件处理
 * @param NULL
 * @return {*}
 */
void move_program(Filelist* flist)
{
    matrix_change_flag = 0;
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            printf("%d ",matrix[i][j]);
        }
        printf("\n");
    }
    int stop = judge_NotMove();
    if(stop == 1)
    {
        game_over(flist);
    }

    switch (touch_event)
    {
    case 1: //上滑事件处理
        {
            for(int i = 0; i < 4; i++)
            {
                for(int j = 0; j < 3; j++)
                {
                    int next = -1;
                    for(int k = j+1; k < 4 ; k++)
                    {
                        if(matrix[k][i] != 0)   //寻找第一个非零元素，并将其列坐标传给next
                        {
                            next = k;  
                            break;
                        }
                    }
                    if(next != -1)        //如果找到非零元素就执行元素合并代码
                    {
                        if(matrix[j][i]==0)
                        {
                            matrix[j][i] = matrix[next][i];
                            matrix[next][i] = 0;
                        }else if(matrix[j][i] == matrix[next][i]){
                            game_score += matrix[j][i];
                            matrix[j][i] *= 2;
                            matrix[next][i] = 0;
                            matrix_change_flag = 1;
                        }
                    }else{
                        break;
                    }

                    
                }
            }


        }
        break;
    case 2: //下滑事件处理
        {
            for(int i = 0; i < 4; i++)
            {
                for(int j = 3; j > 0; j--)
                {
                    int next = -1;
                    for(int k = j-1; k >= 0; k--)
                    {
                        if(matrix[k][i] != 0)
                        {
                            next = k;
                            break;
                        }
                    }
                    if(next != -1)
                    {
                        if(matrix[j][i]==0)
                        {
                            matrix[j][i] = matrix[next][i];
                            matrix[next][i] = 0;
                        }else if(matrix[j][i] == matrix[next][i]){
                            game_score += matrix[j][i];
                            matrix[j][i] *= 2;
                            matrix[next][i] = 0;
                            matrix_change_flag = 1;
                        }
                    }else{
                        break;
                    }

                }
            }

        }
        break;

    case 3: //左滑事件处理
        {
            for(int i = 0; i < 4; i++)
            {
                for(int j = 0; j < 3; j++)
                {
                    int next = -1;
                    for(int k = j+1; k < 4; k++)
                    {
                        if(matrix[i][k] != 0)
                        {
                            next = k;
                            break;
                        }
                    }
                    if(next != -1)
                    {
                        if(matrix[i][j]==0)
                        {
                            matrix[i][j] = matrix[i][next];
                            matrix[i][next] = 0;
                        }else if(matrix[i][j] == matrix[i][next]){
                            game_score += matrix[i][j];
                            matrix[i][j] *= 2;
                            matrix[i][next] = 0;
                            matrix_change_flag = 1;
                        }
                    }else{
                        break;
                    }
                }
            }


            break;
        }

    case 4: //右划事件处理
    {
            for(int i = 0; i < 4; i++)
            {
                for(int j = 3; j > 0; j--)
                {
                    int next = -1;
                    for(int k = j-1; k >= 0; k--)
                    {
                        if(matrix[i][k] != 0)
                        {
                            next = k;
                            break;
                        }
                    }
                    if(next != -1)
                    {
                        if(matrix[i][j]==0)
                        {
                            matrix[i][j] = matrix[i][next];
                            matrix[i][next] = 0;
                        }else if(matrix[i][j] == matrix[i][next]){
                            game_score += matrix[i][j];
                            matrix[i][j] *= 2;
                            matrix[i][next] = 0;
                            matrix_change_flag = 1;
                        }
                    }else{
                        break;
                    }

                }
            }

        }
        break;

    default:
        break;
    }
}


/**
 * @Author: PengJL
 * @Description: 在棋盘上随机产生number张
 * 新牌,如果number数量大于空格数量，就产生
 * 空格数量的牌;
 * @return {*}
 */
void random_program()
{
    int number = rand()%3; //产生随机牌的数量，姑且将2048看作牌吧,默认单次最多产生三张随机牌
    number = number < matrixEmptyNum() ? number : matrixEmptyNum();
    do
    {
        int x = rand()%4; //随机牌的坐标
        int y = rand()%4;
        if(matrix[y][x] == 0)
        {
            matrix[y][x] = (int)pow(2, (rand()%3)+1);
        }else{
            number++;
        }
    }while(--number>0);
}


/**
 * @Author: PengJL
 * @Description: 求2048棋盘空格数量
 * @return {int}
 *              返回棋盘上空格的数量
 */
int matrixEmptyNum()
{
    int num = 0;
    for(int i = 0; i < 4; i++ )
    {
        for(int j = 0; j < 4; j++)
        {
            if(matrix[i][j] == 0)
            {
                num++;
            }
        }
    }
    
    return num;
}


/**
 * @Author: PengJL
 * @Description: 更新游戏的视图 
 * @param {Filelist*} flist: 图片的文件链表
 * @return {*}
 */
void refresh_view(Filelist* flist)
{
    char name[16];
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            if(matrix[i][j] != matrix_view[i][j])
            {
                matrix_view[i][j] = matrix[i][j];
                sprintf(name,"num_%d.jpg",matrix[i][j]);
                display_jpg(flist_findfile(flist,name),10 + j*110, 10 + i*110);
                Invalid_operation_num = 0;
            }
        }
    }
    lcd_draw_rectangle(480,160,300,100,0xffffff);
    display_int(game_score,550,180);
    if(Invalid_operation_num == 5)
    {
        game_over(flist);
    }
}

/**
 * @Author: PengJL
 * @Description: 判断操作是否改变了视图
 * @return {*}
 */
void matrixIsChange()
{
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            if(matrix[i][j] != matrix_view[i][j])
            {
                matrix_change_flag = 1;
            }
        }
    }
}

/**
 * @Author: PengJL
 * @Description: 游戏获胜
 * @param {Filelist*} flist
 * @return {*}
 */
void game_win(Filelist* flist)
{
    display_jpg(flist_findfile(flist,"game_win.jpg"),0,0);
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            matrix[i][j] = 0;
        }
    }
    sleep(1);
    pthread_exit(NULL);
}

/**
 * @Author: PengJL
 * @Description: 游戏结束
 * @param {Filelist*} flist
 * @return {*}
 */
void game_over(Filelist* flist)
{
    display_jpg(flist_findfile(flist,"game_over.jpg"),50,60);
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            matrix[i][j] = matrix_view[i][j] = 0;
        }
    }
    sleep(1);
    pthread_exit(NULL);
}


/**
 * @Author: PengJL
 * @Description: 判断2048是否不可移动
 * @return {int}
 *             1: 如果不可移动就返回1
 *             0: 如果可以移动就返回0
 */
int judge_NotMove()
{
    int judge = 1;
    int num = matrixEmptyNum();
    if(num == 0)
    {
        for(int i = 0; i < 4; i++)
        {
            for(int j = 0; j < 3; j++)
            {
                if(matrix[i][j] == matrix[i][j+1] || matrix[j][i] == matrix[j+1][i])
                {
                    judge = 0;
                    return judge;
                }
            }
        }
    }else{
        judge = 0;
    }
    return judge;
}

/**
 * @Author: PengJL
 * @Description: 判断是否出现了2048，
 *  出现了就终止所有线程
 * @return {*}
 */
void gameIsWin(Filelist *flist)
{
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            if(matrix_view[j][i] == 2048)
            {
                game_win(flist);
            }
        }
    }       
}





