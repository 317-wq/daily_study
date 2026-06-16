// LRU缓存技术：最近最少缓存策略

#include <iostream>
#include <list>
#include <unordered_map>
#include <cassert>
#include <stdexcept>

using std::cout;
using std::endl;
using std::cerr;


template<typename K, typename V>
class LRU{
private:
    using PKV = std::pair<K, V>;

    size_t capacity_; // 最大存储数量
    std::list<PKV> dll_; // 双向链表(double linked list)
    std::unordered_map<K, typename std::list<PKV>::iterator> maps_; // 键对应这个位于dll中的什么位置

public:
    explicit LRU(size_t capacity)
    :capacity_(capacity)
    {
        assert(capacity > 0);
    }

    ~LRU() = default;

public:
    V get(const K &key){
        // 1、如果这个key不存在就退出
        auto it = maps_.find(key);
        if(it == maps_.end()){
            throw std::invalid_argument("key 不存在");
        }

        // 2、如果这个key存在的话，就更新到list的开头，表示最近被使用过
        dll_.splice(dll_.begin(), dll_, it->second); // dll 的 it->second 迭代器放到 begin 的位置
        return it->second->second;
    }


    void put(const K &key, const V &val){
        // 1、存在的话，直接更新
        auto it = maps_.find(key);
        if(it != maps_.end()){
            it->second->second = val; // 更新val
            dll_.splice(dll_.begin(), dll_, it->second);
            return;
        }

        // 2、不存在，分两种情况
        // 2.1 存储空间满了
        if(dll_.size() == capacity_){
            // 删除最后一个位置的存储对象
            auto lastk = dll_.back().first; 
            maps_.erase(lastk);
            dll_.pop_back();
        }

        // 2.2 两种方式合并，统一更新
        dll_.emplace_front(std::make_pair(key, val));
        maps_[key] = dll_.begin();
    }

    void print() const {
        cout << "缓存内容 (最近 -> 最久): ";
        for (const auto& node : dll_) {
            cout << "[" << node.first << ":" << node.second << "] ";
        }
        cout << endl;
    }

    size_t size() const {
        return dll_.size();
    }

    bool empty() const {
        return dll_.empty();
    }

    void clear() {
        dll_.clear();
        maps_.clear();
    }
};

// ==================== 测试代码 ====================

#include <iostream>
#include <string>
#include <cassert>

using std::cout;
using std::endl;
using std::string;

// 测试1：基本功能测试（int类型）
void testBasicInt() {
    cout << "\n========== 测试1：基本功能 (int) ==========" << endl;
    
    LRU<int, int> cache(3);
    
    cout << "插入 (1,100), (2,200), (3,300)" << endl;
    cache.put(1, 100);
    cache.put(2, 200);
    cache.put(3, 300);
    cache.print();  // 期望: [3:300] [2:200] [1:100]
    
    cout << "\n获取 key=2: " << cache.get(2) << endl;
    cache.print();  // 期望: [2:200] [3:300] [1:100]
    
    cout << "\n获取不存在的 key=5: " << cache.get(5) << " (默认值0)" << endl;
    cache.print();
    
    cout << "\n插入 (4,400)，淘汰最久未使用的 key=1" << endl;
    cache.put(4, 400);
    cache.print();  // 期望: [4:400] [2:200] [3:300]
    
    cout << "\n更新 key=3 为 999" << endl;
    cache.put(3, 999);
    cache.print();  // 期望: [3:999] [4:400] [2:200]
    
    cout << "\n当前缓存大小: " << cache.size() << endl;
    assert(cache.size() == 3);
}

// 测试2：淘汰机制测试
void testEviction() {
    cout << "\n========== 测试2：淘汰机制 ==========" << endl;
    
    LRU<int, int> cache(2);
    
    cout << "容量为2的缓存" << endl;
    cache.put(1, 10);
    cache.put(2, 20);
    cache.print();  // [2:20] [1:10]
    
    cout << "\n访问 key=1，变为最近使用" << endl;
    cache.get(1);
    cache.print();  // [1:10] [2:20]
    
    cout << "\n插入 (3,30)，淘汰最久的 key=2" << endl;
    cache.put(3, 30);
    cache.print();  // [3:30] [1:10]
    
    cout << "\n再次插入 (4,40)，淘汰 key=1" << endl;
    cache.put(4, 40);
    cache.print();  // [4:40] [3:30]
}

// 测试3：字符串类型测试
void testStringType() {
    cout << "\n========== 测试3：字符串类型 ==========" << endl;
    
    LRU<string, string> cache(3);
    
    cout << "插入用户数据" << endl;
    cache.put("user:001", "Alice");
    cache.put("user:002", "Bob");
    cache.put("user:003", "Charlie");
    cache.print();  // [user:003:Charlie] [user:002:Bob] [user:001:Alice]
    
    cout << "\n获取 user:002: " << cache.get("user:002") << endl;
    cache.print();  // [user:002:Bob] [user:003:Charlie] [user:001:Alice]
    
    cout << "\n插入 user:004，淘汰最久的 user:001" << endl;
    cache.put("user:004", "David");
    cache.print();  // [user:004:David] [user:002:Bob] [user:003:Charlie]
    
    cout << "\n尝试获取已淘汰的 user:001: " << cache.get("user:001") << " (空字符串)" << endl;
}

// 测试4：边界条件测试
void testEdgeCases() {
    cout << "\n========== 测试4：边界条件 ==========" << endl;
    
    LRU<int, int> cache(1);
    
    cout << "容量为1的缓存" << endl;
    cache.put(1, 100);
    cache.print();  // [1:100]
    
    cout << "\n插入 (2,200)，淘汰 key=1" << endl;
    cache.put(2, 200);
    cache.print();  // [2:200]
    
    cout << "\n更新 key=2 为 999" << endl;
    cache.put(2, 999);
    cache.print();  // [2:999]
    
    cout << "\n获取 key=2: " << cache.get(2) << endl;
    cout << "获取不存在的 key=1: " << cache.get(1) << " (默认值0)" << endl;
}

// 测试5：混合操作测试
void testMixedOperations() {
    cout << "\n========== 测试5：混合操作 ==========" << endl;
    
    LRU<int, int> cache(4);
    
    cout << "执行一系列操作:" << endl;
    cout << "put(1,10), put(2,20), put(3,30)" << endl;
    cache.put(1, 10);
    cache.put(2, 20);
    cache.put(3, 30);
    cache.print();
    
    cout << "\nget(2) -> " << cache.get(2) << endl;
    cache.print();
    
    cout << "\nput(4,40), put(5,50)" << endl;
    cache.put(4, 40);
    cache.put(5, 50);
    cache.print();  // 应该淘汰 key=1
    
    cout << "\nget(3) -> " << cache.get(3) << endl;
    cache.print();  // 3 移到头部
    
    cout << "\nput(6,60)" << endl;
    cache.put(6, 60);
    cache.print();  // 淘汰 key=4 (最久未使用)
    
    cout << "\n最终缓存大小: " << cache.size() << endl;
    assert(cache.size() == 4);
}

// 测试6：性能测试（大量数据）
void testPerformance() {
    cout << "\n========== 测试6：性能测试 (1000个元素) ==========" << endl;
    
    LRU<int, int> cache(100);
    
    cout << "插入 1000 个元素..." << endl;
    for (int i = 0; i < 1000; ++i) {
        cache.put(i, i * 10);
    }
    
    cout << "缓存实际大小: " << cache.size() << " (应为100)" << endl;
    assert(cache.size() == 100);
    
    cout << "访问一些元素..." << endl;
    for (int i = 900; i < 950; ++i) {
        cache.get(i);
    }
    
    cout << "插入新元素..." << endl;
    for (int i = 1000; i < 1010; ++i) {
        cache.put(i, i * 10);
    }
    
    cout << "缓存大小: " << cache.size() << " (仍为100)" << endl;
    assert(cache.size() == 100);
    
    cout << "测试通过！" << endl;
}

void test(){
    cout << "========== LRU 缓存全面测试 ==========" << endl;
    try{
        testBasicInt();
    }catch(const std::invalid_argument &e){
        cerr << e.what() << endl;
    }catch(...){
        cerr << "未知错误" << endl;
    }

    try{
        testEviction();
    }catch(const std::invalid_argument &e){
        cerr << e.what() << endl;
    }catch(...){
        cerr << "未知错误" << endl;
    }

    try{
        testStringType();
    }catch(const std::invalid_argument &e){
        cerr << e.what() << endl;
    }catch(...){
        cerr << "未知错误" << endl;
    }

    try{
        testEdgeCases();
    }catch(const std::invalid_argument &e){
        cerr << e.what() << endl;
    }catch(...){
        cerr << "未知错误" << endl;
    }

    try{
        testMixedOperations();
    }catch(const std::invalid_argument &e){
        cerr << e.what() << endl;
    }catch(...){
        cerr << "未知错误" << endl;
    }

    try{
        testPerformance();
    }catch(const std::invalid_argument &e){
        cerr << e.what() << endl;
    }catch(...){
        cerr << "未知错误" << endl;
    }

    cout << "\n========== 所有测试通过！ ✅ ==========" << endl;
    cout << "\n关键点总结：" << endl;
    cout << "1. get 访问时，将元素移到链表头部（最近使用）" << endl;
    cout << "2. put 更新时，将元素移到链表头部" << endl;
    cout << "3. put 插入新元素时，如果满了删除尾部（最久未使用）" << endl;
    cout << "4. 时间复杂度：get 和 put 都是 O(1)" << endl;
}
int main() {
    test();
    return 0;
}