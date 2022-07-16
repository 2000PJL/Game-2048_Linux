/*
 * @Author: PengJL 
 * @Date: 2022-07-15 20:37:56
 * @LastEditors: PengJL 
 * @LastEditTime: 2022-07-16 20:45:05
 * @Description: 游戏主体逻辑
 * 
 * Copyright (c) by PengJL, All Rights Reserved. 
 */
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

#include"lcd.h"
#include"displaytext.h"
#include"filelist.h"
#include"jpeg.h"
#include"lcdtouch.h"
#include"game_logic.h"


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
    display_jpg(flist_findfile(flist,"backgroud.jpg"),0,0);
    lcd_draw_rectangle(0, 0, 440, 480, 0xBBADA2);
    
}


// void *listen_touch(Touch_point *touch_point, int *touch_event)
// {
//     *touch_event = get_touch(touch_point);
// }

void *listen_touch_thread(void *arg)
{
    Param tmp =*(Param *)arg;
    while (1)
    {
        tmp.touch_event = get_touch(tmp.touch_point);
    }

    //*touch_event = get_touch(touch_point);
}

