#ifndef _RECIEVE_H_
#define _RECIEVE_H_

/* includes ----------------------------------------------------*/
#include <main.h>

/* defines -----------------------------------------------------*/

typedef struct
{
    uint32_t StdId;          // 标准 ID (11 位)
    uint32_t ExtId;          // 扩展 ID (29 位)
    uint32_t IDE;            // ID 类型 (CAN_ID_STD 或 CAN_ID_EXT)
    uint32_t RTR;            // 远程发送请求 (CAN_RTR_DATA 或 CAN_RTR_REMOTE)
    uint8_t  DLC;            // 数据长度码 (0-8)
    uint8_t  Data[8];        // 数据字节
} CAN_DataTypeDef;

extern CAN_DataTypeDef g_canReceivedData;

/* fuction -----------------------------------------------------*/
void reciever_init(CAN_HandleTypeDef *hcan);

#endif 
