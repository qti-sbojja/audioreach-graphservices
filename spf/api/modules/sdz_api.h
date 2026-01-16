/* =========================================================================
*  Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
*  SPDX-License-Identifier: BSD-3-Clause
   ========================================================================= */

#ifndef SDZ_API_H
#define SDZ_API_H

/* =========================================================================
@file sdz_api.h

   API header file for Speaker Diarization Module.
========================================================================= */

/** @h2xml_title1           {Speaker Diarization (SDZ)}
    @h2xml_title_agile_rev  {Speaker Diarization (SDZ)}
    @h2xml_title_date       {Feb 18, 2025} */

#include "module_cmn_api.h"
#include "imcl_fwk_intent_api.h"

/*SDZ module only supports one input port.*/
#define CAPI_SDZ_MAX_INPUT_PORTS  1
#define CAPI_SDZ_MAX_OUTPUT_PORTS 0

/* Stack memory size used by SDZ module library */
#define SDZ_STACK_SIZE                 2048

/*sdz_speaker_segment_t structure speaker_name and speaker_uuid sizes */
#define SDZ_SPEAKER_INFO_SPEAKER_NAME_SIZE 64
#define SDZ_SPEAKER_INFO_SPEAKER_UUID_SIZE 40

/* SDZ client id definitions - Public API enum
 * These values are exposed to external clients and should not be changed
 * for backward compatibility */
typedef enum sdz_client_id_t
{
   SDZ_CLIENT_INVALID = 0,
   SDZ_CLIENT_HLOS = 1,
   SDZ_CLIENT_SENSOR = 2,
   SDZ_CLIENT_MAX = SDZ_CLIENT_SENSOR
} sdz_client_id_t;

/* Global unique Module ID definition
   Module library is independent of this number, it is defined here for static
   loading purpose only */
#define MODULE_ID_SDZ    0x0702C000

/**
    @h2xmlm_module       {"MODULE_ID_SDZ",
                          MODULE_ID_SDZ}
    @h2xmlm_displayName  {"Speaker Diarization"}
    @h2xmlm_description  {SDZ (Speaker Diarization) module identifies a speaker ID and gives out an unique
                          identification number for further consumption and processing by applications which
                          require the speaker identification from single MIC stream\n

    . This module supports the following parameter IDs:\n
        . #PARAM_ID_SDZ_VERSION\n
        . #PARAM_ID_SDZ_INPUT_THRESHOLD\n
        . #PARAM_ID_SDZ_INPUT_THRESHOLD_V2\n
        . #PARAM_ID_SDZ_OUTPUT_CONFIG\n
        . #PARAM_ID_SDZ_OUTPUT_CONFIG_V2\n
        . #PARAM_ID_SDZ_FORCE_OUTPUT\n
        . #PARAM_ID_SDZ_FORCE_OUTPUT_V2\n
        . #PARAM_ID_SDZ_MODEL\n
        . #PARAM_ID_SDZ_OUTPUT\n
        . #PARAM_ID_SDZ_OUTPUT_V2\n
        . #PARAM_ID_SDZ_VOICE_PROFILE\n
        . #PARAM_ID_SDZ_TIMESTAMP_FORMAT\n
        . #EVENT_ID_SDZ_OUTPUT\n
        . #EVENT_ID_SDZ_OUTPUT_V2\n
    \n
    . Supported Input Media Format:      \n
    .           Data Format          : FIXED_POINT \n
    .           fmt_id               : MEDIA_FMT_ID_PCM \n
    .           Sample Rates         : 16000 \n
    .           Number of channels   : 1  \n
    .           Channel type         : C  \n
    .           Bits per sample      : 16 \n
    .           Q format             : Q15 for bps = 16 \n
    .           Interleaving         : Deinterleaved Unpacked \n
    .           Signed/unsigned      : Signed  \n
    }
    @h2xmlm_dataMaxInputPorts   {CAPI_SDZ_MAX_INPUT_PORTS}
    @h2xmlm_dataMaxOutputPorts  {CAPI_SDZ_MAX_OUTPUT_PORTS}
    @h2xmlm_dataInputPorts      {IN=2}
    @h2xmlm_supportedContTypes  {APM_CONTAINER_TYPE_GC}
    @h2xmlm_isOffloadable       {FALSE}
    @h2xmlm_stackSize           {SDZ_STACK_SIZE}
    @h2xmlm_ctrlDynamicPortIntent   { "DAM-SDZ Control" = INTENT_ID_AUDIO_DAM_DETECTION_ENGINE_CTRL,
                                      maxPorts= 1 }
    @h2xmlm_toolPolicy          {Calibration}
    @{                   <-- Start of the Module -->
    @h2xml_Select        {"param_id_module_enable_t"}
    @h2xmlm_InsertParameter

*/

/*  ID of the SDZ version parameter used by MODULE_ID_SDZ.*/
#define PARAM_ID_SDZ_VERSION                            0x08001B08

/*  Structure for the SDZ version parameter of SDZ module. */
typedef struct sdz_version_t sdz_version_t;
/** @h2xmlp_parameter {"PARAM_ID_SDZ_VERSION", PARAM_ID_SDZ_VERSION}
    @h2xmlp_description {Structure to get the current library version.}
    @h2xmlp_toolPolicy {RTC_READONLY} */

#include "spf_begin_pack.h"
#include "spf_begin_pragma.h"

/*  Payload of the PARAM_ID_SDZ_VERSION parameter used by the
    SDZ module.
 */

struct sdz_version_t
{
  uint32_t lib_version_low;
   /**< @h2xmle_description {Lower 32 bits of the 64-bit library version number} */
  uint32_t lib_version_high;
  /**< @h2xmle_description {Higher 32 bits of the 64-bit library version number} */
}
#include "spf_end_pragma.h"
#include "spf_end_pack.h"
;

/*   @h2xml_Select                  {sdz_version_t}
     @h2xmlm_InsertParameter */

/*  ID of the SDZ input threshold parameter used by MODULE_ID_SDZ.*/
#define PARAM_ID_SDZ_INPUT_THRESHOLD                    0x08001B09

/*  Structure for the input threshold parameter of SDZ module. */
typedef struct param_id_sdz_input_threshold_t param_id_sdz_input_threshold_t;
/** @h2xmlp_parameter   {"PARAM_ID_SDZ_INPUT_THRESHOLD", PARAM_ID_SDZ_INPUT_THRESHOLD}
    @h2xmlp_description {Param to control wakeup threshold for SDZ engine based on input buffering i.e.,
                         to control the frequency at which SDZ engine should be woken up periodically }
    @h2xmlp_toolPolicy  {NO_SUPPORT} */

#include "spf_begin_pack.h"
#include "spf_begin_pragma.h"

/*  Payload of the PARAM_ID_SDZ_INPUT_THRESHOLD parameter used by the
    SDZ module.
 */
struct param_id_sdz_input_threshold_t
{
    uint32_t buf_duration_ms;
    /**< @h2xmle_description {Duration of input threshold in milli seconds, used to create input buffering and wakeup control.}
         @h2xmle_range       {200..30000} ms
         @h2xmle_default     {200} */
}
#include "spf_end_pragma.h"
#include "spf_end_pack.h"
;

/*   @h2xml_Select                  {param_id_sdz_input_threshold_t}
     @h2xmlm_InsertParameter */

/*  ID of the SDZ input threshold parameter V2 used by MODULE_ID_SDZ.*/
#define PARAM_ID_SDZ_INPUT_THRESHOLD_V2                   0x08001BC7

/*  Structure for the input threshold parameter V2 of SDZ module with client ID support. */
typedef struct param_id_sdz_input_threshold_v2_t param_id_sdz_input_threshold_v2_t;
/** @h2xmlp_parameter   {"PARAM_ID_SDZ_INPUT_THRESHOLD_V2", PARAM_ID_SDZ_INPUT_THRESHOLD_V2}
    @h2xmlp_description {Param to control wakeup threshold for SDZ engine based on input buffering with client ID support}
    @h2xmlp_toolPolicy  {NO_SUPPORT} */

#include "spf_begin_pack.h"
#include "spf_begin_pragma.h"

/*  Payload of the PARAM_ID_SDZ_INPUT_THRESHOLD_V2 parameter used by the
 SDZ module.
 */
struct param_id_sdz_input_threshold_v2_t
{
    uint32_t client_id;
    /**< @h2xmle_description {Client ID for this input threshold configuration.
                              This API provide the input threshold requirement of each client.  The lowest threshold will be chosen as the operating threshold.}
         @h2xmle_default     {1}
         @h2xmle_rangeList   {"SDZ_CLIENT_HLOS"=1, "SDZ_CLIENT_SENSOR"=2} */
    uint32_t buf_duration_ms;
    /**< @h2xmle_description {Duration of input threshold in milli seconds, used to create input buffering and wakeup control.}
         @h2xmle_range       {200..30000} ms
         @h2xmle_default     {200} */
}
#include "spf_end_pragma.h"
#include "spf_end_pack.h"
;

/*   @h2xml_Select                      {param_id_sdz_input_threshold_v2_t}
     @h2xmlm_InsertParameter */

/*  ID of the SDZ output configuration parameter used by MODULE_ID_SDZ.*/
#define PARAM_ID_SDZ_OUTPUT_CONFIG                      0x08001B0D

/*  Enums defining the mode of output used by param_id_sdz_output_config_t */
typedef enum sdz_output_mode_t
{
    SDZ_NON_BUFFERED = 0,                 /* Send SDZ engine output immediately. Each buffer shall have one payload
                                             corresponding to a partial/complete segment of speech.
                                             Typical usage : live display of output. */

    SDZ_BUFFERED,                         /* Buffer up SDZ engine output based on configured buffer size from client.
                                             Send the event to clients once buffer is full.
                                             Typical usage : save transcription output in background. */
}sdz_output_mode_t;

/*  Structure for the output configuration parameter of SDZ module. */
typedef struct param_id_sdz_output_config_t param_id_sdz_output_config_t;
/** @h2xmlp_parameter   {"PARAM_ID_SDZ_OUTPUT_CONFIG", PARAM_ID_SDZ_OUTPUT_CONFIG}
    @h2xmlp_description {Param to configure required output mode and buffering mode for SDZ engine  }
    @h2xmlp_toolPolicy  {NO_SUPPORT} */

#include "spf_begin_pack.h"
#include "spf_begin_pragma.h"

/*  Payload of the PARAM_ID_SDZ_OUTPUT_CONFIG parameter used by the
    SDZ module.
 */

struct param_id_sdz_output_config_t
{
    uint32_t output_mode;
    /**< @h2xmle_description {Configuration to specify transcription output mode depending on usecase requirement.
                              SDZ_NON_BUFFERED - Output of each SDZ algo process is immediately sent to client.
                              SDZ_BUFFERED     - Output of each SDZ algo process is buffered and sent to client intermittently.}
         @h2xmle_default     {SDZ_NON_BUFFERED}
         @h2xmle_rangeList   {"SDZ_NON_BUFFERED"=0, "SDZ_BUFFERED"=1} */
   uint32_t out_buf_size;
    /**< @h2xmle_description {Configuration to specify transcription output buffer size, 0 for SDZ_NON_BUFFERED output mode.}
         @h2xmle_default     {0}
         @h2xmle_range       {0.. 262144} */
   uint32_t num_bufs;
    /**< @h2xmle_description {Configuration to specify number of output buffers, 0 for SDZ_NON_BUFFERED output mode.}
         @h2xmle_default     {0}
         @h2xmle_range       {0.. 16} */
}

#include "spf_end_pragma.h"
#include "spf_end_pack.h"
;

/*   @h2xml_Select                  {param_id_sdz_output_config_t}
     @h2xmlm_InsertParameter */

/*  Base structure for client-specific output configuration of SDZ module. */
typedef struct param_id_sdz_output_config_v2_base_t param_id_sdz_output_config_v2_base_t;

/** @h2xmlp_subStruct
    @h2xmlp_description {Sub structure for client-specific output configuration of SDZ module} */

#include "spf_begin_pack.h"
#include "spf_begin_pragma.h"
struct param_id_sdz_output_config_v2_base_t
{
    uint32_t client_id;
    /**< @h2xmle_description {Client ID (sdz_client_id_t) for this configuration.
                              This API allow multiple clients to have unique configurations.}
         @h2xmle_default     {1}
         @h2xmle_rangeList   {"SDZ_CLIENT_HLOS"=1, "SDZ_CLIENT_SENSOR"=2} */
    uint32_t output_mode;
    /**< @h2xmle_description {Configuration to specify transcription output mode depending on usecase requirement.
                              SDZ_NON_BUFFERED - Output of each SDZ algo process is immediately sent to client.
                              SDZ_BUFFERED     - Output of each SDZ algo process is buffered and sent to client intermittently.}
         @h2xmle_default     {SDZ_NON_BUFFERED}
         @h2xmle_rangeList   {"SDZ_NON_BUFFERED"=0, "SDZ_BUFFERED"=1} */
    uint32_t out_buf_size;
    /**< @h2xmle_description {Configuration to specify transcription output buffer size depending on usecase requirement.}
         @h2xmle_default     {0}
         @h2xmle_range       {0..262144} */
    uint32_t num_bufs;
    /**< @h2xmle_description {Configuration to specify number of output buffers.}
         @h2xmle_default     {1}
         @h2xmle_range       {1..16} */
}
#include "spf_end_pragma.h"
#include "spf_end_pack.h"
;

/*  ID of the SDZ output configuration V2 parameter used by MODULE_ID_SDZ.*/
#define PARAM_ID_SDZ_OUTPUT_CONFIG_V2                          0x08001BDA

/*  Structure for the multi-client output configuration parameter of SDZ module. */
typedef struct param_id_sdz_output_config_v2_t param_id_sdz_output_config_v2_t;
/** @h2xmlp_parameter   {"PARAM_ID_SDZ_OUTPUT_CONFIG_V2", PARAM_ID_SDZ_OUTPUT_CONFIG_V2}
    @h2xmlp_description {Param to configure output mode and buffering for multiple SDZ clients simultaneously}
    @h2xmlp_toolPolicy  {CALIBRATION} */
#include "spf_begin_pack.h"
#include "spf_begin_pragma.h"
struct param_id_sdz_output_config_v2_t
{
    uint32_t num_clients;
    /**< @h2xmle_description {Number of client configurations provided}
         @h2xmle_default     {1}
         @h2xmle_range       {1..2} */
    param_id_sdz_output_config_v2_base_t client_configs[0];
    /**< @h2xmle_description {Variable array of client-specific output configurations} */
}
#include "spf_end_pragma.h"
#include "spf_end_pack.h"
;

/*   @h2xml_Select                    {param_id_sdz_output_config_v2_t}
     @h2xmlm_InsertParameter */

/*  ID of the SDZ force output parameter used by MODULE_ID_SDZ.*/
#define PARAM_ID_SDZ_FORCE_OUTPUT                       0x08001B0E

/*  Structure for the force output parameter of SDZ module. */
typedef struct param_id_sdz_force_output_t param_id_sdz_force_output_t;
/** @h2xmlp_parameter   {"PARAM_ID_SDZ_FORCE_OUTPUT", PARAM_ID_SDZ_FORCE_OUTPUT}
    @h2xmlp_description {Forces the SDZ module to produce output event irrespective of configured input threshold and current buffering state}
    @h2xmlp_toolPolicy  {NO_SUPPORT} */

#include "spf_begin_pack.h"
#include "spf_begin_pragma.h"

/*  Payload of the PARAM_ID_SDZ_FORCE_OUTPUT parameter used by the
    SDZ module.
 */
struct param_id_sdz_force_output_t
{
    uint32_t force_output;
    /**< @h2xmle_description {forces SDZ module to produce output with current data without waiting for further data accumulation.}
         @h2xmle_default     {0} */
}
#include "spf_end_pragma.h"
#include "spf_end_pack.h"
;

/*   @h2xml_Select                  {param_id_sdz_force_output_t}
     @h2xmlm_InsertParameter */

/*  ID of the SDZ force output V2 parameter used by MODULE_ID_SDZ.*/
#define PARAM_ID_SDZ_FORCE_OUTPUT_V2                    0x08001BDB

/*  Structure for the force output V2 parameter of SDZ module with client ID support. */
typedef struct param_id_sdz_force_output_v2_t param_id_sdz_force_output_v2_t;
/** @h2xmlp_parameter   {"PARAM_ID_SDZ_FORCE_OUTPUT_V2", PARAM_ID_SDZ_FORCE_OUTPUT_V2}
    @h2xmlp_description {Forces the SDZ module to produce output event for a specific client irrespective of configured input threshold and current buffering state}
    @h2xmlp_toolPolicy  {NO_SUPPORT} */

#include "spf_begin_pack.h"
#include "spf_begin_pragma.h"

/*  Payload of the PARAM_ID_SDZ_FORCE_OUTPUT_V2 parameter used by the
    SDZ module.
 */
struct param_id_sdz_force_output_v2_t
{
    uint32_t client_id;
    /**< @h2xmle_description {Client ID (sdz_client_id_t) for which to force output.
                              This allows forcing output for a specific client independently.}
         @h2xmle_default     {1}
         @h2xmle_rangeList   {"SDZ_CLIENT_HLOS"=1, "SDZ_CLIENT_SENSOR"=2} */
    uint32_t force_output;
    /**< @h2xmle_description {forces SDZ module to produce output for the specified client with current data without waiting for further data accumulation.}
         @h2xmle_default     {0} */
}
#include "spf_end_pragma.h"
#include "spf_end_pack.h"
;

/*   @h2xml_Select                  {param_id_sdz_force_output_v2_t}
     @h2xmlm_InsertParameter */

/*  ID of the SDZ output event configuration used by MODULE_ID_SDZ.*/
#define EVENT_ID_SDZ_OUTPUT                             0x08001B0A

/** @h2xmlp_parameter   {"EVENT_ID_SDZ_OUTPUT",
                          EVENT_ID_SDZ_OUTPUT}
    @h2xmlp_description {Event raised by SDZ to the clients; registration and event have different payload types}
    @h2xmlp_toolPolicy  {NO_SUPPORT} */

/*  Structure for the event registration payload of the EVENT_ID_SDZ_OUTPUT. */
typedef struct event_id_sdz_output_reg_cfg_t event_id_sdz_output_reg_cfg_t;

#include "spf_begin_pack.h"
#include "spf_begin_pragma.h"
struct event_id_sdz_output_reg_cfg_t
{
   uint32_t event_payload_type;
   /**< @h2xmle_description {Field to configure event payload type}
                             QUERY     - Client queries the output via getparam upon receiving SDZ output event.
                             INBAND    - SDZ output event has the output payload inband in the event.}
                             DONT_CARE - SDZ output event can be INBAND or QUERY based, flag in event
                                         payload indicates the mode.}
        @h2xmle_default     {0}
        @h2xmle_rangeList   {"QUERY"=0, "INBAND"=1, "DONT_CARE"=2} */
}
#include "spf_end_pragma.h"
#include "spf_end_pack.h"
;

#define EVENT_ID_SDZ_OUTPUT_V2                  0x08001BC8

/** @h2xmlp_parameter   {"EVENT_ID_SDZ_OUTPUT_V2",
                          EVENT_ID_SDZ_OUTPUT_V2}
    @h2xmlp_description {Event raised by SDZ to the clients with client ID support; registration and event have different payload types.}
                        Registering to this event will mean using PARAM_ID_SDZ_OUTPUT_V2. PARAM_ID_SDZ_OUTPUT should not be expected.}
    @h2xmlp_toolPolicy  {NO_SUPPORT} */

/* V2 event registration configuration with client ID */
#include "spf_begin_pack.h"
#include "spf_begin_pragma.h"
struct event_id_sdz_output_reg_cfg_v2_t
{
   uint32_t client_id;
   /**< @h2xmle_description {Client ID (sdz_client_id_t) to identify the client.}
        @h2xmle_default     {1}
        @h2xmle_rangeList   {"SDZ_CLIENT_HLOS"=1, "SDZ_CLIENT_SENSOR"=2} */
   uint32_t event_payload_type;
   /**< @h2xmle_description {Field to configure event payload type}
                             QUERY     - Client queries the output via getparam upon receiving SDZ output event.
                             INBAND    - SDZ output event has the output payload inband in the event.
                             DONT_CARE - SDZ output event can be INBAND or QUERY based, flag in event
                                         payload indicates the mode.}
        @h2xmle_default     {0}
        @h2xmle_rangeList   {"QUERY"=0, "INBAND"=1, "DONT_CARE"=2} */
}
#include "spf_end_pragma.h"
#include "spf_end_pack.h"
;
typedef struct event_id_sdz_output_reg_cfg_v2_t event_id_sdz_output_reg_cfg_v2_t;

/* Sub Structure for the event/param payload used for containing timestamp associated with each speaker ID.*/
/** @h2xmlp_subStruct
    @h2xmlp_description {Sub Structure for the event/param payload used for containing timestamp associated with each speaker ID} */
#include "spf_begin_pack.h"
#include "spf_begin_pragma.h"

struct sdz_speaker_info_t
{
    uint32_t speaker_id;
   /**< @h2xmle_description  {speaker ID detected by the algorithm}
        @h2xmle_default      {0}
        @h2xmle_range        {0..0xFFFFFFFF} */

   uint32_t start_ts_lsw;
   /**< @h2xmle_description  {Field indicating the least-significant 32 bits of the start timestamp for
                              above speaker id.}
        @h2xmle_default      {0}
        @h2xmle_range        {0..0xFFFFFFFF} */

   uint32_t start_ts_msw;
   /**< @h2xmle_description  {Field indicating the most-significant 32 bits of the start timestamp for
                              above speaker id.}
        @h2xmle_default      {0}
        @h2xmle_range        {0..0xFFFFFFFF} */

   uint32_t end_ts_lsw;
   /**< @h2xmle_description  {Field indicating the least-significant 32 bits of the end timestamp for
                              above speaker id.}
        @h2xmle_default      {0}
        @h2xmle_range        {0..0xFFFFFFFF} */

   uint32_t end_ts_msw;
   /**< @h2xmle_description  {Field indicating the most-significant 32 bits of the end timestamp for
                              above speaker id.}
        @h2xmle_default      {0}
        @h2xmle_range        {0..0xFFFFFFFF} */

}
#include "spf_end_pragma.h"
#include "spf_end_pack.h"
;

typedef struct sdz_speaker_info_t sdz_speaker_info_t;

/* Sub Structure for speaker segment information - used when voice profile mode is enabled */
/** @h2xmlp_subStruct
    @h2xmlp_description {Sub Structure for speaker segment information with UUID, speaker name, and confidence scores} */
#include "spf_begin_pack.h"
#include "spf_begin_pragma.h"


struct sdz_speaker_segment_t
{
    uint32_t time_stamp_start_lsw;
   /**< @h2xmle_description  {Field indicating the least-significant 32 bits of the start timestamp in microseconds}
        @h2xmle_default      {0}
        @h2xmle_range        {0..0xFFFFFFFF} */

    uint32_t time_stamp_start_msw;
   /**< @h2xmle_description  {Field indicating the most-significant 32 bits of the start timestamp in microseconds}
        @h2xmle_default      {0}
        @h2xmle_range        {0..0xFFFFFFFF} */

    uint32_t time_stamp_end_lsw;
   /**< @h2xmle_description  {Field indicating the least-significant 32 bits of the end timestamp in microseconds}
        @h2xmle_default      {0}
        @h2xmle_range        {0..0xFFFFFFFF} */

    uint32_t time_stamp_end_msw;
   /**< @h2xmle_description  {Field indicating the most-significant 32 bits of the end timestamp in microseconds}
        @h2xmle_default      {0}
        @h2xmle_range        {0..0xFFFFFFFF} */

    float diarization_score;
   /**< @h2xmle_description  {Confidence value for diarization, reserved for future planned functionalities(reserved for future planned functionalities)}
        @h2xmle_default      {0.0}
        @h2xmle_range        {0.0..100.0}
        @h2xmlp_toolPolicy  {NO_SUPPORT} */

    float identification_score;
   /**< @h2xmle_description  {Confidence value for speaker identification/persistence, reserved for future planned functionalities(reserved for future planned functionalities)}
        @h2xmle_default      {0.0}
        @h2xmle_range        {0.0..100.0}
        @h2xmlp_toolPolicy  {NO_SUPPORT} */

    uint8_t speaker_name[SDZ_SPEAKER_INFO_SPEAKER_NAME_SIZE];
   /**< @h2xmle_description  {Name of the speaker from profile}
        @h2xmle_default      {0} */

    uint8_t speaker_uuid[SDZ_SPEAKER_INFO_SPEAKER_UUID_SIZE];
   /**< @h2xmle_description  {Unique ID associated with a speaker who has an existing profile}
        @h2xmle_default      {0} */

    int32_t speaker_id;
   /**< @h2xmle_description  {Speaker label (e.g., 'talker 1', 'talker 2'), -1 for unidentified speaker}
        @h2xmle_default      {0}
        @h2xmle_range        {-1..0x7FFFFFFF} */

    uint32_t speaker_is_owner;
   /**< @h2xmle_description  {Flag indicating if the current speaker is device owner (from profile)}
        @h2xmle_default      {0}
        @h2xmle_rangeList    {"FALSE"=0, "TRUE"=1} */
}
#include "spf_end_pragma.h"
#include "spf_end_pack.h"
;

typedef struct sdz_speaker_segment_t sdz_speaker_segment_t;

typedef struct sdz_output_status_t sdz_output_status_t;

/* Sub Structure for the event/param payload used for containing information about number of detected spakers and each speaker info.*/
/** @h2xmlp_subStruct
    @h2xmlp_description {Sub Structure for the event/param payload used for containing information about number of detected spakers and each speaker info} */
#include "spf_begin_pack.h"
#include "spf_begin_pragma.h"

struct sdz_output_status_t
{
    uint32_t num_speakers;
    /**< @h2xmle_description  {Field indicating the number of sdz_speaker_info_t present in
                               the speakers_info_list buf}
         @h2xmle_default      {0}
         @h2xmle_range        {0..0xFFFFFFFF} */

    uint32_t overlap_detected;
    /**< @h2xmle_description  {Field indicating if the overlapping detected in the given inference
                               result.}
         @h2xmle_default      {0}
         @h2xmle_rangeList    {"FALSE"=0, "TRUE"=1} */
#if defined(__H2XML__)
    sdz_speaker_info_t speakers_info_list[0];
    /*@variableArraySize      {num_speakers} */
#endif
}
#include "spf_end_pragma.h"
#include "spf_end_pack.h"
;

/* Sub Structure V2 for the event/param payload - used when voice profile mode is enabled */
typedef struct sdz_output_status_v2_t sdz_output_status_v2_t;

/** @h2xmlp_subStruct
    @h2xmlp_description {Sub Structure V2 for the event/param payload containing speaker segments with UUID and speaker name} */

#include "spf_begin_pack.h"
#include "spf_begin_pragma.h"

struct sdz_output_status_v2_t
{
    uint32_t num_segments;
    /**< @h2xmle_description  {Field indicating the number of sdz_speaker_segment_t present in
                               the segments buffer}
         @h2xmle_default      {0}
         @h2xmle_range        {0x0..0xFFFFFFFF} */

#if defined(__H2XML__)
    sdz_speaker_segment_t segments[0];
    /*@variableArraySize      {num_segments} */
#endif
}
#include "spf_end_pragma.h"
#include "spf_end_pack.h"
;

/*  ID of the SDZ output get parameter used by MODULE_ID_SDZ.*/
#define PARAM_ID_SDZ_OUTPUT                             0x08001B0C


/*  Structure for the event payload of EVENT_ID_SDZ_OUTPUT. */
typedef struct event_id_sdz_output_event_t event_id_sdz_output_event_t;

#include "spf_begin_pack.h"
#include "spf_begin_pragma.h"

struct event_id_sdz_output_event_t
{
    uint32_t event_payload_type;
    /**< @h2xmle_description   {event type of SDZ event.
                                Query = Client will query via getparam upon receiving this event, using the
                                        output_token provided as part of this payload.
                                Inband = SDZ output payload follows inband after the payload_size field }
          @h2xmle_default      {0}
          @h2xmle_rangeList    {"Query"=0, "Inband"=1} */
   uint32_t output_token;
   /**< @h2xmle_description   {Token provided by the SDZ engine corresponding to an output.
                               This is used to query the corresponding output payload.
                               If not configured or invalid, Payload corresponds to next output buffer
                               is sent.}
         @h2xmle_default      {0}
         @h2xmle_range        {0..0xFFFFFFFF} */
   uint32_t num_outputs;
   /**< @h2xmle_description   {Number of outputs of type sdz_output_status_t in the payload }
         @h2xmle_default      {0}
         @h2xmle_range        {0..0xFFFFFFFF} */
   uint32_t payload_size;
   /**< @h2xmle_description   {Payload size in bytes. Following this field is the payload of
                              this size. Payload is variable array of type sdz_output_status_t}
         @h2xmle_default      {0}
         @h2xmle_range        {0..0xFFFFFFFF} */
#if defined(__H2XML__)
   sdz_output_status_t buf[0];
   /**< @h2xmle_description   {Inband Payload, This will be filled if event_payload_type is Inband}
   /*@variableArraySize       {payload_size} */
#endif
}

#include "spf_end_pragma.h"
#include "spf_end_pack.h"
;

/*  Structure for the PARAM_ID_SDZ_OUTPUT parameter. */

/** @h2xmlp_parameter   {"PARAM_ID_SDZ_OUTPUT", PARAM_ID_SDZ_OUTPUT}
    @h2xmlp_description {Get Param to query output from the module in response to the QUERY based event.}
    @h2xmlp_toolPolicy  {NO_SUPPORT} */
typedef event_id_sdz_output_event_t param_id_sdz_output_t;

/*   @h2xml_Select                  {param_id_sdz_output_t}
     @h2xmlm_InsertParameter */

/*  Structure for the event payload of EVENT_ID_SDZ_OUTPUT_V2. */
typedef struct event_id_sdz_output_event_v2_t event_id_sdz_output_event_v2_t;

#include "spf_begin_pack.h"
#include "spf_begin_pragma.h"

struct event_id_sdz_output_event_v2_t
{
    uint32_t client_id;
    /**< @h2xmle_description   {Client ID (sdz_client_id_t) to identify which client's output buffer to query.
                                This must match the client_id used in PARAM_ID_SDZ_OUTPUT_CONFIG_V2.
                                Allows multiple clients to independently query their own output buffers.}
          @h2xmle_default      {1}
          @h2xmle_rangeList    {"SDZ_CLIENT_HLOS"=1, "SDZ_CLIENT_SENSOR"=2} */
    uint32_t event_payload_type;
    /**< @h2xmle_description   {event type of SDZ event.
                                Query = Client will query via getparam upon receiving this event, using the
                                        output_token provided as part of this payload.
                                Inband = SDZ output payload follows inband after the payload_size field }
          @h2xmle_default      {0}
          @h2xmle_rangeList    {"Query"=0, "Inband"=1} */
   uint32_t output_token;
   /**< @h2xmle_description   {Token provided by the SDZ engine corresponding to an output.
                               This is used to query the corresponding output payload.
                               If not configured or invalid, Payload corresponds to next output buffer
                               is sent.}
         @h2xmle_default      {0}
         @h2xmle_range        {0..0xFFFFFFFF} */
   uint32_t num_outputs;
   /**< @h2xmle_description   {Number of outputs of type sdz_output_status_v2_t in the payload }
         @h2xmle_default      {0}
         @h2xmle_range        {0..0xFFFFFFFF} */
   uint32_t payload_size;
   /**< @h2xmle_description   {Payload size in bytes. Following this field is the payload of
                              this size. Payload is variable array of type sdz_output_status_v2_t}
         @h2xmle_default      {0}
         @h2xmle_range        {0..0xFFFFFFFF} */
#if defined(__H2XML__)
   sdz_output_status_v2_t buf[0];
   /**< @h2xmle_description   {Inband Payload, This will be filled if event_payload_type is Inband}
   /*@variableArraySize       {payload_size} */
#endif
}

#include "spf_end_pragma.h"
#include "spf_end_pack.h"
;

/*  ID of the SDZ output V2 get parameter used by MODULE_ID_SDZ (with voice profile mode enabled).*/
#define PARAM_ID_SDZ_OUTPUT_V2 0x08001BA7

/*  Structure for the PARAM_ID_SDZ_OUTPUT_V2 parameter. */
typedef struct param_id_sdz_output_v2_t param_id_sdz_output_v2_t;

/** @h2xmlp_parameter   {"PARAM_ID_SDZ_OUTPUT_V2", PARAM_ID_SDZ_OUTPUT_V2}
    @h2xmlp_description {Get Param to query output from the module in response to the QUERY based event.}
    @h2xmlp_toolPolicy  {NO_SUPPORT} */

#include "spf_begin_pack.h"
#include "spf_begin_pragma.h"

struct param_id_sdz_output_v2_t
{
    uint32_t client_id;
    /**< @h2xmle_description   {Client ID (sdz_client_id_t) to identify which client's output buffer to query.
                                This must match the client_id used in PARAM_ID_SDZ_OUTPUT_CONFIG_V2.
                                Allows multiple clients to independently query their own output buffers.}
          @h2xmle_default      {1}
          @h2xmle_rangeList    {"SDZ_CLIENT_HLOS"=1, "SDZ_CLIENT_SENSOR"=2} */
   uint32_t output_token;
   /**< @h2xmle_description   {Token provided by the SDZ engine corresponding to an output.
                               This is used to query the corresponding output payload.
                               If not configured or invalid, Payload corresponds to next output buffer
                               is sent.}
         @h2xmle_default      {0}
         @h2xmle_range        {0..0xFFFFFFFF} */
   uint32_t num_outputs;
   /**< @h2xmle_description   {Number of outputs of type sdz_output_status_v2_t in the payload }
         @h2xmle_default      {0}
         @h2xmle_range        {0..0xFFFFFFFF} */
   uint32_t payload_size;
   /**< @h2xmle_description   {Payload size in bytes. Following this field is the payload of
                              this size. Payload is variable array of type sdz_output_status_v2_t}
         @h2xmle_default      {0}
         @h2xmle_range        {0..0xFFFFFFFF} */
#if defined(__H2XML__)
   sdz_output_status_v2_t buf[0];
   /*@variableArraySize       {payload_size} */
#endif
}

#include "spf_end_pragma.h"
#include "spf_end_pack.h"
;

/*   @h2xml_Select                  {param_id_sdz_output_v2_t}
     @h2xmlm_InsertParameter */

#define PARAM_ID_SDZ_MODEL                              0x08001B0B

/*  Structure for the model parameter of SDZ module. */
typedef struct param_id_sdz_model_t param_id_sdz_model_t;
/** @h2xmlp_parameter           {"PARAM_ID_SDZ_MODEL", PARAM_ID_SDZ_MODEL}
    @h2xmlp_description         {NN Model related parameters}
    @h2xmlp_ToolPolicy          {NO_SUPPORT}
    @h2xmlp_isHwAccel           {FALSE}
    @h2xmlp_isNeuralNetParam    {TRUE}
    @h2xmlp_isOffloaded         {TRUE}
    @h2xmlp_persistType         {Shared}
*/

#include "spf_begin_pack.h"
#include "spf_begin_pragma.h"

/*  Structure for the PARAM_ID_SDZ_OUTPUT parameter. */
struct param_id_sdz_model_t
{
    uint32_t model_align;
    /**< @h2xmle_description {Data Alignment required for the model_data}
         @h2xmle_default     {256}
         @h2xmle_range       {0..8192}
         @h2xmle_group       {SPF}
         @h2xmle_subgroup    {NNVAD}
         @h2xmle_policy      {Advanced} */

    uint32_t model_offset;
    /**< @h2xmle_description {Data offset (bytes) to align the start address of model by model_align}
         @h2xmle_default     {0}
         @h2xmle_range       {0..8191}
         @h2xmle_policy      {Basic} */

    uint32_t model_size;
    /**< @h2xmle_description {Size of the model in bytes}
         @h2xmle_default     {0}
         @h2xmle_range       {0x00000000..0xFFFFFFFF}
         @h2xmle_policy      {Basic} */
#if defined(__H2XML__)
    uint8_t model[0];
    /**< @h2xmle_description {Model data: The path to the model file on the platform}
         @h2xmle_elementType {rawData}
         @h2xmle_displayType {stringField}
         @h2xmle_policy      {Basic} */
#endif
}
#include "spf_end_pragma.h"
#include "spf_end_pack.h"
;

/*   @h2xml_Select                  {param_id_sdz_model_t}
     @h2xmlm_InsertParameter */

/*  ID of the SDZ voice profile parameter used by MODULE_ID_SDZ.*/
#define PARAM_ID_SDZ_VOICE_PROFILE 0x08001BA5

/*  Structure for the voice profile parameter of SDZ module. */
typedef struct param_id_sdz_voice_profile_t param_id_sdz_voice_profile_t;
/** @h2xmlp_parameter   {"PARAM_ID_SDZ_VOICE_PROFILE", PARAM_ID_SDZ_VOICE_PROFILE}
    @h2xmlp_description {Param to set voice profile/voiceprint data for speaker identification}
    @h2xmlp_toolPolicy  {NO_SUPPORT} */

#include "spf_begin_pack.h"
#include "spf_begin_pragma.h"

/*  Payload of the PARAM_ID_SDZ_VOICE_PROFILE parameter used by the
    SDZ module.
 */
struct param_id_sdz_voice_profile_t
{
    uint32_t client_id;
    /**< @h2xmle_description {Client ID for input voice profile.(reserved for future use, currently not being utilized)}
         @h2xmle_default     {1}
         @h2xmle_rangeList   {"SDZ_CLIENT_HLOS"=1} */
    uint32_t voiceprint_size;
    /**< @h2xmle_description {Size of the voiceprint data in bytes}
         @h2xmle_default     {0}
         @h2xmle_range       {0..0xFFFFFFFF} */
    uint8_t voiceprint_data[0];
    /**< @h2xmle_description {Voiceprint/voice profile data blob}
         @h2xmle_variableArraySize {voiceprint_size} */
}
#include "spf_end_pragma.h"
#include "spf_end_pack.h"
;

/*   @h2xml_Select                  {param_id_sdz_voice_profile_t}
     @h2xmlm_InsertParameter */

/*  ID of set SDZ timestamp fmt.*/
#define PARAM_ID_SDZ_TIMESTAMP_FORMAT 0x08001B84
/*  Structure to enable timestamp based on qtimer or UTC converted. */
typedef struct param_id_sdz_ts_fmt_t param_id_sdz_ts_fmt_t;
/** @h2xmlp_parameter   {"PARAM_ID_SDZ_TIMESTAMP_FORMAT", PARAM_ID_SDZ_TIMESTAMP_FORMAT}
    @h2xmlp_description {Set Param to set the timestamp format associated with text. }
    @h2xmlp_toolPolicy  {NO_SUPPORT} */

#include "spf_begin_pack.h"
#include "spf_begin_pragma.h"

/*  Payload of the PARAM_ID_SDZ_TIMESTAMP_FORMAT parameter used by the
 SDZ module.
 */
struct param_id_sdz_ts_fmt_t
{
     uint32_t ts_fmt;
     /**< @h2xmle_description  {Indicates the timestamp associated with speakers}
          @h2xmle_default      {0}
          @h2xmle_rangeList    {"UTC_FMT"=0, "QTIMER_FMT"=1} */
     uint32_t reset_period;
     /**< @h2xmle_description  {Indicates the period to read reference UTC time. Note: '0' means module latches to UTC time only at the beginning of the usecase and doesn't
                              periodically sync to UTC time for dynamic changes. For non-zero values say 'x' seconds, the module will dynamically sync and update the UTC time at every x seconds.
                              The maximum reset period allowed is a day.}
          @h2xmle_default      {0}
          @h2xmle_range        {0..86400} */
     uint32_t timestamp_unit;
     /**< @h2xmle_description  {Indicates the unit of timestamp. Currently only TIME_USEC is supported}
          @h2xmle_default      {1}
          @h2xmle_rangeList    {"TIME_USEC"=1} */
}

#include "spf_end_pragma.h"
#include "spf_end_pack.h"
;
/*   @h2xml_Select                    {param_id_sdz_ts_fmt_t}
     @h2xmlm_InsertParameter */
/** @}                   <-- End of the Module -->*/
#endif /* SDZ_API_H */
