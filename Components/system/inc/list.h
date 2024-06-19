/**
 * @file list.h
 * @author dokee (dokee.39@gmail.com)
 * @brief
 * @version 0.1
 * @date 2023-07-12
 *
 * @copyright Copyright (c) 2023
 *
 * @note Linux Kernel 源码
 */
#ifndef _LIST_H_
#define _LIST_H_

// 获得结构体 TYPE 的变量成员 MEMBER 在此结构体中的偏移量
#define offset_of(type, member) (unsigned int) & ((type *)0)->member
// 根据 "结构体 type 变量" 中的 "域成员变量 member 的指针 ptr" 来获取指向整个结构体变量的指针
#define container_of(ptr, type, member) ((type *)((char *)(ptr)-offset_of(type, member)))

// 双向链表的节点
typedef struct list_structure
{
    struct list_structure *next;
    struct list_structure *prev;
} ListObj;

// 初始化节点, 设置 name 节点的前继节点和后继节点都是指向 name 本身
#define LIST_HEAD_INIT(name) \
    {                        \
        &(name), &(name)     \
    }
// 定义表头节点, 新建双向链表表头 name, 并设置 name 的前继节点和后继节点都是指向 name 本身
#define LIST_HEAD(name) ListObj name = LIST_HEAD_INIT(name)

void list_init(ListObj *list);
void list_insert_after(ListObj *list, ListObj *node);
void list_insert_before(ListObj *list, ListObj *node);
void list_remove(ListObj *node);
int list_isempty(const ListObj *list);
unsigned int list_len(const ListObj *list);

#define list_entry(node, type, member) \
    container_of(node, type, member)

// 遍历节点
#define list_for_each(pos, head) \
    for (pos = (head)->next; pos != (head); pos = pos->next)

// 删除节点时使用
#define list_for_each_safe(pos, n, head)                   \
    for (pos = (head)->next, n = pos->next; pos != (head); \
         pos = n, n = pos->next)

#endif
