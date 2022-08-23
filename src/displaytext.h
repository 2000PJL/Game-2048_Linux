/*
 * @Author: PengJL 
 * @Date: 2022-07-15 09:33:34
 * @LastEditors: PengJL 
 * @LastEditTime: 2022-08-11 17:26:32
 * @Description: 在LCD屏幕上通过点阵字库方式
 * 显示需要显示的文字
 * 
 * Copyright (c) by PengJL, All Rights Reserved. 
 */
#ifndef __DISPLAY_H__
#define __DISPLAY_H__


/**
 * @Author: PengJL
 * @Description: 在LCD屏幕的(x0,y0)坐标处，
 * 显示宽度为width，高度为height的word字符
 * @param {char} word: 需要显示的字符
 * @param {int} x0: 字符word左上角的x轴坐标
 * @param {int} y0: 字符word左上角的y轴坐标
 * @param {int} width: 字符的宽度
 * @param {int} height: 字符的高度
 * @return {*} 无返回值
 */
void display_text(const char word[], int x0, int y0, int width, int height, char mode);


/**
 * @Author: PengJL
 * @Description: 在LCD屏幕上
 * 的(x0,y0)坐标处显示整数number
 * @param {int} number: 需要显示的数字
 * @param {int} x0: 数字number左上角的x轴坐标
 * @param {int} y0: 数字number左上角的y轴坐标
 * @return {*}  无返回值
 */
void display_int(int number, int x0, int y0);



/**
 * @Author: PengJL
 * @Description: 在LCD屏幕上(x0,y0)坐标处
 * 显示小数num，但是只显示小数点后两位
 * @param {double} num: 需要显示的小数num
 * @param {int} x0: num左上角的x轴坐标
 * @param {int} y0: num左上角的y轴坐标
 * @return {*}  无返回值
 */
void display_double(double num, int x0, int y0);

/**
 * @Author: PengJL
 * @Description: 显示分数的标题栏,大约需要 93 * 39 的区域
 * @param {int} x0: x轴坐标
 * @param {int} y0: y轴坐标
 * @return {*}
 */
void display_score(int x0, int y0);


#endif