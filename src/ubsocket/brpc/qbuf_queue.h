/*
 * Copyright (c) Huawei Technologies Co., Ltd. 2026-2026. All rights reserved.
 * Description: Provide the utility for umq buffer, iov, etc
 * Author:
 * Create: 2026-01-10
 * Note:
 * History: 2026-01-10
*/

#ifndef QBUF_QUEUE_H
#define QBUF_QUEUE_H

#include <malloc.h>
#include "umq_types.h"

namespace Brpc {
template <class T>
struct QbufQueueT {
    uint32_t head;
    uint32_t tail;
    uint32_t itemNb;
    T q[0];
};

template <typename T>
class QbufQueue {
public:
    explicit QbufQueue(struct QbufQueueT<T> *q) : m_q(q), m_isMalloc(false)
    {
        mutex = g_external_lock_ops.create(LT_EXCLUSIVE);
        if (mutex == nullptr) {
            RPC_ADPT_VLOG_ERR(ubsocket::UBSocket, "Create qbuf queue lock failed\n");
        }
    }

    explicit QbufQueue(uint32_t itemNb) : m_isMalloc(false), m_isExit(false)
    {
        mutex = g_external_lock_ops.create(LT_EXCLUSIVE);
        if (mutex == nullptr) {
            RPC_ADPT_VLOG_ERR(ubsocket::UBSocket, "Create qbuf queue lock failed\n");
            return;
        }

        if (InitQueue(itemNb) != 0) {
            RPC_ADPT_VLOG_ERR(ubsocket::UBSocket, "Init qbuf queue failed. \n");
            m_isExit = true;
            return;
        }
    }

    ~QbufQueue()
    {
        if (m_isMalloc) {
            m_isExit = true;
            free(m_q);
            m_q = nullptr;
        }
        if (mutex != nullptr) {
            g_external_lock_ops.destroy(mutex);
        }
    }

    inline bool IsEmpty()
    {
        return m_q->head == m_q->tail;
    }

    inline bool IsFull()
    {
        return m_q->head == (m_q->tail + 1) % m_q->itemNb;
    }

    int Enqueue(T data)
    {
        if (m_isExit) {
            RPC_ADPT_VLOG_WARN("Enqueue qbuf queue failed, reason: queue already exit\n");
            return -1;
        }

        if (m_q == nullptr) {
            RPC_ADPT_VLOG_ERR(ubsocket::UBSocket, "Enqueue qbuf queue failed, reason: queue is null\n");
            return -1;
        }

        if (mutex == nullptr) {
            RPC_ADPT_VLOG_ERR(ubsocket::UBSocket, "Enqueue qbuf queue failed, reason: lock is null\n");
            return -1;
        }

        if (IsFull()) {
            RPC_ADPT_VLOG_ERR(ubsocket::UBSocket,
                "Enqueue qbuf queue failed, reason: queue is full, head: %u, tail: %u, itemNb: %u\n",
                m_q->head, m_q->tail, m_q->itemNb);
            return -1;
        }

        ScopedUbExclusiveLocker sLock(mutex);
        m_q->q[m_q->tail] = data;
        m_q->tail = (m_q->tail == m_q->itemNb - 1) ? 0 : m_q->tail + 1;
        return 0;
    }

    int Dequeue(T *data)
    {
        if (data == nullptr) {
            RPC_ADPT_VLOG_ERR(ubsocket::UBSocket, "Dequeue qbuf queue failed, reason: data is null\n");
            return -1;
        }

        if (m_isExit) {
            RPC_ADPT_VLOG_WARN("Dequeue qbuf queue failed, reason: queue already exit\n");
            return -1;
        }

        if (m_q == nullptr) {
            RPC_ADPT_VLOG_ERR(ubsocket::UBSocket, "Dequeue qbuf queue failed, reason: queue is null\n");
            return -1;
        }

        if (mutex == nullptr) {
            RPC_ADPT_VLOG_ERR(ubsocket::UBSocket, "Dequeue qbuf queue failed, reason: lock is null\n");
            return -1;
        }

        if (IsEmpty()) {
            RPC_ADPT_VLOG_WARN(
                "Dequeue qbuf queue failed, reason: queue is empty, head: %u, tail: %u, itemNb: %u\n",
                m_q->head, m_q->tail, m_q->itemNb);
            return -1;
        }

        ScopedUbExclusiveLocker sLock(mutex);
        *data = m_q->q[m_q->head];
        m_q->head = (m_q->head == m_q->itemNb - 1) ? 0 : m_q->head + 1;
        return 0;
    }

    struct QbufQueueT<T> *m_q;

private:
    bool m_isMalloc = false;
    volatile bool m_isExit = false;
    u_external_mutex_t *mutex = nullptr;

    size_t RoundUp(size_t size, size_t align)
    {
        return (size + align - 1) - ((size + align - 1) % align);
    }

    int InitQueue(size_t itemNb)
    {
        size_t pageSize = sysconf(_SC_PAGESIZE);
        size_t headLen = sizeof(struct QbufQueueT<T>) + (itemNb + 1) * sizeof(T);
        headLen = RoundUp(headLen, pageSize);
        m_q = reinterpret_cast<struct QbufQueueT<T> *>(memalign(pageSize, headLen));
        if (m_q == nullptr) {
            RPC_ADPT_VLOG_ERR(ubsocket::UBSocket,
                "Init qbuf queue memalign failed, pageSize: %zu, headLen: %zu, errno: %d\n",
                pageSize, headLen, errno);
            return -1;
        }

        m_isMalloc = true;
        memset_s(m_q, sizeof(struct QbufQueueT<T>), 0, sizeof(struct QbufQueueT<T>));
        m_q->itemNb = itemNb + 1;
        return 0;
    }
};

}

#endif
