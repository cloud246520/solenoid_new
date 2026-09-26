#include <io_control.h>


void single_channel_on(uint8_t num)
{
    switch (num)
    {
        case 0:
            io_control(0x00);
            break;
        case 1:
            io_control(0x01);
            break;
        case 2:
            io_control(0x02);
            break;
        case 3:
            io_control(0x04);
            break;
        case 4:
            io_control(0x08);
            break;
        default:
            break;
    }
}


void io_control(uint8_t cmd)
{
    HAL_GPIO_WritePin(IO_CH1_PORT, IO_CH1_PIN,
                      (cmd & 0x01U) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(IO_CH2_PORT, IO_CH2_PIN,
                      (cmd & 0x02U) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(IO_CH3_PORT, IO_CH3_PIN,
                      (cmd & 0x04U) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(IO_CH4_PORT, IO_CH4_PIN,
                      (cmd & 0x08U) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}
