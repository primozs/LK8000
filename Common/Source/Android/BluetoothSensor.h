/*
 * LK8000 Tactical Flight Computer -  WWW.LK8000.IT
 * Released under GNU/GPL License v.2 or later
 * See CREDITS.TXT file for authors and copyrights
 *
 * File:   BluetoothSensor.h
 * Author: Bruno de Lacheisserie
 */

#ifndef ANDROID_BLUETOOTHSENSOR_H
#define ANDROID_BLUETOOTHSENSOR_H

#include "Thread/Mutex.hpp"
#include "ComPort.h"
#include "Device/Port/Listener.hpp"
#include "IO/DataHandler.hpp"

class PortBridge;

class BluetoothSensor : public ComPort, protected PortListener, DataHandler {
 public:
  using ComPort::ComPort;

  /* override ComPort */
 public:
  bool Initialize() override;
  bool Close() override;

  bool StopRxThread() override;
  bool StartRxThread() override;

  void Purge() override{};
  void Flush() override{};
  void CancelWaitEvent() override;

  bool IsReady() override;

  int SetRxTimeout(int TimeOut) override { return 0; }

  void UpdateStatus() override {}

  unsigned long SetBaudrate(unsigned long) override { return 0; };

  unsigned long GetBaudrate() const override { return 0; }

  size_t Read(void* data, size_t size) override { return 0; };

 protected:
  unsigned RxThread() override;

 private:
  bool Write_Impl(const void* data, size_t size) override;

  PortBridge* bridge = nullptr;

  Mutex mutex;
  Cond newdata;

  bool running = false;

  struct sensor_data {
    sensor_data(uuid_t s, uuid_t c, std::vector<uint8_t>&& _data) : service(s), characteristic(c), data(_data) {}

    uuid_t service;
    uuid_t characteristic;
    std::vector<uint8_t> data;
  };

  std::vector<sensor_data> data_queue;
  std::string device_name;

  void ProcessSensorData(const sensor_data& data);

 private:
  using handler_t = std::function<void(BluetoothSensor*, const std::vector<uint8_t>&)>;
  using enable_handler_t = std::function<bool(const BluetoothSensor*)>;

  struct characteristic_handler_t {
    handler_t handler;
    enable_handler_t enable_handler;
  };

  using characteristic_table_t = std::unordered_map<uuid_t, characteristic_handler_t, uuid_hash>;
  using service_table_t = std::unordered_map<uuid_t, characteristic_table_t, uuid_hash>;

  static
  const service_table_t& service_table();

 public:

  template <auto callback>
  bool Enable() const {
    ScopeLock lock(CritSec_Comm);
    auto port = devGetDeviceOnPort(GetPortIndex());
    return port && port->*callback;
  }

  template <auto callback, typename... Args>
  void OnSensorData(Args&& ...args) {
    ScopeLock lock(CritSec_Comm);
    auto port = devGetDeviceOnPort(GetPortIndex());
    if (port && port->*callback) {
      std::invoke(port->*callback, *port, GPS_INFO, std::forward<Args>(args)...);
    }
  }

  void DeviceName(const std::vector<uint8_t>& data);
  bool DeviceNameEnable() const;

  void BatteryLevel(const std::vector<uint8_t>& data);

  void HeartRateMeasurement(const std::vector<uint8_t>& data);

  void BarometricPressure(const std::vector<uint8_t>& data);
  void OutsideTemperature(const std::vector<uint8_t>& data);

  void Hm10Data(const std::vector<uint8_t>& data) {
    DataReceived(data.data(), data.size());
  }
  bool Hm10DataEnable() const;

  /* override PortListener */
 protected:
  void PortStateChanged() override;
  void PortError(const char* msg) override;

  /* override DataHandler */
 public:
  void DataReceived(const void* data, size_t size) override;
  void OnCharacteristicChanged(uuid_t service, uuid_t characteristic, const void* data, size_t size) override;
  bool DoEnableNotification(uuid_t service, uuid_t characteristic) const override;
};

#endif  // ANDROID_BLUETOOTHSENSOR_H