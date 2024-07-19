#include "modbus_relay.h"
#include <cerrno>
#include <chrono>
#include <cstdint>
#include <stdio.h>
#include <thread>
#include <unistd.h>

ModbusRelay::ModbusRelay() : mb(nullptr) {}

ModbusRelay::~ModbusRelay() { CloseConnection(); }

void ModbusRelay::Configure()
{
  mb = modbus_new_rtu("/dev/ttyUSB0", 9600, 'N', 8, 1); // Open UART serial port
  if (mb == nullptr)
  {
    fprintf(stderr, "Unable to create modbus context\n");
    return;
  }

  if (modbus_set_slave(mb, 1) == -1)
  { // Set modbus slave ID
    fprintf(stderr, "Unable to set modbus slave: %s\n", modbus_strerror(errno));
    CloseConnection();
    return;
  }

  if (modbus_connect(mb) == -1)
  { // Connect to the modbus device
    fprintf(stderr, "Connection failed: %s\n", modbus_strerror(errno));
    CloseConnection();
    return;
  }

  // Setting up callbacks
  modbus_set_response_timeout(mb, 0, 3000000); // set timeout to 1 second
  modbus_set_byte_timeout(mb, 0, 3000000);     // set byte timeout to 1 second
  fprintf(stderr, "Connected to : %s\n", "/dev/ttyUSB0, 9600, 'N', 8, 1");
}

void ModbusRelay::Step() {}

void ModbusRelay::CloseConnection()
{
  if (mb)
  {
    modbus_close(mb); // Close modbus connection
    modbus_free(mb);  // Free modbus context
    mb = nullptr;
  }
}

void ModbusRelay::ReadCoils(bool *coil_status)
{
  // Read coil states from the modbus slave
  int rc = modbus_read_bits(mb, 0, 5, response);
  if (rc == -1)
  {
    // Print connection failure message
    fprintf(stderr, "Connection failed: %s\n", modbus_strerror(errno));
    return;
  }

  // Iterate over each response element
  for (int i = 0; i < 5; ++i)
  {
    // Convert uint8_t response to bool coil_status
    coil_status[i] = response[i] ? true : false;
  }
}

bool ModbusRelay::ReadCoil(int number)
{
  uint8_t coil_status[4];

  int rc = modbus_read_bits(mb, 0, 4, coil_status);
  if (rc == -1)
  {
    fprintf(stderr, "Connection failed: %s\n", modbus_strerror(errno));
  }
  else
  {
    return coil_status[number - 1];
  }
  return false;
}

void ModbusRelay::SetRelay(bool relay1, bool relay2, bool relay3, bool relay4)
{
  InternalSetRelay(false, false, false, false);
  std::this_thread::sleep_for(std::chrono::milliseconds(2000));
  InternalSetRelay(relay1, relay2, relay3, relay4);
}

void ModbusRelay::InternalSetRelay(bool relay1, bool relay2, bool relay3, bool relay4)
{
  auto check = 0;
  if (response[0] != relay1)
  {
    modbus_write_bit(mb, 0, relay1);
  }
  else
  {
    check++;
  }
  if (response[1] != relay2)
  {
    modbus_write_bit(mb, 1, relay2);
  }
  else
  {
    check++;
  }
  if (response[2] != relay3)
  {
    modbus_write_bit(mb, 2, relay3);
  }
  else
  {
    check++;
  }
  if (response[3] != relay4)
  {
    modbus_write_bit(mb, 3, relay4);
  }
  else
  {
    check++;
  }
  if (check < 4)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    InternalSetRelay(relay1, relay2, relay3, relay4);
  }
}
