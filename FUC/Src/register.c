/*include-------------------------------------------*/
#include <register.h>

/*define--------------------------------------------*/
// 当前寄存器值
static uint8_t registerValue = 0x00;

// 宏定义：SI 引脚操作
#define SI_HIGH()   HAL_GPIO_WritePin(SI_GPIO_PORT, SI_PIN, GPIO_PIN_SET)
#define SI_LOW()    HAL_GPIO_WritePin(SI_GPIO_PORT, SI_PIN, GPIO_PIN_RESET)

// 宏定义：SCK 引脚操作
#define SCK_HIGH()  HAL_GPIO_WritePin(SCK_GPIO_PORT, SCK_PIN, GPIO_PIN_SET)
#define SCK_LOW()   HAL_GPIO_WritePin(SCK_GPIO_PORT, SCK_PIN, GPIO_PIN_RESET)

// 宏定义：RCK 引脚操作
#define RCK_HIGH()  HAL_GPIO_WritePin(RCK_GPIO_PORT, RCK_PIN, GPIO_PIN_SET)
#define RCK_LOW()   HAL_GPIO_WritePin(RCK_GPIO_PORT, RCK_PIN, GPIO_PIN_RESET)




/*function----------------------------------------*/
//初始化移位寄存器控制引脚
void register_init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    // 使能 GPIO 时钟
    __HAL_RCC_GPIOA_CLK_ENABLE();
    
    // 配置 SI, SCK, RCK 引脚为推挽输出
    GPIO_InitStruct.Pin = SI_PIN | SCK_PIN | RCK_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(SI_GPIO_PORT, &GPIO_InitStruct);
    
    // 初始化所有引脚为低电平
    SI_LOW();
    SCK_LOW();
    RCK_LOW();
    
    // 清除寄存器值
    registerValue = 0x00;
}


//发送一个字节到移位寄存器
void Register_SendByte(uint8_t data)
{
    uint8_t i;
    
    // 8 位数据，从最高位开始发送
    for(i = 0; i < 8; i++)
    {
        // 设置 SI 引脚
        if(data & 0x80)
        {
            SI_HIGH();
        }
        else
        {
            SI_LOW();
        }
        
        // 左移一位，准备发送下一位
        data <<= 1;
        
        // SCK 上升沿，数据移入寄存器
        SCK_HIGH();
        SCK_LOW();
    }
}

//更新移位寄存器输出 (RCK 上升沿)
void Register_Update(void)
{
    // RCK 上升沿，将移位寄存器数据锁存到输出
    RCK_HIGH();
    RCK_LOW();
}

// 根据 CAN 数据设置输出
void solenoid_update (uint8_t controlValue)
{
    // 只取低 4 位 (0-15)
    registerValue = controlValue & 0x0F;
    
    // 发送数据到移位寄存器
    Register_SendByte(registerValue);
    Register_Update();
}

// 全部拉低
void register_clear (void)
{
    registerValue = 0x00;
    Register_SendByte(registerValue);
    Register_Update();
}
