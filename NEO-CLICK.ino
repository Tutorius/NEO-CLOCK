#include <Adafruit_NeoMatrix.h>

#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <Wire.h>

#define WLANNAME "YourWLAN"
#define WLANPASS "YourPassword"

#define DIGIT1 matrix.Color(0,70,0)
#define DIGIT2 matrix.Color(0,70,0)
#define DIGIT3 matrix.Color(0,70,0)
#define DIGIT4 matrix.Color(0,70,0)
#define SECBAR1 matrix.Color(100,0,0)
#define SECBAR2 matrix.Color(100,50,0)

#define PIN 32

#define SCREEN_WIDTH 16
#define SCREEN_HEIGHT 8

// WLAN-Parameter
#define SSID WLANNAME
#define PASSWORD WLANPASS

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

// Variables to save date and time

int charpix[160]=
{
  // Numbers 0 to 9 4 bits per Char, 7 hoch
  6,9,9,9,9,9,6,0,
  1,3,5,9,1,1,1,0,
  6,9,1,2,4,8,15,0,
  6,9,9,2,9,9,6,0,
  1,2,4,8,10,15,2,0,
  15,8,8,14,1,9,6,0,
  3,4,8,14,9,9,6,0,
  15,1,1,1,2,4,8,0,
  6,9,9,6,9,9,6,0,
  6,9,9,7,1,2,12,0,
  14,10,10,10,10,10,14,0,
  2,2,2,2,2,2,2,0,
  14,2,2,14,8,8,14,0,
  14,2,2,14,2,2,14,0,
  10,10,10,14,2,2,2,0,
  14,8,8,14,2,2,14,0,
  14,8,8,14,10,10,14,0,
  14,2,2,2,2,2,2,0,
  14,10,10,14,10,10,14,0,
  14,10,10,14,2,2,14,0
};

int i,j,flag;

int secs,mins,hours;
int days,months,years;

unsigned long eptime;

unsigned long time1;

float diff;

char s[40],t[40];

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(SCREEN_WIDTH, SCREEN_HEIGHT, PIN,
 NEO_MATRIX_TOP+NEO_MATRIX_LEFT+NEO_MATRIX_COLUMNS+NEO_MATRIX_ROWS+NEO_MATRIX_ZIGZAG,
 NEO_GRB + NEO_KHZ800);

// Init WLAN
void initWifi()
{
  WiFi.mode(WIFI_STA); // WIFI-Station
  WiFi.begin(SSID,PASSWORD); // SSID and Password
  Serial.println("");
  Serial.print("Wifi-Start Scan ");
  while(WiFi.status()!=WL_CONNECTED) // Connect
  {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("");
  Serial.println("IP-Adresse : ");
  Serial.println(WiFi.localIP());
}

void setup()
{
  unsigned long yy;
  unsigned long yt;
  unsigned long yk;
  matrix.clear();
  matrix.setBrightness(50);
  matrix.show();
  secs=0; mins=0; hours=0;
  Serial.begin(115200);
  delay(2000);
  initWifi(); 
  timeClient.begin();
  timeClient.setTimeOffset(3600);
  while(!timeClient.update())
  {
    timeClient.forceUpdate();
  }
  hours=timeClient.getHours();
  mins=timeClient.getMinutes();
  secs=timeClient.getSeconds();
  sprintf(s,"%2d:%2d:%2d",hours,mins,secs);
  Serial.println(s);
  WiFi.disconnect(true,true);
  flag=true;
  time1=millis();
  diff=16/59.0;
  do
  {
    if ((millis()-time1>=500)&&flag)
    {
      matrix.clear();
      for(i=0;i<secs*diff;i++)
      {
        matrix.drawPixel(i,7,SECBAR1);
      }
      sprintf(s,"%2d:%2d",hours,mins);
      Serial.println(s);
      PrintClock(hours,mins,DIGIT1,DIGIT2,DIGIT3,DIGIT4);
      matrix.show();
      flag=false;
    }
    if (millis()-time1>=1000)
    {
      for(i=0;i<secs*diff;i++)
      {
        matrix.drawPixel(i,7,SECBAR2);
      }
      matrix.show();
      flag=true;
      time1+=1000;
      if(secs++>=59)
      {
        secs=0;
        if(mins++>=59)
        {
          mins=0;
          if(hours++>=23)
          {
            hours=0;
            if (years%4==0)
              yt=366;
            else
              yt=365;
            switch(months)
            {
              case 1:
                yk=31; break;
              case 2:
                yk=yt-337; break;
              case 3:
                yk=31; break;
              case 4:
                yk=30; break;
              case 5:
                yk=31; break;
              case 6:
                yk=30; break;
              case 7:
                yk=31; break;
              case 8:
                yk=31; break;
              case 9:
                yk=30; break;
              case 10:
                yk=31; break;
              case 11:
                yk=30; break;
              case 12:
                yk=31; break;
            }
            days++;
            if (days>=yk)
            {
              days=1;
              months++;
              if (months>12)
              {
                months=1;
                years++;
              }
            }     
          }
        }
      }
    }
  }
  while(1==1);  
}

void loop()
{

}


void charpixel(int c,int x,int col)
{
  int y,i,j,k;
  for (y=0;y<7;y++)
  {
    j=charpix[80+c*8+y];
    k=0x08;
    for (i=0;i<4;i++)
    {
      if ((j&k)>0)
      {
        matrix.drawPixel(x+i, y,col);
      }
      else
      {
        matrix.drawPixel(x+i, y,0);
      }
      k=k/2;
    }
  }
}

void PrintClock(int h,int m,int col1,int col2,int col3,int col4)
{
  int i,c;
  int hh,mm;
  int hs1,hs2,ms1,ms2;
  hh=h; hs1=hh/10; 
  hh=hh-hs1*10; hs2=hh;
  charpixel(hs1,0,col1);
  charpixel(hs2,4,col2);
  mm=m; ms1=mm/10;
  mm=mm-ms1*10; ms2=mm;
  charpixel(ms1,8,col3);
  charpixel(ms2,12,col4);
  sprintf(t,"HS %d %d MS %d %d",hs1,hs2,ms1,ms2);
  Serial.println(t);
}

