#include "BTree.h"

// 创建节点
BTreeNode *createNode(int is_leaf)
{
    BTreeNode *node = (BTreeNode *)malloc(sizeof(BTreeNode));
    memset(node, 0, sizeof(BTreeNode));
    node->is_leaf = is_leaf;
    return node;
}

// 查找关键字在节点中的索引位置
static int searchKeyIndex(BTreeNode *node, keyOfBTree key, cmpFuncPtr cmp)
{
    int index = 0;
    while (index < node->num && cmp(&key, &(node->keys[index])) > 0)
    {
        index++;
    }
    return index;
}

// 插入关键字到节点中的指定位置
static void insertKey(BTreeNode *node, keyOfBTree key, cmpFuncPtr cmp)
{
    int index = (int)node->num - 1;
    while (index >= 0 && cmp(&key, &(node->keys[index])) < 0)
    {
        node->keys[index + 1] = node->keys[index];
        index--;
    }
    node->keys[index + 1] = key;

    node->num++;
}

// 分裂一个满节点，将中间的关键字提升为父节点，并创建两个新的子节点
static void splitNode(BTreeNode *parent, int child_index)
{
    BTreeNode *child = parent->childs[child_index];

    BTreeNode *new_node = createNode(child->is_leaf);
    new_node->num = BTREE_ORDER_SIZE / 2 - 1;

    for (int i = 0; i < new_node->num; i++)
    {
        new_node->keys[i] = child->keys[BTREE_ORDER_SIZE / 2 + i];
    }

    if (!child->is_leaf)
    {
        for (int i = 0; i < BTREE_ORDER_SIZE / 2; i++)
        {
            new_node->childs[i] = child->childs[BTREE_ORDER_SIZE / 2 + i];
        }
    }

    child->num = BTREE_ORDER_SIZE / 2 - 1;

    for (int i = (int)parent->num; i > child_index; i--)
    {
        parent->childs[i + 1] = parent->childs[i];
    }

    parent->childs[child_index + 1] = new_node;

    for (int i = (int)parent->num - 1; i >= child_index; i--)
    {
        parent->keys[i + 1] = parent->keys[i];
    }

    parent->keys[child_index] = child->keys[BTREE_ORDER_SIZE / 2 - 1];
    parent->num++;
}

// 在B树中插入关键字
void insert(BTreeNode **root, keyOfBTree key, cmpFuncPtr cmp)
{
    BTreeNode *node = *root;

    // 如果根节点为空，则创建新的根节点
    if (node == NULL)
    {
        *root = createNode(1);
        insertKey(*root, key, cmp);
        return;
    }

    // 如果根节点已满，则需要创建一个新的根节点
    if (node->num == BTREE_ORDER_SIZE - 1)
    {
        BTreeNode *new_root = createNode(0);
        new_root->childs[0] = node;
        *root = new_root;
        splitNode(new_root, 0);
        insert(root, key, cmp); // 递归插入
        return;
    }

    // 如果根节点既非空也未满，直接插入
    while (1)
    {
        if (node->is_leaf)
        {
            insertKey(node, key, cmp);
            break;
        }

        int index = searchKeyIndex(node, key, cmp);
        if (node->childs[index]->num == BTREE_ORDER_SIZE - 1)
        {
            splitNode(node, index);
            if (cmp(&key, &(node->keys[index])) > 0)
            {
                index++;
            }
        }
        node = node->childs[index];
    }
}

// 打印B树的关键字
void printBTree(BTreeNode *node, printFun printKey, int left, int *cnt)
{
    if (node)
    {
        printf("%c%.2d([", "ABCDEFG"[left++], ++*cnt);
        for (int i = 0; i < node->num; i++)
        {
            printKey(node->keys[i]);
        }
        printf("]);\n");

        if (!node->is_leaf)
        {
            int leftL = left - 1;
            int cntL = *cnt;
            for (int i = 0; i <= node->num; i++)
            {
                printf("%c%.2d==>", "ABCDEFG"[leftL], cntL);
                printBTree(node->childs[i], printKey, left, cnt);
            }
            printf("\n");
        }
    }
}

// 释放BTree
void freeBTree(BTreeNode **node)
{
    if (*node)
    {
        // 非叶子节点必有子节点, 递归删除子节点
        if (!(*node)->is_leaf)
        {
            // 子节点的数量不会大于key数量加1, 所以不用free child数组中所有节点;
            for (int i = 0; i <= (*node)->num; i++)
            {
                freeBTree(&((*node)->childs[i]));
            }
        }

        free(*node);
        *node = NULL;
    }
}

// 查找key在BTree中的位置
keyOfBTree *search(BTreeNode *root, keyOfBTree key, cmpFuncPtr cmp)
{
    // 如果root为空, 返回NULL
    if (!root)
    {
        return NULL;
    }

    // 查找key在节点中的索引
    int index = searchKeyIndex(root, key, cmp);

    // 如果节点索引小于节点中key数量, 且key等于node在索引处的key值
    if (index < root->num && cmp(&key, &(root->keys[index])) == 0)
    {
        // 返回key在node中的指针
        return &(root->keys[index]);
    }

    // 如果节点不是叶子节点, 递归搜索索引为index的子节点
    if (!root->is_leaf)
    {
        return search(root->childs[index], key, cmp);
    }

    // 以上全没找到, 返回空指针
    return NULL;
}

// 合并节点
static void mergeNodes(struct BTreeNode *left, struct BTreeNode *root,
                       struct BTreeNode *right, int index)
{
    // 将根节点中的关键字移动到左子节点中
    left->keys[left->num] = root->keys[index];
    for (int i = 0; i < right->num; i++)
    {
        left->keys[left->num + 1 + i] = right->keys[i];
    }

    // 将根节点中的子节点移动到左子节点中
    if (!left->is_leaf)
    {
        for (int i = 0; i < right->num + 1; i++)
        {
            left->childs[left->num + 1 + i] = right->childs[i];
        }
    }

    // 更新左子节点的关键字个数
    left->num += right->num + 1;

    // 从根节点中删除关键字和子节点
    for (int i = index; i < root->num - 1; i++)
    {
        root->keys[i] = root->keys[i + 1];
    }
    for (int i = index + 1; i < root->num; i++)
    {
        root->childs[i] = root->childs[i + 1];
    }
    root->num--;

    // 释放右子节点的内存
    free(right);
}

// 从左右兄弟借关键字
static int borrowNode(struct BTreeNode *node, int index)
{
    // 从左兄弟节点借关键字
    if (index != 0 && node->childs[index - 1]->num >= (BTREE_ORDER_SIZE / 2))
    {
        struct BTreeNode *leftChild = node->childs[index - 1];
        struct BTreeNode *child = node->childs[index];

        // 关键字右移
        for (int i = (int)child->num - 1; i >= 0; i--)
        {
            child->keys[i + 1] = child->keys[i];
        }
        child->keys[0] = node->keys[index - 1];
        node->keys[index - 1] = leftChild->keys[leftChild->num - 1];

        //  子节点右移
        if (!child->is_leaf)
        {
            for (int i = (int)child->num; i >= 0; i--)
            {
                child->childs[i + 1] = child->childs[i];
            }
            child->childs[0] = leftChild->childs[leftChild->num];
        }
        // 更新节点的关键字数量
        child->num++;
        // 在左兄弟节点中删除关键字
        leftChild->num--;
        return 1;
    }

    // 从右兄弟节点借关键字
    if (index != node->num &&
        node->childs[index + 1]->num >= (BTREE_ORDER_SIZE / 2))
    {
        struct BTreeNode *rightChild = node->childs[index + 1];
        struct BTreeNode *child = node->childs[index];

        // 关键字左移
        child->keys[child->num] = node->keys[index];
        node->keys[index] = rightChild->keys[0];

        for (int i = 0; i < rightChild->num - 1; i++)
        {
            rightChild->keys[i] = rightChild->keys[i + 1];
        }

        // 子节点左移
        if (!child->is_leaf)
        {
            child->childs[child->num + 1] = rightChild->childs[0];
            for (int i = 0; i < rightChild->num; i++)
            {
                rightChild->childs[i] = rightChild->childs[i + 1];
            }
        }
        // 更新节点的关键字数量
        child->num++;
        // 在右兄弟节点中删除关键字
        rightChild->num--;
        return 1;
    }
    return 0;
}

// 填充节点
static void fillNode(struct BTreeNode *node, int index)
{
    // 从左右兄弟借关键字
    if (borrowNode(node, index))
    {
        return;
    }

    // 合并子节点
    if (index != node->num)
    {
        mergeNodes(node->childs[index], node, node->childs[index + 1], index);
    }
    else
    {
        mergeNodes(node->childs[index - 1], node, node->childs[index],
                   index - 1);
    }
}

// 检查root是否置空但子节点不空
static void testRootIsZero(BTreeNode **root)
{
    if ((*root)->num == 0 && (*root)->childs[0])
    {
        BTreeNode *temp = *root;
        (*root) = (*root)->childs[0];
        free(temp);
    }
    if ((*root)->num == 0)
    {
        free(*root);
        *root = NULL;
    }
}

// 如果关键字在当前节点中，删除关键字
static int deleteKeyInNode(struct BTreeNode **root, keyOfBTree key,
                           cmpFuncPtr cmp, int index)
{
    // 如果关键字在当前节点中，删除关键字
    if (index < (*root)->num && cmp(&key, &((*root)->keys[index])) == 0)
    {
        if ((*root)->is_leaf)
        {
            // 如果是叶子节点，直接删除关键字
            for (int i = index; i < (*root)->num - 1; i++)
            {
                (*root)->keys[i] = (*root)->keys[i + 1];
            }
            (*root)->num--;
        }
        else
        {
            // 如果不是叶子节点，找到关键字的前驱或后继节点
            struct BTreeNode *predecessor = (*root)->childs[index];
            struct BTreeNode *successor = (*root)->childs[index + 1];

            // 如果前驱节点包含至少 (BTREE_ORDER_SIZE/2) 个关键字
            if (predecessor->num >= (BTREE_ORDER_SIZE / 2))
            {
                // 找到前驱节点中的最后一个关键字，替换为当前关键字
                while (!predecessor->is_leaf)
                {
                    predecessor = predecessor->childs[predecessor->num];
                }
                int lastKey = predecessor->keys[predecessor->num - 1];
                (*root)->keys[index] = lastKey;
                deleteKey(&(*root)->childs[index], lastKey, cmp);
            }
            // 如果后继节点包含至少 (BTREE_ORDER_SIZE/2) 个关键字
            else if (successor->num >= (BTREE_ORDER_SIZE / 2))
            {
                // 找到后继节点中的第一个关键字，替换为当前关键字
                while (!successor->is_leaf)
                {
                    successor = successor->childs[0];
                }
                int firstKey = successor->keys[0];
                (*root)->keys[index] = firstKey;
                deleteKey(&(*root)->childs[index + 1], firstKey, cmp);
            }
            // 如果前驱和后继节点都只包含 (BTREE_ORDER_SIZE/2) -1 个关键字
            else
            {
                // 合并前驱、关键字和后继，删除关键字
                mergeNodes(predecessor, (*root), successor, index);
                deleteKey(&predecessor, key, cmp);
            }
        }
        return 1;
    }

    return 0;
}

// 如果关键字不在当前节点中，向适当的子节点递归删除
static void deleteKeyInChildNode(struct BTreeNode **root, keyOfBTree key,
                                 cmpFuncPtr cmp, int index)
{
    if ((*root)->is_leaf)
    {
        return;
    }
    int flag = (index == (*root)->num); // 是否在最后一个子节点的范围内
    struct BTreeNode *childs = (*root)->childs[index];
    if (childs->num < (BTREE_ORDER_SIZE / 2))
    {
        fillNode((*root), index);
    }
    if (flag && index > (*root)->num)
    {
        deleteKey(&(*root)->childs[index - 1], key, cmp);
    }
    else
    {
        deleteKey(&(*root)->childs[index], key, cmp);
    }
}

// 在B树中删除关键字key
void deleteKey(struct BTreeNode **root, keyOfBTree key, cmpFuncPtr cmp)
{
    if ((*root) == NULL)
    {
        return;
    }

    // 找到要删除的关键字在节点中的位置
    int index = searchKeyIndex((*root), key, cmp);

    if (!deleteKeyInNode(root, key, cmp, index))
    {
        deleteKeyInChildNode(root, key, cmp, index);
    }

    testRootIsZero(root);
}
