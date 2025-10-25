#include "BluetoothSerial.h"
#include "esp_bt.h"
#include "esp_bt_main.h"
#include "esp_gap_bt_api.h"
#include "esp_bt_device.h"

BluetoothSerial SerialBT;
const int ledPin = 2;

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

  if (SerialBT.begin("ESP32_ROBUSTO"))
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
#include "esp_bt.h"
#include "esp_bt_main.h"
#include "esp_gap_bt_api.h"
#include "esp_bt_device.h"

BluetoothSerial SerialBT;
const int ledPin = 2;

bool setup_bluetooth()
{
  // Inicialização manual step-by-step
  Serial.println("  🔹 Desabilitando controlador...");
  esp_bt_controller_disable();
  esp_bt_controller_deinit();
  delay(2000);

  Serial.println("  🔹 Configurando controlador...");
  esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
  bt_cfg.mode = ESP_BT_MODE_CLASSIC_BT;

  if (esp_bt_controller_init(&bt_cfg) != ESP_OK)
  {
    Serial.println("  ❌ Falha na inicialização do controlador");
    return false;
  }

  Serial.println("  🔹 Habilitando modo clássico...");
  if (esp_bt_controller_enable(ESP_BT_MODE_CLASSIC_BT) != ESP_OK)
  {
    Serial.println("  ❌ Falha ao habilitar modo clássico");
    return false;
  }

  Serial.println("  🔹 Inicializando Bluedroid...");
  if (esp_bluedroid_init() != ESP_OK)
  {
    Serial.println("  ❌ Falha na inicialização do Bluedroid");
    return false;
  }

  Serial.println("  🔹 Habilitando Bluedroid...");
  if (esp_bluedroid_enable() != ESP_OK)
  {
    Serial.println("  ❌ Falha ao habilitar Bluedroid");
    return false;
  }

  Serial.println("  🔹 Iniciando SerialBT...");
  delay(1000);
  if (SerialBT.begin("ESP32_ROBUSTO"))
  {
    esp_bt_gap_set_scan_mode(ESP_BT_CONNECTABLE, ESP_BT_GENERAL_DISCOVERABLE);
    Serial.println("  🔹 Dispositivo configurado como descobrível");
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
    Serial.println("💡 LED ligado via comando");
  }
  else if (command == "LED_OFF")
  {
    digitalWrite(ledPin, LOW);
    SerialBT.println("LED_OFF:OK");
    Serial.println("💡 LED desligado via comando");
  }
  else if (command == "STATUS")
  {
    String response = "ONLINE:" + String(millis() / 1000) + "s:RAM:" + String(ESP.getFreeHeap() / 1024) + "KB";
    SerialBT.println(response);
    Serial.println("📊 Status enviado: " + response);
  }
  else if (command == "PING")
  {
    SerialBT.println("PONG");
    Serial.println("🏓 PING-PONG OK");
  }
  else if (command == "INFO")
  {
    SerialBT.println("ESP32_ROBUSTO v1.0");
    // SerialBT.println("MAC:" + SerialBT.getMacString());
    SerialBT.println("UPTIME:" + String(millis() / 1000) + "s");
    Serial.println("ℹ️ Informações enviadas");
  }
  else if (command == "RESET")
  {
    SerialBT.println("RESETTING...");
    Serial.println("🔄 Reiniciando ESP32...");
    delay(1000);
    ESP.restart();
  }
  else
  {
    SerialBT.println("ERROR:UNKNOWN_COMMAND");
    SerialBT.println("COMMANDS:LED_ON,LED_OFF,STATUS,PING,INFO,RESET");
    Serial.println("❓ Comando desconhecido: '" + command + "'");
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