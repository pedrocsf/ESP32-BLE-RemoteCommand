#include "BluetoothSerial.h"
#include "esp_bt.h"
#include "esp_bt_main.h"
#include "esp_gap_bt_api.h"
#include "esp_bt_device.h"

BluetoothSerial SerialBT;
const int ledPin = 2;

void checkLedStatus() {
  int ledStatus = digitalRead(ledPin);
  SerialBT.print("Estado do LED: ");
  SerialBT.println(ledStatus == HIGH ? "Ligado" : "Desligado" );
}

bool setup_bluetooth()
{
  esp_bt_controller_disable();
  esp_bt_controller_deinit();
  delay(1000);

  esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
  bt_cfg.mode = ESP_BT_MODE_CLASSIC_BT;

  if (esp_bt_controller_init(&bt_cfg) != ESP_OK)
  {
    return false;
  }

  if (esp_bt_controller_enable(ESP_BT_MODE_CLASSIC_BT) != ESP_OK)
  {
    return false;
  }

  if (esp_bluedroid_init() != ESP_OK)
  {
    return false;
  }

  if (esp_bluedroid_enable() != ESP_OK)
  {
    return false;
  }

  if (SerialBT.begin("ESP32"))
  {
    esp_bt_gap_set_scan_mode(ESP_BT_CONNECTABLE, ESP_BT_GENERAL_DISCOVERABLE);
    return true;
  }
  return false;
}

void processCommand(String command)
{
  if (command == "LED_ON")
  {
    digitalWrite(ledPin, HIGH);
    SerialBT.println("LED_ON:OK");
    Serial.println("LED ON");
  }
  else if (command == "LED_OFF")
  {
    digitalWrite(ledPin, LOW);
    SerialBT.println("LED_OFF:OK");
    Serial.println("LED OFF");
  }
  else if (command == "PING")
  {
    SerialBT.println("PONG");
    Serial.println("PING recebido");
  }
  else if (command == "STATUS"){
    checkLedStatus();
  }
  else
  {
    SerialBT.println("ERROR:UNKNOWN_COMMAND");
  }
}

void setup()
{
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);

  Serial.println("Inicializando Bluetooth...");

  if (setup_bluetooth())
  {
    Serial.println("Bluetooth iniciado com sucesso!");
    digitalWrite(ledPin, HIGH);
  }
  else
  {
    Serial.println("Falha ao inicializar Bluetooth");
    while (true)
    {
      digitalWrite(ledPin, !digitalRead(ledPin));
      delay(500);
    }
  }
}

void loop()
{
  if (SerialBT.available())
  {
    String command = SerialBT.readStringUntil('\n');
    command.trim();
    if (command.length() > 0)
    {
      processCommand(command);
    }
  }
  delay(100);
}