#pragma once

#include <modbus/modbus.h>

class ModbusRelay
{
public:
  ModbusRelay();
  ~ModbusRelay();

  void Configure();
  void Step();
  void ReadCoils(bool *coil_status);
  void SetRelay(bool relay1, bool relay2, bool relay3, bool relay4);
  bool ReadCoil(int number);

protected:
  void CloseConnection();
  void InternalSetRelay(bool relay1, bool relay2, bool relay3, bool relay4);

private:
  modbus_t *mb;
  uint8_t response[4];
};