#ifndef __PROTO_PDU___
#define __PROTO_PDU___

#include <stdlib.h>
#include <stdbool.h>

/*PDU stands for Protocol Data Unit*/
#define PDU_PREAMBLE            0xA7

#define CMD_CmdForwardToThermal 0x01
#define CMD_CmdForwardTo1080P   0x02

/**
 * |----------------------------------------------------------------------------------------------------------|
 * |  PREAMBLE(always 0xA7) |   sizeof(CMD + PAYLOAD + CRC)   |   CMD     |    PAYLOAD          |     CRC     |
 * |----------------------------------------------------------------------------------------------------------|
 * |   1 byte               |       1 byte                    |   1 byte  |    N bytes, N >= 0  |      1 byte |
 * |----------------------------------------------------------------------------------------------------------|
 */
#pragma pack(1)
typedef struct proto_pdu {
    unsigned char preamble; // 0xA7
    /**
     * len = sizeof(cmd + data), in bytes
     */
    unsigned char len;
    unsigned char cmd;
    /**
     * data := payload (0, 1 or 1+ bytes) + crc(1 bytes)
     * sizeof(data) >= 1, in case of the payload data is 0.
     */
    unsigned char data[1];
} proto_pdu;
#pragma pack()

/**
 * to STM32 developer:
 * there should be ONE proto_pdu_cache instance for each one of UART(UART1, UART2, ...). 
 * please note that the incoming data for each UART is seqencial, one-by-one byte stream
 */
typedef struct proto_pdu_cache_t
{
    int bytesToRead;
    int bytesRead;

    volatile bool isReady;
    proto_pdu *pdu;
} proto_pdu_cache;

/**
 * assume that the size of protocal data unit should NOT be greater than MAX_PROTO_PDU*/
#define MAX_PROTO_PDU 128

/**
 * for STM32 developers:
 *
 * ### from UART Interruption Request Handler
 * static proto_pdu_cache *cache_UART3 = pdu_create();
 * 
 * void uart3_IRQHandler(void) {
 *      ....
 *      // if the UART read ready
 *      if(pdu_is_ready(cache_UART3)) {
 *          // previously frame NOT processed, so waiting there.
 *          return;
 *      }
 * 
 *      unsigned char dat = readOneByteFromUART();
 * 
 *      int ret = pdu_push_byte(cache_UART3, dat);
 *      if(ret == -1) {
 *          pdu_reset(cache_UART3);
 *      }
 *      ...
 * }
 *
 *
 * ## from main loop
 * void main(void) {
 *      ....
 *      while(1) {
 *          ...
 *          if(pdu_is_ready(cache_UART3)) {
 *               // handle pdu
 *               ....
 *               pdu_reset(cache_UART3);
 *          }
 *          ...
 *      }
 * }
 */

proto_pdu* pdu_buffer_create();
proto_pdu_cache* pdu_create();
/**
 * to STM32 developer:
 * always do NOT call pdu_destroy for each UART port.
 */
void pdu_destroy(proto_pdu_cache* buff);

/*validate the pdu with CRC algorithm*/
bool pdu_check_crc(proto_pdu* buff);


#define PDU_READY               1
#define PDU_MORE_BYTES_REQUIRED 0

void    pdu_reset(proto_pdu_cache *cache);
bool    pdu_is_ready(proto_pdu_cache* cache);
/**
 * @return:
 *          PDU_MORE_BYTES_REQUIRED: more bytes required to complete this one of PDU.
 * 
 *          PDU_READY: all data of this PDU is now ready. you have got a distinct and whole PDU.
 *              it is now valid to call proto_pdu_cache->pdu to get the pdu buffer. 
 * 
 *          -1: bad arguments
 */
int     pdu_push_byte(proto_pdu_cache* pdu, unsigned char cData);

int     pdu_fill_cmd(proto_pdu* pdu, unsigned char cmd, unsigned char *data, int len);


void    test_pdu_module();

#endif
