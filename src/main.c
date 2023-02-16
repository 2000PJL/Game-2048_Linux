/*
 * @Author: PengJL 
 * @Date: 2022-07-14 11:14:48
 * @LastEditors: PengJL 
 * @LastEditTime: 2023-02-16 15:05:17
 * @Description: 本文件是2048游戏的主文件，
 * 在该文件中实现2048这款游戏的主要控制逻辑
 * 
 * Copyright (c) by PengJL, All Rights Reserved. 
 */

#include<stdio.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
#include<unistd.h>
#include<pthread.h>


#include"lcd.h"
#include"filelist.h"
#include"bmp.h"
#include"displaytext.h"
#include"jpeg.h"
#include"lcdtouch.h"
#include"game_logic.h"
#include"main.h"



pthread_t th_listen_touch; //触摸屏监控线程
pthread_t th_main;
pthread_t th_mp3;


int main()
{

    //创建jpg图片文件链表
    Filelist* image_flist = NULL;
    image_flist = create_flist();
    int ret = findFile(image_flist,"./",".jpg");

    if(ret > 0)
    {
        printf("查找到%d个jpg文件\n", ret);
    }else{
        printf("未找到jpg文件\n");
        return 0;
    }

    //创建mp3音频文件链表
    Filelist* mp3_flist = NULL;
    mp3_flist = create_flist();
    int ret1 = findFile(mp3_flist,"./",".mp3");
    
    if(ret1 > 0)
    {
        printf("查找到%d个mp3文件\n", ret1);
    }else{
        printf("未找到mp3文件\n");
        return 0;
    }

    lcd_open();
    mainpage_init(image_flist); //主界面初始化
    
    //创建屏幕触控监听线程
    int ret_t = pthread_create(&th_listen_touch,NULL,listen_touch_thread,NULL); 
    if(ret_t != 0)
    {
        printf("Failed to create listen_touch_thread\n");
        exit(1);
    }

    //创建游戏主逻辑线程
    ret_t = pthread_create(&th_main,NULL,game_logic_thread,image_flist);
    if(ret_t != 0)
    {
        printf("Failed to create game_logic_thread\n");
        exit(1);
    }

    //创建背景音乐线程
    ret_t = pthread_create(&th_mp3,NULL,mp3_play_thread,mp3_flist);
    if(ret_t != 0)
    {
        printf("Failed to create mp3_play_thread\n");
        exit(1);
    }
    
    //线程阻塞，等待游戏主逻辑线程的结束
    pthread_join(th_main,NULL);
    
    //杀死背景音乐线程开启的madplay音乐播放进程
    system("killall madplay");

    lcd_close();

    return 0;
}