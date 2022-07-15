/*
 * @Author: PengJL 
 * @Date: 2022-07-14 11:14:48
 * @LastEditors: PengJL 
 * @LastEditTime: 2022-07-15 20:34:36
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


#include"lcd.h"
#include"filelist.h"
#include"bmp.h"
#include"displaytext.h"
#include"jpeg.h"

int main()
{
    Filelist* image_flist = NULL;
    image_flist = create_flist();
    if(findFile(image_flist,"/home/PengJL",".jpg")>0)
    {
        printf("查找jpg图片成功\n");
    }else{
        printf("查找jpg图片失败\n");
    }
    lcd_open();
    display_jpg(flist_findfile(image_flist,"backgroud.jpg"),0,0);
    display_jpg(flist_findfile(image_flist,"mainpage.jpg"),250,30); //300*300
    display_jpg(flist_findfile(image_flist,"copyright.jpg"),200,420); //400*40
    display_jpg(flist_findfile(image_flist,"start_game.jpg"),70,350); //300*60
    display_jpg(flist_findfile(image_flist,"stop_game.jpg"),430,350); //300*60

    lcd_close();
    return 0;
}