#pragma once

#include <cstdint>

namespace e3
{

namespace cofhe
{

constexpr uint32_t bit2byte(uint32_t b) { return b>>3; }
constexpr uint32_t byte2bit(uint32_t b) { return b<<3; }
constexpr uint32_t power2  (uint32_t e) { return 1<<e; }
// #define BIT_TO_BYTE(b) (b>>3)
// #define BYTE_TO_BIT(b) (b<<3)

const uint32_t DEFAULT_FREQUENCY = 24000000; // hz

const uint32_t GPCFG_UARTSTX_PAD_CTL = 0x42000008;
const uint32_t GPCFG_HOSTIRQ_PAD_CTL = 0x42000018;
const uint32_t GPCFG_FHECTL2         = 0x420000a8;
const uint32_t GPCFG_FHECTL_ADDR     = 0x42008004;
const uint32_t GPCFG_COMMNDFIFO      = 0x42008018;
const uint32_t GPCFG_BASE            = 0x42000000;
const uint32_t GPCFG_DBG_ADDR[]      = { GPCFG_BASE | 0x9B00, GPCFG_BASE | 0x9B04, GPCFG_BASE | 0x9B08, GPCFG_BASE | 0x9B0C, GPCFG_BASE | 0x9B10, GPCFG_BASE | 0x9B14, GPCFG_BASE | 0x9B18, GPCFG_BASE | 0x9B1C, GPCFG_BASE | 0x9B20, GPCFG_BASE | 0x9B24, GPCFG_BASE | 0x9B28, GPCFG_BASE | 0x9B2C, GPCFG_BASE | 0x9B30, GPCFG_BASE | 0x9B34, GPCFG_BASE | 0x9B38, GPCFG_BASE | 0x9B3C, GPCFG_BASE | 0x9B40, GPCFG_BASE | 0x9B44, GPCFG_BASE | 0x9B48, GPCFG_BASE | 0x9B4C, GPCFG_BASE | 0x9B50, GPCFG_BASE | 0x9B54, GPCFG_BASE | 0x9B58, GPCFG_BASE | 0x9B5C, GPCFG_BASE | 0x9B60, GPCFG_BASE | 0x9B64, GPCFG_BASE | 0x9B68, GPCFG_BASE | 0x9B6C, GPCFG_BASE | 0x9B70, GPCFG_BASE | 0x9B74, GPCFG_BASE | 0x9B78, GPCFG_BASE | 0x9B7C };
const uint32_t GPCFG_N_ADDR[]        = { GPCFG_BASE | 0x9000, GPCFG_BASE| 0x9004, GPCFG_BASE| 0x9008, GPCFG_BASE| 0x900C, GPCFG_BASE| 0x9010, GPCFG_BASE| 0x9014, GPCFG_BASE| 0x9018, GPCFG_BASE| 0x901C, GPCFG_BASE| 0x9020, GPCFG_BASE| 0x9024, GPCFG_BASE| 0x9028, GPCFG_BASE| 0x902C, GPCFG_BASE| 0x9030, GPCFG_BASE| 0x9034, GPCFG_BASE| 0x9038, GPCFG_BASE| 0x903C, GPCFG_BASE| 0x9040, GPCFG_BASE| 0x9044, GPCFG_BASE| 0x9048, GPCFG_BASE| 0x904C, GPCFG_BASE| 0x9050, GPCFG_BASE| 0x9054, GPCFG_BASE| 0x9058, GPCFG_BASE| 0x905C, GPCFG_BASE| 0x9060, GPCFG_BASE| 0x9064, GPCFG_BASE| 0x9068, GPCFG_BASE| 0x906C, GPCFG_BASE| 0x9070, GPCFG_BASE| 0x9074, GPCFG_BASE| 0x9078, GPCFG_BASE| 0x907C };
const uint32_t GPCFG_NINV_ADDR[]     = { GPCFG_BASE | 0x9080, GPCFG_BASE| 0x9084, GPCFG_BASE| 0x9088, GPCFG_BASE| 0x908C, GPCFG_BASE| 0x9090, GPCFG_BASE| 0x9094, GPCFG_BASE| 0x9098, GPCFG_BASE| 0x909C, GPCFG_BASE| 0x90A0, GPCFG_BASE| 0x90A4, GPCFG_BASE| 0x90A8, GPCFG_BASE| 0x90AC, GPCFG_BASE| 0x90B0, GPCFG_BASE| 0x90B4, GPCFG_BASE| 0x90B8, GPCFG_BASE| 0x90BC, GPCFG_BASE| 0x90C0, GPCFG_BASE| 0x90C4, GPCFG_BASE| 0x90C8, GPCFG_BASE| 0x90CC, GPCFG_BASE| 0x90D0, GPCFG_BASE| 0x90D4, GPCFG_BASE| 0x90D8, GPCFG_BASE| 0x90DC, GPCFG_BASE| 0x90E0, GPCFG_BASE| 0x90E4, GPCFG_BASE| 0x90E8, GPCFG_BASE| 0x90EC, GPCFG_BASE| 0x90F0, GPCFG_BASE| 0x90F4, GPCFG_BASE| 0x90F8, GPCFG_BASE| 0x90FC };
const uint32_t GPCFG_NSQ_ADDR[]      = { GPCFG_BASE | 0x9100, GPCFG_BASE | 0x9104, GPCFG_BASE | 0x9108, GPCFG_BASE | 0x910C, GPCFG_BASE | 0x9110, GPCFG_BASE | 0x9114, GPCFG_BASE | 0x9118, GPCFG_BASE | 0x911C, GPCFG_BASE | 0x9120, GPCFG_BASE | 0x9124, GPCFG_BASE | 0x9128, GPCFG_BASE | 0x912C, GPCFG_BASE | 0x9130, GPCFG_BASE | 0x9134, GPCFG_BASE | 0x9138, GPCFG_BASE | 0x913C, GPCFG_BASE | 0x9140, GPCFG_BASE | 0x9144, GPCFG_BASE | 0x9148, GPCFG_BASE | 0x914C, GPCFG_BASE | 0x9150, GPCFG_BASE | 0x9154, GPCFG_BASE | 0x9158, GPCFG_BASE | 0x915C, GPCFG_BASE | 0x9160, GPCFG_BASE | 0x9164, GPCFG_BASE | 0x9168, GPCFG_BASE | 0x916C, GPCFG_BASE | 0x9170, GPCFG_BASE | 0x9174, GPCFG_BASE | 0x9178, GPCFG_BASE | 0x917C, GPCFG_BASE | 0x9180, GPCFG_BASE | 0x9184, GPCFG_BASE | 0x9188, GPCFG_BASE | 0x918C, GPCFG_BASE | 0x9190, GPCFG_BASE | 0x9194, GPCFG_BASE | 0x9198, GPCFG_BASE | 0x919C, GPCFG_BASE | 0x91A0, GPCFG_BASE | 0x91A4, GPCFG_BASE | 0x91A8, GPCFG_BASE | 0x91AC, GPCFG_BASE | 0x91B0, GPCFG_BASE | 0x91B4, GPCFG_BASE | 0x91B8, GPCFG_BASE | 0x91BC, GPCFG_BASE | 0x91C0, GPCFG_BASE | 0x91C4, GPCFG_BASE | 0x91C8, GPCFG_BASE | 0x91CC, GPCFG_BASE | 0x91D0, GPCFG_BASE | 0x91D4, GPCFG_BASE | 0x91D8, GPCFG_BASE | 0x91DC, GPCFG_BASE | 0x91E0, GPCFG_BASE | 0x91E4, GPCFG_BASE | 0x91E8, GPCFG_BASE | 0x91EC, GPCFG_BASE | 0x91F0, GPCFG_BASE | 0x91F4, GPCFG_BASE | 0x91F8, GPCFG_BASE | 0x91FC };

const uint32_t GPCFG_HOSTIRQ_PAD_CTL_CFG = 0x0012001A;
const uint32_t GPCFG_UARTSTX_PAD_CTL_CFG = 0x0010001A;

const uint32_t CMD_ADD               = 0x04000000;
const uint32_t CMD_CMUL              = 0x06000000;
const uint32_t CMD_DMA               = 0x07000000;
const uint32_t CMD_INTT              = 0x02000000;
const uint32_t CMD_MUL               = 0x03000000;
const uint32_t CMD_NTT               = 0x01000000;
const uint32_t CMD_READ              = 0x4d4d4d4d;
const uint32_t CMD_SET_N             = 0x00400000;
const uint32_t CMD_WRITE             = 0x34343434;

// masks
const uint32_t DUAL_PORT_MASK        = 0x80000000;
const uint32_t BYPASS_INTERRUPT_MASK = 0x40000000;

const uint32_t FHEMEM_BASE[]    = { 0x21000000, 0x22000000, 0x23000000, 0x24000000, 0x25000000, 0x26000000, 0x27000000,
                                    0x21010000, 0x22010000, 0x23010000, 0x24010000, 0x25010000, 0x26010000, 0x27010000 };
const uint32_t FHEMEM_DP_BASE[] = { 0x31000000, 0x32000000, 0x33000000 };

const uint32_t SIGNATURE_ADDRESS = 0x420000CC;
const uint8_t  SIGNATURE[] = { 0x02, 0x03, 0xC5, 0x0C };
const uint32_t BYTE     =  8;
const uint32_t WORDSIZE = 32;
const uint32_t BYTE_BYTES     = bit2byte(BYTE);
const uint32_t WORDSIZE_BYTES = bit2byte(WORDSIZE);

// DMA workaround
const uint32_t GPCFG73_ADDR       = GPCFG_BASE | 0x0124;
const uint32_t GPCFG74_ADDR       = GPCFG_BASE | 0x0128;
const uint32_t GPCFG_FHECTLP_ADDR = GPCFG_BASE | 0x8000;
const uint32_t CMD_DMA_WORKAROUND = 0x00000040;

// UART
const int DEFAULT_FREQ2UART_RATIO = 2500; // Default Frequency to UART ratio
const uint32_t GPCFG_UARTM_BAUD_CTL = GPCFG_BASE | 0x0044;
const uint32_t GPCFG_UARTS_BAUD_CTL = GPCFG_BASE | 0x0088;

} // cofhe

} // e3