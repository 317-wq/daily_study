#include <string>
#include <sw/redis++/redis++.h>

using sw::redis::Redis;

class RedisClient{
private:
    Redis _redis;

public:
    explicit RedisClient(const std::string &url = "tcp://127.0.0.1:6379")
    :_redis(url)
    {}

    ~RedisClient() = default;

public:
    void setCode(const std::string &phone, const std::string &smscode, int expire_sec = 60){
        std::string key = "sms:" + phone;
        _redis.setex(key, expire_sec, smscode); // 设置过期时间
    }

    std::string getCode(const std::string &phone){
        std::string key = "sms:" + phone;
        if(!_redis.exists(key))
            return "";
        
        auto val = _redis.get(key);
        return val ? *val : "";
    }

    bool delCode(const std::string &phone){
        std::string key = "sms:" + phone;
        if(!_redis.exists(key))
            return "";
        _redis.del(key);
        return true;
    }
};
