//抖音爆改车间主任光绘棒代码 v0.2 更新web图像管理
//微信公众号“爆改车间”获取主任更多代码
//关注抖音"爆改车间主任”观看主任最新视频
//代码大部分是JPEGDecoder库示例代码中拷贝
//适配esp8266开发板，esp-12f或e模块最优。
//NodeMUC开发板适用
//WiFi链接成功后，在同WiFi的电脑上浏览器输入开发板IP地址，即可打开图像管理页面
//图像格式JPG格式，尺寸128*多少都行，但长必须是128像素，因为灯带是128个灯珠的。

#include <FS.h>
#include <LittleFS.h>  
#include<FastLED.h> //ws2812灯带驱动库  项目-加载库-库管理-搜索安装
#include <time.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#ifndef STASSID
#define STASSID "********"  //输入要连接的WiFi名和密码
#define STAPSK  "********"
#endif

const char *ssid = STASSID;
const char *pass = STAPSK;
File uploadFile;

String Fname[255];
uint8_t filename_num,select_num,modeste;

long timezone = 2;
byte daysavetime = 1;

ESP8266WebServer server(80);

// JPEG decoder library
#include <JPEGDecoder.h>   //jpg图像解码库  项目-加载库-库管理-搜索安装
#include "OneButton.h"     //按键管理库 项目-加载库-库管理-搜索安装

#define PIN_INPUT_2 D2     //按键接口 单击切换图像，双击切换模式
#define PIN_INPUT_1 D1     //按键接口 单击开始刷图或者切换光剑颜色，双击光剑光效
OneButton button_2(PIN_INPUT_2, true);
OneButton button_1(PIN_INPUT_1, true);

ICACHE_RAM_ATTR void checkTicks_2()
{

  button_2.tick();
}

ICACHE_RAM_ATTR void checkTicks_1()
{

  button_1.tick();
}


#include <TFT_eSPI.h>      //TFT显示屏驱动库，注意要在库文件目录中修改显示屏驱动信息，本程序适配128*160分辨率显示屏。项目-加载库-库管理-搜索安装

TFT_eSPI tft = TFT_eSPI(); // Invoke custom library

#define N_PIXELS  128   //led灯珠数量
#define LED_PIN   6   //led灯带接口
#define LED_TYPE  WS2812
#define BRIGHTNESS  100     // led亮度
#define COLOR_ORDER GRB
CRGB leds[N_PIXELS];


//====================================================================================
//                                    Setup
//====================================================================================
void setup()
{
  
  Serial.begin(115200);
  // We start by connecting to a WiFi network
  pinMode(PIN_INPUT_1, INPUT_PULLUP);
  pinMode(PIN_INPUT_2, INPUT_PULLUP);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  

  WiFi.begin(ssid, pass);
  tft.begin();
  tft.setRotation(0);
  tft.setTextSize(1);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.drawString("WiFi connecting", 0, 0, 2);
  tft.drawString("Any key to Skip", 0, 15, 2);
  while (WiFi.status() != WL_CONNECTED) {    //链接WiFi
    delay(100);
    Serial.print(".");
    if(digitalRead(PIN_INPUT_1) == LOW or digitalRead(PIN_INPUT_2) == LOW )break;   //按任意键跳过WiFi链接
  }
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  tft.fillScreen(TFT_BLACK);
  tft.drawString("WiFi connected", 0, 0, 2);
  String ip ="IP: "+ String(WiFi.localIP()[0])+"."+String(WiFi.localIP()[1])+"."+String(WiFi.localIP()[2])+"."+String(WiFi.localIP()[3]);
  tft.drawString(ip, 0, 15, 2);
  delay(1000);
  Serial.println("Contacting Time Server");
  configTime(3600 * timezone, daysavetime * 3600, "time.nist.gov", "0.pool.ntp.org", "1.pool.ntp.org");
  struct tm tmstruct ;
  delay(2000);
  tmstruct.tm_year = 0;
  getLocalTime(&tmstruct, 5000);
  Serial.printf("\nNow is : %d-%02d-%02d %02d:%02d:%02d\n", (tmstruct.tm_year) + 1900, (tmstruct.tm_mon) + 1, tmstruct.tm_mday, tmstruct.tm_hour, tmstruct.tm_min, tmstruct.tm_sec);
  Serial.println("");
  //LittleFS.format(););
  Serial.println("Mount LittleFS");
  if (!LittleFS.begin()) {
    Serial.println("LittleFS mount failed");
    return;
  }
  
  delay(10);
  Serial.println("NodeMCU decoder test!");
  attachInterrupt(digitalPinToInterrupt(PIN_INPUT_1), checkTicks_1, CHANGE);
  button_1.attachClick(singleClick_1);
  button_1.attachDoubleClick(doubleClick_1);
  
  attachInterrupt(digitalPinToInterrupt(PIN_INPUT_2), checkTicks_2, CHANGE);
  button_2.attachClick(singleClick_2);
  button_2.attachDoubleClick(doubleClick_2);

  //webserver页面配置
  server.on("/", handleRoot);

  server.on("/postdel/", handledel);

  server.on("/postform/", HTTP_POST ,respondOK, handleForm);

  server.onNotFound(handleUserRequest);


  server.begin();//启动web服务
  Serial.println("HTTP server started");
  
  
  FastLED.addLeds<LED_TYPE,LED_PIN,COLOR_ORDER>(leds,N_PIXELS).setCorrection(TypicalLEDStrip);  //气动LED驱动
  FastLED.setBrightness(BRIGHTNESS);


  listDir("/");  //刷新图像文件列表

  returnImg();

  for(int i = 0;i<N_PIXELS;i++){
    leds[i] = CRGB(0,0,0);
  }
  FastLED.show();
}

//====================================================================================
//                                    Loop
//====================================================================================
void loop()
{
  button_1.tick();
  button_2.tick();
  server.handleClient();  

}
//====================================================================================
