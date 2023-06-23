#include "defines.h"

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");
WiFiManager wifiManager;

constexpr char WIFI_SSID[] = "SOFTITO";
constexpr char WIFI_PASSWORD[] = "Soft2022iTO";
constexpr char TOKEN[] = "Ikomv6yGxenrzHduhKMh";
constexpr char THINGSBOARD_SERVER[] = "thingsboard.cloud";
constexpr uint16_t THINGSBOARD_PORT = 1883U;     // MQTT port used to communicate with the server, 1883 is the default unencrypted MQTT port.
constexpr uint32_t MAX_MESSAGE_SIZE = 256U;      // Maximum size packets
constexpr uint32_t SERIAL_DEBUG_BAUD = 115200U;  // Baud rate

WiFiClient wifiClient;
ThingsBoard tb(wifiClient, MAX_MESSAGE_SIZE);  // Initialize ThingsBoard instance with the maximum needed buffer size
constexpr char BLINKING_INTERVAL_ATTR[] = "blinkingInterval";
constexpr char LED_MODE_ATTR[] = "ledMode";
constexpr char LED_STATE_ATTR[] = "ledState";

// Statuses for subscribing to rpc
bool subscribed = false;


// handle led state and mode changes
volatile bool attributesChanged = false;

// LED modes: 0 - continious state, 1 - blinking
volatile int ledMode = 0;

// Current led state
volatile bool ledState = false;

// Settings for interval in blinking mode
constexpr uint16_t BLINKING_INTERVAL_MS_MIN = 10U;
constexpr uint16_t BLINKING_INTERVAL_MS_MAX = 60000U;
volatile uint16_t blinkingInterval = 1000U;

uint32_t previousStateChange;

// For telemetry
constexpr int16_t telemetrySendInterval = 2000U;
uint32_t previousDataSend;

// List of shared attributes for subscribing to their updates
constexpr std::array<const char *, 2U> SHARED_ATTRIBUTES_LIST = {
  LED_STATE_ATTR,
  BLINKING_INTERVAL_ATTR
};

// List of client attributes for requesting them (Using to initialize device states)
constexpr std::array<const char *, 1U> CLIENT_ATTRIBUTES_LIST = {
  LED_MODE_ATTR
};


// will endlessly delay until a WiFi connection has been successfully established
void InitWiFi() {
  int i;

  i = 0;
  Serial.println("Connecting to AP ...");
  // Attempting to establish a connection to the given WiFi network
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);


  while (1) {
    // Delay 500ms until a connection has been succesfully established
    if (i > 20) {
      wifiManager.autoConnect();
      delay(11000);
      break;
    }
    delay(500);
    Serial.print(".");
    i++;
    if (WiFi.status() == WL_CONNECTED)
      break;
  }
  Serial.println("Connected to AP");
}

/// @brief Reconnects the WiFi uses InitWiFi if the connection has been removed
/// @return Returns true as soon as a connection has been established again
const bool reconnect() {
  const wl_status_t status = WiFi.status();  // Check to ensure we aren't connected yet
  if (status == WL_CONNECTED) {
    return true;
  }

  // If we aren't establish a new connection to the given WiFi network
  InitWiFi();
  return true;
}


/// @brief Processes function for RPC call "setLedMode"
/// RPC_Data is a JSON variant, that can be queried using operator[]
/// See https://arduinojson.org/v5/api/jsonvariant/subscript/ for more details
/// @param data Data containing the rpc data that was called and its current value
/// @return Response that should be sent to the cloud. Useful for getMethods
RPC_Response processSetLedMode(const RPC_Data &data) {
  Serial.println("Received the set led state RPC method");

  // Process data
  int new_mode = data;

  Serial.print("Mode to change: ");
  Serial.println(new_mode);

  if (new_mode != 0 && new_mode != 1) {
    return RPC_Response("error", "Unknown mode!");
  }

  ledMode = new_mode;

  attributesChanged = true;

  // Returning current mode
  return RPC_Response("newMode", (int)ledMode);
}


// Optional, keep subscribed shared attributes empty instead,
// and the callback will be called for every shared attribute changed on the device,
// instead of only the one that were entered instead
const std::array<RPC_Callback, 1U> callbacks = {
  RPC_Callback{ "setLedMode", processSetLedMode }
};


/// @brief Update callback that will be called as soon as one of the provided shared attributes changes value,
/// if none are provided we subscribe to any shared attribute change instead
/// @param data Data containing the shared attributes that were changed and their current value
void processSharedAttributes(const Shared_Attribute_Data &data) {
  for (auto it = data.begin(); it != data.end(); ++it) {
    if (strcmp(it->key().c_str(), BLINKING_INTERVAL_ATTR) == 0) {
      const uint16_t new_interval = it->value().as<uint16_t>();
      if (new_interval >= BLINKING_INTERVAL_MS_MIN && new_interval <= BLINKING_INTERVAL_MS_MAX) {
        blinkingInterval = new_interval;
        Serial.print("Updated blinking interval to: ");
        Serial.println(new_interval);
      }
    } else if (strcmp(it->key().c_str(), LED_STATE_ATTR) == 0) {
      ledState = it->value().as<bool>();
      digitalWrite(LED_BUILTIN, ledState ? HIGH : LOW);
      Serial.print("Updated state to: ");
      Serial.println(ledState);
    }
  }
  attributesChanged = true;
}

void processClientAttributes(const Shared_Attribute_Data &data) {
  for (auto it = data.begin(); it != data.end(); ++it) {
    if (strcmp(it->key().c_str(), LED_MODE_ATTR) == 0) {
      const uint16_t new_mode = it->value().as<uint16_t>();
      ledMode = new_mode;
    }
  }
}

const Shared_Attribute_Callback attributes_callback(SHARED_ATTRIBUTES_LIST.cbegin(), SHARED_ATTRIBUTES_LIST.cend(), &processSharedAttributes);
const Attribute_Request_Callback attribute_shared_request_callback(SHARED_ATTRIBUTES_LIST.cbegin(), SHARED_ATTRIBUTES_LIST.cend(), &processSharedAttributes);
const Attribute_Request_Callback attribute_client_request_callback(CLIENT_ATTRIBUTES_LIST.cbegin(), CLIENT_ATTRIBUTES_LIST.cend(), &processClientAttributes);

void thingsboard_setup() {
  InitWiFi();
  timeClient.begin();
  timeClient.setTimeOffset(10800);
}

void thingsboard_loop() {
  timeClient.update();
  time_t epochTime = timeClient.getEpochTime();
  String formattedTime = timeClient.getFormattedTime();
  int currentHour = timeClient.getHours();
  int currentMinute = timeClient.getMinutes();
  int currentSecond = timeClient.getSeconds();

  //Get a time structure
  struct tm *ptm = gmtime((time_t *)&epochTime);
  int monthDay = ptm->tm_mday;
  int currentMonth = ptm->tm_mon + 1;
  int currentYear = ptm->tm_year + 1900;
  //Print complete date:
  String currentDate = String(currentYear) + "-" + String(currentMonth) + "-" + String(monthDay);


  if (!reconnect()) {
    subscribed = false;
    return;
  }

  if (!tb.connected()) {
    subscribed = false;
    // Connect to the ThingsBoard
    Serial.print("Connecting to: ");
    Serial.print(THINGSBOARD_SERVER);
    Serial.print(" with token ");
    Serial.println(TOKEN);
    if (!tb.connect(THINGSBOARD_SERVER, TOKEN, THINGSBOARD_PORT)) {
      Serial.println("Failed to connect");
      return;
    }
    // Sending a MAC address as an attribute
    tb.sendAttributeString("macAddress", WiFi.macAddress().c_str());
  }

  if (!subscribed) {
    Serial.println("Subscribing for RPC...");
    // Perform a subscription. All consequent data processing will happen in
    // processSetLedState() and processSetLedMode() functions,
    // as denoted by callbacks array.
    if (!tb.RPC_Subscribe(callbacks.cbegin(), callbacks.cend())) {
      Serial.println("Failed to subscribe for RPC");
      return;
    }

    if (!tb.Shared_Attributes_Subscribe(attributes_callback)) {
      Serial.println("Failed to subscribe for shared attribute updates");
      return;
    }

    Serial.println("Subscribe done");
    subscribed = true;

    // Request current states of shared attributes
    if (!tb.Shared_Attributes_Request(attribute_shared_request_callback)) {
      Serial.println("Failed to request for shared attributes");
      return;
    }

    // Request current states of client attributes
    if (!tb.Client_Attributes_Request(attribute_client_request_callback)) {
      Serial.println("Failed to request for client attributes");
      return;
    }
  }
  // Sending telemetry every telemetrySendInterval time
  if (millis() - previousDataSend > telemetrySendInterval) {
    previousDataSend = millis();
    tb.sendTelemetryInt("rssi", WiFi.RSSI());
    tb.sendTelemetryInt("channel", WiFi.channel());
    tb.sendTelemetryString("bssid", WiFi.BSSIDstr().c_str());
    tb.sendTelemetryString("ssid", WiFi.SSID().c_str());
    tb.sendTelemetryString("LOCAL_IP", WiFi.localIP().toString().c_str());
    tb.sendTelemetryString("MAC", WiFi.macAddress().c_str());
    tb.sendTelemetryInt("Timestamp", timeClient.getEpochTime());
    tb.sendTelemetryString("Saat", timeClient.getFormattedTime().c_str());
    tb.sendTelemetryString("Tarih", String(currentDate).c_str());
    String nhm = get_web_hour_night() + ':' + get_web_minute_night();
    String mhm = get_web_hour_morning() + ':' + get_web_minute_morning();
    Serial.println(nhm);
    tb.sendTelemetryString("AksamSaati", String(nhm).c_str());
    tb.sendTelemetryString("SabahSaati", String(mhm).c_str());
    int full;

    full = get_fullness();
    if (full > 100)
        full = 100;
    else if (full < 0)
      full = 0;

    tb.sendTelemetryString("Miktar", String(full).c_str());
    //sendMamaMiktari(13);
    if (isNight() && currentHour == get_web_hour_night().toInt() && currentMinute == get_web_minute_night().toInt() == 1) {
      step_get(get_web_food_value().toInt());
      Serial.println("mama dokuldu");
      out_buzzer();
      delay(60000);
    }
    if (isMornign() && currentHour == get_web_hour_morning().toInt() && currentMinute == get_web_minute_morning().toInt() == 1) {
      step_get(get_web_food_value().toInt());
      Serial.println("mama dokuldu");
      out_buzzer();
      delay(60000);
    }

    if (foodCheck()) {
      step_get(get_web_food_value().toInt());
      Serial.println("mama dokuldu");
      out_buzzer();
      delay(60000);
    }
  }
  tb.loop();
}

void sendMamaMiktari(float DISTANCE) {
  tb.sendTelemetryInt("Mama Miktari", DISTANCE);
}
