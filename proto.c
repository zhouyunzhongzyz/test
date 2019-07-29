#include "proto.h"
#include <string.h>
#include <stdio.h>

static unsigned char CRC8(const unsigned char *data,int length)
{
    unsigned char crc = 0x00;
    unsigned char extract;
    unsigned char sum;
    for(int i = 0; i < length; i ++) {
        extract = *data;
        for (char tempI = 8; tempI; tempI --) {
            sum = (crc ^ extract) & 0x01;
            crc >>= 1;
            if (sum)
                crc ^= 0x8C;
            extract >>= 1;
        }
        data++;
    }
    return crc;
}

proto_pdu* pdu_buffer_create() {
    proto_pdu* buf = (proto_pdu*)malloc(MAX_PROTO_PDU);
    memset(buf, 0, MAX_PROTO_PDU);
    buf->preamble = PDU_PREAMBLE;
    return buf;
}

proto_pdu_cache* pdu_create() {
    proto_pdu* buff = (proto_pdu*)malloc(MAX_PROTO_PDU);
    memset(buff, 0, MAX_PROTO_PDU);
    buff->preamble = PDU_PREAMBLE;

    proto_pdu_cache* cache = (proto_pdu_cache*)malloc(sizeof(proto_pdu_cache));
    memset(cache, 0, sizeof(proto_pdu_cache));
    cache->pdu = buff;
    return cache;
}

void pdu_destroy(proto_pdu_cache* buff) {
    if(buff) {
        if(buff->pdu) {
            free(buff->pdu);
        }

        free(buff);
    }
}

bool pdu_check_crc(proto_pdu* buff) {
    if(buff == NULL) {
        return false;
    }

    if(buff->len > MAX_PROTO_PDU) {
        return false; 
    }

    /*the last appended char should be always CRC*/
    unsigned char crc = buff->data[buff->len - 1 - 1]; // 1 = cmd
    buff->data[buff->len - 1 - 1] = 0;
    unsigned char crcCalc = CRC8((unsigned char*)buff, buff->len + 2);
    buff->data[buff->len - 1 - 1] = crc;

    return (crc == crcCalc);
}

bool pdu_is_ready(proto_pdu_cache* cache) {
    if(!cache) {
        return false;
    }
    return cache->isReady;
}

void pdu_reset(proto_pdu_cache *cache) {
    if(cache == NULL) {
        return;
    }

    cache->bytesRead = 0;
    cache->bytesToRead = 0;
    cache->isReady = false;
}

int pdu_push_byte(proto_pdu_cache* cache, unsigned char cData) {
    if(cache == NULL) {
        return -1;
    }

    ++ (cache->bytesRead);
    if(cache->bytesRead == 1) {
        if(cData != PDU_PREAMBLE) {
            return -1;
        }
        cache->pdu->preamble = cData;
        return PDU_MORE_BYTES_REQUIRED;
    } else if(cache->bytesRead == 2) {
        cache->bytesToRead = cData + 1 + 1; // 2 = preamble + len
        cache->pdu->len = cData;
        return PDU_MORE_BYTES_REQUIRED;
    } else if(cache->bytesRead == 3) {
        cache->pdu->cmd = cData;
        return PDU_MORE_BYTES_REQUIRED;
    } else if(cache->bytesRead >= 4) {
        cache->pdu->data[cache->bytesRead - 4] = cData;
        if(cache->bytesRead >= cache->bytesToRead) {
            cache->isReady = true;
            return PDU_READY;
        } else {
            return PDU_MORE_BYTES_REQUIRED;
        }
    }

    return -1;
}


int pdu_fill_cmd(proto_pdu* pdu, unsigned char cmd, unsigned char *data, int len) {
    if(pdu == NULL) {
        return -1;
    }

    if(len > MAX_PROTO_PDU) {
        return -1;
    }

    pdu->len = len + 2; // cmd + CRC = 2 bytes
    pdu->cmd = cmd;
    pdu->preamble = PDU_PREAMBLE;
    memcpy(pdu->data, data, len);
    pdu->data[len] = 0;

    // calculate the crc, compiler will optimize 1+1+1+1 = 4
    unsigned char crc = CRC8((unsigned char*)pdu, len + 1 + 1 + 1 + 1); // 4 = preamble + len + cmd + crc

    pdu->data[len] = crc;
    return 0;
}

void test_pdu_module() {

    unsigned char demoCmdData[] = {0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8};
    unsigned char cmd = CMD_CmdForwardToThermal;

    for( int i = 0;i < sizeof(demoCmdData); ++i) {
        printf("%x ", demoCmdData[i]);
    }
    printf("\n");

    proto_pdu* pdu = pdu_buffer_create();
    pdu_fill_cmd(pdu, cmd, demoCmdData, sizeof(demoCmdData)/sizeof(char));

    bool ok = pdu_check_crc(pdu);
    printf("test CRC: %s\n", ok ? "successfully" : "failure");


    unsigned char* ptr = (unsigned char*)pdu;
    int len = sizeof(proto_pdu) + sizeof(demoCmdData);
    proto_pdu_cache* cache = pdu_create();
    int i = 0;
    for( i = 0; i < len; ++i) {
        int ret = pdu_push_byte(cache, ptr[i]);
        if(ret == -1) {
            printf("test pdu_push_byte() test failure");
            break;
        }
    }

    ok = pdu_is_ready(cache); // should be in ready state
    if(ok) {
        ok = (i == len);
        if(ok) {
            ok = (cache->pdu->len == pdu->len); // size should be the same
            if(ok ) {
                for( i = 0; i < pdu->len - 1; ++ i) { // - cmd byte
                    printf("cached: 0x%x, origin: 0x%x; \n", *(cache->pdu->data + i), *(pdu->data + i));
                }
                ok = pdu_check_crc(cache->pdu);  // crc should be validated.
            }
        }
    }

    printf("test pdu_push_byte(): %s\n",
            ok ? "Successfully" : "Failure");
}
