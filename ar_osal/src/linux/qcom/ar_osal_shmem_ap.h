/**
 *
 * \file ar_osal_shmem.h
 *
 * \brief
 *      Defines public APIs for shared memory allocation for ARM master processor.
 * \copyright
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include <stdbool.h>
#include <stdatomic.h>
#include "ar_osal_shmem.h" // Public API
#include "ar_osal_error.h" // Error codes
#include "ar_osal_log.h"
#define AR_OSAL_SHMEM_LOG_TAG "COSH"

// --- Backend-specific prototypes (defined in your existing sources) ---
// ap backend (ar_osal_shmem_ap.c)
int32_t ar_shmem_ap_init(void);
int32_t ar_shmem_ap_alloc(ar_shmem_info *info);
int32_t ar_shmem_ap_free(ar_shmem_info *info);
int32_t ar_shmem_ap_map(ar_shmem_info *info);
int32_t ar_shmem_ap_unmap(ar_shmem_info *info);
int32_t ar_shmem_ap_hyp_assign_phys(ar_shmem_hyp_assign_phys_info *info);
int32_t ar_shmem_ap_get_uid(uint64_t alloc_handle, uint64_t *uid);
int32_t ar_shmem_ap_deinit(void);
