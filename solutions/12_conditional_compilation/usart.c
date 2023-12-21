// usart.c
// See project README.md for disclaimer and additional information.
// Feabhas Ltd

#include <stdint.h>
#include "usart.h"
#include "usart_utils.h"
#include "timer.h"

// -----------------------------------------------------------------------------
// Bit field structures
//

typedef struct
{
    uint32_t SBK    : 1;
    uint32_t RWU    : 1;
    uint32_t RE     : 1;
    uint32_t TE     : 1;
    uint32_t IDLEIE : 1;
    uint32_t RXNEIE : 1;
    uint32_t TCIE   : 1;
    uint32_t TXEIE  : 1;
    uint32_t PEIE   : 1;
    uint32_t PS     : 1;
    uint32_t PCE    : 1;
    uint32_t WAKE   : 1;
    uint32_t M      : 1;
    uint32_t UE     : 1;
    uint32_t        : 0;
} Control1_t;


typedef struct
{
    uint32_t ADD    : 4;
    uint32_t        : 1;
    uint32_t LBDL   : 1;
    uint32_t LBDIE  : 1;
    uint32_t        : 1;
    uint32_t LBCL   : 1;
    uint32_t CPHA   : 1;
    uint32_t CPOL   : 1;
    uint32_t CLKEN  : 1;
    uint32_t STOP   : 2;
    uint32_t LINEN  : 1;
    uint32_t        : 0;
} Control2_t;


typedef union
{
    uint32_t tx;
    uint32_t rx;
} Data_t;


typedef struct
{
    uint32_t fraction : 4;
    uint32_t mantissa : 12;
} Baud_rate_t;


typedef struct
{
    uint32_t PE     : 1;
    uint32_t FE     : 1;
    uint32_t NE     : 1;
    uint32_t ORE    : 1;
    uint32_t IDLE   : 1;
    uint32_t RXNE   : 1;
    uint32_t TC     : 1;
    uint32_t TXE    : 1;
    uint32_t LBD    : 1;
    uint32_t CTS    : 1;
    uint32_t        : 0;
} Status_t;


// -----------------------------------------------------------------------------
// Configuration settings
//
typedef enum
{
  BAUD_2400   = 0x1A0B,
  BAUD_9600   = 0x0683,
  BAUD_57600  = 0x0116,
  BAUD_115200 = 0x008B
} Baud_rate;


typedef enum
{
  DATA_8_BITS,
  DATA_9_BITS
} Data_length;


typedef enum
{
  STOP_1_BIT,
  STOP_05_BIT,
  STOP_2_BIT,
  STOP_15_BIT
} Stop_bits;


typedef enum
{
  PARITY_DISABLE,
  PARITY_ENABLE
} Parity_enable;


typedef enum
{
  PARITY_EVEN,
  PARITY_ODD
} Parity;


// -----------------------------------------------------------------------------
// Hardware overlays
//
#define USART_BASE  (0x40004800)

static volatile Control1_t  * const ctrl_reg_1     = (Control1_t*) (USART_BASE + 0x0C);
static volatile Control2_t  * const ctrl_reg_2     = (Control2_t*) (USART_BASE + 0x10);
static volatile Data_t      * const data           = (Data_t*)     (USART_BASE + 0x04);
static volatile Baud_rate_t * const divisor        = (Baud_rate_t*)(USART_BASE + 0x08);
static volatile Status_t    * const status         = (Status_t*)   (USART_BASE + 0x00);


// -----------------------------------------------------------------------------
// Helper functions
//
static void enable_clock(void);
static void enable(void);
static void disable(void);
static void configure_frame(Data_length len, Stop_bits stop, Parity_enable parity);
static void configure_baud_rate(Baud_rate baud);

// -----------------------------------------------------------------------------
// ISR (weak linkage) functions
//
extern void USART3_IRQHandler(void);


// -----------------------------------------------------------------------------
//
void usart_init(void)
{
    enable_clock();
    disable();
    configure_frame(DATA_8_BITS, STOP_1_BIT, PARITY_DISABLE);
    configure_baud_rate(BAUD_115200);
    usart_enable_IO();
    enable();
    // enable hardware clock ticks for QEMU
    sleep(0);
}


void usart_send(uint8_t c)
{
    while (status->TXE == 0)
    {
        ; // Wait...
    }
    data->tx = (uint32_t)c;
}


void usart_send_string(const char* str)
{
    while (*str != '\0')
    {
        usart_send((uint8_t)*str);
        ++str;
    }
}


uint8_t usart_get(void)
{
    bool data_received;
    uint8_t  data;
    do
    {
        data_received = usart_try_get(&data);
    } while (!data_received);

    return data;
}


bool usart_try_get(uint8_t * const chr)
{
    if (status->RXNE != 0)
    {
        *chr = (uint8_t)(data->rx);
        return true;
    }
    else
    {
        return false;
    }
}


void enable_clock(void)
{
    volatile uint32_t * const RCC_enable = (uint32_t*)0x40023840;
    *RCC_enable |= (1 << 18);
}


void enable(void)
{
    ctrl_reg_1->RE = 1;
    ctrl_reg_1->TE = 1;
    ctrl_reg_1->UE = 1;
}


void disable(void)
{
    ctrl_reg_1->RE = 0;
    ctrl_reg_1->TE = 0;
    ctrl_reg_1->UE = 0;
}


void configure_frame(Data_length len, Stop_bits stop, Parity_enable parity)
{
    ctrl_reg_1->M    = len;
    ctrl_reg_1->PCE  = parity;
    ctrl_reg_2->STOP = stop;
}


void configure_baud_rate(Baud_rate baud)
{
    divisor->mantissa = (uint16_t)(baud >> 4) & 0xFFFu;
    divisor->fraction = (uint16_t)(baud & 0x0F);
}
