/*
 * @Author: PengJL 
 * @Date: 2022-07-15 19:10:29
 * @LastEditors: PengJL 
 * @LastEditTime: 2022-07-15 19:27:46
 * @Description: 调用jpeg库去操作jpg文件的相关声明
 * 
 * Copyright (c) by PengJL, All Rights Reserved. 
 */
#ifndef  __JPEG_H__
#define  __JPEG_H__


/**
 * @Author: PengJL
 * @Description: 在LCD屏幕上(x0,y0)坐标
 * 处显示fname指向的jpg图片
 * @param {char} *fname: 需要显示的jpg图片的名字
 * @param {int} x0: 图片左上角的x轴坐标
 * @param {int} y0: 图片左上角的y轴坐标
 * @return {*}
 */
void display_jpg(char *fname, int x0, int y0);


#endif