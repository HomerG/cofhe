#define SRAM_BASE    0x20000000
#define FHEMEM0_BASE 0x21000000
#define FHEMEM1_BASE 0x22000000
#define FHEMEM2_BASE 0x23000000
#define FHEMEM3_BASE 0x24000000
#define FHEMEM4_BASE 0x25000000
#define FHEMEM5_BASE 0x26000000
#define FHEMEM6_BASE 0x27000000

#define GPCFG_BASE    0x42000000
#define GPIO_BASE     0x43000000
#define MULTPOOL_BASE 0x46000000


#define SRAM_ADDR0      SRAM_BASE   
#define FHEMEM0_ADDR0   FHEMEM0_BASE
#define FHEMEM1_ADDR0   FHEMEM1_BASE
#define FHEMEM2_ADDR0   FHEMEM2_BASE
#define FHEMEM3_ADDR0   FHEMEM3_BASE
#define FHEMEM4_ADDR0   FHEMEM4_BASE
#define FHEMEM5_ADDR0   FHEMEM5_BASE
#define FHEMEM6_ADDR0   FHEMEM6_BASE

#define GPCFG_UARTMTX_PAD_CTL   GPCFG_BASE + 0x00
#define GPCFG_UARTMRX_PAD_CTL   GPCFG_BASE + 0x04
#define GPCFG_UARTSTX_PAD_CTL   GPCFG_BASE + 0x08
#define GPCFG_UARTSRX_PAD_CTL   GPCFG_BASE + 0x0C
#define GPCFG_SPICLK_PAD_CTL    GPCFG_BASE + 0x10
#define GPCFG_SPICSN_PAD_CTL    GPCFG_BASE + 0x14
#define GPCFG_HOSTIRQ_PAD_CTL   GPCFG_BASE + 0x18
#define GPCFG_GPIO0_PAD_CTL     GPCFG_BASE + 0x1C
#define GPCFG_PAD11_PAD_CTL     GPCFG_BASE + 0x20
#define GPCFG_PAD12_PAD_CTL     GPCFG_BASE + 0x24
#define GPCFG_PAD13_PAD_CTL     GPCFG_BASE + 0x28
#define GPCFG_PAD14_PAD_CTL     GPCFG_BASE + 0x2c
#define GPCFG_PAD15_PAD_CTL     GPCFG_BASE + 0x30
#define GPCFG_PAD16_PAD_CTL     GPCFG_BASE + 0x34
#define GPCFG_PAD17_PAD_CTL     GPCFG_BASE + 0x38
#define GPCFG_PAD18_PAD_CTL     GPCFG_BASE + 0x3c
#define GPCFG_PAD19_PAD_CTL     GPCFG_BASE + 0x40

#define GPCFG_SFLL0_ADDR        GPCFG_BASE + 0xAC
#define GPCFG_SFLL1_ADDR        GPCFG_BASE + 0xB0
#define GPCFG_SFLL2_ADDR        GPCFG_BASE + 0xB4
#define GPCFG_SFLL3_ADDR        GPCFG_BASE + 0xB8

#define GPCFG_FLL0_ADDR         GPCFG_BASE + 0xBC
#define GPCFG_FLL1_ADDR         GPCFG_BASE + 0xC0
#define GPCFG_FLL2_ADDR         GPCFG_BASE + 0xC4
#define GPCFG_FLL3_ADDR         GPCFG_BASE + 0xC8


//Bits in GPCG_*PADCT
//#define  PADOEN      (1U << 0)      //0 = > Output enabled, 1 => HiZ,pull up or pull down depends on pull setting
//#define  PADRXEN     (1U << 1)      //0 = > Input disabled, 1 => Input Enabled
//#define  PADPULL0    (1U << 2)      //00 => HiZ, 01 => Pull Up, 10 => Pull down, 11 => Repeater
//#define  PADPULL1    (1U << 3)      //00 => HiZ, 01 => Pull Up, 10 => Pull down, 11 => Repeater
//#define  PADPULL2    (1U << 2)      //00 => HiZ, 01 => Pull Up, 10 => Pull down, 11 => Repeater
//#define  PADDRIVE0   (1U << 4)      //00 => 2mA, 01 => 4mA,     10 => 8mA,       11 => 12mA
//#define  PADDRIVE1   (1U << 4)      //00 => 2mA, 01 => 4mA,     10 => 8mA,       11 => 12mA
//#define  PADSCHMITT  (1U << 6)      //0  => Scmitt Disabled.       1 => Enabled
//#define  PADSLEW     (1U << 7)      //0  => Slow (half frequency). 1 => Fast
//#define  PADPOSCTL   (1U << 8)      //0  => PAD is in hig-Z due to loss of core power.
//                                   #//1  => PAD is pulled down. This happens when core supply is not there.
//#define  ALTFUNCSEL0  (1U << 16)   //
//#define  ALTFUNCSEL1  (1U << 17)   //
//#define  ALTFUNCGPIO  (1U << 24)   //




#define GPCFG_UARTM_BAUD_CTL    GPCFG_BASE + 0x44
#define GPCFG_UARTS_BAUD_CTL    GPCFG_BASE + 0x88

#define GPCFG_UARTM_CTL         GPCFG_BASE + 0x48
#define GPCFG_UARTS_CTL         GPCFG_BASE + 0x8c

//#define  UARTMDWIDTH0       (1U << 0)     //00 => 1 byte, 01 => 2 byte, 10 => 4 byte, 11 => 1 byte
//#define  UARTMDWIDTH1       (1U << 1)     //00 => 1 byte, 01 => 2 byte, 10 => 4 byte, 11 => 1 byte
//#define  UARTMPARTEN        (1U << 2)     //0  => No Parity,  1 => Parity Enabled
//#define  UARTMPARTYP        (1U << 3)     //0  => Odd parity, 1 => Even Parity

#define GPCFG_UARTS_TXDATA      GPCFG_BASE + 0x90
#define GPCFG_UARTS_RXDATA      GPCFG_BASE + 0x94
#define GPCFG_UARTS_TXSEND      GPCFG_BASE + 0x98

#define GPCFG_SPARE0            GPCFG_BASE + 0x9c
#define GPCFG_SPARE1            GPCFG_BASE + 0xa0
#define GPCFG_SPARE2            GPCFG_BASE + 0xa4
#define GPCFG_FHECTL2           GPCFG_BASE + 0xa8

#define GPCFG_FHECTL3          GPCFG_BASE + 0x800C
#define GPCFG_PADTESTOUT       GPCFG_BASE + 0x8010
#define GPCFG_PADTESTIN        GPCFG_BASE + 0x8014
#define GPCFG_COMMNDFIFO       GPCFG_BASE + 0x8018

//#//parameter CLEQCTL2_LOG2OFN       = 12'hFFF << 0;

#define GPCFG_SIGNATURE         GPCFG_BASE + 0x00cc

#define GPCFG_FHECTLP_ADDR      GPCFG_BASE + 0x8000
//#define  CLCTLP_ENMODMUL       (1U << 0)
//#define  CLCTLP_ENMODEXP       (1U << 1)
//#define  CLCTLP_ENEXTGCD       (1U << 2)
//#define  CLCTLP_ENGFUNC        (1U << 3)
//#define  CLCTLP_ENHWRNG        (1U << 4)
//#define  CLCTLP_FHERHIRQ        (1U << 8)


#define GPCFG_FHECTL_ADDR      GPCFG_BASE + 0x8004

#define GPCFG_FHESTATUS_ADDR   GPCFG_BASE + 0x8008

#define GPCFG_N_ADDR          = [GPCFG_BASE + 0x9000, GPCFG_BASE + 0x9004, GPCFG_BASE + 0x9008, GPCFG_BASE + 0x900C, GPCFG_BASE + 0x9010, GPCFG_BASE + 0x9014, GPCFG_BASE + 0x9018, GPCFG_BASE + 0x901C, GPCFG_BASE + 0x9020, GPCFG_BASE + 0x9024, GPCFG_BASE + 0x9028, GPCFG_BASE + 0x902C, GPCFG_BASE + 0x9030, GPCFG_BASE + 0x9034, GPCFG_BASE + 0x9038, GPCFG_BASE + 0x903C, GPCFG_BASE + 0x9040, GPCFG_BASE + 0x9044, GPCFG_BASE + 0x9048, GPCFG_BASE + 0x904C, GPCFG_BASE + 0x9050, GPCFG_BASE + 0x9054, GPCFG_BASE + 0x9058, GPCFG_BASE + 0x905C, GPCFG_BASE + 0x9060, GPCFG_BASE + 0x9064, GPCFG_BASE + 0x9068, GPCFG_BASE + 0x906C, GPCFG_BASE + 0x9070, GPCFG_BASE + 0x9074, GPCFG_BASE + 0x9078, GPCFG_BASE + 0x907C]

#define GPCFG_NINV_ADDR          = [GPCFG_BASE + 0x9080, GPCFG_BASE + 0x9084, GPCFG_BASE + 0x9088, GPCFG_BASE + 0x908C, GPCFG_BASE + 0x9090, GPCFG_BASE + 0x9094, GPCFG_BASE + 0x9098, GPCFG_BASE + 0x909C, GPCFG_BASE + 0x90A0, GPCFG_BASE + 0x90A4, GPCFG_BASE + 0x90A8, GPCFG_BASE + 0x90AC, GPCFG_BASE + 0x90B0, GPCFG_BASE + 0x90B4, GPCFG_BASE + 0x90B8, GPCFG_BASE + 0x90BC, GPCFG_BASE + 0x90C0, GPCFG_BASE + 0x90C4, GPCFG_BASE + 0x90C8, GPCFG_BASE + 0x90CC, GPCFG_BASE + 0x90D0, GPCFG_BASE + 0x90D4, GPCFG_BASE + 0x90D8, GPCFG_BASE + 0x90DC, GPCFG_BASE + 0x90E0, GPCFG_BASE + 0x90E4, GPCFG_BASE + 0x90E8, GPCFG_BASE + 0x90EC, GPCFG_BASE + 0x90F0, GPCFG_BASE + 0x90F4, GPCFG_BASE + 0x90F8, GPCFG_BASE + 0x90FC]


#define GPCFG_DBG_ADDR          = [GPCFG_BASE + 0x9B00, GPCFG_BASE + 0x9B04, GPCFG_BASE + 0x9B08, GPCFG_BASE + 0x9B0C, GPCFG_BASE + 0x9B10, GPCFG_BASE + 0x9B14, GPCFG_BASE + 0x9B18, GPCFG_BASE + 0x9B1C, GPCFG_BASE + 0x9B20, GPCFG_BASE + 0x9B24, GPCFG_BASE + 0x9B28, GPCFG_BASE + 0x9B2C, GPCFG_BASE + 0x9B30, GPCFG_BASE + 0x9B34, GPCFG_BASE + 0x9B38, GPCFG_BASE + 0x9B3C, GPCFG_BASE + 0x9B40, GPCFG_BASE + 0x9B44, GPCFG_BASE + 0x9B48, GPCFG_BASE + 0x9B4C, GPCFG_BASE + 0x9B50, GPCFG_BASE + 0x9B54, GPCFG_BASE + 0x9B58, GPCFG_BASE + 0x9B5C, GPCFG_BASE + 0x9B60, GPCFG_BASE + 0x9B64, GPCFG_BASE + 0x9B68, GPCFG_BASE + 0x9B6C, GPCFG_BASE + 0x9B70, GPCFG_BASE + 0x9B74, GPCFG_BASE + 0x9B78, GPCFG_BASE + 0x9B7C]

//#define GPIO_GPIO0_CTL         (*(unit32_t *) (GPIO_BASE + 0x0000))
//#define GPIO_GPIO0_OUT         (*(unit32_t *) (GPIO_BASE + 0x0004))
//#define GPIO_GPIO0_IN          (*(unit32_t *) (GPIO_BASE + 0x0008))
//#define GPIO_GPIO1_CTL         (*(unit32_t *) (GPIO_BASE + 0x000c))
//#define GPIO_GPIO1_OUT         (*(unit32_t *) (GPIO_BASE + 0x0010))
//#define GPIO_GPIO1_IN          (*(unit32_t *) (GPIO_BASE + 0x0014))
//#define GPIO_GPIO2_CTL         (*(unit32_t *) (GPIO_BASE + 0x0018))
//#define GPIO_GPIO2_OUT         (*(unit32_t *) (GPIO_BASE + 0x001c))
//#define GPIO_GPIO2_IN          (*(unit32_t *) (GPIO_BASE + 0x0020))
//#define GPIO_GPIO3_CTL         (*(unit32_t *) (GPIO_BASE + 0x0024))
//#define GPIO_GPIO3_OUT         (*(unit32_t *) (GPIO_BASE + 0x0028))
//#define GPIO_GPIO3_IN          (*(unit32_t *) (GPIO_BASE + 0x002c))

