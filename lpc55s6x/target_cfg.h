/*
 * Copyright (c) 2018-2020 Arm Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __TARGET_CFG_H__
#define __TARGET_CFG_H__

#include "tfm_peripherals_def.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief Defines peripheral identifiers
 */
enum periph_id_e
{
    PERIPHERAL_NONE = -1,
    PERIPHERAL_SYSCON = 0,
    PERIPHERAL_IOCON,
    PERIPHERAL_GINT0,
    PERIPHERAL_GINT1,
    PERIPHERAL_PINT,
    PERIPHERAL_SEC_PINT,
    PERIPHERAL_INPUTMUX,
    PERIPHERAL_CTIMER0,
    PERIPHERAL_CTIMER1,
    PERIPHERAL_WWDT,
    PERIPHERAL_MRT,
    PERIPHERAL_UTICK,
    PERIPHERAL_ANACTRL,
    PERIPHERAL_PMC,
    PERIPHERAL_SYSCTRL,
    PERIPHERAL_CTIMER2,
    PERIPHERAL_CTIMER3,
    PERIPHERAL_CTIMER4,
    PERIPHERAL_RTC,
    PERIPHERAL_OSEVENT,
    PERIPHERAL_FLASH_CTRL,
    PERIPHERAL_PRINCE,
    PERIPHERAL_USBHPHY,
    PERIPHERAL_RNG,
    PERIPHERAL_PUF,
    PERIPHERAL_PLU,
    PERIPHERAL_ROMPC,
    PERIPHERAL_DMA0,
    PERIPHERAL_FS_USB_DEV,
    PERIPHERAL_SCT,
    PERIPHERAL_FLEXCOMM0,
    PERIPHERAL_FLEXCOMM1,
    PERIPHERAL_FLEXCOMM2,
    PERIPHERAL_FLEXCOMM3,
    PERIPHERAL_FLEXCOMM4,
    PERIPHERAL_MAILBOX,
    PERIPHERAL_GPIO0,
    PERIPHERAL_USB_HS_DEV,
    PERIPHERAL_CRC,
    PERIPHERAL_FLEXCOMM5,
    PERIPHERAL_FLEXCOMM6,
    PERIPHERAL_FLEXCOMM7,
    PERIPHERAL_SDIO,
    PERIPHERAL_DBG_MAILBOX,
    PERIPHERAL_HS_LSPI,
    PERIPHERAL_ADC0,
    PERIPHERAL_USB_FS_HOST,
    PERIPHERAL_USB_HS_HOST,
    PERIPHERAL_HASH,
    PERIPHERAL_CASPER,
    PERIPHERAL_PQ,
    PERIPHERAL_DMA1,
    PERIPHERAL_GPIO1,
    PERIPHERAL_AHB_SEC_CTRL
};

/**
 * \brief Store the addresses of memory regions
 */
struct memory_region_limits {
    uint32_t non_secure_code_start;
    uint32_t non_secure_partition_base;
    uint32_t non_secure_partition_limit;
    uint32_t veneer_base;
    uint32_t veneer_limit;
#ifdef BL2
    uint32_t secondary_partition_base;
    uint32_t secondary_partition_limit;
#endif /* BL2 */
};

/**
 * \brief Holds the data necessary to do isolation for a specific peripheral.
 */
struct tfm_spm_partition_platform_data_t
{
    uint32_t         periph_start;
    uint32_t         periph_limit;
    enum periph_id_e periph_id;
};

/**
 * \brief Configures the Memory Protection Controller.
 */
void mpc_init_cfg(void);

/**
 * \brief Configures the Peripheral Protection Controller.
 */
void ppc_init_cfg(void);

/**
 * \brief Restict access to peripheral to secure
 */
void ppc_configure_to_secure(enum periph_id_e periph_id);

/**
 * \brief Allow non-secure access to peripheral
 */
void ppc_configure_to_non_secure(enum periph_id_e periph_id);

/**
 * \brief Enable secure unprivileged access to peripheral
 */
void ppc_en_secure_unpriv(enum periph_id_e periph_id);

/**
 * \brief Clear secure unprivileged access to peripheral
 */
void ppc_clr_secure_unpriv(enum periph_id_e periph_id);

/**
 * \brief Configures SAU and IDAU.
 */
void sau_and_idau_cfg(void);

/**
 * \brief Enables the fault handlers and sets priorities.
 *
 * \return Returns values as specified by the \ref tfm_plat_err_t
 */
enum tfm_plat_err_t enable_fault_handlers(void);

/**
 * \brief Configures the system reset request properties
 *
 * \return Returns values as specified by the \ref tfm_plat_err_t
 */
enum tfm_plat_err_t system_reset_cfg(void);

/**
 * \brief Configures the system debug properties.
 *
 * \return Returns values as specified by the \ref tfm_plat_err_t
 */
enum tfm_plat_err_t init_debug(void);

/**
 * \brief Configures all external interrupts to target the
 *        NS state, apart for the ones associated to secure
 *        peripherals (plus MPC and PPC)
 *
 * \return Returns values as specified by the \ref tfm_plat_err_t
 */
enum tfm_plat_err_t nvic_interrupt_target_state_cfg(void);

/**
 * \brief This function enable the interrupts associated
 *        to the secure peripherals (plus the isolation boundary violation
 *        interrupts)
 *
 * \return Returns values as specified by the \ref tfm_plat_err_t
 */
enum tfm_plat_err_t nvic_interrupt_enable(void);

#ifdef __cplusplus
}
#endif

#endif /* __TARGET_CFG_H__ */
