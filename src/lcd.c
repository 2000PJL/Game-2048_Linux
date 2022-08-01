/*
 * @Author: PengJL 
 * @Date: 2022-07-14 16:18:30
 * @LastEditors: PengJL 
 * @LastEditTime: 2022-07-14 19:28:34
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



int fb_fd = -1; //帧缓冲设备的文件描述符
int lcd_width;  //屏幕每行像素的数量
int lcd_height;  //屏幕像素的行数
int bytes_per_pixel; //屏幕像素单元占用的字节数

int *plcd = NULL; //指向屏幕帧缓冲区首地址的指针

/**
 * @Author: PengJL
 * @Description: 完成屏幕的初始化，
 * 从内核中读取屏幕的相关参数信息，并
 * 将相关参数打印到终端
 * @return {*} 无返回值
 */
void lcd_open(void)
{
    int fd = open(FB_FILE,O_RDWR);
    if(fd == -1)
    {
        perror("failed to open fb0");
        return ;
    }

    fb_fd = fd;

    struct fb_var_screeninfo fb_var; //用来保存屏幕信息的结构体
    int ret = ioctl(fb_fd, FBIOGET_VSCREENINFO,&fb_var);
    if(ret == -1)
    {
        perror("failed to ioctl FBIOGET_VSCREENINFO");
        close(fb_fd);
        fb_fd = -1;
        return ;
    }
    
    //读取到屏幕的信息后将其存入到全局变量中
    //这些信息包括屏幕的宽度，高度，像素位深字节
    lcd_width = fb_var.xres;
    lcd_height = fb_var.yres;
    bytes_per_pixel= fb_var.bits_per_pixel/8;

    printf("The resolution of LCD screen is %d * %d\n", lcd_height, lcd_width);
    printf("A singal ipxel need %d bytes\n", bytes_per_pixel);
    
    //内存映射,将屏幕刷新缓冲区的首地址传给plcd
    plcd = (int *)mmap(NULL, lcd_width * lcd_height * bytes_per_pixel, PROT_WRITE, MAP_SHARED, fb_fd, 0);
    if(plcd == MAP_FAILED)
    {
        perror("failed to mmap");
        close(fb_fd);
        fb_fd = -1;
        return ;
    }

}

/**
 * @Author: PengJL
 * @Description: 解除
 * 屏幕缓冲区的内存映射,并释放
 * 屏幕的设备文件
 * @return {*}  无返回值
 */
void lcd_close(void)
{
    //解除内存映射
    munmap(plcd, lcd_width * lcd_height * bytes_per_pixel);

    //释放设备
    close(fb_fd);
    fb_fd = -1;
}


/**
 * @Author: PengJL
 * @Description: 在指定的(x,y)坐标位置绘制
 * 指定(color)颜色的点，使用到了内联(inline)
 * ，提高函数的执行效率，节约绘制时间
 * @param {int} x: 屏幕上x轴坐标
 * @param {int} y: 屏幕上y轴坐标
 * @param {int} color: 用24位整数表示的RGB色彩
 * @return {*}  无返回值
 */
inline void lcd_draw_point(int x, int y, int color)
{
    if(x >= 0 && x < lcd_width && y >= 0 && y < lcd_height)
    {
        *(plcd + y * lcd_width + x) = color;
    }
}


/**
 * @Author: PengJL
 * @Description: 以(x0,y0)为矩形的左上角，
 * 将其当做绘制的起点，绘制宽度为width，高度
 * 为height，颜色为color的实心矩形
 * 
 * @param {int} x0: 矩形左上角的x坐标
 * @param {int} y0: 矩形左上角的y坐标
 * @param {int} width: 矩形的宽度width
 * @param {int} height: 矩形的高度height
 * @param {int} color： 矩形内部的填充颜色
 * @return {*} 无返回值
 */
void lcd_draw_rectangle(int x0, int y0, int width, int height, int color)
{
    int x,y;
    //点(x,y)为矩形内的点，坐标应该满足如下条件
    //    x >= x0 && x < x0 + width
    //    y >= y0 && y < y0 + height

    for(y = y0; y < y0 + height; y++)
    {
        for(x = x0; x < x0 + width; x++)
        {
            lcd_draw_point(x, y, color);
        }
    }

}


