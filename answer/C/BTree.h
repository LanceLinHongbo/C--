#ifndef BTREE_
#define BTREE_

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// B树的阶数，决定每个节点的孩子数量
#define BTREE_ORDER_SIZE 6

// 比较函数指针类型
typedef int (*cmpFuncPtr)(void *, void *);

// 定义B树的key类型, 利于泛型
typedef int keyOfBTree;

// 打印函数指针类型
typedef void (*printFun)(keyOfBTree);

// B树的节点结构
typedef struct BTreeNode
{
    keyOfBTree keys[BTREE_ORDER_SIZE - 1];      // 关键字数组
    struct BTreeNode *childs[BTREE_ORDER_SIZE]; // 孩子节点指针数组
    uint32_t num; // 当前节点中的关键字数量
    int is_leaf;  // 是否为叶子节点
} BTreeNode;

typedef BTreeNode *BTree;

// 创建节点
BTreeNode *createNode(int is_leaf);

// 在B树中插入关键字
void insert(BTreeNode **root, keyOfBTree key, cmpFuncPtr cmp);

// 打印B树的关键字
void printBTree(BTreeNode *node, printFun printKey, int left, int *cnt);

// 释放BTree
void freeBTree(BTreeNode **node);

// 查找key在BTree中的位置
keyOfBTree *search(BTreeNode *root, keyOfBTree key, cmpFuncPtr cmp);

// 在B树中删除关键字key
void deleteKey(struct BTreeNode **root, keyOfBTree key, cmpFuncPtr cmp);

#endif
