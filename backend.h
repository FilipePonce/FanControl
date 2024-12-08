#pragma once

#include <chrono>
#include <curl/curl.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <thread>
#include <thread>
#include <algorithm>
#include <cctype>
#include <atomic>

#include "httplib.h"
#include "modbus_relay.h"
#include <nlohmann/json.hpp>

class Backend
{
public:
  Backend(ModbusRelay *modbus_relay, httplib::Server *server)
      : modbus_relay_(modbus_relay),
       svr(server),
       scheduler_running(true),
       on_hour(6),
       on_minute(0),
       off_hour(0),
       off_minute(0) {}

  ~Backend() { StopServer(); }
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
  std::string getCurrentSystemTime();

  void StartSonoffScheduler();
  void StopSonoffScheduler();
  void StartAdditionalSonoffScheduler();
  void StopAdditionalSonoffScheduler(); 
  void StopServer();
  void saveSchedulerSettings(const std::string &filename);
  void loadSchedulerSettings(const std::string &filename);

private:
  ModbusRelay *modbus_relay_;
  httplib::Server *svr;
  bool coils_[4]{};
  std::thread timer_thread_;
  bool running_;
  void StartTimer();
  void StopTimer();
  int last_speed_selection_ = 1;

  std::atomic<bool> scheduler_running;
  std::atomic<int> on_hour;
  std::atomic<int> on_minute;
  std::atomic<int> off_hour;
  std::atomic<int> off_minute;

  std::atomic<bool> additional_scheduler_running;
  std::atomic<int> custom_on_hour;    
  std::atomic<int> custom_on_minute; 
  std::atomic<int> custom_off_hour; 
  std::atomic<int> custom_off_minute;
};
