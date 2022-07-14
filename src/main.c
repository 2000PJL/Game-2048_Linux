/*
 * @Author: PengJL 
 * @Date: 2022-07-14 11:14:48
 * @LastEditors: PengJL 
 * @LastEditTime: 2022-07-14 19:24:53
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


int main()
{
    lcd_open();
    lcd_draw_rectangle(10,10,20,20,0xffffff);
    lcd_close();
    return 0;
}