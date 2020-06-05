/*
 * Copyright (c) 2018-2020 Arm Limited. All rights reserved.
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

#include CMSIS_device_header
#include "tz_config.h"
#include "target_cfg.h"
#include "region_defs.h"
#include "tfm_plat_defs.h"

/* Macros to pick linker symbols */
#define REGION(a, b, c) a##b##c
#define REGION_NAME(a, b, c) REGION(a, b, c)
#define REGION_DECLARE(a, b, c) extern uint32_t REGION_NAME(a, b, c)

/* The section names come from the scatter file */
REGION_DECLARE(Load$$LR$$, LR_NS_PARTITION, $$Base);
REGION_DECLARE(Load$$LR$$, LR_VENEER, $$Base);
REGION_DECLARE(Load$$LR$$, LR_VENEER, $$Limit);
#ifdef BL2
REGION_DECLARE(Load$$LR$$, LR_SECONDARY_PARTITION, $$Base);
#endif /* BL2 */

const struct memory_region_limits memory_regions = {
    .non_secure_code_start =
        (uint32_t)&REGION_NAME(Load$$LR$$, LR_NS_PARTITION, $$Base) +
        BL2_HEADER_SIZE,

    .non_secure_partition_base =
        (uint32_t)&REGION_NAME(Load$$LR$$, LR_NS_PARTITION, $$Base),

    .non_secure_partition_limit =
        (uint32_t)&REGION_NAME(Load$$LR$$, LR_NS_PARTITION, $$Base) +
        NS_PARTITION_SIZE - 1,

    .veneer_base =
        (uint32_t)&REGION_NAME(Load$$LR$$, LR_VENEER, $$Base),

    .veneer_limit =
        (uint32_t)&REGION_NAME(Load$$LR$$, LR_VENEER, $$Limit),

#ifdef BL2
    .secondary_partition_base =
        (uint32_t)&REGION_NAME(Load$$LR$$, LR_SECONDARY_PARTITION, $$Base),

    .secondary_partition_limit =
        (uint32_t)&REGION_NAME(Load$$LR$$, LR_SECONDARY_PARTITION, $$Base) +
        SECONDARY_PARTITION_SIZE - 1,
#endif /* BL2 */
};

/* Define Peripherals NS address range for the platform */
#define PERIPHERALS_BASE_NS_START (0x40000000)
#define PERIPHERALS_BASE_NS_END   (0x4FFFFFFF)

/* To write into AIRCR register, 0x5FA value must be write to the VECTKEY field,
 * otherwise the processor ignores the write.
 */
#define SCB_AIRCR_WRITE_MASK ((0x5FAUL << SCB_AIRCR_VECTKEY_Pos))

static const struct {
  volatile uint32_t *addr;
           uint32_t  pos;
} periph_data[] =
{
  /*PERIPHERAL_SYSCON*/
  { &AHB_SECURE_CTRL->SEC_CTRL_APB_BRIDGE[0].SEC_CTRL_APB_BRIDGE0_MEM_CTRL0,  0U },
  /*PERIPHERAL_IOCON*/
  { &AHB_SECURE_CTRL->SEC_CTRL_APB_BRIDGE[0].SEC_CTRL_APB_BRIDGE0_MEM_CTRL0,  4U },
  /*PERIPHERAL_GINT0*/
  { &AHB_SECURE_CTRL->SEC_CTRL_APB_BRIDGE[0].SEC_CTRL_APB_BRIDGE0_MEM_CTRL0,  8U },
  /*PERIPHERAL_GINT1*/
  { &AHB_SECURE_CTRL->SEC_CTRL_APB_BRIDGE[0].SEC_CTRL_APB_BRIDGE0_MEM_CTRL0, 12U },
  /*PERIPHERAL_PINT*/
  { &AHB_SECURE_CTRL->SEC_CTRL_APB_BRIDGE[0].SEC_CTRL_APB_BRIDGE0_MEM_CTRL0, 16U },
  /*PERIPHERAL_SEC_PINT*/
  { &AHB_SECURE_CTRL->SEC_CTRL_APB_BRIDGE[0].SEC_CTRL_APB_BRIDGE0_MEM_CTRL0, 20U },
  /*PERIPHERAL_INPUTMUX*/
  { &AHB_SECURE_CTRL->SEC_CTRL_APB_BRIDGE[0].SEC_CTRL_APB_BRIDGE0_MEM_CTRL0, 24U },

  /*PERIPHERAL_CTIMER0*/
  { &AHB_SECURE_CTRL->SEC_CTRL_APB_BRIDGE[0].SEC_CTRL_APB_BRIDGE0_MEM_CTRL1,  0U },
  /*PERIPHERAL_CTIMER1*/
  { &AHB_SECURE_CTRL->SEC_CTRL_APB_BRIDGE[0].SEC_CTRL_APB_BRIDGE0_MEM_CTRL1,  4U },
  /*PERIPHERAL_WWDT*/
  { &AHB_SECURE_CTRL->SEC_CTRL_APB_BRIDGE[0].SEC_CTRL_APB_BRIDGE0_MEM_CTRL1, 16U },
  /*PERIPHERAL_MRT*/
  { &AHB_SECURE_CTRL->SEC_CTRL_APB_BRIDGE[0].SEC_CTRL_APB_BRIDGE0_MEM_CTRL1, 20U },
  /*PERIPHERAL_UTICK*/
  { &AHB_SECURE_CTRL->SEC_CTRL_APB_BRIDGE[0].SEC_CTRL_APB_BRIDGE0_MEM_CTRL1, 24U },

  /*PERIPHERAL_ANACTRL*/
  { &AHB_SECURE_CTRL->SEC_CTRL_APB_BRIDGE[0].SEC_CTRL_APB_BRIDGE0_MEM_CTRL2, 12U },

  /*PERIPHERAL_PMC*/
  { &AHB_SECURE_CTRL->SEC_CTRL_APB_BRIDGE[1].SEC_CTRL_APB_BRIDGE1_MEM_CTRL0,  0U },
  /*PERIPHERAL_SYSCTRL*/
  { &AHB_SECURE_CTRL->SEC_CTRL_APB_BRIDGE[1].SEC_CTRL_APB_BRIDGE1_MEM_CTRL0, 12U },

  /*PERIPHERAL_CTIMER2*/
  { &AHB_SECURE_CTRL->SEC_CTRL_APB_BRIDGE[1].SEC_CTRL_APB_BRIDGE1_MEM_CTRL1,  0U },
  /*PERIPHERAL_CTIMER3*/
  { &AHB_SECURE_CTRL->SEC_CTRL_APB_BRIDGE[1].SEC_CTRL_APB_BRIDGE1_MEM_CTRL1,  4U },
  /*PERIPHERAL_CTIMER4*/
  { &AHB_SECURE_CTRL->SEC_CTRL_APB_BRIDGE[1].SEC_CTRL_APB_BRIDGE1_MEM_CTRL1,  8U },
  /*PERIPHERAL_RTC*/
  { &AHB_SECURE_CTRL->SEC_CTRL_APB_BRIDGE[1].SEC_CTRL_APB_BRIDGE1_MEM_CTRL1, 16U },
  /*PERIPHERAL_OSEVENT*/
  { &AHB_SECURE_CTRL->SEC_CTRL_APB_BRIDGE[1].SEC_CTRL_APB_BRIDGE1_MEM_CTRL1, 20U },

  /*PERIPHERAL_FLASH_CTRL*/
  { &AHB_SECURE_CTRL->SEC_CTRL_APB_BRIDGE[1].SEC_CTRL_APB_BRIDGE1_MEM_CTRL2, 16U },
  /*PERIPHERAL_PRINCE*/
  { &AHB_SECURE_CTRL->SEC_CTRL_APB_BRIDGE[1].SEC_CTRL_APB_BRIDGE1_MEM_CTRL2, 20U },

  /*PERIPHERAL_USBHPHY*/
  { &AHB_SECURE_CTRL->SEC_CTRL_APB_BRIDGE[1].SEC_CTRL_APB_BRIDGE1_MEM_CTRL3,  0U },
  /*PERIPHERAL_RNG*/
  { &AHB_SECURE_CTRL->SEC_CTRL_APB_BRIDGE[1].SEC_CTRL_APB_BRIDGE1_MEM_CTRL3,  8U },
  /*PERIPHERAL_PUF*/
  { &AHB_SECURE_CTRL->SEC_CTRL_APB_BRIDGE[1].SEC_CTRL_APB_BRIDGE1_MEM_CTRL3, 12U },
  /*PERIPHERAL_PLU*/
  { &AHB_SECURE_CTRL->SEC_CTRL_APB_BRIDGE[1].SEC_CTRL_APB_BRIDGE1_MEM_CTRL3, 20U },
  /*PERIPHERAL_ROMPC*/
  { &AHB_SECURE_CTRL->SEC_CTRL_APB_BRIDGE[1].SEC_CTRL_APB_BRIDGE1_MEM_CTRL3, 24U },

  /*PERIPHERAL_DMA0*/
  { &AHB_SECURE_CTRL->SEC_CTRL_AHB_PORT8_SLAVE0_RULE,   8U },
  /*PERIPHERAL_FS_USB_DEV*/
  { &AHB_SECURE_CTRL->SEC_CTRL_AHB_PORT8_SLAVE0_RULE,  16U },
  /*PERIPHERAL_SCT*/
  { &AHB_SECURE_CTRL->SEC_CTRL_AHB_PORT8_SLAVE0_RULE,  20U },
  /*PERIPHERAL_FLEXCOMM0*/
  { &AHB_SECURE_CTRL->SEC_CTRL_AHB_PORT8_SLAVE0_RULE,  24U },
  /*PERIPHERAL_FLEXCOMM1*/
  { &AHB_SECURE_CTRL->SEC_CTRL_AHB_PORT8_SLAVE0_RULE,  28U },

  /*PERIPHERAL_FLEXCOMM2*/
  { &AHB_SECURE_CTRL->SEC_CTRL_AHB_PORT8_SLAVE1_RULE,   0U },
  /*PERIPHERAL_FLEXCOMM3*/
  { &AHB_SECURE_CTRL->SEC_CTRL_AHB_PORT8_SLAVE1_RULE,   4U },
  /*PERIPHERAL_FLEXCOMM4*/
  { &AHB_SECURE_CTRL->SEC_CTRL_AHB_PORT8_SLAVE1_RULE,   8U },
  /*PERIPHERAL_MAILBOX*/
  { &AHB_SECURE_CTRL->SEC_CTRL_AHB_PORT8_SLAVE1_RULE,  12U },
  /*PERIPHERAL_GPIO0*/
  { &AHB_SECURE_CTRL->SEC_CTRL_AHB_PORT8_SLAVE1_RULE,  16U },

  /*PERIPHERAL_USB_HS_DEV*/
  { &AHB_SECURE_CTRL->SEC_CTRL_AHB_PORT9_SLAVE0_RULE,  16U },
  /*PERIPHERAL_CRC*/
  { &AHB_SECURE_CTRL->SEC_CTRL_AHB_PORT9_SLAVE0_RULE,  20U },
  /*PERIPHERAL_FLEXCOMM5*/
  { &AHB_SECURE_CTRL->SEC_CTRL_AHB_PORT9_SLAVE0_RULE,  24U },
  /*PERIPHERAL_FLEXCOMM6*/
  { &AHB_SECURE_CTRL->SEC_CTRL_AHB_PORT9_SLAVE0_RULE,  28U },

  /*PERIPHERAL_FLEXCOMM7*/
  { &AHB_SECURE_CTRL->SEC_CTRL_AHB_PORT9_SLAVE1_RULE,   0U },
  /*PERIPHERAL_SDIO*/
  { &AHB_SECURE_CTRL->SEC_CTRL_AHB_PORT9_SLAVE1_RULE,  12U },
  /*PERIPHERAL_DBG_MAILBOX*/
  { &AHB_SECURE_CTRL->SEC_CTRL_AHB_PORT9_SLAVE1_RULE,  16U },
  /*PERIPHERAL_HS_LSPI*/
  { &AHB_SECURE_CTRL->SEC_CTRL_AHB_PORT9_SLAVE1_RULE,  28U },

  /*PERIPHERAL_ADC0*/
  { &AHB_SECURE_CTRL->SEC_CTRL_AHB_PORT10[0].SLAVE0_RULE,  0U },
  /*PERIPHERAL_USB_FS_HOST*/
  { &AHB_SECURE_CTRL->SEC_CTRL_AHB_PORT10[0].SLAVE0_RULE,  8U },
  /*PERIPHERAL_USB_HS_HOST*/
  { &AHB_SECURE_CTRL->SEC_CTRL_AHB_PORT10[0].SLAVE0_RULE, 12U },
  /*PERIPHERAL_HASH*/
  { &AHB_SECURE_CTRL->SEC_CTRL_AHB_PORT10[0].SLAVE0_RULE, 16U },
  /*PERIPHERAL_CASPER*/
  { &AHB_SECURE_CTRL->SEC_CTRL_AHB_PORT10[0].SLAVE0_RULE, 20U },
  /*PERIPHERAL_PQ*/
  { &AHB_SECURE_CTRL->SEC_CTRL_AHB_PORT10[0].SLAVE0_RULE, 24U },
  /*PERIPHERAL_DMA1*/
  { &AHB_SECURE_CTRL->SEC_CTRL_AHB_PORT10[0].SLAVE0_RULE, 28U },

  /*PERIPHERAL_GPIO1*/
  { &AHB_SECURE_CTRL->SEC_CTRL_AHB_PORT10[0].SLAVE1_RULE,  0U },
  /*PERIPHERAL_AHB_SEC_CTRL*/
  { &AHB_SECURE_CTRL->SEC_CTRL_AHB_PORT10[0].SLAVE1_RULE,  4U },
};

struct tfm_spm_partition_platform_data_t tfm_peripheral_std_uart = {
  /* FLEXCOMM0 (NS) */
  0x40086000U,
  0x40086FFFU,
  PERIPHERAL_NONE /*PERIPHERAL_FLEXCOMM0*/
};

struct tfm_spm_partition_platform_data_t tfm_peripheral_timer0 = {
  /* CTIMER0 (NS) */
  0x40008000U,
  0x40008FFFU,
  PERIPHERAL_NONE /*PERIPHERAL_CTIMER0*/
};

enum tfm_plat_err_t enable_fault_handlers(void)
{
    /* Explicitly set secure fault priority to the highest */
    NVIC_SetPriority(SecureFault_IRQn, 0);

    /* Enables BUS, MEM, USG and Secure faults */
    SCB->SHCSR |= SCB_SHCSR_USGFAULTENA_Msk
                  | SCB_SHCSR_BUSFAULTENA_Msk
                  | SCB_SHCSR_MEMFAULTENA_Msk
                  | SCB_SHCSR_SECUREFAULTENA_Msk;
    return TFM_PLAT_ERR_SUCCESS;
}

enum tfm_plat_err_t system_reset_cfg(void)
{
    uint32_t reg_value = SCB->AIRCR;

    /* Clear SCB_AIRCR_VECTKEY value */
    reg_value &= ~(uint32_t)(SCB_AIRCR_VECTKEY_Msk);

    /* Enable system reset request only to the secure world */
    reg_value |= (uint32_t)(SCB_AIRCR_WRITE_MASK | SCB_AIRCR_SYSRESETREQS_Msk);

    SCB->AIRCR = reg_value;

    return TFM_PLAT_ERR_SUCCESS;
}

enum tfm_plat_err_t init_debug(void)
{
//#if defined(DAUTH_NONE)
//#else

//#if !defined(DAUTH_CHIP_DEFAULT)
//#error "No debug authentication setting is provided."
//#endif

//#endif
    return TFM_PLAT_ERR_SUCCESS;
}

/*----------------- NVIC interrupt target state to NS configuration ----------*/
enum tfm_plat_err_t nvic_interrupt_target_state_cfg(void)
{
    /* Target every interrupt to NS; unimplemented interrupts will be WI */
//  Configured via CMSIS-Zone generated code
    TZ_Config_NVIC();
//  for (uint8_t i=0; i<sizeof(NVIC->ITNS)/sizeof(NVIC->ITNS[0]); i++) {
//      NVIC->ITNS[i] = 0xFFFFFFFF;
//  }

    /* Make sure that Secure VIO is targeted to S state */
    NVIC_ClearTargetState(SEC_VIO_IRQn);
    return TFM_PLAT_ERR_SUCCESS;
}

/*----------------- NVIC interrupt enabling for S peripherals ----------------*/
enum tfm_plat_err_t nvic_interrupt_enable(void)
{
    NVIC_EnableIRQ(SEC_VIO_IRQn);
    return TFM_PLAT_ERR_SUCCESS;
}

/*------------------- SAU/IDAU configuration functions -----------------------*/

void sau_and_idau_cfg(void)
{
    TZ_Config_SAU();
}

/*------------------- Memory configuration functions -------------------------*/

void mpc_init_cfg(void)
{
    TZ_Config_MPC();
}

/*---------------------- PPC configuration functions -------------------------*/

void ppc_init_cfg(void)
{
    TZ_Config_PPC();
}

void ppc_configure_to_non_secure(enum periph_id_e periph_id)
{
    /* Clear Secure flag for peripheral to enable NS access */
    *periph_data[periph_id].addr &= ~(2U << (periph_data[periph_id].pos));
}

void ppc_configure_to_secure(enum periph_id_e periph_id)
{
    /* Set Secure flag for peripheral to prevent NS access */
    *periph_data[periph_id].addr |= 2U << (periph_data[periph_id].pos);
}

void ppc_en_secure_unpriv(enum periph_id_e periph_id)
{
    /* Clear Privilege access flag for peripheral */
    *periph_data[periph_id].addr &= ~(1U << (periph_data[periph_id].pos));
}

void ppc_clr_secure_unpriv(enum periph_id_e periph_id)
{
    /* Set Privilege access flag for peripheral */
    *periph_data[periph_id].addr |= 1U << (periph_data[periph_id].pos);
}
