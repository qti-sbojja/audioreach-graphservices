#ifndef _SOFT_PAUSE_API_H_
#define _SOFT_PAUSE_API_H_

/*==============================================================================
 @file soft_pause_api.h
 @brief This file contains Soft pause module APIs

  Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
  SPDX-License-Identifier: BSD-3-Clause
==============================================================================*/
// clang-format off
/* =========================================================================
   Edit History

   $Header: //components/rel/avs.spm_pp/1.0.15/volume_control/capi/soft_pause/api/soft_pause_api.h#1 $

   when       who       what, where, why
   --------   -------   ----------------------------------------------------
   7/15/18      np        Created file
   ======================================================================== */
// clang-format on

/*------------------------------------------------------------------------
  Include files
  -----------------------------------------------------------------------*/
#include "module_cmn_api.h"

/*# @h2xml_title1          {Soft Pause Module API}
    @h2xml_title_agile_rev {Soft Pause Module API}
    @h2xml_title_date      {July 15, 2018} */

/*==============================================================================
   Defines
==============================================================================*/

/** @ingroup ar_spf_mod_soft_pause_mod
    Soft stepping linear ramping curve. */
#define PARAM_PAUSE_RAMPINGCURVE_LINEAR                               0

/** @ingroup ar_spf_mod_soft_pause_mod
    Exponential ramping curve. */
#define PARAM_PAUSE_RAMPINGCURVE_EXP                                  1

/** @ingroup ar_spf_mod_soft_pause_mod
    Logarithmic ramping curve. */
#define PARAM_PAUSE_RAMPINGCURVE_LOG                                  2

/** @ingroup ar_spf_mod_soft_pause_mod
    Fractional exponent ramping curve. */
#define PARAM_PAUSE_RAMPINGCURVE_FRAC_EXP                             3

/** @ingroup ar_spf_mod_soft_pause_mod
    Input port ID for the Soft Pause module. */
#define PAUSE_DATA_INPUT_PORT                                         2

/** @ingroup ar_spf_mod_soft_pause_mod
    Output port ID for the Soft Pause module. @newpage */
#define PAUSE_DATA_OUTPUT_PORT                                        1

/*------------------------------------------------------------------------------
   Module
------------------------------------------------------------------------------*/

/** @ingroup ar_spf_mod_soft_pause_mod
    Identifier for the Soft Pause module.

    @subhead4{Supported parameter IDs}
    - #PARAM_ID_SOFT_PAUSE_RAMP_PARAMETERS @lstsp1
    - #PARAM_ID_SOFT_PAUSE_START @lstsp1
    - #PARAM_ID_SOFT_PAUSE_RESUME @lstsp1
    - #PARAM_ID_SOFT_PAUSE_RESUME_THRESHOLD @lstsp1
    - #PARAM_ID_SOFT_PAUSE_DOWNSTREAM_DELAY @lstsp1
    - #PARAM_ID_SOFT_PAUSE_RESUME_DEFERRED_RAMPUP

    @subhead4{Supported input media format ID}
    - Data format       : #DATA_FORMAT_FIXED_POINT @lstsp1
    - fmt_id            : Don't care @lstsp1
    - Sample rates      : Don't care @lstsp1
    - Number of channels: 1..128 (for certain products this module supports only 32 channels)@lstsp1
    - Channel type      : 0..128 @lstsp1
    - Bits per sample   : 16, 32 @lstsp1
    - Q format          : Don't care @lstsp1
    - Interleaving      : De-interleaved unpacked @lstsp1
    - Signed/unsigned   : Signed
*/
#define MODULE_ID_SOFT_PAUSE 0x07001019

/*# @h2xmlm_module             {"MODULE_ID_SOFT_PAUSE", MODULE_ID_SOFT_PAUSE}
    @h2xmlm_displayName        {"Soft Pause"}
    @h2xmlm_modSearchKeys	   {Audio}
    @h2xmlm_description        {ID for the Soft Pause module. For more
                                details, see AudioReach Signal Processing Framework (SPF) API Reference.}
    @h2xmlm_dataInputPorts     {IN  = PAUSE_DATA_INPUT_PORT}
    @h2xmlm_dataOutputPorts    {OUT = PAUSE_DATA_OUTPUT_PORT}
    @h2xmlm_dataMaxInputPorts  {1}
    @h2xmlm_dataMaxOutputPorts {1}
    @h2xmlm_supportedContTypes {APM_CONTAINER_TYPE_GC,APM_CONTAINER_TYPE_SC}
    @h2xmlm_isOffloadable      {false}
    @h2xmlm_stackSize          {4096}

    @{                     <-- Start of the Module --> */

/*==============================================================================
   API definitions
==============================================================================*/

/** @ingroup ar_spf_mod_soft_pause_mod
    Identifier for the parameter that configures the ramp curve parameters for
    pause, where:
    - Period is the duration of the ramp.
    - Step is the duration for which same gain is applied before
      calculating next gain in the curve.
    - Ramping curve is the shape of the curve (linear, exponential,
      logarithmic, fractional exponent).

    This parameter is used to set ramp parameters or get ramp parameters.

    @msgpayload
    pause_ramp_params_t
*/
#define PARAM_ID_SOFT_PAUSE_RAMP_PARAMETERS  0x0800102D

/*# @h2xmlp_parameter   {"PARAM_ID_SOFT_PAUSE_RAMP_PARAMETERS",
                          PARAM_ID_SOFT_PAUSE_RAMP_PARAMETERS}
    @h2xmlp_description {ID for the parameter that configures the ramp curve
                         parameters for pause. This parameter is used to set
                         ramp parameters or get ramp parameters. For more
                         details, see AudioReach Signal Processing Framework (SPF) API Reference.}
    @h2xmlp_toolPolicy  {RTC; Calibration} */

/** @ingroup ar_spf_mod_soft_pause_mod
    Payload for the #PARAM_ID_SOFT_PAUSE_RAMP_PARAMETERS parameter.
*/
#include "spf_begin_pack.h"
struct pause_ramp_params_t
{
   uint32_t period_ms_ramp_up;
   /**< Duration of the up ramp.

        @values 0 through 15000 milliseconds */

   /*#< @h2xmle_description {Duration of the up ramp specified in
                             milliseconds.}
        @h2xmle_range       {0..15000}
        @h2xmle_default     {0} */

   uint32_t step_us_ramp_up;
   /**< Duration for which a constant gain is applied before calculating the
        next gain in the ramp up curve.

        @values 0 through 15000000 microseconds */

   /*#< @h2xmle_description {Duration (in microseconds) for which a constant
                             gain is applied before calculating the next gain
                             in the ramp up curve.}
        @h2xmle_range       {0..15000000}
        @h2xmle_default     {0} */

   uint32_t ramping_curve_ramp_up;
   /**< Specifies ramp up of the ramping curve.

        @valuesbul
        - #PARAM_PAUSE_RAMPINGCURVE_LINEAR (Default)
        - #PARAM_PAUSE_RAMPINGCURVE_EXP
        - #PARAM_PAUSE_RAMPINGCURVE_LOG
        - #PARAM_PAUSE_RAMPINGCURVE_FRAC_EXP @tablebulletend */

   /*#< @h2xmle_description {Specifies the ramp up of the ramping curve.}
        @h2xmle_rangeList   {"PARAM_PAUSE_RAMPINGCURVE_LINEAR" = 0;
                             "PARAM_PAUSE_RAMPINGCURVE_EXP" = 1;
                             "PARAM_PAUSE_RAMPINGCURVE_LOG" = 2;
                             "PARAM_PAUSE_RAMPINGCURVE_FRAC_EXP" = 3}
        @h2xmle_default     {0} */

   uint32_t period_ms_ramp_down;
   /**< Duration of the down ramp.

        @values 0 through 15000 milliseconds */

   /*#< @h2xmle_description {Duration of the down ramp specified in
                             milliseconds.}
        @h2xmle_range       {0..15000}
        @h2xmle_default     {0} */

   uint32_t step_us_ramp_down;
   /**< Duration for which the same gain is applied before calculating the next
        gain in the ramp down curve.

        @values 0 through 15000000 microseconds */

   /*#< @h2xmle_description {Duration (in microseconds) for which the same gain
                             is applied before calculating the next gain in the
                             ramp down curve.}
        @h2xmle_range       {0..15000000}
        @h2xmle_default     {0} */

   uint32_t ramping_curve_ramp_down;
   /**< Specifies the ramp down of the ramping curve.

        @valuesbul
        - #PARAM_PAUSE_RAMPINGCURVE_LINEAR (Default)
        - #PARAM_PAUSE_RAMPINGCURVE_EXP
        - #PARAM_PAUSE_RAMPINGCURVE_LOG
        - #PARAM_PAUSE_RAMPINGCURVE_FRAC_EXP @tablebulletend */

   /*#< @h2xmle_description {Specifies the ramp down of the ramping curve.}
        @h2xmle_rangeList   {"PARAM_PAUSE_RAMPINGCURVE_LINEAR" = 0;
                             "PARAM_PAUSE_RAMPINGCURVE_EXP" = 1;
                             "PARAM_PAUSE_RAMPINGCURVE_LOG" = 2;
                             "PARAM_PAUSE_RAMPINGCURVE_FRAC_EXP" = 3}
        @h2xmle_default     {0} */
}
#include "spf_end_pack.h"
;
typedef struct pause_ramp_params_t pause_ramp_params_t;


/** @ingroup ar_spf_mod_soft_pause_mod
    Identifier for the parameter used to signal a pause to start. */
#define PARAM_ID_SOFT_PAUSE_START            0x0800102E

/*# @h2xmlp_emptyParameter {"PARAM_ID_SOFT_PAUSE_START",
                             PARAM_ID_SOFT_PAUSE_START}
    @h2xmlp_description    {ID for the parameter used to signal a pause to
                            start.}
    @h2xmlp_toolPolicy     {Calibration} */


/** @ingroup ar_spf_mod_soft_pause_mod
    Identifier for the parameter used to signal a resume to start. @newpage */
#define PARAM_ID_SOFT_PAUSE_RESUME           0x0800102F

/*# @h2xmlp_emptyParameter {"PARAM_ID_SOFT_PAUSE_RESUME",
                             PARAM_ID_SOFT_PAUSE_RESUME}
    @h2xmlp_description    {ID for the parameter used to signal a resume to start.}
    @h2xmlp_toolPolicy     {Calibration} */


/** @ingroup ar_spf_mod_soft_pause_mod
    Identifier for the parameter that configures the threshold required for
    a pause to resume.

    The threshold is an amplitude value in Q15 format. It is the minimum value
    a sample should have to notify the module to start ramp up when it is in
    the Pause state.

    @msgpayload
    pause_resume_threshold_t
*/
#define PARAM_ID_SOFT_PAUSE_RESUME_THRESHOLD 0x08001030

/*# @h2xmlp_parameter   {"PARAM_ID_SOFT_PAUSE_RESUME_THRESHOLD",
                          PARAM_ID_SOFT_PAUSE_RESUME_THRESHOLD}
    @h2xmlp_description {ID for the parameter that configures the threshold
                         required for a pause to resume. \n
                         The threshold is an amplitude value in Q15 format. It
                         is the minimum value a sample should have to notify
                         the module to start ramp up when it is in the Pause
                         state.}
    @h2xmlp_toolPolicy  {RTC; Calibration} */

/** @ingroup ar_spf_mod_soft_pause_mod
    Payload of the #PARAM_ID_SOFT_PAUSE_RESUME_THRESHOLD parameter.
*/
#include "spf_begin_pack.h"
struct pause_resume_threshold_t
{
   uint16_t threshold_Q15;
   /**< Specifies the set or get resume threshold (in Q15 format).

        @values 0 through 32767 (Default = 0) @newpagetable */

   /*#< @h2xmle_description {Specifies the set or get resume threshold (in Q15
                             format).}
        @h2xmle_range       {0..32767}
        @h2xmle_default     {0} */
}
#include "spf_end_pack.h"
;
typedef struct pause_resume_threshold_t pause_resume_threshold_t;


/** @ingroup ar_spf_mod_soft_pause_mod
    Identifier for the parameter used to configure the downstream delay from
    the stream to a device leg.

    @msgpayload
    pause_downstream_delay_t
*/
#define PARAM_ID_SOFT_PAUSE_DOWNSTREAM_DELAY  0x0800103E

/*# @h2xmlp_parameter   {"PARAM_ID_SOFT_PAUSE_DOWNSTREAM_DELAY",
                          PARAM_ID_SOFT_PAUSE_DOWNSTREAM_DELAY}
    @h2xmlp_description {ID for the parameter used to configure the downstream
                         delay from the stream to a device leg.}
    @h2xmlp_toolPolicy  {RTC; Calibration} */

/** @ingroup ar_spf_mod_soft_pause_mod
    Payload of the #PARAM_ID_SOFT_PAUSE_DOWNSTREAM_DELAY parameter.
*/
#include "spf_begin_pack.h"
struct pause_downstream_delay_t
{
   uint32_t delay_ms;
   /**< Specifies the downstream delay from the stream to the device leg.

        @values 0 through 65535 milliseconds (Default = 25) */

   /*#< @h2xmle_description {Specifies the downstream delay from the stream to
                             the device leg (in milliseconds).}
        @h2xmle_range       {0..65535}
        @h2xmle_default     {25} */
}
#include "spf_end_pack.h"
;
typedef struct pause_downstream_delay_t pause_downstream_delay_t;


/** @ingroup ar_spf_mod_soft_pause_mod
    Identifier for the parameter that configures the ramp up delay.

    The module mutes the signal for delay_ms time before ramping up during
    resume.

    @msgpayload
    pause_rampup_delay_t
*/
#define PARAM_ID_SOFT_PAUSE_RESUME_DEFERRED_RAMPUP  0x08001170

/*# @h2xmlp_parameter   {"PARAM_ID_SOFT_PAUSE_RESUME_DEFERRED_RAMPUP",
                          PARAM_ID_SOFT_PAUSE_RESUME_DEFERRED_RAMPUP}
    @h2xmlp_description {ID for the parameter that configures the ramp up
                         delay. The module mutes the signal for delay_ms time
                         before ramping up during resume.}
    @h2xmlp_toolPolicy  {RTC; Calibration} */

/** @ingroup ar_spf_mod_soft_pause_mod
    Payload of the #PARAM_ID_SOFT_PAUSE_RESUME_DEFERRED_RAMPUP parameter.
*/
#include "spf_begin_pack.h"
struct pause_rampup_delay_t
{
   uint32_t delay_ms;
   /**< Specifies the time that the module mutes before ramping up during
        resume.

        @values 0 through 1000 milliseconds */

   /*#< @h2xmle_description {Specifies the time (in milliseconds) that the
                             module mutes before ramping up during resume.}
        @h2xmle_range       {0..1000}
        @h2xmle_default     {0} */
}
#include "spf_end_pack.h"
;
typedef struct pause_rampup_delay_t pause_rampup_delay_t;

/*# @}                      <-- End of the Module --> */


/** @ingroup ar_spf_mod_soft_pause_mod
    Identifier for the Pause Complete event that the Soft Pause module sends to
    the client. */
#define EVENT_ID_SOFT_PAUSE_PAUSE_COMPLETE   0x0800103F

/** @ingroup ar_spf_mod_soft_pause_mod
    Identifier for the Resume Complete event that the Soft Pause module sends
    to the client. @newpage */
#define EVENT_ID_SOFT_PAUSE_RESUME_COMPLETE  0x08001043


#endif //_SOFT_PAUSE_API_H_
