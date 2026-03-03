# SPDX-License-Identifier: MIT
# Copyright (c) Huawei Technologies Co., Ltd. 2026-2026. All rights reserved.

add_library(urma_stub SHARED urma_stub.c)
target_include_directories(urma_stub PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
add_library(urma SHARED urma_stub.c)
target_include_directories(urma PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
add_library(urma_common SHARED urma_stub.c)
target_include_directories(urma_common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
add_library(tpsa SHARED urma_stub.c)
target_include_directories(tpsa PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})