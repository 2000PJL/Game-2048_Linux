/*
 * @Author: PengJL 
 * @Date: 2022-07-15 19:09:08
 * @LastEditors: PengJL 
 * @LastEditTime: 2022-07-20 08:42:39
 * @Description: 调用官方的jpeg库去操作jpg图片
 * 
 * Copyright (c) by PengJL, All Rights Reserved. 
 */
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<unistd.h>


#include"jpeglib.h"
#include"jpeg.h"
#include"lcd.h"



/**
 * @Author: PengJL
 * @Description: 在LCD屏幕上(x0,y0)坐标
 * 处显示fname指向的jpg图片
 * @param {char} *fname: 需要显示的jpg图片的名字
 * @param {int} x0: 图片左上角的x轴坐标
 * @param {int} y0: 图片左上角的y轴坐标
 * @return {*}
 */
void display_jpg(char *fname, int x0, int y0)
{
    if(fname == NULL)
    {
        printf("没有传入jpg\n");
        return;
    }

    struct jpeg_decompress_struct dinfo;//申明一个解压对象,用来存储解压的图片对象

    struct jpeg_error_mgr jerr;//声明一个出错信息的对象

    dinfo.err=jpeg_std_error(&jerr);

    jpeg_create_decompress(&dinfo);//初始化dinfo这个对象


    FILE *fd=fopen(fname,"r");
    if(fd==NULL)
    {
        perror("faile to open fd");
        return ;
    }
    
    //将jpg图片fd解压给dinfo
    jpeg_stdio_src(&dinfo, fd);


    //读取jpg文件的文件头，获取图片的基本信息
    jpeg_read_header(&dinfo, TRUE);


    //启动jpeg解压器
    jpeg_start_decompress(&dinfo);

    // -（5）在调用完上面这个函数之后，jpeg对象dinfo中
    // >	下面几个字段（成员变量）将会比较有用：
    // >	dinfo.output_width: 解压后图像的输出宽度，一行占多少个像素点
    // >   dinfo.output_height: 解压后图像的输出高度，占多少行
    // >   dinfo.output_components: 每个像素点的分量数，每个像素点占多少个字节
    // >>					3 RGB
    // >>					4 ARGB
    // >	总字节数=dinfo.output_width*dinfo.output_height*dinfo.output_components
                        
                
    // >  在调用完jpeg_start_decompress()后，往往需要为解压后的扫描线上所有的像素点分配空间
    // >  存一行：	malloc(dinfo.output_width * dinfo.output_components);	jpeg_start_decompress(&dinfo);

    //printf("the resolution of jpg is %d * %d\n", dinfo.output_width, dinfo.output_height);

    unsigned char * buffer = (unsigned char*) malloc( dinfo.output_width * dinfo.output_components);
    // - (6)读取一行或者多行扫描线上数据处理通常是这样的
	// 对扫描线的读取是按照从上到下的顺序进行的，也就是说图像最上方的扫描线最先
    // 被jpeg_read_scanlines()读入到存储空间中，紧接着是第二行扫描线，最后是
    // 图像底边的扫描线被读入到存储空间中去。		
    //dinfo.output_scanline   表示的意思是已经扫描了多少行
    unsigned int colors[dinfo.output_height][dinfo.output_width];

    
    while(dinfo.output_scanline<dinfo.output_height)
    {
        //一次从dinfo中读取一行像素放到buffer中去
        jpeg_read_scanlines(&dinfo,&buffer,1);
        int i = 0;
        for (int x = 0; x < dinfo.output_width; x++)
        {
            unsigned char a = 0, r, g, b;
            int color;
            if (dinfo.output_components == 3)
            {
                a = 0;
                r = buffer[i++];
                g = buffer[i++];
                b = buffer[i++];
            }
            else if (dinfo.output_components == 4) 
            {
                a = buffer[i++];
                r = buffer[i++];
                g = buffer[i++];
                b = buffer[i++];
            }
            
            color = (a << 24) | (r << 16) | (g << 8) | b;
            colors[dinfo.output_scanline - 1][x] = color;
            //lcd_draw_point(x0 + x, y0 + dinfo.output_scanline - 1, color);
        }
    }

    int width = dinfo.output_width;
    int height = dinfo.output_height;

    
    
    for(int y1 = 0,y2 = height-1; y1 <= y2; y1++,y2--)
    {
        for(int x1 = 0,x2 = width-1; x1 <= x2 ; x1++,x2--)
        {
            lcd_draw_point(x0 + x1, y0 + y1, colors[y1][x1]);
            lcd_draw_point(x0 + x1, y0 + y2, colors[y2][x1]);
            lcd_draw_point(x0 + x2, y0 + y1, colors[y1][x2]);
            lcd_draw_point(x0 + x2, y0 + y2, colors[y2][x2]);

        }
        //usleep(10);
    }
			
    //调用jpeg_finish_decompress()完成解压过程
    jpeg_finish_decompress(&dinfo);

    //调用jpeg_destroy_decompress释放jpeg解压对象dinfo
    jpeg_destroy_decompress(&dinfo);


    free(buffer);
    fclose(fd);

}

