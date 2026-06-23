// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <unistd.h>
// #include <errno.h>
// #include <fcntl.h>
// #include <sys/socket.h>
// #include <sys/epoll.h>
// #include <netinet/in.h>
// #include <arpa/inet.h>

// #define MAX_EVENTS 1024
// #define BUFFER_SIZE 1024
// #define PORT 8888

// // 设置文件描述符为非阻塞 -> LT模式
// int set_nonblocking(int fd) {
//     int flags = fcntl(fd, F_GETFL, 0);
//     if (flags == -1) {
//         perror("fcntl F_GETFL");
//         return -1;
//     }
//     if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) {
//         perror("fcntl F_SETFL");
//         return -1;
//     }
//     return 0;
// }

// // 创建并绑定监听socket
// int create_listen_socket(int port) {
//     int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
//     if (listen_fd == -1) {
//         perror("socket");
//         return -1;
//     }

//     // 设置端口复用，避免TIME_WAIT状态导致bind失败
//     int opt = 1;
//     if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
//         perror("setsockopt");
//         close(listen_fd);
//         return -1;
//     }

//     struct sockaddr_in server_addr;
//     memset(&server_addr, 0, sizeof(server_addr));
//     server_addr.sin_family = AF_INET;
//     server_addr.sin_addr.s_addr = INADDR_ANY;
//     server_addr.sin_port = htons(port);

//     if (bind(listen_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
//         perror("bind");
//         close(listen_fd);
//         return -1;
//     }

//     if (listen(listen_fd, 128) == -1) {
//         perror("listen");
//         close(listen_fd);
//         return -1;
//     }

//     // 设置监听socket为非阻塞（建议）
//     if (set_nonblocking(listen_fd) == -1) {
//         close(listen_fd);
//         return -1;
//     }

//     printf("Server listening on port %d\n", port);
//     return listen_fd;
// }

// // 处理新连接
// int handle_new_connection(int epfd, int listen_fd) {
//     struct sockaddr_in client_addr;
//     socklen_t client_len = sizeof(client_addr);
    
//     // 循环accept，处理可能存在的多个连接（ET模式需要）
//     while (1) {
//         int conn_fd = accept(listen_fd, (struct sockaddr*)&client_addr, &client_len);
//         if (conn_fd == -1) {
//             if (errno == EAGAIN || errno == EWOULDBLOCK) {
//                 // 所有连接已处理完
//                 break;
//             } else {
//                 perror("accept");
//                 return -1;
//             }
//         }

//         // 获取客户端IP和端口
//         char client_ip[INET_ADDRSTRLEN];
//         inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, sizeof(client_ip));
//         printf("New connection from %s:%d, fd=%d\n", 
//                client_ip, ntohs(client_addr.sin_port), conn_fd);

//         // 设置新连接为非阻塞
//         if (set_nonblocking(conn_fd) == -1) {
//             close(conn_fd);
//             continue;
//         }

//         // 添加到epoll，使用ET模式（边缘触发）
//         struct epoll_event ev;
//         // ev.events = EPOLLIN | EPOLLET;  // ET模式
//         // 如果使用LT模式，改为: ev.events = EPOLLIN;
//         ev.events = EPOLLIN;
//         ev.data.fd = conn_fd;
        
//         if (epoll_ctl(epfd, EPOLL_CTL_ADD, conn_fd, &ev) == -1) {
//             perror("epoll_ctl add");
//             close(conn_fd);
//             continue;
//         }
//     }
//     return 0;
// }

// // 处理客户端数据
// int handle_client_data(int epfd, int fd) {
//     char buf[BUFFER_SIZE];
//     int total_read = 0;
    
//     // ET模式必须循环读取直到EAGAIN
//     while (1) {
//         int n = read(fd, buf, sizeof(buf));
//         if (n == -1) {
//             if (errno == EAGAIN || errno == EWOULDBLOCK) {
//                 // 数据已读完
//                 break;
//             } else {
//                 // 读取出错
//                 perror("read");
//                 close(fd);
//                 // 从epoll中移除
//                 epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL);
//                 return -1;
//             }
//         } else if (n == 0) {
//             // 客户端关闭连接
//             printf("Client fd=%d closed connection\n", fd);
//             close(fd);
//             epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL);
//             return 0;
//         } else {
//             // 成功读取数据，回显（Echo）
//             total_read += n;
//             printf("Received %d bytes from fd=%d\n", n, fd);
            
//             // 发送数据回客户端（简单回显）
//             int write_pos = 0;
//             while (write_pos < n) {
//                 int written = write(fd, buf + write_pos, n - write_pos);
//                 if (written == -1) {
//                     if (errno == EAGAIN || errno == EWOULDBLOCK) {
//                         // 写缓冲区满了，这里简化处理，实际应该缓存数据
//                         // 或者注册EPOLLOUT事件等待可写
//                         usleep(1000);
//                         continue;
//                     }
//                     perror("write");
//                     close(fd);
//                     epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL);
//                     return -1;
//                 }
//                 write_pos += written;
//             }
//         }
//     }
//     return 0;
// }

// int main() {
//     // 1. 创建监听socket
//     int listen_fd = create_listen_socket(PORT);
//     if (listen_fd == -1) {
//         exit(EXIT_FAILURE);
//     }

//     // 2. 创建epoll实例
//     int epfd = epoll_create(1);
//     if (epfd == -1) {
//         perror("epoll_create");
//         close(listen_fd);
//         exit(EXIT_FAILURE);
//     }

//     // 3. 将监听fd添加到epoll
//     struct epoll_event ev;
//     ev.events = EPOLLIN;  // 监听socket使用LT模式即可
//     ev.data.fd = listen_fd;
//     if (epoll_ctl(epfd, EPOLL_CTL_ADD, listen_fd, &ev) == -1) {
//         perror("epoll_ctl add listen_fd");
//         close(listen_fd);
//         close(epfd);
//         exit(EXIT_FAILURE);
//     }

//     // 4. 事件循环
//     struct epoll_event events[MAX_EVENTS];
//     printf("Epoll Echo Server started, waiting for connections...\n");

//     while (1) {
//         // 等待事件发生，-1表示无限等待
//         int nfds = epoll_wait(epfd, events, MAX_EVENTS, -1);
//         if (nfds == -1) {
//             if (errno == EINTR) {
//                 // 被信号中断，继续
//                 continue;
//             }
//             perror("epoll_wait");
//             break;
//         }

//         // 处理所有就绪事件
//         for (int i = 0; i < nfds; i++) {
//             int fd = events[i].data.fd;

//             // 检查是否发生错误
//             if (events[i].events & (EPOLLERR | EPOLLHUP)) {
//                 printf("Error or hangup on fd=%d\n", fd);
//                 close(fd);
//                 epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL);
//                 continue;
//             }

//             if (fd == listen_fd) {
//                 // 监听socket有事件：新连接到来
//                 if (handle_new_connection(epfd, listen_fd) == -1) {
//                     // 错误处理
//                 }
//             } else {
//                 // 客户端socket有事件：可读
//                 if (events[i].events & EPOLLIN) {
//                     handle_client_data(epfd, fd);
//                 }
                
//                 // 如果有EPOLLOUT事件（写就绪），可以在这里处理
//                 // 本示例为了简洁，在handle_client_data中同步写回
//             }
//         }
//     }

//     // 清理
//     close(listen_fd);
//     close(epfd);
//     return 0;
// }

#include <iostream>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/statement.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
#include "SimpleIni.h"

int main() {
    // 1. 创建 SimpleIni 对象并加载配置
    CSimpleIniA ini;
    ini.SetUnicode(); // 支持 UTF-8
    
    SI_Error rc = ini.LoadFile("config.ini");
    if (rc < 0) {
        std::cerr << "❌ 错误: 无法加载 config.ini 文件" << std::endl;
        std::cerr << "   请确保文件存在且格式正确" << std::endl;
        return 1;
    }

    try {
        // 2. 读取配置（带默认值，增强健壮性）
        const char* host = ini.GetValue("database", "host", "127.0.0.1");
        long port = ini.GetLongValue("database", "port", 3306);
        const char* user = ini.GetValue("database", "user", "");
        const char* password = ini.GetValue("database", "password", "");
        const char* database = ini.GetValue("database", "database", "");
        
        // 检查必要的配置项是否为空
        if (user[0] == '\0' || password[0] == '\0' || database[0] == '\0') {
            std::cerr << "❌ 错误: config.ini 中缺少必要的配置项" << std::endl;
            return 1;
        }

        // 3. 连接数据库
        sql::mysql::MySQL_Driver *driver;
        sql::Connection *con;

        driver = sql::mysql::get_mysql_driver_instance();
        std::string connection_string = "tcp://" + std::string(host) + ":" + std::to_string(port);
        
        std::cout << "🔗 正在连接数据库: " << connection_string << std::endl;
        con = driver->connect(connection_string, user, password);
        con->setSchema(database);

        std::cout << "✅ 数据库连接成功！" << std::endl;
        
        // 4. 测试查询（可选）
        sql::Statement *stmt = con->createStatement();
        sql::ResultSet *res = stmt->executeQuery("SELECT VERSION()");
        if (res->next()) {
            std::cout << "📦 MySQL 版本: " << res->getString(1) << std::endl;
        }
        
        // 5. 释放资源
        delete res;
        delete stmt;
        delete con;
        
    } catch (sql::SQLException &e) {
        std::cerr << "❌ SQL错误: " << e.what() << std::endl;
        std::cerr << "   错误码: " << e.getErrorCode() << std::endl;
        std::cerr << "   SQL状态: " << e.getSQLState() << std::endl;
        return 1;
    } catch (std::exception &e) {
        std::cerr << "❌ 系统错误: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}