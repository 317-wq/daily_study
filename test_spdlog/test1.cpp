#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h> // 文件输出相关
#include <spdlog/sinks/rotating_file_sink.h> // 轮转输出函数相关
#include <spdlog/sinks/stdout_color_sinks.h> // 带颜色的控制台输出
#include <spdlog/async.h> // 异步输出头文件
#include <string>
#include <iostream>
#include <memory>

using namespace std;

// 输出console
void func1(){
    // 设置全局输出等级
    spdlog::set_level(spdlog::level::trace);
    // 自定义输出格式
    string pattern = "[%Y-%m-%d %H:%M:%S] [%^%l%$] [thread %t] %v";
    // 应用输出格式
    spdlog::set_pattern(pattern);
    spdlog::info("自定义之后的信息");
}

// 输出到文件
void func2(){
    try{
        // 创建一个线程安全的文件日志器
        auto file_logger = spdlog::basic_logger_mt("file_logger", "logs/app.log");
        // 设置输出等级，输出格式
        file_logger->set_level(spdlog::level::debug);
        file_logger->set_pattern("[%Y-%m-%d %H:%M:%S] [%l] %v");

        // 将信息输出到文件里面
        file_logger->info("info信息");
        file_logger->error("error信息");
    }catch(const spdlog::spdlog_ex& e){
        cout << "日志初始化失败: " << e.what() << endl;
    }catch(...){
        cout << "不知道" << endl;
    }
}

// 输出到文件，策略按照文件大小轮转
void func3(){
    // 每个文件不超过5mb，最大文件数为3
    auto rotate_logger = spdlog::rotating_logger_mt(
                                    "rotate", 
                                    "logs/ratating.txt", 
                                    1024 * 1024 * 5,
                                    3);
    for(int i = 0; i < 500; ++i){
        rotate_logger->info("这是第 {} 条日志", i);
    }
}   

// 多sinks输出
void func4(){
    // 创建两个sink
    auto console_sink = make_shared<spdlog::sinks::stdout_color_sink_mt>();
    auto file_sink = make_shared<spdlog::sinks::rotating_file_sink_mt>(
        "logs/multi.log", 1024*1024*5, 3
    );

    // 单独给每个sink设置过滤级别
    console_sink->set_level(spdlog::level::info);
    file_sink->set_level(spdlog::level::trace);

    // 将多个sink组合到同一个日志输出器中
    spdlog::logger multi_logger("multi", {console_sink, file_sink});
    multi_logger.info("同时输出到两个sink里面");
}

// 异步输出
void func5(){
    // 创建异步输出器
    auto async_logger = spdlog::basic_logger_mt<spdlog::async_factory>(
        "async_logger", "logs/async.log"
    );
    async_logger->info("异步输出");

    // 确保异步日志写完
    spdlog::shutdown();
}

int main(){
    func5();
    return 0;
}