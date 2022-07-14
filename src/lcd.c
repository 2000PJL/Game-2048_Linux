/*
 * @Author: PengJL 
 * @Date: 2022-07-14 16:18:30
 * @LastEditors: PengJL 
 * @LastEditTime: 2022-07-14 16:52:53
 * @Description: 800*480LCD屏幕的基本控制函数实现
 * 
 * Copyright (c) by PengJL, All Rights Reserved. 
 */
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<errno.h>
#include<fcntl.h>
#include<unistd.h>
#include<sys/ioctl.h>
#include<sys/mman.h>
#include<linux/fb.h>
#include"lcd.h"


#define FB_FILE "/dev/fb0" //定义一个宏指向屏幕的设备文件

struct fb_var_screeninfo var; //用来保存屏幕信息的结构体

int fb_fd = -1; //帧缓冲设备的文件描述符
int lcd_width;  //屏幕每行像素的数量
int lcd_height;  //屏幕像素的行数
int byte_per_pixel; //屏幕像素单元占用的字节数

void open_lcd(void)
{
    int fd = open(FB_FILE,O_WRONLY);
    
}


