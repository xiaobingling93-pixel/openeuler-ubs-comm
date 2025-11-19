/*
 *Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 *Description: Provide the utility for umq buffer, iov, etc
 *Author:
 *Create: 2025-08-08
 *Note:
 *History: 2025-08-08
*/

#include "brpc_context.h"
#include "brpc_file_descriptor.h"

namespace Brpc {

std::atomic<int> Context::m_ref = {0};

::SocketFd *Context::CreateSocketFd(int fd, int event_fd)
{
    ::SocketFd *socket_fd = nullptr;
    try {
        switch (m_socket_fd_trans_mode) {
            case SOCKET_FD_TRANS_MODE_UMQ:
            case SOCKET_FD_TRANS_MODE_UMQ_ZERO_COPY:
                socket_fd = (::SocketFd *)new Brpc::SocketFd(fd, event_fd);
                break;
            default:
                break;      
        }
    } catch (std::exception& e) {
        RPC_ADPT_VLOG_ERR("%s\n", e.what());
        return nullptr;
    }

    return socket_fd;
}

::SocketFd *Context::CreateSocketFd(int fd)
{
    return CreateSocketFd(fd, -1);
}

::EpollFd *Context::CreateEpollFd(int fd)
{
    ::EpollFd *epoll_fd = nullptr;
    try {
        switch (m_socket_fd_trans_mode) {
            case SOCKET_FD_TRANS_MODE_UMQ:
            case SOCKET_FD_TRANS_MODE_UMQ_ZERO_COPY:
                epoll_fd = (::EpollFd *)new Brpc::EpollFd(fd);
                break;
            default:
                break;      
        }
    } catch (std::exception& e) {
        RPC_ADPT_VLOG_ERR("%s\n", e.what());
        return nullptr;
    }

    return epoll_fd;
}
}