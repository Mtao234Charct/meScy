#define CPPHTTPLIB_OPENSSL_SUPPORT 
#include "information.h"
#include "httplib.h"
#include <thread>
#include <atomic>
#include <memory>
#include <mutex>
#include <future>
#include <chrono>
#include <condition_variable>
#include <fstream>
#include <filesystem>
#include <ranges>
#include <functional>
#include <print>  // C++23打印功能 (编译需支持)
 
using namespace std::chrono_literals; // C++20时间字面量 
namespace fs = std::filesystem;
 
// 全局停止信号 
std::stop_source stopSrc;
std::atomic<bool> serverReady{false};
 
// 智能指针管理服务器实例 
std::atomic<std::shared_ptr<httplib::Server>> httpSvr{nullptr};
std::atomic<std::shared_ptr<httplib::SSLServer>> httpsSvr{nullptr};
 
// 控制台锁封装 
struct LockedConsole {
    std::lock_guard<std::mutex> lock;
    LockedConsole() : lock(consoleMutex) {}
};
 
// 文件读取辅助函数 
std::optional<std::string> readFile(const fs::path& path) {
    if (std::ifstream file(path, std::ios::binary); file) {
        return {{std::istreambuf_iterator<char>(file), 
                 std::istreambuf_iterator<char>()}};
    }
    return std::nullopt;
}
 
// 路由设置 
template<typename ServerType>
void setupServerRoutes(ServerType& svr) {
    svr.set_read_timeout(5s);    // C++20时间字面量 
    svr.set_write_timeout(5s);   // C++20时间字面量 
    
    // 主路由 
    svr.Get("/", [dpath = fs::path(dpath)](const httplib::Request&, httplib::Response& res) {
        if (auto content = readFile(dpath)) {
            res.set_content(std::move(*content),  "text/html");
        } else {
            res.status  = 404;
            res.set_content("<h1>File  not found</h1>", "text/html");
        }
    });
    
    // 动态路由注册 
    std::ranges::for_each(route, [&](const auto& r) {
        svr.Get(r.first.c_str(),  [file_path = r.second](const  httplib::Request&, httplib::Response& res) {
            if (auto content = readFile(file_path)) {
                res.set_content(std::move(*content),  "text/html");
            } else {
                res.status  = 404;
                res.set_content("<h1>File  not found</h1>", "text/html");
            }
        });
    });
    
    // 健康检查端点 
    svr.Get("/health", [](const httplib::Request&, httplib::Response& res) {
        res.set_content("Server  is running", "text/plain");
    });
    
    // 优雅停止端点 
    svr.Get("/stop", [](const httplib::Request&, httplib::Response& res) {
        stopSrc.request_stop(); 
        res.set_content("Server  stopping...", "text/plain");
    });
}
 
void runHTTPServer() {
    try {
        auto svr = std::make_shared<httplib::Server>();
        httpSvr.store(svr); 
        
        if (!svr->is_valid()) {
            throw std::runtime_error("HTTP server initialization failed");
        }
        
        setupServerRoutes(*svr);
        
        {
            LockedConsole console;
            std::println("HTTP server starting at http://{}:{}", bindon, httpPort);
        }
        
        // 使用jthread自动管理线程生命周期 
        std::jthread listenThread([svr, bindAddr = bindon, port = httpPort] {
            if (svr->listen(bindAddr.c_str(), port)) {
                {
                    LockedConsole console;
                    std::println("HTTP server listening at http://{}:{}", bindAddr, port);
                }
                serverReady = true;
            } else {
                LockedConsole console;
                std::println("HTTP server failed to start");
            }
        });
        
        // 等待服务器启动 
        while (!serverReady && !stopSrc.stop_requested())  {
            std::this_thread::sleep_for(100ms);
        }
        
        // 等待停止信号 
        while (!stopSrc.stop_requested())  {
            std::this_thread::sleep_for(100ms);
        }
        
        // 安全停止服务器 
        svr->stop();
        
        // 清理资源 
        httpSvr.store(nullptr); 
        
        {
            LockedConsole console;
            std::println("HTTP server stopped");
        }
    } catch (const std::exception& e) {
        LockedConsole console;
        std::println("HTTP server error: {}", e.what()); 
    }
}
 
void runHTTPSServer() {
    try {
        auto svr = std::make_shared<httplib::SSLServer>(
            cert_file.c_str(), 
            private_key_file.c_str()
        );
        httpsSvr.store(svr); 
        
        if (!svr->is_valid()) {
            throw std::runtime_error("HTTPS server initialization failed");
        }
        
        setupServerRoutes(*svr);
        
        {
            LockedConsole console;
            std::println("HTTPS server starting at https://{}:{}", bindon, httpsPort);
        }
        
        // 使用jthread自动管理线程生命周期 
        std::jthread listenThread([svr, bindAddr = bindon, port = httpsPort] {
            if (svr->listen(bindAddr.c_str(), port)) {
                {
                    LockedConsole console;
                    std::println("HTTPS server listening at https://{}:{}", bindAddr, port);
                }
                serverReady = true;
            } else {
                LockedConsole console;
                std::println("HTTPS server failed to start");
            }
        });
        
        // 等待服务器启动 
        while (!serverReady && !stopSrc.stop_requested())  {
            std::this_thread::sleep_for(100ms);
        }
        
        // 等待停止信号 
        while (!stopSrc.stop_requested())  {
            std::this_thread::sleep_for(100ms);
        }
        
        // 安全停止服务器 
        svr->stop();
        
        // 清理资源 
        httpsSvr.store(nullptr); 
        
        {
            LockedConsole console;
            std::println("HTTPS server stopped");
        }
    } catch (const std::exception& e) {
        LockedConsole console;
        std::println("HTTPS server error: {}", e.what()); 
    }
}
 
int main() {
    using namespace std::literals;
    
    std::println("Starting server... (Current time: 2025年7月15日 21:05)");
    
    // 加载配置文件 
    try {
        EasyScriptsRouter::runCommand("config.esr");  
    } catch (const std::exception& e) {
        std::println("Config error: {}", e.what()); 
    }
    
    // 自动管理线程容器 
    std::vector<std::jthread> serverThreads;
    
    // 启动HTTP服务器 
    if (httpPort > 0) {
        serverThreads.emplace_back(runHTTPServer); 
    } else {
        std::println("HTTP server disabled (port not set)");
    }
    
    // 启动HTTPS服务器 
    if (httpsPort > 0) {
        serverThreads.emplace_back(runHTTPSServer); 
    } else {
        std::println("HTTPS server disabled (port not set)");
    }
    
    // 命令处理 
    std::println("Type 'end' to stop servers or 'status' for status...");
    std::string command;
    
    while (!stopSrc.stop_requested())  {
        std::cin >> command;
        if (command == "end") {
            stopSrc.request_stop(); 
            break;
        } else if (command == "status") {
            LockedConsole console;
            std::println("Server status: {}", 
                         stopSrc.stop_requested()  ? "STOPPING" : "RUNNING");
            
            auto httpActive = httpSvr.load(); 
            std::println("HTTP: {}{}", 
                         httpActive ? "ACTIVE" : "INACTIVE",
                         httpActive && httpActive->is_running() ? " (RUNNING)" : "");
            
            auto httpsActive = httpsSvr.load(); 
            std::println("HTTPS: {}{}", 
                         httpsActive ? "ACTIVE" : "INACTIVE",
                         httpsActive && httpsActive->is_running() ? " (RUNNING)" : "");
        } else {
            std::println("Unknown command. Type 'end' to stop or 'status' for server status.");
        }
    }
    
    std::println("All servers stopped. Exiting application...");
    return 0;
}