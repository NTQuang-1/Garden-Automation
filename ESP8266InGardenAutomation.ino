#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID           "TMPLF99mIPvm"
#define BLYNK_DEVICE_NAME           "Quickstart Device"
#define BLYNK_AUTH_TOKEN            "98lpKKT5T0AjgUcpAT3s7KJdJlIt9FBJ"
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <NTPClient.h> // THư viện để sử dụng các hàm thời gian
#include <WiFiUdp.h>
#define ENA   5
#define ENB   12
#define IN1   4
#define IN2   0
#define IN3   2
#define IN4   14
#define Den   16
#define MayBomNuoc  15
// khai bao chân ảo
#define HienThi     V4
#define DoAmDat     V5
#define Thoitiet    V6
#define Luong_nuoc  V7
#define Chan_ao     V8
// khai báo mạng để kết nối
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "NTQ";
char pass[] = "uabcyekh";
String s = "";
int Val_CBD;
int Val_CBM;
int Val_CBMN;
int TrangThaiVom = 0; // lúc đầu vòm ở trang thái mở, khi vòm đóng thì trạng thái là 1
int CoMua = 0;
const long Gio_VN = 7*3600; // VN GMT +7
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};//các ngày trong tuần
int h_now, m_now, s_now;
int minRange = 312;
int maxRange = 712;
int tocdomotor = 200;
// định nghĩa NTP để nhận thời gian
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", Gio_VN);
BlynkTimer timer;

// các hàm chức năng
// Nhận dữ liệu cảm biến từ Arduino qua giao tiếp UART
void nhangiatri()
{
  String m1 = "";
  String m2 = "";
  String m3 = "";
  int n1 = 0;
  int n2 = 0;
  int n3 = 0;
  if(Serial.available())
  {
    s = Serial.readString();
    Serial.print("Nhận data gui tu Arduino : ");
    Serial.println(s);
    for(int i=0;i<=s.length();i++)
    {
      switch (s[i])
      {
        case 'A':   n1=1;n2=0;n3=0;
        break;
        case 'B':   n1=0;n2=1;n3=0;
        break;
        case 'C':   n1=0;n2=0;n3=1;
        break;
        default:
          if(n1==1){
            m1 +=s[i]; s[i]=0;
          }
          if(n2==1){
            m2 +=s[i]; s[i]=0;
          }
          if(n3==1){
            m3 +=s[i]; s[i]=0;
          }
        break;
      }
    }
    Val_CBD = m1.toInt(); 
    Val_CBM = m2.toInt();
    Val_CBMN = m3.toInt();
    s = "";
  }
}

void Gui_du_lieu_len_blynk()
{
  nhangiatri();
  Serial.print("Do am Dat : ");
  Serial.print(Val_CBD);
  Serial.println("%");
  Serial.print("Tinh trang thoi tiet : ");
  if(Val_CBM > 50)
  {
    Serial.println("It's rain!!!");
    CoMua = 1;
  }else
  {
    Serial.println("cloudless sky");
    CoMua = 0;
  }
  Serial.print("Luong nuoc con lai : ");
  Serial.print(Val_CBMN);
  Serial.println("%");
  if(Val_CBMN < 40)
  {
    Blynk.virtualWrite(HienThi, "Sắp hết nước!!!");
  }
  Serial.println("--------------------");
  Blynk.virtualWrite(DoAmDat,Val_CBD);  // gửi dữ liệu độ ẩm đất lên blynk qua chân ảo V5
  if(CoMua == 1)
  {
    Blynk.virtualWrite(Thoitiet,CoMua);   // gửi dữ liệu thời tiết lên blynk qua chân ảo V6
  }
  Blynk.virtualWrite(Luong_nuoc,Val_CBMN);// gửi dữ liệu lượng nước lên blynk qua chân ảo V7
  timeClient.update(); // Lấy ngày giờ hệ thống
  h_now = timeClient.getHours();
  m_now = timeClient.getMinutes();
  s_now = timeClient.getSeconds();
  Serial.print(daysOfTheWeek[timeClient.getDay()]);
  Serial.print(", ");
  Serial.print(h_now);
  Serial.print(":");
  Serial.print(m_now);
  Serial.print(":");
  Serial.println(s_now);
}

void dongvom()
{
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, tocdomotor);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, tocdomotor);
}

void movom(){
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, tocdomotor);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, tocdomotor);
}

void DungDongCo()
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, tocdomotor);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, tocdomotor);
}

void BatMayBomNuoc()
{
  digitalWrite(MayBomNuoc, HIGH);
}

void TatMayBomNuoc()
{
  digitalWrite(MayBomNuoc, LOW);
}

void BatDen()
{
  digitalWrite(Den, HIGH);
}

void TatDen()
{
  digitalWrite(Den, LOW);
}
// các hàm chân ảo

BLYNK_CONNECTED()
{
  Serial.println("Blynk da duoc ket noi");
}

// BLYNK_WRITE để nhận tín hiệu từ chân ảo xuống phần cứng

BLYNK_WRITE(Chan_ao)
{
  if (param.asInt() == 1 )
  {
    BatDen();
  }
  if (param.asInt() == 0 )
  {
    TatDen();
  }
}

void setup()
{
  Serial.begin(115200);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(Den, OUTPUT);

  digitalWrite(ENA, LOW);
  digitalWrite(ENB, LOW);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  digitalWrite(Den, LOW);

  Blynk.begin(auth, ssid, pass); //bắt đầu kết nối với wifi
  timeClient.begin();
  timer.setInterval(1000L, Gui_du_lieu_len_blynk); // tự cập nhật lên blynk mỗi giây
}

void loop()
{
  Blynk.run();
  timer.run();
  if((Val_CBD < 40)&&(Val_CBM < 50)&&(Val_CBMN < 40)){

    if(TrangThaiVom == 0)
    {
      movom();
      delay(1000);
      DungDongCo();
      TrangThaiVom = 1;
    }
    TatMayBomNuoc();
  }
  if((Val_CBD < 40)&&(Val_CBM > 50)){
    TatMayBomNuoc();
    if(TrangThaiVom == 0)
    {
      movom();
      delay(1000);
      DungDongCo();
      TrangThaiVom = 1;
    }
  }
  if((Val_CBD >= 40)&&(Val_CBM <50)){
    TatMayBomNuoc();
    if(TrangThaiVom == 0)
    {
      movom();
      delay(1000);
      DungDongCo();
      TrangThaiVom = 1;
    }
  }
  if((Val_CBD < 40)&&(Val_CBM < 50)&&(Val_CBMN > 40)){
    BatMayBomNuoc();
    if(TrangThaiVom == 0)
    {
      movom();
      delay(1000);
      DungDongCo();
      TrangThaiVom = 1;
    }
  }
  if((Val_CBD >= 40)&&(Val_CBM > 50)){
    TatMayBomNuoc();
    if(TrangThaiVom == 1)
    {
      dongvom();
      delay(1000);
      DungDongCo();
      TrangThaiVom = 0;
    }
  }
}
