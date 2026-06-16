#include <iostream>
using namespace std;

#include <sw/redis++/redis++.h>
#include <unordered_map>
#include <string>
#include <vector>
#include <chrono>
#include <unordered_set>
using sw::redis::Redis;

// void test(sw::redis::Redis &redis){
//     {
//         redis.flushdb();
//         unordered_map<string, string> kvs1 = {
//             {"firstname", "Jack"},
//             {"lastname", "Stuntman"},
//             {"age", "35"}};

//         // 一次性设置多个kv
//         redis.mset(kvs1.begin(), kvs1.end());
//         vector<string> keys;
//         insert_iterator<vector<string>> ins = inserter(keys, keys.begin());
//         redis.keys();
//     }
// }

void test1()
{
    sw::redis::Redis redis("tcp://127.0.0.1:6379");

    redis.flushdb(); // 将目前的redis库键值清空
    redis.set("name", "张三");

    auto val = redis.get("name");
    // cout << "name: " << *redis.get("name") << endl;
    cout << "name: " << *val << endl;

    cout << redis.exists("name") << endl; // 0/1

    // 查找键
    redis.set("tel", "19501122262");
    redis.set("sex", "female");

    vector<string> keys;
    auto ins = inserter(keys, keys.begin()); // keys头插迭代器
    redis.keys("*", ins);
    for (auto &k : keys)
        cout << k << " ";
    cout << endl;

    // vector<string> str;
    // auto iins = inserter(str, str.begin());
    // redis.keys("*", iins);
    // for(auto &t : str)
    //     cout << t << " ";

    // 设置超时时间[过期时间]
    redis.expire("sex", chrono::seconds(10));
    int tm;
    while (tm = redis.ttl("sex"))
    {
        if (tm == -1 || tm == -2)
            break;
        cout << "剩余时间: " << tm << endl;
        this_thread::sleep_for(chrono::seconds(1));
    }

    // 删除键
    redis.del("sex");
    cout << "删除成功" << endl;
    redis.del("name");
    cout << "删除成功" << endl;

    redis.del("tel");
    cout << "删除成功" << endl;

    vector<string> tkeys;
    auto tins = inserter(tkeys, tkeys.begin()); // keys头插迭代器
    redis.keys("*", tins);
    for (auto &k : tkeys)
        cout << k << " ";
    cout << endl;
}


void test2(){
    sw::redis::Redis redis("tcp://127.0.0.1:6379");
    // redis.ping()
    // redis.set("user1:name", "张三");
    // auto val = redis.get("user1:name");
    // if(val){
    //     cout << *val << endl;
    // }

    // // 设置过期时间 expire setex setpx
    // redis.set("session", "token_value", chrono::seconds(10));
    // redis.flushdb();
    // bool lock = redis.setnx("locker", "locked"); // 设置分布式锁
    // if(lock){
    //     redis.del("locker");
    // }

    // redis.set("views", "5");
    // redis.set("stores", "5");
    // redis.incr("views");
    // redis.get("views");

    // redis.decr("stores");
    // redis.get("stores");

    // redis.incrby("views", 50);
    // redis.get("views");

    // redis.decrby("stores", 3);
    // redis.get("stores");

    // 批量操作，减少网络请求
    // redis.flushdb();
    // unordered_map<string, string> maps{
    //     {"k", "v"},
    //     {"k1", "v1"},
    //     {"k2", "v2"}};
    
    // redis.mset(maps.begin(), maps.end());

    // vector<string> vs;
    // // auto ins = inserter(vs, vs.begin());
    // vector<string> ks = {"k", "k1", "k2"};
    // redis.mget(ks.begin(), ks.end(), inserter(vs, vs.begin()));
    // for(auto &v : vs)
    //     cout << v << " ";
    // cout << endl;
}

void test3(){
    sw::redis::Redis redis("tcp://127.0.0.1:6379");
    // redis.lpush("mylist", "1");
    // redis.lpush("mylist", "2");
    // redis.lpush("mylist", {"3", "4", "5"});

    // redis.rpush("mylist", "6");
    // redis.rpush("mylist", "7");
    // auto v = redis.lpop("mylist");
    // if(v)
    //     cout << *v << endl;
    // v = redis.rpop("mylist");
    // if(v)
    //     cout << *v << endl;
    // 阻塞等待，直到有元素或超时 事件时间设置为0就是永远阻塞
    // auto p = redis.blpop({"queue1", "queue2"}, std::chrono::seconds(5));
    // if (p)
    // {
    //     std::cout << "来自: " << p->first << ", 值: " << p->second << std::endl;
    //     // 处理消息...
    // }

    // cout << redis.llen("mylist") << endl;
    // vector<string> vs;
    // redis.lrange("mylist", 0, -1, back_inserter(vs));
    // for(auto &v : vs)
    //     cout << v << " ";
    // cout << endl;
}

void test4(){
    sw::redis::Redis redis("tcp://127.0.0.1:6379");
    // redis.flushdb();
    // using ll = long long;

    // ll cnt = redis.sadd("myset", "hello"); // 实际添加的数量
    // redis.sadd("myset", "a");
    // cout << cnt << endl;
    // cout << redis.sismember("myset", "xxx") << endl;
    // cout << redis.sismember("myset", "hello") << endl;
    // cout << redis.scard("myset") << endl;
    // cout << redis.srem("myset", "a") << endl;

    // unordered_set<string> vs;
    // redis.smembers("myset", inserter(vs, vs.begin()));
    // for(auto &v : vs)
    //     cout << v << " ";
    // cout << endl;

    // auto v = redis.spop("myset");
    // if(v){
    //     cout << *v << endl;
    // }

    // unordered_set<string> vs;
    // redis.spop("myset", 2, inserter(vs, vs.begin()));
    // for(auto &v : vs)
    //     cout << v << " ";
    // cout << endl;

    // // 交集（共同好友）
    // std::unordered_set<std::string> result;
    // redis.sinter({"set1", "set2"}, std::inserter(result, result.begin()));

    // // 并集
    // redis.sunion({"set1", "set2"}, std::inserter(result, result.begin()));

    // // 差集（在set1中但不在set2中）
    // redis.sdiff({"set1", "set2"}, std::inserter(result, result.begin()));
}


void test5(){
    sw::redis::Redis redis("tcp://127.0.0.1:6379");
    // // 设置单个字段
    // redis.hset("user:1001", "name", "zhangsan");
    // redis.hset("user:1001", "age", "25");
    // redis.hset("user:1001", "city", "北京");

    // // 获取单个字段
    // auto name = redis.hget("user:1001", "name");
    // if (name)
    // {
    //     std::cout << *name << std::endl;
    // }
    // // unordered_map<string, string> kvs = {{"name", "zs"}, {"age", "25"}};
    // // redis.hset("user001", kvs.begin(), kvs.end());
    // std::vector<std::pair<std::string, std::string>> fields = {
    //     {"name", "zhangsan"},
    //     {"age", "25"},
    //     {"city", "北京"}};
    // redis.hmset("user:1001", fields.begin(), fields.end());

    // std::vector<std::string> fieldNames = {"name", "age", "city"};
    // std::vector<sw::redis::OptionalString> values;
    // auto it = std::back_inserter(values);
    // redis.hmget("user:1001", fieldNames.begin(), fieldNames.end(), it);

    // for (auto &val : values)
    // {
    //     std::cout << *val << std::endl;
    // }

    // long long count = redis.hlen("user:1001");  // 返回字段个数

    // // 获取所有字段和值
    // std::vector<std::pair<std::string, std::string>> all;
    // redis.hgetall("user:1001", std::back_inserter(all));
    // for (auto &[key, val] : all)
    // {
    //     std::cout << key << " = " << val << std::endl;
    // }

    // // 只获取所有字段名
    // std::vector<std::string> keys;
    // redis.hkeys("user:1001", std::back_inserter(keys));

    // // 只获取所有值
    // std::vector<std::string> values;
    // redis.hvals("user:1001", std::back_inserter(values));

    // // 整型自增
    // long long newAge = redis.hincrby("user:1001", "age", 1); // 年龄+1
    // redis.hincrby("user:1001", "score", 10);                 // 分数+10

    // // 浮点型自增
    // double newScore = redis.hincrbyfloat("user:1001", "score", 0.5);
}

void test6(){
    Redis redis("tcp://127.0.0.1:6379");
    // 添加单个元素（分数在前，成员在后）
    redis.zadd("ranking", "吕布", 100);
    redis.zadd("ranking", "赵云", 98);
    redis.zadd("ranking", "典韦", 95);

    // 获取指定范围的成员（按分数从小到大）
    std::vector<std::string> members;
    auto it = std::back_inserter(members);
    redis.zrange("ranking", 0, 4, it); // 获取前5名

    // 获取成员和分数
    std::vector<std::pair<std::string, double>> result;
    auto it2 = std::back_inserter(result);
    redis.zrange("ranking", 0, 4, it2); // 带分数

    // 删除指定成员
    long long n = redis.zrem("ranking", "吕布");

    // 增加分数（用于更新）
    double newScore = redis.zincrby("ranking", 5, "赵云"); // 赵云+5分
    redis.zincrby("ranking", -2, "吕布");                  // 吕布-2分

    long long count = redis.zcard("ranking");

    // 统计分数在 [92, 100] 之间的成员数量
    count = redis.zcount("ranking",
                                   sw::redis::BoundedInterval<double>(92, 100, sw::redis::BoundType::CLOSED));
}

int main()
{
    // test1(); // 基础命令
    // test2(); // string 数据结构
    // test3(); // list 数据结构
    // test4(); // set 数据结构
    // test5(); // hash 数据结构
    test6(); // zset 数据结构
    return 0;
}