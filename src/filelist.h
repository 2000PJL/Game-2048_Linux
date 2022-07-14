/*
 * @Author: PengJL 
 * @Date: 2022-07-14 19:51:31
 * @LastEditors: PengJL 
 * @LastEditTime: 2022-07-14 21:22:51
 * @Description: 文件链表相关函数的声明
 * 
 * Copyright (c) by PengJL, All Rights Reserved. 
 */
#ifndef __FILELIST_H__
#define __FILELIST_H__

/*
    用来存储文件名的节点
*/
typedef struct filenode
{
    char name[512];
    struct filenode* pre;
    struct filenode* next;  
}Filenode;

/*
    用来索引节点的文件列表
*/
typedef struct filelist
{
    Filenode* first;
    Filenode* last;
}Filelist;




#endif