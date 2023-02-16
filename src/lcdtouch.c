/*
 * @Author: PengJL 
 * @Date: 2022-07-14 08:56:12
 * @LastEditors: PengJL 
 * @LastEditTime: 2023-02-07 18:31:43
 * @Description: 
 * 
 * Copyright (c) by PengJL, All Rights Reserved. 
 */
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<linux/input.h>
#include"lcdtouch.h"
#include<errno.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>


void get_coordinate(Touch_point *pot)
{
    int touch_fd = open("/dev/input/event0",O_RDONLY);
    if(touch_fd == -1)
    {
        perror("failed to open input/event0");
        return ;
    }
    struct input_event ev;
    
    int x1 = -1, y1 = -1, x2 = 0, y2 =0;

    while(1)
    {
        int ret = read(touch_fd,&ev,sizeof(ev));
        if( ret != sizeof(ev))
        {
            continue;
        }
        if(ev.type == EV_ABS && ev.code == ABS_X)
        {
            if(x1 == -1)
            {
                x1 = ev.value;
            }
            x2 = ev.value;
        }
        if(ev.type == EV_ABS && ev.code == ABS_Y)
        {
            if(y1 == -1)
            {
                y1 = ev.value;
            }
            y2 = ev.value;
        }

        if ( (ev.type == EV_KEY && ev.code == BTN_TOUCH &&  ev.value == 0) ||
            (ev.type == EV_ABS && ev.code == ABS_PRESSURE && ev.value ==0) )
        {
            x1 = x1 * 800.0 / 1024;
            x2 = x2 * 800.0 / 1024;

            y1 = y1 * 480.0 / 600;
            y2 = y2 * 480.0 / 600;

            pot->x = x2;
            pot->y = y2;
            break;
        }

    }
    
    close(touch_fd);

}


move_dir get_move_dir(void)
{
    int touch_fd = open("/dev/input/event0",O_RDONLY);
    if(touch_fd == -1)
    {
        perror("failed to open input/event0");
        return -1;
    }
    struct input_event ev;
    
    move_dir dir = MOVE_UNKNOWN;
    int x1 = -1, y1 = -1, x2 = 0, y2 =0;

    while(1)
    {
        int ret = read(touch_fd,&ev,sizeof(ev));
        if( ret != sizeof(ev))
        {
            continue;
        }
        if(ev.type == EV_ABS && ev.code == ABS_X)
        {
            if(x1 == -1)
            {
                x1 = ev.value;
            }
            x2 = ev.value;
        }
        if(ev.type == EV_ABS && ev.code == ABS_Y)
        {
            if(y1 == -1)
            {
                y1 = ev.value;
            }
            y2 = ev.value;
        }

        if ( (ev.type == EV_KEY && ev.code == BTN_TOUCH &&  ev.value == 0) ||
            (ev.type == EV_ABS && ev.code == ABS_PRESSURE && ev.value ==0) )
        {
            int delt_x = abs(x2 - x1);
            int delt_y = abs(y2 - y1);

            //当一个方向上两点坐标的差值在x，y轴上出现两倍的关系时，才将其当做是我们需要的事件
            if (delt_x >= 2 * delt_y) 
            {
                if (x2 > x1)
                {
                    dir = MOVE_RIGHT;
                }
                else
                {
                    dir = MOVE_LEFT;
                }

                break;
            }
            else if (delt_y >= 2 * delt_x)
            {
                if (y2 > y1)
                {
                    dir = MOVE_DOWN;
                }
                else
                {
                    dir = MOVE_UP;
                }
                break;
            }
            else 
            {
                x1 = -1;
                y1 = -1;
            }
            
        }

    }
    
    close(touch_fd);
    return dir;
}


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
int get_touch(volatile Touch_point *pot)
{
    int touch_fd = open("/dev/input/event0",O_RDONLY);
    if(touch_fd == -1)
    {
        perror("failed to open input/event0");
        return -1;
    }
    struct input_event ev;
    

    int x1 = -1, y1 = -1, x2 = 0, y2 =0, dir = -1;

    while(1)
    {
        int ret = read(touch_fd,&ev,sizeof(ev));
        if( ret != sizeof(ev))
        {
            continue;
        }
        if(ev.type == EV_ABS && ev.code == ABS_X)
        {
            if(x1 == -1)
            {
                x1 = ev.value;
            }
            x2 = ev.value;
        }
        if(ev.type == EV_ABS && ev.code == ABS_Y)
        {
            if(y1 == -1)
            {
                y1 = ev.value;
            }
            y2 = ev.value;
        }



        if ( (ev.type == EV_KEY && ev.code == BTN_TOUCH &&  ev.value == 0) ||
            (ev.type == EV_ABS && ev.code == ABS_PRESSURE && ev.value ==0) )
        {
            int delt_x = abs(x2 - x1);
            int delt_y = abs(y2 - y1);
            x1 = x1 * 800.0 / 1024;
            x2 = x2 * 800.0 / 1024;

            y1 = y1 * 480.0 / 600;
            y2 = y2 * 480.0 / 600;
            
            //printf("x1 = %d,y1 = %d, x2 = %d, y2 =%d\n",x1,y2,x2,y2);

            //如果松手时的坐标与点击时的坐标差值在30个像素点以内，就认为是点击事件
            if(delt_x * delt_x + delt_y * delt_y < 900)
            {
                pot->x = x2;
                pot->y = y2;
                return 5;
            }

            //当一个方向上两点坐标的差值在x，y轴上出现两倍的关系时，才将其当做是我们需要的事件
            if (delt_x >= 2 * delt_y) 
            {
                if (x2 > x1)
                {
                    dir = 4;
                }
                else
                {
                    dir = 3;
                }

                break;
            }
            else if (delt_y >= 2 * delt_x)
            {
                if (y2 > y1)
                {
                    dir = 2;
                }
                else
                {
                    dir = 1;
                }
                break;
            }
            else 
            {
                x1 = -1;
                y1 = -1;
            }
            
        }

    }
    
    close(touch_fd);
    return dir;
}

