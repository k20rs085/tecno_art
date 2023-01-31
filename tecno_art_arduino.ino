// #include <Servo.h> // サーボモーター ライブラリ利用

////////////////////  初期設定  ////////////////////
const int sb = 2; // スタートボタン sb
const int s_tx = 3; // シャツ送信
const int s_rx = 4; // シャツ受信
const int p_tx = 5; // ズボン送信
const int p_rx = 6; // ズボン受信
const int a_tx = 7; // アクセ送信
const int a_rx = 8; // アクセ受信
// オプションモード信号
// 他のM5がオプションモードに移行しているから
// 完了するまで止まっているように知らせる信号
const int s_op = 33; // シャツ オプションモード信号
const int p_op = 34; // ズボン --
const int a_op = 35; // アクセ --

//////////////////// モーター設定 ////////////////////
const int s_mi   = 9;  // シャツモーター 1
const int s_mii  = 10; // シャツモーター 2
const int s_miii = 11; // シャツモーター 3
const int s_miv  = 12; // シャツモーター 4
const int s_mv   = 13; // シャツモーター 5
const int s_mvi  = 14; // シャツモーター 6
const int s_mvii = 15; // シャツモーター 7
const int s_mviii= 16; // シャツモーター 8

const int p_mi   = 17; // ズボンモーター 1
const int p_mii  = 18; // ズボンモーター 2
const int p_miii = 19; // ズボンモーター 3
const int p_miv  = 20; // ズボンモーター 4
const int p_mv   = 21; // ズボンモーター 5
const int p_mvi  = 22; // ズボンモーター 6
const int p_mvii = 23; // ズボンモーター 7
const int p_mviii= 24; // ズボンモーター 8

const int a_mi   = 25; // アクセモーター 1
const int a_mii  = 26; // アクセモーター 2
const int a_miii = 27; // アクセモーター 3
const int a_miv  = 28; // アクセモーター 4
const int a_mv   = 29; // アクセモーター 5
const int a_mvi  = 30; // アクセモーター 6
const int a_mvii = 31; // アクセモーター 7
const int a_mviii= 32; // アクセモーター 8
////////////////////////////////////////////////////

int s_num[] = {0,0,0}; // シャツ 受信時使用
int p_num[] = {0,0,0}; // ズボン --
int a_num[] = {0,0,0}; // アクセ --

int bonus = 0; // 確変抽選
int state = 0; // 状態
int shirt = 0; int pants = 0; int acce = 0; // シャツ、ズボン、アクセ

int sb_state = 0; // スタートボタンの状態

int delay_t = 10; // delay time

boolean s_exist[8]; // シャツ 中身があるかどうか
boolean p_exist[8]; // ズボン 中身があるかどうか
boolean a_exist[8]; // アクセ 中身があるかどうか

boolean s_m = false; // シャツのM5Stack 押してもいいかな？
boolean p_m = false; // ズボンのM5Stack --
boolean a_m = false; // アクセのM5Stack --

// 送信 & 2bit変換
void send(int num, int add){ // num:変換する値, add:送り先
  // 2bit 変換
  // 送信する値を格納する(0bit目はスタートビット, 1bit目は確変bit)
  int bit[] = {1,1,0,0,0}; 
  for(int i = 2; i < 5; i++){
    bit[i] = (num%2);
    num = (int)(num/2);
  }
  
  // 送信
  for(int i = 0; i < 5; i++){
    if(add == 0){ // addが0ならシャツに送る
      digitalWrite(s_tx, bit[i]);
    } else if(add == 1){ // addが1ならズボンに送る
      digitalWrite(p_tx, bit[i]);
    } else { // それ以外ならアクセに送る
      digitalWrite(a_tx, bit[i]);
    }
    delay(delay_t); // 10ms待機
  }
  
  // 初期状態のLOWに戻す
  if(add == 0){
    digitalWrite(s_tx, LOW);
  } else if(add == 1){
    digitalWrite(p_tx, LOW);
  } else {
    digitalWrite(a_tx, LOW);
  }
}

// 受信
void receive(int add){  // 送り主 0:シャツ, 1:ズボン, 2:アクセ
  // 11ms待機
  delay(delay_t + 1);
  
  if(add == 0){
    // s_num[]に受信した値を格納
    for(int i = 0; i < 3; i++){
      s_num[i] = digitalRead(s_rx);
      delay(delay_t + 1);
    }
    // s_num[]の値を10進数に変換
    for(int i = 2; i >= 0; i--){
      shirt = shirt*2 + s_num[i];
    }

  } else if(add == 1){
    // p_num[]に受信した値を格納
    for(int i = 0; i < 3; i++){
      p_num[i] = digitalRead(p_rx);
      delay(delay_t + 1);
    }
    // p_num[]の値を10進数に変換
    for(int i = 2; i >= 0; i--){
      pants = pants*2 + p_num[i];
    }

  } else if(add == 2){
    // a_num[]に受信した値を格納
    for(int i = 0; i < 3; i++){
      a_num[i] = digitalRead(a_rx);
      delay(delay_t + 1);
    }
    // a_num[]の値を10進数に変換
    for(int i = 2; i >= 0; i--){
      acce = acce*2 + a_num[i];
    }
  }
}


// シャツ 商品排出
void s_discharge(){
  if(shirt == 0){
    digitalWrite(s_mi,HIGH);
    delayMicroseconds(1000);
    digitalWrite(s_mi,LOW);
    delayMicroseconds(1000);
  } else if(shirt == 1){
    digitalWrite(s_mii,HIGH);
    delayMicroseconds(1000);
    digitalWrite(s_mii,LOW);
    delayMicroseconds(1000);
  } else if(shirt == 2){
    digitalWrite(s_miii,HIGH);
    delayMicroseconds(1000);
    digitalWrite(s_miii,LOW);
    delayMicroseconds(1000);
  } else if(shirt == 3){
    digitalWrite(s_miv,HIGH);
    delayMicroseconds(1000);
    digitalWrite(s_miv,LOW);
    delayMicroseconds(1000);
  } else if(shirt == 4){
    digitalWrite(s_mv,HIGH);
    delayMicroseconds(1000);
    digitalWrite(s_mv,LOW);
    delayMicroseconds(1000);
  } else if(shirt == 5){
    digitalWrite(s_mvi,HIGH);
    delayMicroseconds(1000);
    digitalWrite(s_mvi,LOW);
    delayMicroseconds(1000);
  } else if(shirt == 6){
    digitalWrite(s_mvii,HIGH);
    delayMicroseconds(1000);
    digitalWrite(s_mvii,LOW);
    delayMicroseconds(1000);
  } else {
    digitalWrite(s_mviii,HIGH);
    delayMicroseconds(1000);
    digitalWrite(s_mviii,LOW);
    delayMicroseconds(1000);
  }
  delay(100);
}

// ズボン 商品排出
void p_discharge(){
  if(pants == 0){
    digitalWrite(p_mi,HIGH);
    delayMicroseconds(1000);
    digitalWrite(p_mi,LOW);
    delayMicroseconds(1000);
  } else if(pants == 1){
    digitalWrite(p_mii,HIGH);
    delayMicroseconds(1000);
    digitalWrite(p_mii,LOW);
    delayMicroseconds(1000);
  } else if(pants == 2){
    digitalWrite(p_miii,HIGH);
    delayMicroseconds(1000);
    digitalWrite(p_miii,LOW);
    delayMicroseconds(1000);
  } else if(pants == 3){
    digitalWrite(p_miv,HIGH);
    delayMicroseconds(1000);
    digitalWrite(p_miv,LOW);
    delayMicroseconds(1000);
  } else if(pants == 4){
    digitalWrite(p_mv,HIGH);
    delayMicroseconds(1000);
    digitalWrite(p_mv,LOW);
    delayMicroseconds(1000);
  } else if(pants == 5){
    digitalWrite(p_mvi,HIGH);
    delayMicroseconds(1000);
    digitalWrite(p_mvi,LOW);
    delayMicroseconds(1000);
  } else if(pants == 6){
    digitalWrite(p_mvii,HIGH);
    delayMicroseconds(1000);
    digitalWrite(p_mvii,LOW);
    delayMicroseconds(1000);
  } else {
    digitalWrite(p_mviii,HIGH);
    delayMicroseconds(1000);
    digitalWrite(p_mviii,LOW);
    delayMicroseconds(1000);
  }
  delay(100);
}

// アクセ 商品排出
void a_discharge(){
  if(acce == 0){
    digitalWrite(a_mi,HIGH);
    delayMicroseconds(1000);
    digitalWrite(a_mi,LOW);
    delayMicroseconds(1000);
  } else if(acce == 1){
    digitalWrite(a_mii,HIGH);
    delayMicroseconds(1000);
    digitalWrite(a_mii,LOW);
    delayMicroseconds(1000);
  } else if(acce == 2){
    digitalWrite(a_miii,HIGH);
    delayMicroseconds(1000);
    digitalWrite(a_miii,LOW);
    delayMicroseconds(1000);
  } else if(acce == 3){
    digitalWrite(a_miv,HIGH);
    delayMicroseconds(1000);
    digitalWrite(a_miv,LOW);
    delayMicroseconds(1000);
  } else if(acce == 4){
    digitalWrite(a_mv,HIGH);
    delayMicroseconds(1000);
    digitalWrite(a_mv,LOW);
    delayMicroseconds(1000);
  } else if(acce == 5){
    digitalWrite(a_mvi,HIGH);
    delayMicroseconds(1000);
    digitalWrite(a_mvi,LOW);
    delayMicroseconds(1000);
  } else if(acce == 6){
    digitalWrite(a_mvii,HIGH);
    delayMicroseconds(1000);
    digitalWrite(a_mvii,LOW);
    delayMicroseconds(1000);
  } else {
    digitalWrite(a_mviii,HIGH);
    delayMicroseconds(1000);
    digitalWrite(a_mviii,LOW);
    delayMicroseconds(1000);
  }
  delay(100);
}


// セットアップ
void setup(){
  pinMode(sb, INPUT_PULLUP);
  pinMode(s_tx, OUTPUT);
  pinMode(s_rx, INPUT);
  pinMode(p_tx, OUTPUT);
  pinMode(p_rx, INPUT);
  pinMode(a_tx, OUTPUT);
  pinMode(a_rx, INPUT);

  // 送信用ポートの初期状態は LOW
  digitalWrite(s_tx, LOW);
  digitalWrite(p_tx, LOW);
  digitalWrite(a_tx, LOW);

  // モーター設定 シャツ
  pinMode(s_mi, OUTPUT);
  pinMode(s_mii, OUTPUT);
  pinMode(s_miii, OUTPUT);
  pinMode(s_miv, OUTPUT);
  pinMode(s_mv, OUTPUT);
  pinMode(s_mvi, OUTPUT);
  pinMode(s_mvii, OUTPUT);
  pinMode(s_mviii, OUTPUT);

  // モーター ズボン
  pinMode(p_mi, OUTPUT);
  pinMode(p_mii, OUTPUT);
  pinMode(p_miii, OUTPUT);
  pinMode(p_miv, OUTPUT);
  pinMode(p_mv, OUTPUT);
  pinMode(p_mvi, OUTPUT);
  pinMode(p_mvii, OUTPUT);
  pinMode(p_mviii, OUTPUT);

  // モーター アクセ
  pinMode(a_mi, OUTPUT);
  pinMode(a_mii, OUTPUT);
  pinMode(a_miii, OUTPUT);
  pinMode(a_miv, OUTPUT);
  pinMode(a_mv, OUTPUT);
  pinMode(a_mvi, OUTPUT);
  pinMode(a_mvii, OUTPUT);
  pinMode(a_mviii, OUTPUT);

  // オプションモード 信号
  // 初期状態 LOW
  pinMode(s_op, OUTPUT);
  digitalWrite(s_op, LOW);
  pinMode(p_op, OUTPUT);
  digitalWrite(p_op, LOW);
  pinMode(a_op, OUTPUT);
  digitalWrite(a_op, LOW);

  // 初期状態はすべてtrue
  for(int i = 0; i < 8; i++){
    s_exist[i] = true;
    p_exist[i] = true;
    a_exist[i] = true;
  }

  Serial.begin(9600); // シリアルモニター デバッグ(確認)用
}

void loop(){
  // stateの値によって分岐

  // state 0 のとき
  if(state == 0){
    // 各値 初期化
    shirt = 0; pants = 0; acce = 0;
    
    // 相手の状態 初期化
    s_m = true; p_m = true; a_m = true;

    // 確変 抽選
    bonus = random(10); // 0～9からランダムで代入
      
    if(bonus == 0){ // もし、確変なら
      Serial.println("bonus");
      state++; // case 1 へ
    } else { // それ以外なら
      
      shirt = random(8); // 0～7からランダムで代入
      while(!s_exist[shirt]){ // 中身がない場合は次の数字
        shirt++;
        shirt = shirt%8;
      }
      
      pants = random(8); // --
      while(!p_exist[pants]){ // --
        pants++;
        pants = pants%8;
      }
      
      acce = random(8); // --
      while(!a_exist[acce]){ // --
        acce++;
        acce = acce%8;
      }
      
      Serial.println("nomal");
      state = 3; // case 3 へ
    }

    // state 1 のとき
    // 確変 送信
  } else if(state == 1){
    // オプションモード
    // 相手からの信号を受信したら state 6へ
    if(digitalRead(s_rx) == HIGH){
      s_m = false; // state 6 にて使う
      digitalWrite(p_op, HIGH); // オプションモード信号送信
      digitalWrite(a_op, HIGH); // --
      delay(delay_t + 2);
      state = 6;
    }
    if(digitalRead(p_rx) == HIGH){
      p_m = false;
      digitalWrite(s_op, HIGH); // オプションモード信号送信
      digitalWrite(a_op, HIGH); // --
      delay(delay_t + 2);
      state = 6;
    }
    if(digitalRead(a_rx) == HIGH){
      a_m = false;
      digitalWrite(s_op, HIGH); // オプションモード信号送信
      digitalWrite(p_op, HIGH); // --
      delay(delay_t + 2);
      state = 6;
    }

    if(digitalRead(sb) == LOW){
      // スタートビット(HIGH)送信
      digitalWrite(s_tx, HIGH);
      digitalWrite(p_tx, HIGH);
      digitalWrite(a_tx, HIGH);
  
      // 10ms待機
      delay(delay_t);
  
      // 確変ビット(LOW)送信
      digitalWrite(s_tx, LOW);
      digitalWrite(p_tx, LOW);
      digitalWrite(a_tx, LOW);
          
      // 10ms待機の後、LOWに初期化
      delay(delay_t);
      digitalWrite(s_tx, LOW);
      digitalWrite(p_tx, LOW);
      digitalWrite(a_tx, LOW);
  
      state++; // state 2 へ
    }

    // state 2 のとき
    // 確変 受信
  } else if(state == 2){
    // s_rxがスタートビットを受信したら
    if(digitalRead(s_rx) == HIGH){
      // シャツ値 受信
      receive(0);
      Serial.print("shirt : ");
      Serial.println(shirt);
      // 受信終了したことを示す
      s_m = false;
    }

    // p_rxがスタートビットを受信したら
    if(digitalRead(p_rx) == HIGH){
      // ズボン値 受信
      receive(1);
      Serial.print("pants : ");
      Serial.println(pants);
      // 受信終了したことを示す
      p_m = false;
    }


    // a_rxがスタートビットを受信したら
    if(digitalRead(a_rx) == HIGH){
      // シャツ値 受信
      receive(2);
      Serial.print("acce : ");
      Serial.println(acce);
      // 受信終了したことを示す
      a_m = false;
    }

    if(!s_m && !p_m && !a_m){ // 全ての受信が終了したら (p_mとa_mも追加する)
      // case 5 へ  商品排出処理はそこで行う
      state = 5;
    }

    // state 3 のとき
    // ノーマル 送信
  } else if(state == 3){
    // オプションモード
    // 相手からの信号を受信したら state 6へ
    if(digitalRead(s_rx) == HIGH){
      s_m = false; // state 6 にて使う
      digitalWrite(p_op, HIGH); // オプションモード信号送信
      digitalWrite(a_op, HIGH); // --
      delay(delay_t + 2);
      state = 6;
    }
    if(digitalRead(p_rx) == HIGH){
      p_m = false;
      digitalWrite(s_op, HIGH); // オプションモード信号送信
      digitalWrite(a_op, HIGH); // --
      delay(delay_t + 2);
      state = 6;
    }
    if(digitalRead(a_rx) == HIGH){
      a_m = false;
      digitalWrite(s_op, HIGH); // オプションモード信号送信
      digitalWrite(p_op, HIGH); // --
      delay(delay_t + 2);
      state = 6;
    }

    if(digitalRead(sb) == LOW){
      // 送信
      send(shirt, 0); // シャツ
      send(pants, 1); // ズボン
      send(acce, 2); // アクセ
      Serial.print("shirt : ");
      Serial.println(shirt);
      Serial.print("pants : ");
      Serial.println(pants);
      Serial.print("acce : ");
      Serial.println(acce);
      state++; // case 4 へ
    }

    // state 4 のとき
    // ノーマル 受信(相手動作終了)
  } else if(state == 4){
    // s_rxが終了合図を受けたら
    if(digitalRead(s_rx) == HIGH){
      s_m = false;
    }

    // p_rxが終了合図を受けたら
    if(digitalRead(p_rx) == HIGH){
      p_m = false;
    }

    // a_rxが終了合図を受けたら
    if(digitalRead(a_rx) == HIGH){
      a_m = false;
    }

      
    if(!s_m && !p_m && !a_m){ // 全ての受信が終了したら (p_mとa_mも追加する)
      // case 5 へ  商品排出処理はそこで行う
      state = 5;
    }

    // state 5 のとき
    // 商品排出中
  } else if(state == 5){
    for(int i = 0; i < 40; i++){
      s_discharge();
      p_discharge();
      a_discharge();
    }

    // 排出したので中身がなくなる
    s_exist[shirt] = false;
    p_exist[pants] = false;
    a_exist[acce] = false;
    state = 0;

    // state = 6 のとき
    // オプションモード
  } else if(state == 6){
    if(!s_m && digitalRead(s_rx) == HIGH){
      // シャツ値 初期化
      shirt = 0;
      
      // シャツ値 受信
      receive(0);
  
      // 受信した値を有効にする
      s_exist[shirt] = true;
      Serial.print("shirt : ");
      Serial.println(shirt);
      
      digitalWrite(p_op, LOW); // オプションモード信号送信
      digitalWrite(a_op, LOW); // --
      
      // state 0 へ
      state = 0;
    }

    if(!p_m && digitalRead(p_rx) == HIGH){
      // ズボン値 初期化
      pants = 0;
      
      // ズボン値 受信
      receive(1);

      // 受信した値を有効にする
      p_exist[pants] = true;
      Serial.print("pants : ");
      Serial.println(pants);

      digitalWrite(s_op, LOW); // オプションモード信号送信
      digitalWrite(a_op, LOW); // --

      // state 0 へ
      state = 0;
    }
    
    if(!a_m && digitalRead(a_rx) == HIGH){
      // アクセ値 初期化
      acce = 0;
      
      // アクセ値 受信
      receive(2);

      // 受信した値を有効にする
      a_exist[acce] = true;
      Serial.print("acce : ");
      Serial.println(acce);

      digitalWrite(s_op, LOW); // オプションモード信号送信
      digitalWrite(p_op, LOW); // --
      
      // state 0 へ
      state = 0;
    }
  }
}
