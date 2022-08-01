/*
 * @Author: PengJL 
 * @Date: 2022-07-15 20:36:04
 * @LastEditors: PengJL 
 * @LastEditTime: 2022-07-23 17:01:22
 * @Description: 2048游戏主体逻辑的相关函数声明
 * 
 * Copyright (c) by PengJL, All Rights Reserved. 
 */
#ifndef __GAME_LOGIC_H__
#define __GAME_LOGIC_H__

typedef struct param
{
    Filelist *flist;
}Param;


void mainpage_init(Filelist *image_flist);

void gamepage_init(Filelist *flist);

void *listen_touch_thread(void *arg);

/**
 * @Author: PengJL
 * @Description: 游戏主处理逻辑线程
 * @param {void} *arg: 传入image_flist的地址，
 * 函数内部能直接调用主进程中创建的jpg图片的文件链表
 * 
 * @return {*}
 */
void *game_logic_thread(void *arg);

/**
 * @Author: PengJL
 * @Description: 后台播放指定背景音乐
 * @param {void} *argv: 文件链表指针
 * @return {*}
 */
void *mp3_play_thread(void *arg);

/**
 * @Author: PengJL
 * @Description: 获取按键触摸响应值
 * @return {int}
 *              1: 开始游戏
 *              0: 退出游戏
 */
int get_button();


/**
 * @Author: PengJL
 * @Description: 触摸事件响应
 * @param {Filelist} *image_flist
 * @return {*}
 */
void touch_program(Filelist *image_flist);


/**
 * @Author: PengJL
 * @Description: 在棋盘上随机产生number张
 * 新牌,如果number数量大于空格数量，就产生
 * 空格数量的牌;
 * @return {*}
 */
void random_program();


/**
 * @Author: PengJL
 * @Description: 操作是否改变了视图
 * @return {*}
 */
void matrixIsChange();


/**
 * @Author: PengJL
 * @Description: 屏幕滑动事件处理
 * @param NULL
 * @return {*}
 */
void move_program(Filelist* flist);

/**
 * @Author: PengJL
 * @Description: 求2048棋盘空格数量
 * @return {int}
 *              返回棋盘上空格的数量
 */
int matrixEmptyNum();


/**
 * @Author: PengJL
 * @Description: 更新游戏的视图 
 * @param {Filelist*} flist: 图片的文件链表
 * @return {*}
 */
void refresh_view(Filelist* flist);

/**
 * @Author: PengJL
 * @Description: 游戏获胜
 * @param {Filelist*} flist
 * @return {*}
 */
void game_win(Filelist* flist);


/**
 * @Author: PengJL
 * @Description: 游戏结束
 * @param {Filelist*} flist
 * @return {*}
 */
void game_over(Filelist* flist);

/**
 * @Author: PengJL
 * @Description: 判断2048是否不可移动
 * @return {int}
 *             1: 如果不可移动就返回1
 *             0: 如果可以移动就返回0
 */
int judge_NotMove();


/**
 * @Author: PengJL
 * @Description: 判断是否出现了2048，
 *  出现了就终止所有线程
 * @return {*}
 */
void gameIsWin(Filelist *flist);



#endif