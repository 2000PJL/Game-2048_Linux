/*
 * @Author: PengJL 
 * @Date: 2022-07-14 19:32:52
 * @LastEditors: PengJL 
 * @LastEditTime: 2022-10-13 17:21:03
 * @Description: 文件双向链表，用来保存资源文件的名字
 * 
 * Copyright (c) by PengJL, All Rights Reserved. 
 */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<dirent.h>
#include<errno.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
#include<unistd.h>


#include"filelist.h"



/**
 * @Author: PengJL
 * @Description: 将查找到的文件名name保存到flist中去
 * @param {Filelist*} flist: 文件链表的地址
 * @param {char} name: 文件名
 * @return {*}  
 *          -1: 文件链表未创建
 *           1: 成功将name添加到文件链表中
 */
int filelist_Insertnode(Filelist* flist,char *name)
{
    if(flist == NULL)
    {
        printf("you are not create filelsit!\n");
        return -1;
    }

    Filenode* fnode = (Filenode *)malloc(sizeof(Filenode));
    strcpy(fnode->name,name);

    if(flist->first == NULL)
    {
        flist->first = fnode;
        flist->last = fnode;
        fnode->next = NULL;
        fnode->pre = NULL;
    }else{
        flist->last->next = fnode;
        fnode->pre = flist->last;
        flist->last = fnode;
        fnode->next = NULL;
    }
    return 1;
}

/**
 * @Author: PengJL
 * @Description: 创建一个文件链表，
 * @return {Filelist *} 
 *                  返回创建的文件链表的指针
 */
Filelist *create_flist()
{
    Filelist* flist = (Filelist *)malloc(sizeof(Filelist));
    flist->first = NULL;
    flist->last = NULL;
    return flist;
}


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
char *flist_findfile(Filelist *flist, char *name)
{
    if(flist == NULL)
    {
        printf("文件链表中没有节点，或没有找到相应资源\n");
        return NULL;
    }
    Filenode *fnode = (Filenode *)malloc(sizeof(Filenode));
    fnode = flist->first;

    while (1)
    {
        if(fnode == NULL)
        {
            return NULL;
        }

        if(strstr(fnode->name,name))
        {
            return fnode->name;
        }

        fnode = fnode->next;
    }
    
}


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
int findFile(Filelist* flist, char *dirname, char *file_postfix)
{
    int number = 0;
    DIR *dp = opendir(dirname);
    if(dp == NULL)
    {
        perror("failed to opendir");
        return 0;
    }
    struct dirent *dir;
    struct stat st;
    char filename[512];
    while(dir = readdir(dp))
    {
        if(strcmp(dir->d_name,".")==0||strcmp(dir->d_name,"..")==0) //获取到的dir是.或..时不处理
        {
            continue;
        }
        sprintf(filename,"%s/%s",dirname,dir->d_name); 
        int ret = stat(filename,&st);  //获取filename的属性
        if(ret == -1)
        {
            perror("failed to stat");
            return 0;
        }
        if(S_ISDIR(st.st_mode)) //当filename是目录文件时才进去查找
        {

            number+=findFile(flist,filename,file_postfix); //  

        }else if(S_ISREG(st.st_mode))  //当filename是普通文件时才判断它是否为file_postfix结尾的文件
        {
            int len = strlen(dir->d_name);
            char d[512];
            strcpy(d,dir->d_name);
            if(len>4&&strcmp(dir->d_name+len-4,file_postfix)==0) //当文件是以file_postfix结尾时,就将其加入链表中
            {
                number+=1;
                printf("%s\r\n",filename);
                filelist_Insertnode(flist,filename);
            }
        }
    }
    closedir(dp);
    return number;
}



/**
 * @Author: PengJL
 * @Description: 查找文件列表中fnode的下一个节点
 * @param {Filenode} *fnode: 文件链表节点的指针
 * @return {Filenode *}
 *              查找失败返回原节点的地址
 *              查找成功返回下一个节点的地址
 */
Filenode *flist_nextnode(Filelist* flist, Filenode* fnode)
{
    if(fnode->next == NULL)
    {
        printf("文件链表已到尾部!\n");
        return flist->first; 
    }

    return fnode->next;
}

/**
 * @Author: PengJL
 * @Description: 查找文件列表中fnode的上一个节点
 * @param {Filenode} *fnode: 文件链表节点的指针
 * @return {Filenode *}
 *              查找失败返回原节点的地址
 *              查找成功返回上一个节点的地址
 */
Filenode *flist_prenode(Filelist* flist, Filenode* fnode)
{
    if(fnode->pre == NULL)
    {
        printf("文件链表已到头部!\n");
        return flist->last; 
    }

    return fnode->pre;
}




/**
 * @Author: PengJL
 * @Description: 从文件链表flist中查找
 * 文件name的完整文件名
 * @param {Filelist} *flist: 文件链表的指针
 * @param {char} *name: 需要查找的文件名
 * @return {char *}
 *              查找失败返回NULL
 *              查找成功返回完整文件名的指针
 */
int flist_findnode(Filelist *flist, Filenode *fnode)
{
    if(flist == NULL)
    {
        printf("文件链表中没有节点，或没有找到相应资源\n");
        return -1;
    }

    if(flist->first == flist->last)
    {
        if(strstr(flist->first->name,fnode->name))
        {
            fnode = flist->first;
            return 1;
        }else{
            printf("没有找到%s\n",fnode->name);
            return -1;
        }
    }
    
    Filenode *fnode1 = (Filenode *)malloc(sizeof(Filenode));

    
    fnode1 = flist->first->next;

    while (1)
    {
        if(fnode1 == flist->last)
        {
            fnode1 = flist->first;
            if(strstr(fnode1->name,fnode->name))
            {
                //free(*fnode);
                fnode = fnode1;
                return 1;
            }else{
                printf("没有找到%s\n",fnode->name);
                return -1;
            }
        }

        if(strstr(fnode1->name,fnode->name))
        {
            //free(*fnode);
            fnode = fnode1;
            return 1;
        }else{
            fnode1 = fnode1->next;
        }
    }   
}

