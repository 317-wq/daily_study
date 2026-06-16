#include <spdlog/spdlog.h>

int main() {
    spdlog::debug("456");
    spdlog::info("spdlog installed successfully!");
    spdlog::error("9999");
    spdlog::warn("456");
    spdlog::critical("0000");
    return 0;
}