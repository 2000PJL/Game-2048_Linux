/*
 * @Author: PengJL 
 * @Date: 2022-07-14 11:14:48
 * @LastEditors: PengJL 
 * @LastEditTime: 2022-07-19 18:30:01
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



int main()
{

    Filelist* image_flist = NULL;
    image_flist = create_flist();



    if(findFile(image_flist,"../",".jpg")>0)
    {
        printf("查找jpg图片成功\n");
    }else{
        printf("查找jpg图片失败\n");
        return 0;
    }

    lcd_open();
    mainpage_init(image_flist);
    
       
    int ret = pthread_create(&th_listen_touch,NULL,listen_touch_thread,NULL); 
    if(ret != 0)
    {
        printf("Create pthread1 error!\n");
        exit(1);
    }


    ret = pthread_create(&th_main,NULL,game_logic_thread,image_flist);

    if(ret != 0)
    {
        printf("Create pthread2 error!\n");
        exit(1);
    }
    
    pthread_join(th_main,NULL);
  
    lcd_close();
    return 0;
}