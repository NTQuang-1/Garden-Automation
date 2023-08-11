// khai bao cong ket noi
#include <SoftwareSerial.h>
#include <LiquidCrystal.h>
#define Rx 11             //Định nghĩa chân 11 là Rx
#define Tx 12             //Định nghĩa chân 12 là Tx  
SoftwareSerial mySerial(Rx, Tx); //Khởi tạo cổng serial mềm
LiquidCrystal lcd(8, 9, 5, 4, 3, 2); // rs, e, d4,d5,d6,d7
#define CBD   A0   // 1023 to 0
#define CBM   A1   // 1023 to 0
#define CBMN  A2  // 0 to 550
// Khai báo các biến ban đầu
int     value1,value2,value3;
long    last = 0;
String  a = "";

void setup() {
  Serial.begin(115200);
  lcd.begin(16,2);
  mySerial.begin(115200);
  pinMode(CBD, INPUT); // data cảm biến độ ẩm đất
  pinMode(CBM, INPUT); // data cảm biến mưa
  pinMode(CBMN, INPUT); // data cảm biến mức nước
  last = millis();
}

void loop()
{
  Doc_du_lieu();
  Gui_du_lieu();
}  

int doc_phan_tram(uint8_t CBx)
{
  int tb = analogRead(CBx);
  int phan_tram = map(tb,0,1023,0,100);
  return 100 - phan_tram;
}

void Doc_du_lieu()
{
  Serial.println("----------------------------------");
  Serial.println("Read Data : ");
  Serial.print("Data cam bien do am dat : "); // dữ liệu cảm biến độ ẩm đất
  value1 = doc_phan_tram(CBD);
  Serial.print(value1);
  Serial.println("%");
  lcd.setCursor(0, 0);
  lcd.print("Do Am Dat : ");
  lcd.print(value1);
  lcd.println("%");
  Serial.print("Data cam bien Mua : "); // dữ liệu cảm biến mưa
  value2 = doc_phan_tram(CBM);
  Serial.print(value2);
  Serial.println("%");
  Serial.print("Data cam bien Muc nuoc : "); // dữ liệu cảm biến mức nước
  value3 = 100 - doc_phan_tram(CBMN);
  Serial.print(value3);
  Serial.println("%");
  lcd.setCursor(0,1);
  lcd.print("Luong Nuoc:");
  lcd.print(value3);
  lcd.println("%");
  delay(1000);
}

void gui_du_lieu(int gt1,int gt2, int gt3)
{
  if(millis() - last >= 2000)
  {
    a+= 'A' + String(gt1);
    a+= 'B' + String(gt2);
    a+= 'C' + String(gt3); // dữ liệu truyền : 'A'+(gia tri cb Dất)+'B'+(giá trị cảm b mua)+'C'+(giá trị cảm biến nước)
    Serial.print("Gui du lieu cho ESP : ");
    Serial.println(a);
    mySerial.println(a);
    Serial.flush();
    a = "";

  }
}

void Gui_du_lieu(void)
{
    gui_du_lieu(value1,value2,value3);
    delay(1000);
}
