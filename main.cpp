////////////////////  初期設定  ////////////////////
#include <M5Stack.h>
#include <Arduino.h>

const int tx = 2; // 送信
const int rx = 5; // 受信
const int button = 17; // ボタン

int b_num[] = {0,0,0}; // 受信時使用
boolean exist[8]; // 中身があるかどうか
int reel = random(8); // 表示番号
int num = 0; // 送受信する番号

int delay_t = 11; // delay time

int state = 0; // 状態

// 画像の切りかえ
void picture(){
  reel = reel%8;
  while(!exist[reel]){
    reel++;
  }
  if(reel == 0){
    M5.Lcd.drawJpgFile(SD, "/m0.jpg");
  } else if(reel == 1){
    M5.Lcd.drawJpgFile(SD, "/m1.jpg");
  } else if(reel == 2){
    M5.Lcd.drawJpgFile(SD, "/m2.jpg");
  } else if(reel == 3){
    M5.Lcd.drawJpgFile(SD, "/m3.jpg");
  } else if(reel == 4){
    M5.Lcd.drawJpgFile(SD, "/m4.jpg");
  } else if(reel == 5){
    M5.Lcd.drawJpgFile(SD, "/m5.jpg");
  } else if(reel == 6){
    M5.Lcd.drawJpgFile(SD, "/m6.jpg");
  } else {
    M5.Lcd.drawJpgFile(SD, "/m7.jpg");
  }
}

// 送信 & 2bit変換
void send(int s_num){ // s_num:変換する値
  // 2bit 変換
  // 送信する値を格納する(0bit目はスタートビット)
  int bit[] = {1,0,0,0}; 
  for(int i = 1; i < 4; i++){
    bit[i] = (s_num%2);
    s_num = (int)(s_num/2);
  }

  // 送信
  for(int i = 0; i < 4; i++){
    digitalWrite(tx, bit[i]);
    delay(delay_t); // 10ms待機
  }
  digitalWrite(tx, LOW);
}

// 受信
void receive(){
  // rxがスタートビットを受信したら
  if(digitalRead(rx) == HIGH){
    // num 初期化
    num = 0;
    // 10ms待機
    delay(delay_t);
    // rxがLOW(確変モード)を受信したら
    if(digitalRead(rx) == LOW){
      state++; // 次の状態へ
    } else { // 通常モードの場合
      // 10ms待機
      delay(delay_t);
      // num[]に受信した値を格納
      for(int i = 0; i < 3; i++){
        b_num[i] = digitalRead(rx);
        delay(delay_t);
      }
      // num[]の値を10進数に変換
      for(int i = 2; i >= 0; i--){
        num = num*2 + b_num[i];
      }
      state = 3;
    }
  }
}

// 割り込み用
void read(){
  if(state == 1 || state == 3){
    state++; 
  }
}

// セットアップ
void setup() {
  pinMode(tx, OUTPUT);
  pinMode(rx, INPUT);
  pinMode(button, INPUT_PULLUP);
	M5.begin();
  M5.Power.begin();

  // 送信用ポートの初期状態は LOW
  digitalWrite(tx, LOW);

  // 初期状態はすべてtrue
  for(int i = 0; i < 8; i++){
    exist[i] = true;
  }

  // ボタンの割り込み
  attachInterrupt(button, read, FALLING);
}

void loop(){
  switch(state){
    // 開始待ち
    case 0:
      if(M5.BtnA.wasPressed() || M5.BtnB.wasPressed() || M5.BtnC.wasPressed()){
        state = 5;
      }
      receive();
      break;

    // 確変モード
    case 1:
      reel++;
      picture();
      for(int i = 0; i < 1000; i++){
        delay(1);
      }
      break;

    case 2:
      num = reel;
      send(num);
      exist[num] = false; // 排出したので中身がなくなる

      // すべて空になったら case 5 へ
      
        
      state = 0;
      break;
    
    // ノーマルモード
    case 3:
      picture();
      reel++;
      break;

    case 4:
      reel = num;
      picture();
      exist[num] = false; // 排出したので中身がなくなる
      //M5.Lcd.print(num);
      digitalWrite(tx, HIGH);
      delay(delay_t);
      digitalWrite(tx, LOW);

      // すべて空になったら case 5 へ
      
      state = 0;
      break;

      case 5:
        reel = reel%8;
        while(exist[reel]){
          reel++;
        }
        if(reel == 0){
          M5.Lcd.drawJpgFile(SD, "/m0.jpg");
        } else if(reel == 1){
          M5.Lcd.drawJpgFile(SD, "/m1.jpg");
        } else if(reel == 2){
          M5.Lcd.drawJpgFile(SD, "/m2.jpg");
        } else if(reel == 3){
          M5.Lcd.drawJpgFile(SD, "/m3.jpg");
        } else if(reel == 4){
          M5.Lcd.drawJpgFile(SD, "/m4.jpg");
        } else if(reel == 5){
          M5.Lcd.drawJpgFile(SD, "/m5.jpg");
        } else if(reel == 6){
          M5.Lcd.drawJpgFile(SD, "/m6.jpg");
        } else {
          M5.Lcd.drawJpgFile(SD, "/m7.jpg");
        }
        state++;
        break;

      case 6:
        if(M5.BtnA.wasPressed()){
          reel--;
          while(exist[reel]){
            reel--;
          }
          state--;
        }
        if(M5.BtnC.wasPressed()){
          reel++;
          while(exist[reel]){
            reel++;
          }
          state--;
        }
        if(M5.BtnB.wasPressed()){
          exist[reel] = true;
          state = 0;
        }
        break;
  }	
}