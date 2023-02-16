/*
 * @Author: PengJL 
 * @Date: 2022-07-14 19:51:31
 * @LastEditors: PengJL 
 * @LastEditTime: 2022-10-22 21:46:56
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
    char name[256];
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


/**
 * @Author: PengJL
 * @Description: 将查找到的文件名name保存到flist中去
 * @param {Filelist*} flist: 文件链表的地址
 * @param {char} name: 文件名
 * @return {*}  
 *          -1: 文件链表未创建
 *           1: 成功将name添加到文件链表中
 */
int filelist_Insertnode(Filelist* flist,char *name);


/**
 * @Author: PengJL
 * @Description: 创建一个文件链表，
 * @return {Filelist *} 
 *                  返回创建的文件链表的指针
 */
Filelist *create_flist();



/**
 * @Author: PengJL
 * @Description: 从文件链表flist中查找
 * 文件name的的完整文件名
 * @param {Filelist} *flist: 文件链表的指针
 * @param {char} *name: 需要查找的文件名
 * @return {char *}
 *              查找失败返回NULL
 *              查找成功返回完整文件名的指针
 */
char *flist_findfile(Filelist *flist, char *name);


/**
 * @Author: PengJL
 * @Description: 从dirname路径下查找以
 * file_postfix结尾的文件，找到符合规则的
 * 节点后，将其文件名加入到flist文件链表中去
 * @param {Filelist*} flist: 需要存储的文件链表
 * @param {char} *dirname: 需要查找的目录
 * @param {char} *file_postfix: 以file_postfix结尾的文件
 * @return {*}  num: 返回找到的符合要求的文件数量
 */
int findFile(Filelist* flist, char *dirname, char *file_postfix);



#endif