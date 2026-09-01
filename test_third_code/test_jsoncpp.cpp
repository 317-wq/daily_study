#include <iostream>
#include <jsoncpp/json/json.h>
#include <memory>
#include <sstream>

using namespace std;

// 序列化 [对象 -> 字符串]
string serialize(){
    // 1. 创建json对象，并进行初始化
    Json::Value root;
    root["姓名"] = "小明";
    root["年龄"] = 18;
    root["成绩"].append(88);
    root["成绩"].append(98.5);
    root["成绩"].append(78.6);

    // 2. 使用StreamWriterBuilder创建写入器
    Json::StreamWriterBuilder swb;
    unique_ptr<Json::StreamWriter> sw(swb.newStreamWriter());

    // 3. 利用选择器将json对象转化为json字符串
    stringstream ss;
    if(sw->write(root, &ss) != 0){
        cout << "序列化失败" << endl;
        return "";
    }
    return ss.str();
}

// 反序列化 [字符串 -> 对象]
void deserialize(const string &str){
    // 1. 创建json对象和使用CharReaderBuilder解析器
    Json::Value root;
    Json::CharReaderBuilder crb;
    unique_ptr<Json::CharReader> cr(crb.newCharReader());

    // 2. 对字符串进行解析操作
    string errs;
    if(cr->parse(str.c_str(), str.c_str() + str.size(), &root, &errs) == false){
        cout << "反序列化失败" << endl;
        cout << errs << endl;
        return ;
    }

    // 3. 提取数据
    string name = root["姓名"].asString();
    int age = root["年龄"].asInt();
    cout << name << endl << age << endl;
    // 多个成绩
    Json::Value scores = root["成绩"];
    if(scores.isArray()){
        int size = scores.size();
        for(int i = 0; i < size; ++i){
            cout << scores[i].asDouble() << endl;
        }
        return ;
    }
    cout << "failed" << endl;
    return ;
}

int main(){
    string str = serialize();
    deserialize(str);
    return 0;
}