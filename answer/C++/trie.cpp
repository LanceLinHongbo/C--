import std;

// 定义TrieNode类
class TrieNode
{
  public:
    // 假设我们只处理小写字母
    std::array<TrieNode *, 26> children;

    // 记录经过该节点的字符串数量
    std::uint32_t strSize = 0;

    // 标记该节点是否为单词结尾
    bool isEndOfWord = false;
};

// 定义Trie类
class Trie
{
  public:
    // 构造函数
    Trie()
    {
        // 初始化根节点
        root = new TrieNode();
    }

    // 禁止拷贝构造和赋值操作
    Trie(const Trie &) = delete;
    auto operator=(const Trie &) -> Trie & = delete;

    // 析构函数
    ~Trie()
    {
        // 释放所有节点
        deleteAllNodes(root);
        // 将根节点指针置空
        root = nullptr;
    }

    // 插入字符串
    void insert(const std::string &key)
    {
        TrieNode *currentNode = root;
        for (char chr : key)
        {
            // 获取字符索引
            int index = chr - 'a';

            // 如果没有子节点，则创建一个新节点
            if (currentNode->children[index] == nullptr)
            {
                currentNode->children[index] = new TrieNode();
            }
            // 移动到子节点
            currentNode = currentNode->children[index];
            // 经过该节点的字符串数量增加
            currentNode->strSize++;
        }
        // 标记为单词结尾
        currentNode->isEndOfWord = true;
    }

    // 查找字符串
    auto search(const std::string &key) -> bool
    {
        TrieNode *currentNode = root;
        for (char chr : key)
        {
            // 获取字符索引
            int index = chr - 'a';
            // 如果找不到对应的子节点，则字符串不存在
            if (currentNode->children[index] == nullptr)
            {
                return false;
            }
            // 移动到子节点
            currentNode = currentNode->children[index];
        }
        // 返回是否为单词结尾
        return currentNode->isEndOfWord;
    }

    // 删除字符串
    auto remove(const std::string &key) -> bool
    {
        // 先检查字符串是否存在
        if (search(key))
        {
            TrieNode *currentNode = root;
            TrieNode *temp = nullptr;
            int index = 0;

            for (char chr : key)
            {
                // 获取字符索引
                index = chr - 'a';
                temp = currentNode;
                // 移动到子节点
                currentNode = currentNode->children[index];
                // 经过该节点的字符串数量减少
                currentNode->strSize--;
                if (currentNode->strSize == 0)
                {
                    // 删除无用的子树
                    deleteAllNodes(currentNode);
                    // 清除子节点指针
                    temp->children[index] = nullptr;
                    // 成功删除
                    return true;
                }
            }
            // 标记为非单词结尾
            currentNode->isEndOfWord = false;
            // 成功删除
            return true;
        }
        // 未找到字符串
        return false;
    }

  private:
    // Trie的根节点
    TrieNode *root;

    // 辅助函数，用于递归删除节点
    void deleteAllNodes(TrieNode *node)
    {
        if (node == nullptr)
        {
            // 如果节点为空，直接返回
            return;
        }
        // 遍历子节点
        for (auto *child : node->children)
        {
            if (child != nullptr)
            {
                // 递归删除子节点
                deleteAllNodes(child);
            }
        }
        // 删除当前节点
        delete node;
    }
};

auto main() -> int
{
    // 创建一个Trie对象
    Trie trie;

    // 插入字符串"hi"
    trie.insert("hi");

    // 插入字符串"his"
    trie.insert("his");

    // 插入字符串"wo"
    trie.insert("wo");

    // 查找字符串"hi"
    std::cout << "Search 'hi': " << trie.search("hi") << std::endl;

    // 删除字符串"his"
    trie.remove("his");

    // 查找字符串"hi"
    std::cout << "Search 'hi': " << trie.search("hi") << std::endl;

    // 删除字符串"hi"
    trie.remove("hi");

    // 再次查找字符串"hi"
    std::cout << "Search 'hi': " << trie.search("hi") << std::endl;

    // 查找字符串"wo"
    std::cout << "Search 'wo': " << trie.search("wo") << std::endl;

    return 0;
}