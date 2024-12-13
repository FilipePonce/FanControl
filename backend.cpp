#include "backend.h"

void Backend::Configure()
{
  loadSchedulerSettings("scheduler_settings.json");
  StartTimer();
  StartSonoffScheduler();
  StartAdditionalSonoffScheduler();

  std::string ip = "0.0.0.0";

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
    modbus_relay_->SetRelay(false, true, false, false);

    res.set_content(GetStatus(), "text/plain"); });

  svr->Get("/2", [&](const httplib::Request &req, httplib::Response &res)
           {
    last_speed_selection_ = 2;
    modbus_relay_->SetRelay(true, true, false, false);

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

svr->Get("/system/status", [&](const httplib::Request &req, httplib::Response &res) {
    try {
        // Execute the script to get CPU temperature
        std::string response = exec("/root/git/CheckTemp.sh");
        std::string cpuTempJson = ParseSensorInfo(response); // Parse CPU temperature into JSON format

        // Get current system time
        std::string systemTime = getCurrentSystemTime();

        // Create JSON response combining CPU temperature and system time
        std::string json = R"({"cpu_temp":)" + cpuTempJson + R"(,"system_time":")" + systemTime + R"("})";

        res.set_content(json, "application/json");
    } catch (const std::exception &e) {
        // Handle errors gracefully
        res.status = 500;
        res.set_content(R"({"status":"error","message":"Unable to retrieve system status."})", "application/json");
        std::cerr << "Error in /system/status: " << e.what() << std::endl;
    }
});

svr->Get(R"(/on_time/(\d{1,2}):(\d{2}))", [&](const httplib::Request &req, httplib::Response &res) {
    try {
        // Debugging: Check if matches are captured
        if (req.matches.size() < 3) {
            res.status = 400;
            res.set_content("{\"status\":\"error\",\"message\":\"Invalid request format.\"}", "application/json");
            return;
        }
        // Parse hour and minute from URL
        int new_on_hour = std::stoi(req.matches[1].str()); // .str() ensures correct conversion
        int new_on_minute = std::stoi(req.matches[2].str());
        // Validate hour and minute
        if (new_on_hour < 0 || new_on_hour > 23 || new_on_minute < 0 || new_on_minute > 59) {
            res.status = 400;
            res.set_content("{\"status\":\"error\",\"message\":\"Invalid time. Hour must be 0-23 and minute must be 0-59.\"}", "application/json");
            return;
        }
        // Update the schedule variables
        on_hour = new_on_hour;
        on_minute = new_on_minute;
        saveSchedulerSettings("scheduler_settings.json");
        // Debugging: Print values to console
        std::cout << "Updated on_hour: " << on_hour.load() << ", on_minute: " << on_minute.load() << std::endl;
        // Construct the response string
        std::string response = "{\"status\":\"success\",\"message\":\"on_time updated\",\"on_hour\":" +
                               std::to_string(on_hour.load()) + ",\"on_minute\":" +
                               std::to_string(on_minute.load()) + "}";
        // Send the response
        res.set_content(response, "application/json");
    } catch (const std::exception &e) {
        // Debugging: Print error to console
        std::cerr << "Error: " << e.what() << std::endl;

        res.status = 400;
        res.set_content("{\"status\":\"error\",\"message\":\"Invalid request.\"}", "application/json");
    }
});

svr->Get(R"(/custom_on_time/(\d{1,2}):(\d{2}))", [&](const httplib::Request &req, httplib::Response &res) {
    try {
        if (req.matches.size() < 3) {
            res.status = 400;
            res.set_content("{\"status\":\"error\",\"message\":\"Invalid request format.\"}", "application/json");
            return;
        }

        int new_on_hour = std::stoi(req.matches[1].str());
        int new_on_minute = std::stoi(req.matches[2].str());

        if (new_on_hour < 0 || new_on_hour > 23 || new_on_minute < 0 || new_on_minute > 59) {
            res.status = 400;
            res.set_content("{\"status\":\"error\",\"message\":\"Invalid time. Hour must be 0-23 and minute must be 0-59.\"}", "application/json");
            return;
        }

        custom_on_hour = new_on_hour;
        custom_on_minute = new_on_minute;
        saveSchedulerSettings("scheduler_settings.json");
        std::string response = "{\"status\":\"success\",\"message\":\"custom_on_time updated\",\"custom_on_hour\":" +
                               std::to_string(custom_on_hour.load()) + ",\"custom_on_minute\":" +
                               std::to_string(custom_on_minute.load()) + "}";
        res.set_content(response, "application/json");
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        res.status = 400;
        res.set_content("{\"status\":\"error\",\"message\":\"Invalid request.\"}", "application/json");
    }
});

svr->Get(R"(/off_time/(\d{1,2}):(\d{2}))", [&](const httplib::Request &req, httplib::Response &res) {
    try {
        // Debugging: Check if matches are captured
        if (req.matches.size() < 3) {
            res.status = 400;
            res.set_content("{\"status\":\"error\",\"message\":\"Invalid request format.\"}", "application/json");
            return;
        }

        // Parse hour and minute from URL
        int new_off_hour = std::stoi(req.matches[1].str()); // .str() ensures correct conversion
        int new_off_minute = std::stoi(req.matches[2].str());
        // Validate hour and minute
        if (new_off_hour < 0 || new_off_hour > 23 || new_off_minute < 0 || new_off_minute > 59) {
            res.status = 400;
            res.set_content("{\"status\":\"error\",\"message\":\"Invalid time. Hour must be 0-23 and minute must be 0-59.\"}", "application/json");
            return;
        }

        // Update the schedule variables
        off_hour = new_off_hour;
        off_minute = new_off_minute;
        saveSchedulerSettings("scheduler_settings.json");

        // Debugging: Print values to console
        std::cout << "Updated off_hour: " << off_hour.load() << ", off_minute: " << off_minute.load() << std::endl;

        // Construct the response string
        std::string response = "{\"status\":\"success\",\"message\":\"off_time updated\",\"off_hour\":" +
                               std::to_string(off_hour.load()) + ",\"off_minute\":" +
                               std::to_string(off_minute.load()) + "}";

        // Send the response
        res.set_content(response, "application/json");
    } catch (const std::exception &e) {
        // Debugging: Print error to console
        std::cerr << "Error: " << e.what() << std::endl;

        res.status = 400;
        res.set_content("{\"status\":\"error\",\"message\":\"Invalid request.\"}", "application/json");
    }
});

svr->Get(R"(/custom_off_time/(\d{1,2}):(\d{2}))", [&](const httplib::Request &req, httplib::Response &res) {
    try {
        if (req.matches.size() < 3) {
            res.status = 400;
            res.set_content("{\"status\":\"error\",\"message\":\"Invalid request format.\"}", "application/json");
            return;
        }

        int new_off_hour = std::stoi(req.matches[1].str());
        int new_off_minute = std::stoi(req.matches[2].str());

        if (new_off_hour < 0 || new_off_hour > 23 || new_off_minute < 0 || new_off_minute > 59) {
            res.status = 400;
            res.set_content("{\"status\":\"error\",\"message\":\"Invalid time. Hour must be 0-23 and minute must be 0-59.\"}", "application/json");
            return;
        }

        custom_off_hour = new_off_hour;
        custom_off_minute = new_off_minute;
        saveSchedulerSettings("scheduler_settings.json");

        std::string response = "{\"status\":\"success\",\"message\":\"custom_off_time updated\",\"custom_off_hour\":" +
                               std::to_string(custom_off_hour.load()) + ",\"custom_off_minute\":" +
                               std::to_string(custom_off_minute.load()) + "}";
        res.set_content(response, "application/json");
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        res.status = 400;
        res.set_content("{\"status\":\"error\",\"message\":\"Invalid request.\"}", "application/json");
    }
});


svr->Get("/current_times", [&](const httplib::Request &req, httplib::Response &res) {
    try {
        char formatted_on_time[6];
        char formatted_off_time[6];
        snprintf(formatted_on_time, sizeof(formatted_on_time), "%02d:%02d", on_hour.load(), on_minute.load());
        snprintf(formatted_off_time, sizeof(formatted_off_time), "%02d:%02d", off_hour.load(), off_minute.load());
        
        std::string response = "{\"on_time\":\"" + std::string(formatted_on_time) +
                               "\",\"off_time\":\"" + std::string(formatted_off_time) + "\"}";
        res.set_content(response, "application/json");
    } catch (const std::exception &e) {
        res.status = 500;
        res.set_content("{\"status\":\"error\",\"message\":\"Unable to fetch times.\"}", "application/json");
    }
});

svr->Get("/current_custom_times", [&](const httplib::Request &req, httplib::Response &res) {
    try {
        char formatted_on_time[6];
        char formatted_off_time[6];
        snprintf(formatted_on_time, sizeof(formatted_on_time), "%02d:%02d", custom_on_hour.load(), custom_on_minute.load());
        snprintf(formatted_off_time, sizeof(formatted_off_time), "%02d:%02d", custom_off_hour.load(), custom_off_minute.load());

        std::string response = "{\"custom_on_time\":\"" + std::string(formatted_on_time) +
                               "\",\"custom_off_time\":\"" + std::string(formatted_off_time) + "\"}";
        res.set_content(response, "application/json");
    } catch (const std::exception &e) {
        res.status = 500;
        res.set_content("{\"status\":\"error\",\"message\":\"Unable to fetch custom times.\"}", "application/json");
    }
});


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

// Function to remove all occurrences of a character from a string
std::string Backend::removeChar(const std::string& str, char charToRemove) {
    std::string result = str;
    result.erase(std::remove(result.begin(), result.end(), charToRemove), result.end());
    return result;
}

// Function to trim whitespace from both ends of a string
std::string Backend::trim(const std::string &str) {
    size_t start = str.find_first_not_of(" \t\n\r\f\v");
    size_t end = str.find_last_not_of(" \t\n\r\f\v");
    return (start == std::string::npos || end == std::string::npos) ? "" : str.substr(start, end - start + 1);
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

            // Trim any whitespace from the value
            value = trim(value);
            
            // Ensure the value is properly formatted for JSON
            std::string formatted_value;
            if (key == "htemp" || key == "otemp") {
                formatted_value = "\"" + value + "°C\"";
            } else {
                formatted_value = "\"" + value + "\"";
            }
            
            ss << "\"" << key << "\" : " << formatted_value << ", ";
        }
    }

    // Remove trailing comma and space
    std::string json_str = ss.str();
    if (!json_str.empty())
    {
        json_str = json_str.substr(0, json_str.size() - 2); // Remove last ", "
    }

    json_str += " }";

    // Remove single quotes from the JSON string
    json_str = removeChar(json_str, '\'');

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

std::string Backend::exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

std::string Backend::getCurrentSystemTime() {
    auto now = std::chrono::system_clock::now();
    auto current_time_t = std::chrono::system_clock::to_time_t(now);
    std::tm local_time = *std::localtime(&current_time_t);

    std::ostringstream oss;
    oss << std::put_time(&local_time, "%Y-%m-%d %H:%M:%S"); // Format: YYYY-MM-DD HH:MM:SS
    return oss.str();
}

void Backend::StartSonoffScheduler()
{
    scheduler_running = true;
    std::thread scheduler_thread([this]() { // Capture 'this' for member function access
        while (scheduler_running) {
            auto now = std::chrono::system_clock::now();
            auto current_time = std::chrono::system_clock::to_time_t(now);
            auto local_time = *std::localtime(&current_time);

            if (local_time.tm_hour == on_hour.load() && local_time.tm_min == on_minute.load()) {
                std::cout << "Turning lights ON";
                // Turn on Sonoff 1 and 2
                this->sendPostRequest("http://192.168.1.101:8081/zeroconf/switch",
                                      R"({"deviceid": "10011cc672", "data": {"switch": "on"}})");
                this->sendPostRequest("http://192.168.1.102:8081/zeroconf/switch",
                                      R"({"deviceid": "10011c493b", "data": {"switch": "on"}})");
                std::this_thread::sleep_for(std::chrono::minutes(1)); // Avoid re-triggering within the same minute
            }
            if (local_time.tm_hour == off_hour.load() && local_time.tm_min == off_minute.load()) {
                // Turn off Sonoff 1 and 2
                this->sendPostRequest("http://192.168.1.101:8081/zeroconf/switch",
                                      R"({"deviceid": "10011cc672", "data": {"switch": "off"}})");
                this->sendPostRequest("http://192.168.1.102:8081/zeroconf/switch",
                                      R"({"deviceid": "10011c493b", "data": {"switch": "off"}})");
                std::this_thread::sleep_for(std::chrono::minutes(1)); // Avoid re-triggering within the same minute
            }

            // Sleep for 30 seconds to avoid excessive CPU usage
            std::this_thread::sleep_for(std::chrono::seconds(30));
        }
    });

    scheduler_thread.detach(); // Detach the thread so it runs independently
}

void Backend::StopSonoffScheduler()
{
    scheduler_running = false;
}

void Backend::StartAdditionalSonoffScheduler() {
    additional_scheduler_running = true;
    std::thread additional_scheduler_thread([this]() { // Capture 'this' for member function access
        while (additional_scheduler_running) {
            auto now = std::chrono::system_clock::now();
            auto current_time = std::chrono::system_clock::to_time_t(now);
            auto local_time = *std::localtime(&current_time);

            // Custom logic for the 192.168.1.103 device
            if (local_time.tm_hour == custom_on_hour.load() && local_time.tm_min == custom_on_minute.load()) {
                std::cout << "Turning device at 192.168.1.103 ON\n";
                this->sendPostRequest("http://192.168.1.103:8081/zeroconf/switch",
                                      R"({"deviceid": "10011c1234", "data": {"switch": "on"}})");
                std::this_thread::sleep_for(std::chrono::minutes(1)); // Avoid re-triggering within the same minute
            }
            if (local_time.tm_hour == custom_off_hour.load() && local_time.tm_min == custom_off_minute.load()) {
                std::cout << "Turning device at 192.168.1.103 OFF\n";
                this->sendPostRequest("http://192.168.1.103:8081/zeroconf/switch",
                                      R"({"deviceid": "10011c1234", "data": {"switch": "off"}})");
                std::this_thread::sleep_for(std::chrono::minutes(1)); // Avoid re-triggering within the same minute
            }

            // Sleep for 30 seconds to avoid excessive CPU usage
            std::this_thread::sleep_for(std::chrono::seconds(30));
        }
    });

    additional_scheduler_thread.detach(); // Detach the thread so it runs independently
}

void Backend::StopAdditionalSonoffScheduler() {
    additional_scheduler_running = false;
}

void Backend::StopServer()
{
    saveSchedulerSettings("scheduler_settings.json");
    StopTimer();
    StopSonoffScheduler();
    StopAdditionalSonoffScheduler();
    if (svr) {
        delete svr;
        svr = nullptr;
    }
}

// Save scheduler settings to a file
void Backend::saveSchedulerSettings(const std::string &filename) {
    nlohmann::json json;

    // Populate JSON object with values
    json["scheduler_running"] = scheduler_running.load();
    json["on_hour"] = on_hour.load();
    json["on_minute"] = on_minute.load();
    json["off_hour"] = off_hour.load();
    json["off_minute"] = off_minute.load();

    json["additional_scheduler_running"] = additional_scheduler_running.load();
    json["custom_on_hour"] = custom_on_hour.load();
    json["custom_on_minute"] = custom_on_minute.load();
    json["custom_off_hour"] = custom_off_hour.load();
    json["custom_off_minute"] = custom_off_minute.load();

    // Write JSON to file
    std::ofstream file(filename);
    if (file.is_open()) {
        file << json.dump(4); // Pretty print with 4 spaces
        file.close();
    } else {
        std::cerr << "Failed to open file for saving scheduler settings: " << filename << std::endl;
    }
}

// Load scheduler settings from a file
void Backend::loadSchedulerSettings(const std::string &filename) {
    std::ifstream file(filename);
    if (file.is_open()) {
        nlohmann::json json;
        try {
            file >> json; // Parse JSON from file

            // Restore values from JSON
            scheduler_running = json.value("scheduler_running", false);
            on_hour = json.value("on_hour", 0);
            on_minute = json.value("on_minute", 0);
            off_hour = json.value("off_hour", 0);
            off_minute = json.value("off_minute", 0);

            additional_scheduler_running = json.value("additional_scheduler_running", false);
            custom_on_hour = json.value("custom_on_hour", 6);
            custom_on_minute = json.value("custom_on_minute", 0);
            custom_off_hour = json.value("custom_off_hour", 7);
            custom_off_minute = json.value("custom_off_minute", 0);
        } catch (const std::exception &e) {
        }
        file.close();
    } else {
        std::cerr << "Scheduler settings file not found. Using default values." << std::endl;
    }
}