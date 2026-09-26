/*include--------------------------------------*/
#include <recieve.h>
#include <register.h>
#include <io_control.h>
#include <string.h>

/*define---------------------------------------*/
#define TARGET_ID 0x123 //给根据发送方控制帧自行更改
static uint8_t              canRxData[8];
static CAN_RxHeaderTypeDef canRxHeader;
CAN_DataTypeDef g_canReceivedData;


/*function-------------------------------------*/
void reciever_init(CAN_HandleTypeDef *hcan)
{
    /* ---- 配置滤波器：只接收 StdId = 0x123 的标准帧 ---- */
    CAN_FilterTypeDef sFilterConfig;

    sFilterConfig.FilterBank           = 0;
    sFilterConfig.FilterMode           = CAN_FILTERMODE_IDMASK;
    sFilterConfig.FilterScale          = CAN_FILTERSCALE_32BIT;
    sFilterConfig.FilterIdHigh         = (TARGET_ID << 5);   // 标准 ID 左移 5 位对齐
    sFilterConfig.FilterIdLow          = 0x0000;
    sFilterConfig.FilterMaskIdHigh     = 0x7FF << 5;                 // 掩码：精确匹配 11 位 ID
    sFilterConfig.FilterMaskIdLow      = 0x0000;
    sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
    sFilterConfig.FilterActivation     = ENABLE;
    sFilterConfig.SlaveStartFilterBank = 14;

    if (HAL_CAN_ConfigFilter(hcan, &sFilterConfig) != HAL_OK)
    {
        Error_Handler();
    }

    /* ---- 使能 RX FIFO0 消息挂起中断 ---- */
    if (HAL_CAN_ActivateNotification(hcan, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
    {
        Error_Handler();
    }

    /* ---- 启动 CAN 外设 ---- */
    if (HAL_CAN_Start(hcan) != HAL_OK)
    {
        Error_Handler();
    }
}


void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    /* 从 FIFO0 读取 CAN 消息 */
    if (HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &canRxHeader, canRxData) != HAL_OK)
    {
        return;  // 读取失败，直接返回
    }

    /* 填充全局数据结构 */
    g_canReceivedData.StdId = canRxHeader.StdId;
    g_canReceivedData.ExtId = canRxHeader.ExtId;
    g_canReceivedData.IDE   = canRxHeader.IDE;
    g_canReceivedData.RTR   = canRxHeader.RTR;
    g_canReceivedData.DLC   = canRxHeader.DLC;
    memcpy(g_canReceivedData.Data, canRxData, 8);

    /* ---- 解析数据并控制移位寄存器 ---- */
    /* 只处理目标 ID 的标准数据帧 */
    if (canRxHeader.StdId == TARGET_ID &&
        canRxHeader.IDE   == CAN_ID_STD &&
        canRxHeader.RTR   == CAN_RTR_DATA &&
        canRxHeader.DLC   >= 1)
    {
        // 取第一个字节的低 4 位控制 4 个通道
        // bit0 = CH1(QA), bit1 = CH2(QB), bit2 = CH3(QC), bit3 = CH4(QD)
        uint8_t controlValue = canRxData[0];
        //solenoid_update(controlValue);
        io_control(controlValue);
    }
}
