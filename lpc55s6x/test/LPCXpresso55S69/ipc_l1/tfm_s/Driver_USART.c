/*
 * Copyright (c) 2013-2018 Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "fsl_usart.h"
#include "Driver_USART.h"

#define ARM_USART_DRV_VERSION    ARM_DRIVER_VERSION_MAJOR_MINOR(2, 0)  /* driver version */

/* USART Status */
static ARM_USART_STATUS UsartStatus;

/* USART Modem Status */
static ARM_USART_MODEM_STATUS UsartModemStatus;

/* Driver Version */
static const ARM_DRIVER_VERSION DriverVersion = { 
    ARM_USART_API_VERSION,
    ARM_USART_DRV_VERSION
};

/* Driver Capabilities */
static const ARM_USART_CAPABILITIES DriverCapabilities = {
    1, /* supports UART (Asynchronous) mode */
    0, /* supports Synchronous Master mode */
    0, /* supports Synchronous Slave mode */
    0, /* supports UART Single-wire mode */
    0, /* supports UART IrDA mode */
    0, /* supports UART Smart Card mode */
    0, /* Smart Card Clock generator available */
    0, /* RTS Flow Control available */
    0, /* CTS Flow Control available */
    0, /* Transmit completed event: \ref ARM_USART_EVENT_TX_COMPLETE */
    0, /* Signal receive character timeout event: \ref ARM_USART_EVENT_RX_TIMEOUT */
    0, /* RTS Line: 0=not available, 1=available */
    0, /* CTS Line: 0=not available, 1=available */
    0, /* DTR Line: 0=not available, 1=available */
    0, /* DSR Line: 0=not available, 1=available */
    0, /* DCD Line: 0=not available, 1=available */
    0, /* RI Line: 0=not available, 1=available */
    0, /* Signal CTS change event: \ref ARM_USART_EVENT_CTS */
    0, /* Signal DSR change event: \ref ARM_USART_EVENT_DSR */
    0, /* Signal DCD change event: \ref ARM_USART_EVENT_DCD */
    0  /* Signal RI change event: \ref ARM_USART_EVENT_RI */
};

//
//   Functions
//

ARM_DRIVER_VERSION ARM_USART_GetVersion(void)
{
    return DriverVersion;
}

ARM_USART_CAPABILITIES ARM_USART_GetCapabilities(void)
{
    return DriverCapabilities;
}

int32_t ARM_USART_Initialize(ARM_USART_SignalEvent_t cb_event)
{
    usart_config_t config;
  
    USART_GetDefaultConfig(&config);
    config.baudRate_Bps    = 115200U;
    config.parityMode      = kUSART_ParityDisabled;
    config.stopBitCount    = kUSART_OneStopBit;
    config.bitCountPerChar = kUSART_8BitsPerChar;
    config.enableRx        = false;
    config.enableTx        = true;

    USART_Init(USART0, &config, 12000000U);

    return ARM_DRIVER_OK;
}

int32_t ARM_USART_Uninitialize(void)
{
    USART_Deinit(USART0);
    return ARM_DRIVER_OK;
}

int32_t ARM_USART_PowerControl(ARM_POWER_STATE state)
{
    switch (state)
    {
    case ARM_POWER_OFF:
        break;

    case ARM_POWER_LOW:
        break;

    case ARM_POWER_FULL:
        break;

    default:
        return ARM_DRIVER_ERROR_UNSUPPORTED;
    }
    return ARM_DRIVER_OK;
}

int32_t ARM_USART_Send(const void *data, uint32_t num)
{
    USART_WriteBlocking(USART0, data, num);
    return ARM_DRIVER_OK;
}

int32_t ARM_USART_Receive(void *data, uint32_t num)
{
    return ARM_DRIVER_OK;
}

int32_t ARM_USART_Transfer(const void *data_out, void *data_in, uint32_t num)
{
    return ARM_DRIVER_OK;
}

uint32_t ARM_USART_GetTxCount(void)
{
    return 0U;
}

uint32_t ARM_USART_GetRxCount(void)
{
    return 0U;
}

int32_t ARM_USART_Control(uint32_t control, uint32_t arg)
{
    return ARM_DRIVER_OK;
}

ARM_USART_STATUS ARM_USART_GetStatus(void)
{
    return UsartStatus;
}

int32_t ARM_USART_SetModemControl(ARM_USART_MODEM_CONTROL control)
{
    return ARM_DRIVER_OK;
}

ARM_USART_MODEM_STATUS ARM_USART_GetModemStatus(void)
{
    return UsartModemStatus;
}

void ARM_USART_SignalEvent(uint32_t event)
{
    // function body
}

// End USART Interface

ARM_DRIVER_USART Driver_USART1 = {
    ARM_USART_GetVersion,
    ARM_USART_GetCapabilities,
    ARM_USART_Initialize,
    ARM_USART_Uninitialize,
    ARM_USART_PowerControl,
    ARM_USART_Send,
    ARM_USART_Receive,
    ARM_USART_Transfer,
    ARM_USART_GetTxCount,
    ARM_USART_GetRxCount,
    ARM_USART_Control,
    ARM_USART_GetStatus,
    ARM_USART_SetModemControl,
    ARM_USART_GetModemStatus
};
