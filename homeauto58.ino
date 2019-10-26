#include <UniversalTelegramBot.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <WiFiManager.h> 
#include <DNSServer.h>
//char ssid[] = "homenet";
//char password[] = "Redhat@12345";

// Initialize Telegram BOT
#define BOTtoken "941971545:AAH8U8PSnmWqBug6pXVO6P0tGaaEhkE18DA"
WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

//for LED status
#include <Ticker.h>
Ticker ticker;

void tick()
{
  //toggle state
  int state = digitalRead(BUILTIN_LED);  // get the current state of GPIO1 pin
  digitalWrite(BUILTIN_LED, !state);     // set pin to the opposite state
}

//gets called when WiFiManager enters configuration mode
void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());
  //if you used auto generated SSID, print it
  Serial.println(myWiFiManager->getConfigPortalSSID());
  //entered config mode, make led toggle faster
  ticker.attach(0.2, tick);
}

int Bot_mtbs = 50;
long Bot_lasttime;
bool Start = false;
String chat_id2;
bool flag = false;

void handleNewMessages(int numNewMessages) {
  Serial.println("handleNewMessages");
  Serial.println(String(numNewMessages));

  for (int i = 0; i < numNewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);
    chat_id2 = chat_id;
    String text = bot.messages[i].text;

    String from_name = bot.messages[i].from_name;
    if (from_name == "") from_name = "Guest";

    if (text == "/start") {
      String welcome = "Welcome to Smart Home, " + from_name + ".\n";
      welcome += "I am the SmartHomeBot \n";
      welcome += "Type /options if you want to see what I can do.\n";
      //welcome += "/send_test_action : to send test chat action message\n";
      bot.sendMessage(chat_id, welcome);
    }

    if (text == "/options") {
      String keyboardJson = "[[\"/Relay1-ON\", \"/Relay1-OFF\"],[\"/Light-ON\",\"/Light-OFF\"],[\"/Fan-ON\",\"/Fan-OFF\"],[\"/Relay4-ON\",\"/Relay4-OFF\"]]";
      bot.sendMessageWithReplyKeyboard(chat_id, "Select one of the following options:", "", keyboardJson, true);
    }

    if (text == "/Relay1-ON") {
      digitalWrite(D2, LOW);
      bot.sendMessage(chat_id, "Relay1 Switched on!");
    }

    if (text == "/Relay1-OFF") {
      digitalWrite(D2, HIGH);
      bot.sendMessage(chat_id, "Relay1 Switched off!");
    }
    
    if (text == "/Light-ON") {
      digitalWrite(D5, LOW);
      bot.sendMessage(chat_id, "Light Switched on!");
    }

    if (text == "/Light-OFF") {
      digitalWrite(D5, HIGH);
      bot.sendMessage(chat_id, "Light Switched off!");
    }

    
    if (text == "/Fan-ON") {
      digitalWrite(D6, LOW);
      bot.sendMessage(chat_id, "Fan Switched on!");
    }

    if (text == "/Fan-OFF") {
      digitalWrite(D6, HIGH);
      bot.sendMessage(chat_id, "Fan Switched off!");
    }

    
    if (text == "/Relay4-ON") {
      digitalWrite(D7, LOW);
      bot.sendMessage(chat_id, "Relay4 Switched on!");
    }

    if (text == "/Relay4-OFF") {
      digitalWrite(D7, HIGH);
      bot.sendMessage(chat_id, "Relay4 Switched off!");
    }

//    if (text == "/sensor") {
//      if (digitalRead(4) == 0) {
//        bot.sendMessage(chat_id, "Sleep Sensor!!");
//      } else {
//        bot.sendMessage(chat_id, "Sensor Activated !!");
//      }
//    }
  }
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  
  //set led pin as output
  pinMode(BUILTIN_LED, OUTPUT);
  // start ticker with 0.5 because we start in AP mode and try to connect
  ticker.attach(0.6, tick);

  //WiFiManager
  //Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;
  //reset settings - for testing
  //wifiManager.resetSettings();

  //set callback that gets called when connecting to previous WiFi fails, and enters Access Point mode
  wifiManager.setAPCallback(configModeCallback);

  if (!wifiManager.autoConnect("SmartHomeBot", "password*")) {
    Serial.println("failed to connect and hit timeout");
    //reset and try again, or maybe put it to deep sleep
    ESP.reset();
    delay(1000);
  }
  //if you get here you have connected to the WiFi
  Serial.println("connected...yeey :)");
  ticker.detach();
  //keep LED on
  digitalWrite(BUILTIN_LED, LOW);
  
  pinMode(D2, OUTPUT);
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);
  digitalWrite(D2, HIGH);
  digitalWrite(D5, HIGH);
  digitalWrite(D6, HIGH);
  digitalWrite(D7, HIGH);
//  pinMode(4, INPUT_PULLUP);
}

void loop() {
  if (millis() > Bot_lasttime + Bot_mtbs)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while (numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }

//    if (digitalRead(4) == 1) {
//      if (flag == false) {
//        bot.sendMessage(chat_id2, "Sensor is activated!!!");
//        flag = true;
//      }
//    } else {
//      flag = false;
//    }


    Bot_lasttime = millis();
  }
}
