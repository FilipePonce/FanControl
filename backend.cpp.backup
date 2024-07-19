#include "backend.h"
#include "httplib.h"
#include <chrono>
#include <curl/curl.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <thread>

void Backend::Configure()
{
  StartTimer();
  std::string ip = "192.168.1.188";

  svr = new httplib::Server();

  svr->set_base_dir("./frontend");

  svr->Get("/", [&](const httplib::Request &req, httplib::Response &res)
           {
    try {
      std::string html = read_html("/index.html");
      res.set_content(html, "text/html");
    } catch (const std::exception &e) {
      res.status = 500;
      res.set_content(e.what(), "text/plain");
    } });

  svr->Get("/status", [&](const httplib::Request &req, httplib::Response &res)
           { res.set_content(GetStatus(), "text/plain"); });

  svr->Get("/Set", [&](const httplib::Request &req, httplib::Response &res)
           {
    last_speed_selection_ = 5;
    modbus_relay_->SetRelay(true, true, true, true);

    res.set_content(GetStatus(), "text/plain"); });

  svr->Get("/Reset", [&](const httplib::Request &req, httplib::Response &res)
           {
    last_speed_selection_ = 0;
    modbus_relay_->SetRelay(false, false, false, false);

    res.set_content(GetStatus(), "text/plain"); });

  svr->Get("/1", [&](const httplib::Request &req, httplib::Response &res)
           {
    last_speed_selection_ = 1;
    modbus_relay_->SetRelay(true, true, false, false);

    res.set_content(GetStatus(), "text/plain"); });

  svr->Get("/2", [&](const httplib::Request &req, httplib::Response &res)
           {
    last_speed_selection_ = 2;
    modbus_relay_->SetRelay(false, false, true, false);

    res.set_content(GetStatus(), "text/plain"); });

  svr->Get("/3", [&](const httplib::Request &req, httplib::Response &res)
           {
    last_speed_selection_ = 3;
    modbus_relay_->SetRelay(true, false, true, false);

    res.set_content(GetStatus(), "text/plain"); });

  svr->Get("/4", [&](const httplib::Request &req, httplib::Response &res)
           {
    last_speed_selection_ = 4;
    modbus_relay_->SetRelay(true, true, true, false);

    res.set_content(GetStatus(), "text/plain"); });

  svr->Get("/aircon/get_sensor_info",
           [&](const httplib::Request &req, httplib::Response &res)
           {
             std::string sensor_info = FetchAirconSensorInfo();
             res.set_content(sensor_info, "application/json");
           });

  svr->Get("/Sonoff01/Status", [&](const httplib::Request &req, httplib::Response &res)
           {
        std::string url = "http://192.168.1.101:8081/zeroconf/info";
        std::string json_data = R"({"deviceid": "10011cc672", "data": {"switch": "off"}})";
        std::string response = sendPostRequest(url, json_data);
        res.set_content(response, "application/json"); });

  svr->Get("/Sonoff01/1", [&](const httplib::Request &req, httplib::Response &res)
           {
        std::string url = "http://192.168.1.101:8081/zeroconf/switch";
        std::string json_data = R"({"deviceid": "10011cc672", "data": {"switch": "on"}})";
        std::string response = sendPostRequest(url, json_data);
        res.set_content(response, "application/json"); });

  svr->Get("/Sonoff01/0", [&](const httplib::Request &req, httplib::Response &res)
           {
        std::string url = "http://192.168.1.101:8081/zeroconf/switch";
        std::string json_data = R"({"deviceid": "10011cc672", "data": {"switch": "off"}})";
        std::string response = sendPostRequest(url, json_data);
        res.set_content(response, "application/json"); });

  svr->Get("/Sonoff02/Status", [&](const httplib::Request &req, httplib::Response &res)
           {
        std::string url = "http://192.168.1.102:8081/zeroconf/info";
        std::string json_data = R"({"deviceid": "10011c493b", "data": {"switch": "off"}})";
        std::string response = sendPostRequest(url, json_data);
        res.set_content(response, "application/json"); });

  svr->Get("/Sonoff02/1", [&](const httplib::Request &req, httplib::Response &res)
           {
        std::string url = "http://192.168.1.102:8081/zeroconf/switch";
        std::string json_data = R"({"deviceid": "10011c493b", "data": {"switch": "on"}})";
        std::string response = sendPostRequest(url, json_data);
        res.set_content(response, "application/json"); });

  svr->Get("/Sonoff02/0", [&](const httplib::Request &req, httplib::Response &res)
           {
        std::string url = "http://192.168.1.102:8081/zeroconf/switch";
        std::string json_data = R"({"deviceid": "10011c493b", "data": {"switch": "off"}})";
        std::string response = sendPostRequest(url, json_data);
        res.set_content(response, "application/json"); });

  svr->Get("/Sonoff03/Status", [&](const httplib::Request &req, httplib::Response &res)
           {
        std::string url = "http://192.168.1.103:8081/zeroconf/info";
        std::string json_data = R"({"deviceid": "10011b5af6", "data": {"switch": "off"}})";
        std::string response = sendPostRequest(url, json_data);
        res.set_content(response, "application/json"); });

  svr->Get("/Sonoff03/1", [&](const httplib::Request &req, httplib::Response &res)
           {
        std::string url = "http://192.168.1.103:8081/zeroconf/switch";
        std::string json_data = R"({"deviceid": "10011b5af6", "data": {"switch": "on"}})";
        std::string response = sendPostRequest(url, json_data);
        res.set_content(response, "application/json"); });

  svr->Get("/Sonoff03/0", [&](const httplib::Request &req, httplib::Response &res)
           {
        std::string url = "http://192.168.1.103:8081/zeroconf/switch";
        std::string json_data = R"({"deviceid": "10011b5af6", "data": {"switch": "off"}})";
        std::string response = sendPostRequest(url, json_data);
        res.set_content(response, "application/json"); });

  std::cout << "Starting http server at : http://" << ip.c_str() << ":80"
            << std::endl;

  // Listen on specified IP and port
  svr->listen(ip.c_str(), 80);

  UpdateStatus();
}

std::string Backend::GetStatus()
{
  std::stringstream ss;

  ss << "{ \"status\" : { "
     << "\"relay1\" : \"" << coils_[0] << "\", "
     << "\"relay2\" : \"" << coils_[1] << "\", "
     << "\"relay3\" : \"" << coils_[2] << "\", "
     << "\"relay4\" : \"" << coils_[3] << "\", "
     << "\"LastCommand\" : \"" << last_speed_selection_ << "\" } }";

  return ss.str();
}

std::string Backend::FetchAirconSensorInfo()
{
  CURL *curl;
  CURLcode res;
  std::string readBuffer;

  curl = curl_easy_init();
  if (curl)
  {
    curl_easy_setopt(curl, CURLOPT_URL,
                     "http://192.168.1.13/aircon/get_sensor_info");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK)
    {
      std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res)
                << std::endl;
      return "{}";
    }
  }

  // Parse the response into JSON format
  std::string json = ParseSensorInfo(readBuffer);

  return json;
}

std::string Backend::ParseSensorInfo(const std::string &sensor_info_str)
{
  std::stringstream ss;
  ss << "{ ";

  // Example response:
  // ret=OK,htemp=22.0,hhum=-,otemp=19.0,err=0,cmpfreq=0,mompow=1
  std::istringstream iss(sensor_info_str);
  std::string token;
  while (std::getline(iss, token, ','))
  {
    // Split key-value pairs by '='
    size_t pos = token.find('=');
    if (pos != std::string::npos)
    {
      std::string key = token.substr(0, pos);
      std::string value = token.substr(pos + 1);
      ss << "\"" << key << "\" : \"" << value << "\", ";
    }
  }

  // Remove trailing comma and space
  std::string json_str = ss.str();
  if (!json_str.empty())
  {
    json_str = json_str.substr(0, json_str.size() - 2); // Remove last ", "
  }

  json_str += " }";
  return json_str;
}

size_t Backend::WriteCallback(void *contents, size_t size, size_t nmemb,
                              std::string *s)
{
  size_t newLength = size * nmemb;
  s->append((char *)contents, newLength);
  return newLength;
}

std::string Backend::read_html(const std::string &path)
{
  std::ifstream file(path);
  if (!file.is_open())
  {
    throw std::runtime_error("Could not open file " + path);
  }
  std::stringstream buffer;
  buffer << file.rdbuf();
  return buffer.str();
}

void Backend::UpdateStatus()
{
  // std::cout << "Updating status" << std::endl;
  modbus_relay_->ReadCoils(coils_);
}

void Backend::StartTimer()
{
  running_ = true;
  timer_thread_ = std::thread([this]()
                              {
    while (running_) {
      std::this_thread::sleep_for(std::chrono::seconds(1));
      UpdateStatus();
    } });
}

void Backend::StopTimer()
{
  running_ = false;
  if (timer_thread_.joinable())
  {
    timer_thread_.join();
  }
}

std::string Backend::sendPostRequest(const std::string &url, const std::string &json_data)
{
  CURL *curl;
  CURLcode res;
  std::string readBuffer;

  curl_global_init(CURL_GLOBAL_DEFAULT);
  curl = curl_easy_init();
  if (curl)
  {
    struct curl_slist *headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/json");

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_data.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

    res = curl_easy_perform(curl);

    if (res != CURLE_OK)
    {
      std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
      readBuffer = R"({"error": "Failed to perform POST request"})";
    }

    curl_easy_cleanup(curl);
    curl_slist_free_all(headers);
  }
  else
  {
    readBuffer = R"({"error": "Failed to initialize CURL"})";
  }

  curl_global_cleanup();
  return readBuffer;
}
