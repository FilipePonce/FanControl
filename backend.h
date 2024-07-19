#pragma once

#include <chrono>
#include <curl/curl.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <thread>

#include "httplib.h"
#include "modbus_relay.h"

class Backend
{
public:
  Backend(ModbusRelay *modbus_relay, httplib::Server *server)
      : modbus_relay_(modbus_relay), svr(server) {}
  ~Backend() { StopTimer(); }
  void Configure();
  void UpdateStatus();

  std::string GetStatus();
  std::string read_html(const std::string &path);
  std::string FetchAirconSensorInfo();
  static size_t WriteCallback(void *contents, size_t size, size_t nmemb,
                              std::string *s);

  std::string ParseSensorInfo(const std::string &sensor_info_str);
  std::string sendPostRequest(const std::string &url, const std::string &json_data);
  std::string exec(const char* cmd);
  std::string removeChar(const std::string& str, char charToRemove);
  std::string trim(const std::string &str);

private:
  ModbusRelay *modbus_relay_;
  httplib::Server *svr;
  bool coils_[4]{};
  std::thread timer_thread_;
  bool running_;
  void StartTimer();
  void StopTimer();
  int last_speed_selection_ = 1;
};
