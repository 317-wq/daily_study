#include "CodeService.cpp"
#include <iostream>

int main(){
    CodeService service;
    std::string phone;
    std::cout << "输入手机号: ";
    std::cin >> phone;
    std::string smscode = service.sendCode(phone);
    if(smscode.empty())
        return 1;
    std::cout << "验证码为: " << smscode << std::endl;
    std::string code;
    std::cout << "输入验证码: ";
    std::cin >> code;

    service.login(phone, code);
    return 0;
}