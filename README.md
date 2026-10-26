# CAN 控制电磁阀 四路输出控制使用说明  

## 1. 项目简介

本工程基于 STM32F0 微控制器，主要功能是通过 CAN 总线接收控制命令，并将命令解析为 4 路输出状态，用于控制电磁阀、继电器、驱动器或其他数字开关设备。

当前项目代码中，CAN 接收逻辑在 [FUC/Src/recieve.c](FUC/Src/recieve.c)，输出控制逻辑在 [FUC/Src/io_control.c](FUC/Src/io_control.c)，GPIO 定义在 [FUC/Inc/io_control.h](FUC/Inc/io_control.h)。

工程实际运行方式如下：

- 设备上电后初始化 CAN、GPIO ；
- 监听指定 CAN 标准帧；
- 当接收到目标 ID 的 CAN 数据帧时，读取数据字节；
- 取第 1 个数据字节的低 4 位，分别控制 4 个输出通道；
- 对应输出口高低电平变化，从而控制外部负载。

---

## 2. 硬件接口说明（具体引脚配置见.ioc文件，注意can时间片的配置）

### 2.1 输出通道定义

项目中 4 路输出对应关系如下：

| 通道 | GPIO | 说明 |
| --- | --- | --- |
| CH1 | PA11 | 输出通道 1 |
| CH2 | PA12 | 输出通道 2 |
| CH3 | PB6 | 输出通道 3 |
| CH4 | PB7 | 输出通道 4 |

定义文件：

- [FUC/Inc/io_control.h](FUC/Inc/io_control.h)
- [FUC/Src/io_control.c](FUC/Src/io_control.c)

### 2.2 额外寄存器控制接口（不再使用）

工程里还保留了移位寄存器控制代码，相关定义在：

- [FUC/Inc/register.h](FUC/Inc/register.h)
- [FUC/Src/register.c](FUC/Src/register.c)

其中：

- SI -> PA2
- SCK -> PA0
- RCK -> PA1

这部分代码用于控制 8 位移位寄存器，但当前主程序实际控制逻辑中，已使用更直接的 GPIO 输出控制方式。也就是说，当前工程默认以 4 路 GPIO 输出。

---

## 3. CAN 通信协议

### 3.1 接收 ID

默认接收目标 CAN ID 为：

- 0x123  //可根据自己需求更改 CAN ID

该值定义在 [FUC/Src/recieve.c](FUC/Src/recieve.c) 中的 TARGET_ID 宏。若需要更改控制设备的 CAN 标识，修改该参数即可。

### 3.2 CAN 帧要求

当前代码只处理：

- 标准帧（CAN_ID_STD）
- 数据帧（CAN_RTR_DATA）
- CAN ID 等于 0x123
- 数据长度 DLC >= 1

### 3.3 数据格式

当收到目标帧后，程序读取第一个数据字节：

- Data[0] 作为控制字节
- 低 4 位用于控制 4 个通道
- bit0 控制 CH1
- bit1 控制 CH2
- bit2 控制 CH3
- bit3 控制 CH4

对应关系：1=开启，0=关闭

---

## 4. 程序启动流程

程序入口在 [Core/Src/main.c](Core/Src/main.c)。

主循环本身没有额外业务处理，核心逻辑都在 CAN 中断回调中完成：

- HAL_CAN_RxFifo0MsgPendingCallback

在该回调函数中，代码会读取接收缓冲区、保存帧信息，并执行输出控制。

---

## 5. 调试建议

### 5.1 常见问题

- 无法接收到 CAN 帧：
  - 检查 CAN 波特率是否匹配；
  - 检查 ID 是否为 0x123；
  - 检查过滤器配置是否正确；
  - 检查 CAN 收发器和 MCU 连接是否正常。

- 输出不正常：
  - 检查目标通道 GPIO 是否接错；
  - 检查外部驱动负载是否超出能力；
  - 检查 Data[0] 控制值是否正确；
  - 检查程序是否进入 HAL_CAN_RxFifo0MsgPendingCallback。

- 程序不启动：
  - 检查系统时钟配置；
  - 检查芯片型号是否正确；
  - 检查编译器/工程参数是否与芯片匹配。

---

## 6. 注意事项

1. 当前代码中默认控制码只针对低 4 位有效，超过 0x0F 的高位会被忽略；
2. 如果更换 CAN 标识，需要同步修改 TARGET_ID；
3. 项目中默认使用标准帧，不支持扩展帧控制；

