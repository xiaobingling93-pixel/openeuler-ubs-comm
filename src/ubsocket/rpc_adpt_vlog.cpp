/*
 *Copyright (c) Huawei Technologies Co., Ltd. 2025-2025. All rights reserved.
 *Description: Provide the utility for umq buffer, iov, etc
 *Author:
 *Create: 2025-07-28
 *Note:
 *History: 2025-07-28
*/
#include <mutex>

#include "securec.h"
#include "rpc_adpt_vlog.h"

static util_vlog_ctx_t *g_rpc_adpt_vlog_ctx = nullptr;
static std::mutex g_mutex;
const char g_vlog_name[] = "RPC_ADPT";

util_vlog_ctx_t *RpcAdptGetLogCtx(void)
{
    if (g_rpc_adpt_vlog_ctx ==  nullptr) {
        std::lock_guard<std::mutex> lock(g_mutex);
        if (g_rpc_adpt_vlog_ctx ==  nullptr) {
            g_rpc_adpt_vlog_ctx = new (std::nothrow) util_vlog_ctx_t();
            if (g_rpc_adpt_vlog_ctx ==  nullptr) {
                return nullptr;
            }
            g_rpc_adpt_vlog_ctx->level = UTIL_VLOG_LEVEL_INFO;
            if ((memcpy_s(g_rpc_adpt_vlog_ctx->vlog_name, UTIL_VLOG_NAME_STR_LEN, g_vlog_name, strlen(g_vlog_name)) != 0)) {
                return nullptr;
            }
            g_rpc_adpt_vlog_ctx->vlog_output_func = default_vlog_output;
            g_rpc_adpt_vlog_ctx->rate_limited.interval_ms = UTIL_VLOG_PRINT_PERIOD_MS;
            g_rpc_adpt_vlog_ctx->rate_limited.num = UTIL_VLOG_PRINT_TIMES;
        }
    }
    return g_rpc_adpt_vlog_ctx;
}
