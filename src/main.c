/*
 * @Author: PengJL 
 * @Date: 2022-07-14 11:14:48
 * @LastEditors: PengJL 
 * @LastEditTime: 2022-07-16 20:49:18
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







int main()
{

    Filelist* image_flist = NULL;
    image_flist = create_flist();

    int touch_event = 0;
    Touch_point* touch_point = (Touch_point *)malloc(sizeof(Touch_point));


    if(findFile(image_flist,"../",".jpg")>0)
    {
        printf("查找jpg图片成功\n");
    }else{
        printf("查找jpg图片失败\n");
    }

    Param param1;
    param1.touch_point = touch_point;
    param1.touch_event = touch_event;

    pthread_t th1;
    int ret = pthread_create(&th1,NULL,(void *)listen_touch_thread,&param1); 
    if(ret != 0)
    {
        printf("Create pthread error!\n");
        exit(1);
    }
    pthread_join(th1,NULL);

    lcd_open();

    mainpage_init(image_flist);
    sleep(1);
    gamepage_init(image_flist);

    lcd_close();
    return 0;
}