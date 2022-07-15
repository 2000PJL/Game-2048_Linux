/*
 * @Author: PengJL 
 * @Date: 2022-07-15 14:57:00
 * @LastEditors: PengJL 
 * @LastEditTime: 2022-07-15 16:00:13
 * @Description: 在LCD屏幕上显示bmp图片的相关函数实现
 * 
 * Copyright (c) by PengJL, All Rights Reserved. 
 */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

#include "lcd.h"


/**
 * @Author: PengJL
 * @Description: 在(x0,y0)坐标处显示fname图片
 * @param {char} *fname: 图片文件名
 * @param {int} x0: 图像的左上角x轴坐标
 * @param {int} y0: 图像的左上角y轴坐标
 * @return {*}
 */
void display_bmp(char *fname, int x0, int y0)
{
    int fd = open(fname, O_RDONLY); //bmp文件的文件描述符
    if (fd == -1)
    {
        perror("failed to open bmp_file");
        return ;
    }

    int width, height;
    short depth;
    unsigned char buf[4];

    //微软bmp文件中，单个数据项是用小端模式保存，应考虑到大小端数据转换的问题
    //X86和ARM的数据存储格式不一样
    lseek(fd, 0x12, SEEK_SET);
    read(fd, buf, 4);
    width = (buf[3] << 24) | (buf[2] << 16) | (buf[1] << 8) | buf[0];


    lseek(fd, 0x16, SEEK_SET);
    read(fd, buf, 4);
    height = (buf[3] << 24) | (buf[2] << 16) | (buf[1] << 8) | buf[0];

    lseek(fd, 0x1C, SEEK_SET);
    read(fd, buf, 2);
    depth =  (buf[1] << 8) | buf[0];

    printf("The resolution of image is %d * %d\n", width, height);

    if ( !(depth == 24 || depth == 32))
    {
        printf("Sorry, The file format of the image is not supported!\n");
        close(fd);

        return ;
    }


    int valid_bytes_per_line; //每一行有效的数据字节数
    int temp_num = 0; // 每一行填充的无效数据字节数
    int total_bytes_per_line; //每一行实际的字节数.
    int total_bytes; //整个像素数组的字节数

    //微软定义bmp文件的规则中，要求存储的bmp文件中，每行像素的字节数为4的倍数，
    //不足的在每行后面补0，这些数据是无效数据，读取后应该丢弃
    valid_bytes_per_line =  abs(width) * (depth / 8);
    if (valid_bytes_per_line % 4)
    {
        temp_num = 4 - valid_bytes_per_line % 4;
    }

    total_bytes_per_line = valid_bytes_per_line + temp_num;
    total_bytes = abs(height) * total_bytes_per_line;

    unsigned char * pixel = (unsigned char*) malloc( total_bytes );
    lseek(fd, 54, SEEK_SET);
    read(fd, pixel, total_bytes);//直接将bmp文件中的图像数据读取到pixel中

    // 解析像素数据，并在屏幕上显示
    unsigned char a, r, g, b;
    int color;
    int i = 0;
    int x, y;

    unsigned int *images = (unsigned int *)malloc(sizeof(int)*height*width);
    

    for (y = 0; y < abs(height); y++)
    {
        for (x = 0; x < abs(width); x++)
        {
            b = pixel[i++];
            g = pixel[i++];
            r = pixel[i++];
            //判断图像是否为32位图像，即是否用到了alpha域，透明度的数据。
            //后面想办法换jpg图像，bmp图像占用了很大的存储，而且转换麻烦
            if (depth == 32) 
            {
                a = pixel[i++];
            }
            else
            {
                a = 0;
            }
            color = (a << 24) | (r << 16) | (g << 8) | b;

            int x1, y1; //该像素点在屏幕上显示的坐标

            x1 = (width > 0) ? (x0 + x) : (x0 + abs(width) - 1 - x); //判断width，来确定bmp图像存储时的行扫描方向
            y1 = (height > 0) ?  (y0 + height - 1 - y) : y0 + y;    //判断height，来确定bmp图像存储时的列扫描方向
            
            //lcd_draw_point(x1, y1, color);
            *(images + (y1*width) + x1) = color;  
        }

        i += temp_num; //不处理无效数据位
    }
    free(pixel);

    for(int y = 0; y < height; y++)
    {
        for(int x = 0; x < width; x++)
        {
            lcd_draw_point(x, y, *(images + (y*width) + x));
        }
        usleep(1000);
    }
    free(images);
}