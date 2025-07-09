# Bluetooth-stack-spp

**Bluetooth-stack-spp** 是一个开源的、面向教育用途的 Bluetooth Classic 协议栈。
 目前仅支持 **SPP（串口通信协议）**，主要用于帮助初学者理解蓝牙通信的核心原理，包括：

- 协议层次结构
- 数据包的分包与组包
- 通道的建立过程

------

## ✨ 功能特点

- ✅ 支持 SPP 通信
- ✅ 基于回调机制的简单架构
- ✅ 易于移植到其他平台

------

## 📱 测试环境

- **测试手机**：Google Pixel、小米系列
- **蓝牙控制器**：CSR8311

------

## ⚙️ 配置说明

### 📌 串口配置

请根据实际串口设备修改源码中的串口名称，例如：

```c
#define PHYBUSIF_NAME "/dev/ttyS3"
```

### 🧩 编译配置（`config.mk`）

可配置内容包括：

- 是否开启 HCI 日志
- 日志输出等级
- 是否开启彩色日志输出

------

## 🖥️ 开发环境

- 操作系统：WSL（Windows Subsystem for Linux）
- Linux 发行版：Ubuntu 22.04.2 LTS

------

## 🔧 编译与运行

```bash
./build.sh        # 编译项目
./build/main      # 运行协议栈
```

------

## 🧪 示例用法

在 Service 层注册回调函数，即可处理接收到的 SPP 数据并发送回去：

```c
void app_spp_data_handler(uint8_t * data, uint16_t len)
{
    TRACE_DEBUG("app spp data: %s", data);
    bt_hex_dump(data, len);

    spp_send_data(data, len);
}

void app_spp_init()
{
    spp_register_data_callback(app_spp_data_handler);
    TRACE_DEBUG("spp init done.");
}
```

下面是使用串口工具进行 SPP 测试的截图：

![](doc\spp_test02.jpg)