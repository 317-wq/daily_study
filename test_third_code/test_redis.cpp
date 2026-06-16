#include <iostream>
#include <sw/redis++/redis++.h>

int main() {
    try {
        // 连接本地Redis服务器，默认端口6379
        // 如果你的Redis在别的地址，修改 "tcp://127.0.0.1:6379" 即可
        sw::redis::Redis redis("tcp://127.0.0.1:6379");

        // 发送PING命令测试连接
        std::string pong = redis.ping();
        std::cout << "Redis response: " << pong << std::endl;

        // 设置一个键值对
        redis.set("hello", "world");

        // 读取键值对
        auto val = redis.get("hello");
        if (val) {
            std::cout << "Value of 'hello': " << *val << std::endl;
        }

    } catch (const sw::redis::Error& err) {
        std::cerr << "Redis error: " << err.what() << std::endl;
        return 1;
    }

    return 0;
}