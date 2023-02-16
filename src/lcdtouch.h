/*
 * @Author: PengJL 
 * @Date: 2022-07-14 08:55:20
 * @LastEditors: PengJL 
 * @LastEditTime: 2023-02-07 18:30:50
 * @Description: 
 * 
 * Copyright (c) by PengJL, All Rights Reserved. 
 */
#ifndef __LCDTOUCH_H__
#define __LCDTOUCH_H__


/*
    用来存储屏幕触摸坐标的信息
*/
typedef struct touch_point
{
    int x;
    int y;
}Touch_point; 


/*
    枚举类型，用来给滑动事件返回参数
*/
typedef enum 
{
    MOVE_UP = 1, //向上滑动
    MOVE_DOWN,   //向下滑动
    MOVE_LEFT,   //向左滑动
    MOVE_RIGHT,   //向右滑动

    MOVE_UNKNOWN = 100
} move_dir;

void get_coordinate(Touch_point* pot);

move_dir get_move_dir(void);

/**
 * @Author: PengJL
 * @Description: 用来获取屏幕上的触摸事件和滑动事件
 * @param {Touch_point} *pot: 存放触摸的坐标
 * @return {int}
 *              1: 上
 *              2: 下
 *              3: 左
 *              4: 右
 *              5: 触摸        
 */
int get_touch(volatile Touch_point *pot);

#endif