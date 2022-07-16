/*
 * @Author: PengJL 
 * @Date: 2022-07-15 20:36:04
 * @LastEditors: PengJL 
 * @LastEditTime: 2022-07-16 20:49:08
 * @Description: 2048游戏主体逻辑的相关函数声明
 * 
 * Copyright (c) by PengJL, All Rights Reserved. 
 */
#ifndef __GAME_LOGIC_H__
#define __GAME_LOGIC_H__

typedef struct param
{
    Touch_point *touch_point;
    int touch_event;
}Param;


void mainpage_init(Filelist *image_flist);

void gamepage_init(Filelist *flist);

void *listen_touch_thread(void *arg);

#endif