/*
 * @Author: PengJL 
 * @Date: 2022-07-14 19:20:13
 * @LastEditors: PengJL 
 * @LastEditTime: 2022-07-14 19:24:33
 * @Description: LCD屏幕控制函数的声明
 * 
 * Copyright (c) by PengJL, All Rights Reserved. 
 */
#ifndef __LCD_H__
#define __LCD_H__

/**
 * @Author: PengJL
 * @Description: 完成屏幕的初始化，
 * 从内核中读取屏幕的相关参数信息，并
 * 将相关参数打印到终端
 * @return {*} 无返回值
 */
void lcd_open(void);


/**
 * @Author: PengJL
 * @Description: 解除
 * 屏幕缓冲区的内存映射,并释放
 * 屏幕的设备文件
 * @return {*}  无返回值
 */
void lcd_close(void);


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
void lcd_draw_point(int x, int y, int color);


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
void lcd_draw_rectangle(int x0, int y0, int width, int height, int color);



#endif