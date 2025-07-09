# Bluetooth-stack-spp

**Bluetooth-stack-spp** is an open-source educational Bluetooth Classic protocol stack.  
It currently supports only **Serial Port Profile (SPP)** communication and is designed to help beginners understand the principles of Bluetooth communication, including:

- Protocol layering
- Packet segmentation and reassembly
- Channel establishment

---

## ✨ Features

- ✅ SPP (Serial Port Profile) communication
- ✅ Simple callback-based architecture
- ✅ Easy to port to other platforms

---

## 📱 Test Environment

- **Mobile phones**: Google Pixel, Xiaomi devices
- **Bluetooth controller**: CSR8311

---

## ⚙️ Configuration

### 📌 Serial Port
Modify `PHYBUSIF_NAME` in the source code to match your serial device:

```c
#define PHYBUSIF_NAME "/dev/ttyS3"
```

### 🧩 Build Configuration

You can modify `config.mk` to:

- Enable or disable HCI log
- Set log level
- Enable or disable log color output

## 🖥️ Development Environment

- OS: WSL (Windows Subsystem for Linux)
- Distro: Ubuntu 22.04.2 LTS

## 🔧 Build & Run

```bash
./build.sh        # Build the project
./build/main      # Run the Bluetooth stack
```

## 🧪 Example Usage

To receive and send SPP data, register a callback in the service layer:

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

Here is a screenshot of SPP testing using a serial port tool:

![](doc\spp_test02.jpg)