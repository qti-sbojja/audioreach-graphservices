#ifndef _RX_HAPTCIS_SHMEM_API_H_
#define _RX_HAPTCIS_SHMEM_API_H_

/**
 * \file rx_haptics_shmem_api.h
 *
 * \brief
 *
 *
 * Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*------------------------------------------------------------------------
 * Include files
 * -----------------------------------------------------------------------*/
#include "ar_defs.h"

/*==============================================================================
   Shared memory communication protocol for haptics
==============================================================================*/

/* Type of configuration which client specifies to communicate with the module*/
typedef enum rx_haptics_shmem_config_type
{
   /* client wants to set the configuration from shared memory. */
   SET_CFG = 0,

   /* client wants to get the configuration in shared memory. */
   GET_CFG = 1,

} rx_haptics_shmem_config_type_t;

typedef enum rx_haptics_shmem_msg_mode
{
   // specifies global message would be sent once after that client
   // will only write data in shared memory and module needs to poll
   POLLING_BASED = 0,

   // specifies global message would be trigger based, meaning
   // every message will be sent via PARAM_ID_HAPTICS_GLOBAL_SHMEM_MSG
   TRIGGER_BASED = 1,
} rx_haptics_shmem_msg_mode_t;

typedef enum rx_haptics_shmem_rsp_type
{
   // specifies synchronization mechanism is polling based
   RSP_POLLING = 0,

   // specifies synchronization mechanism is event based
   RSP_EVENT_TRIGGER = 1,
} rx_haptics_shmem_rsp_type_t;

/* per config structure specified by the client to the module. */
typedef struct rx_haptics_shmem_config
{
   //config_type_t: set_cfg, get_cfg
   rx_haptics_shmem_config_type_t config_type;

   //pid for set_cfg or get_cfg
   uint32_t pid;

   //address offset from the base virtual base where this config data is provided or requested
   uint32_t config_addr_offset;

   //size of the memory block for the config data from config_addr_offset
   uint32_t config_size;

   // result code for the corresponding set/get param
   uint32_t err_code;
} rx_haptics_shmem_config_t;

typedef struct rx_haptics_shmem_payload_hdr
{
   // num of configurations part of payload, when set to 0 it indicates
   // that only global message header is passed for configuring module
   uint32_t num_config;

   //mode of response (event/polling)
   rx_haptics_shmem_rsp_type_t shmem_config_rsp_mode;

   // mode of msg (Trigger/polling)
   rx_haptics_shmem_msg_mode_t shmem_msg_mode;

   //Bit 0 is the client msg flag that indicates client has written msg if set
   // module needs to reset it once read is complete
   //Bit 1 is the module msg flag that indicates module has written msg if set
   // client needs to reset it once read is complete
   uint32_t shmem_property_flag;

} rx_haptics_shmem_payload_hdr_t;

/*header structure placed in the shared memory by the client to specifies one of multiple configuration to the module.*/
typedef struct rx_haptics_shmem_ipc_protocol
{
   uint32_t version;

   uint32_t payload_header_size;
#ifdef __H2XML__
   // payload header pointer which is of type rx_haptics_shmem_payload_hdr_t
   uint32_t payload_header[0];
  // config_ptr which is array of type rx_haptics_shmem_config_t
   uint32_t config_ptr[0];
#endif

} rx_haptics_shmem_ipc_protocol_t;

#endif //_RX_HAPTCIS_SHMEM_API_H_
