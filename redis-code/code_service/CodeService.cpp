#include "RedisClient.cpp"
#include <random>
#include <regex>
#include <iostream>
#include <spdlog/spdlog.h>

class CodeService{
private:
    RedisClient _redis;

public:
    CodeService() = default;
    ~CodeService() = default;

private:
    // 默认生成六位验证码
    std::string generateCode(int min = 100000, int max = 999999){
        static std::random_device rd; // 只创建一次生成器
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<int> dist(min, max);
        return std::to_string(dist(gen));
    }

    // 正则检验
    bool isVaildPhone(const std::string &phone){
        std::regex pattern(R"(^1[3-9]\d{9}$)");
        return std::regex_match(phone, pattern);
    }

public:
    // 模拟登录
    bool login(const std::string &phone, const std::string &smscode){
        std::string code = _redis.getCode(phone);
        if(code.empty()){
            spdlog::debug("验证码不存在或者已过期!");
            return false;
        }

        if(code != smscode){
            spdlog::debug("验证码错误!");
            return false;
        }

        spdlog::debug("登录成功!");
        _redis.delCode(phone); // 删除对应的验证码kv
        return true;
    }

    // 发送验证码
    std::string sendCode(const std::string &phone){
        if(!isVaildPhone(phone)){
            spdlog::warn("Invalid phone number: %s", phone.c_str());
            return "";
        }

        // 生成验证码
        std::string smscode = generateCode();
        spdlog::debug("验证码：%s", smscode.c_str());
        _redis.setCode(phone, smscode);
        return smscode;
    }
};