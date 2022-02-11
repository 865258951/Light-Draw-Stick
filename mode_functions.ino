//模式切换和显示控制按钮设置部分
void mode_0(){

      tft.fillScreen(TFT_BLACK);
      for(int i = 0;i<N_PIXELS;i++){
        leds[i] = CRGB(0,0,0);
      }
      FastLED.show();
      String s = Fname[select_num];
      Serial.print("filenamenum: ");
      Serial.println(select_num-1);
      jpegLEDrow(s);
      
      for(int i = 0;i<N_PIXELS;i++){
        leds[i] = CRGB(0,0,0);
      }
      FastLED.show();
      Serial.println("jpegLEDrow");

}

uint8_t mode_1_0_ste;
void mode_1_0(){
  uint8_t t = 1;
  switch(mode_1_0_ste){
    case 0:
      for(int i = 0;i<N_PIXELS;i++){
        leds[i] = CRGB(255,0,0);
        tft.fillRect(60,0,8,120,tft.color565(255,0,0)); 
        FastLED.show();
        delay(t);
      }
    break;
    case 1:
      for(int i = 0;i<N_PIXELS;i++){
        leds[i] = CRGB(0,255,0);
        tft.fillRect(60,0,8,120,tft.color565(0,255,0)); 
        FastLED.show();
        delay(t);
      }
    break;
    case 2:
      for(int i = 0;i<N_PIXELS;i++){
        leds[i] = CRGB(0,0,255);
        tft.fillRect(60,0,8,120,tft.color565(0,0,255)); 
        FastLED.show();
        delay(t);
      }
    break;
    case 3:
      for(int i = 0;i<N_PIXELS;i++){
        leds[i] = CRGB(255,255,0);
        tft.fillRect(60,0,8,120,tft.color565(255,255,0)); 
        FastLED.show();
        delay(t);
      }
    break;
    case 4:
      for(int i = 0;i<N_PIXELS;i++){
        leds[i] = CRGB(0,255,255);
        tft.fillRect(60,0,8,120,tft.color565(0,255,255)); 
        FastLED.show();
        delay(t);
      }
    break;
    case 5:
      for(int i = 0;i<N_PIXELS;i++){
        leds[i] = CRGB(255,0,255);
        tft.fillRect(60,0,8,120,tft.color565(255,0,255)); 
        FastLED.show();
        delay(t);
      }
    break;
    case 6:
      for(int i = 0;i<N_PIXELS;i++){
        leds[i] = CRGB(255,255,255);
        tft.fillRect(60,0,8,120,tft.color565(255,255,255)); 
        FastLED.show();
        delay(t);
      }
    break;
  }
  mode_1_0_ste++;
  if(mode_1_0_ste>6)mode_1_0_ste=0;
  Serial.println("mode_1_0");

}

void mode_1_1(){
  CRGB t = leds[0];
  for(int i = 0;i<N_PIXELS+10;i++){
    for(int l = 0;l<10;l++){
      if((i-l) >=0 and (i-l)<128 )leds[i-l] = CRGB(map(l,0,10,255,0),map(l,0,10,255,0),map(l,0,10,255,0));
    }
    if((i-10) >=0 and (i-10)<128 )leds[i-10] = t;
    FastLED.show();
  }
  Serial.println("mode_1_1");
}

void singleClick_2()
{ 
 
  switch(modeste){
    case 0:{
      select_num++;
      if(select_num==filename_num)select_num=0;
      Serial.println("singleClick() detected.");
      returnImg();
      }
    break;
    
    case 1:

    break;
    
    case 2:

    break;
  }
}


void doubleClick_2()
{
  Serial.println("doubleClick(2) detected.");
  modeste++;
  if(modeste>1)modeste=0;
  switch(modeste){
    case 0:{
      for(int i = 0;i<N_PIXELS;i++){
        leds[i] = CRGB(0,0,0);
      }
      FastLED.show();
      returnImg();
      }
      
    break;
    
    case 1:
      tft.fillScreen(TFT_BLACK);
      for(int i=0;i<255;i+=2){
        tft.fillScreen(tft.color565(i,i,i));
        delay(1);
      }
      for(int i=0;i<20;i++){
        tft.fillRect(64-i,0,i*2,160,TFT_BLACK);
        delay(5);
      }
      tft.fillRoundRect(58,120,12,40,2,TFT_WHITE);
      tft.fillRect(60,0,8,120,tft.color565(255,0,0)); 
    break;
    
  }
}


void singleClick_1()
{
  switch(modeste){
    case 0:
      mode_0();
    break;
    case 1:
      mode_1_0();
    break;
  }
}

void doubleClick_1()
{
  switch(modeste){
    case 0:
      mode_0();
    break;
    case 1:
      mode_1_1();
    break;
  }
}

void returnImg(){
  tft.fillScreen(TFT_BLACK);
  String s = Fname[select_num];
  drawJpeg(s, 0 , 0); 
  s += "   ";
  s += select_num+1;
  s += "/";
  s += filename_num;
  tft.setTextSize(1);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.drawString(s, 0, 0, 2);
}
