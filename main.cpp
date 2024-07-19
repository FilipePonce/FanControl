#include "backend.h"
#include "modbus_relay.h"
#include "httplib.h"
#include <iostream>

int main()
{
  // Create instances of necessary classes
  ModbusRelay modbus_relay_;
  httplib::Server svr;

  // Configure and start the Modbus relay
  std::cout << "Configuring Modbus RTU" << std::endl;
  modbus_relay_.Configure();
  modbus_relay_.Step();

  // Configure and start the Backend
  Backend backend_(&modbus_relay_, &svr);
  std::cout << "Configuring Backend" << std::endl;
  backend_.Configure();

  // Set Fan speed to minimum
  modbus_relay_.SetRelay(true, true, false, false);

  return 0;
}
