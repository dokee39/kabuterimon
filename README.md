# kabuterimon
~ Sensorless BLDC Control ~

## 项目说明

- [![wakatime](https://wakatime.com/badge/user/bfcd022b-5701-445b-9c00-22ff2d260468/project/edf0ed10-47e5-485d-99bf-eead4d34ebdf.svg?style=flat-square)](https://wakatime.com/@bfcd022b-5701-445b-9c00-22ff2d260468/projects/wcmbaigibo)
- 代码仓库: https://github.com/dokee39/kabuterimon
- 立创硬件开源平台: 四合一电调 *TODO*
- 基于沁恒青稞 RISC-V 通用系列产品 CH32V203F8U6 的无刷电子调速器 (ESC)
- 2024 年全国大学生嵌入式芯片与系统设计竞赛
    - 芯片应用赛道
    - 南京沁恒微电子股份有限公司
    - 无刷电机控制及应用 

## 代码结构

本项目主控使用 `CH32V203F8U6`, 工具链为 `mounriver-studio-toolchain`, 使用 `Makefile` 进行项目构建, 使用 `openocd-wch-riscv` 与 `WCH-LinkE` 调试器进行代码烧录与调试. 代码仓库的结构如下: 

```
.
├── App        // 应用层
│   ├── bldc_callback.c   // BLDC 涉及到的中断回调函数
│   ├── bldc_ctrl.c       // 模式切换与控制值设置的轮询任务
│   ├── bldc_mos_ctrl.c   // MOS 管控制
│   ├── bldc_tim_cnt.c    // 定时器控制
│   ├── bldc_zero_cross.c // 过零检测
│   ├── led_task.c
│   └── inc
│       └── ...
├── Components // 组件
│   ├── support    // 常用数学工具
│   │   ├── user_lib.c
│   │   └── inc
│   │       └── ...
│   └── system     // 基于侵入式链表实现的时间片任务轮询
│       ├── list.c
│       ├── timeslice_task.c
│       └── inc
│           └── ...
├── Core       // 初始化与中断回调
│   ├── ch32v20x_it.c
│   ├── main.c
│   ├── system_ch32v20x.c
│   └── inc
│       ├── ch32v20x_conf.h
│       ├── ch32v20x_it.h
│       ├── main.h
│       └── system_ch32v20x.h
├── Driver     // 驱动
│   ├── CMSIS
│   │   └── ...
│   └── Peripheral
│       └── ...
├── compile_commands.json
├── Link.ld
├── Makefile
├── README.md
└── startup_ch32v20x_D6.s
```



