/*
 * @Author: PengJL 
 * @Date: 2022-07-15 14:56:21
 * @LastEditors: PengJL 
 * @LastEditTime: 2022-07-15 15:30:37
 * @Description: 显示bmp图片的相关函数声明
 * 
 * Copyright (c) by PengJL, All Rights Reserved. 
 */
#ifndef __BMP_H__
#define __BMP_H__

/**
 * @Author: PengJL
 * @Description: 在(x0,y0)坐标处显示fname图片
 * @param {char} *fname: 图片文件名
 * @param {int} x0: 图像的左上角x轴坐标
 * @param {int} y0: 图像的左上角y轴坐标
 * @return {*}
 */
void display_bmp(char *fname, int x0, int y0);


#endif