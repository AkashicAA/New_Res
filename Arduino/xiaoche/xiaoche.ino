
#include <ESP8266WiFi.h>

#define TCP_SERVER_ADDR "bemfa.com" //巴法云服务器地址默认即可
#define TCP_SERVER_PORT "8344" //服务器端口，tcp创客云端口8344
#define DEFAULT_STASSID  "poli"     //WIFI名称，区分大小写，不要写错
#define DEFAULT_STAPSW   "12345678"  //WIFI密码
String UID = "dba6ac8efa663ecaa1a7d6c3d470e5bb";  //用户私钥，可在控制台获取
String TOPIC = "xiaoche";         //主题名字，可在控制台新建
//定义各脚关
int D1=5;
int D2=4;
int D3=0;
int D4=2;
int D5=14;
int D6=12;
int D7=13;
int D8=15;
//最大字节数
#define MAX_PACKETSIZE 512
//设置心跳值30s
#define KEEPALIVEATIME 30*1000
//tcp客户端相关初始化，默认即可
WiFiClient TCPclient;
String TcpClient_Buff = "";//初始化字符串，用于接收服务器发来的数据
unsigned int TcpClient_BuffIndex = 0;
unsigned long TcpClient_preTick = 0;
unsigned long preHeartTick = 0;//心跳
unsigned long preTCPStartTick = 0;//连接
bool preTCPConnected = false;
//相关函数初始化
//连接WIFI
void doWiFiTick();
void startSTA();

//TCP初始化连接
void doTCPClientTick();
void startTCPClient();
void sendtoTCPServer(String p);

//led控制函数，具体函数内容见下方
void turnOnLed();
void turnOffLed();



/*
  *发送数据到TCP服务器
 */
void sendtoTCPServer(String p){
  if (!TCPclient.connected()) 
  {
    Serial.println("Client is not readly");
    return;
  }
  TCPclient.print(p);
}


/*
  *初始化和服务器建立连接
*/
void startTCPClient(){
  if(TCPclient.connect(TCP_SERVER_ADDR, atoi(TCP_SERVER_PORT))){
    Serial.print("\nConnected to server:");
    Serial.printf("%s:%d\r\n",TCP_SERVER_ADDR,atoi(TCP_SERVER_PORT));
    
    String tcpTemp="";  //初始化字符串
    tcpTemp = "cmd=1&uid="+UID+"&topic="+TOPIC+"\r\n"; //构建订阅指令
    sendtoTCPServer(tcpTemp); //发送订阅指令
    tcpTemp="";//清空
    /*
     //如果需要订阅多个主题，可再次发送订阅指令
      tcpTemp = "cmd=1&uid="+UID+"&topic="+主题2+"\r\n"; //构建订阅指令
      sendtoTCPServer(tcpTemp); //发送订阅指令
      tcpTemp="";//清空
     */
    
    preTCPConnected = true;
    preHeartTick = millis();
    TCPclient.setNoDelay(true);
  }
  else{
    Serial.print("Failed connected to server:");
    Serial.println(TCP_SERVER_ADDR);
    TCPclient.stop();
    preTCPConnected = false;
  }
  preTCPStartTick = millis();
}


/*
  *检查数据，发送心跳
*/
void doTCPClientTick(){
 //检查是否断开，断开后重连
   if(WiFi.status() != WL_CONNECTED) return;
  if (!TCPclient.connected()) {//断开重连
  if(preTCPConnected == true){
    preTCPConnected = false;
    preTCPStartTick = millis();
    Serial.println();
    Serial.println("TCP Client disconnected.");
    TCPclient.stop();
  }
  else if(millis() - preTCPStartTick > 1*1000)//重新连接
    startTCPClient();
  }
  else
  {
    if (TCPclient.available()) {//收数据
      char c =TCPclient.read();
      TcpClient_Buff +=c;
      TcpClient_BuffIndex++;
      TcpClient_preTick = millis();
      
      if(TcpClient_BuffIndex>=MAX_PACKETSIZE - 1){
        TcpClient_BuffIndex = MAX_PACKETSIZE-2;
        TcpClient_preTick = TcpClient_preTick - 200;
      }
      preHeartTick = millis();
    }
    if(millis() - preHeartTick >= KEEPALIVEATIME){//保持心跳
      preHeartTick = millis();
      Serial.println("--Keep alive:");
      sendtoTCPServer("ping\r\n"); //发送心跳，指令需\r\n结尾
    }
  }
  if((TcpClient_Buff.length() >= 1) && (millis() - TcpClient_preTick>=200))
  {
    TCPclient.flush();
    Serial.print("Rev string: ");
    TcpClient_Buff.trim(); //去掉首位空格
    Serial.println(TcpClient_Buff); //打印接收到的消息
    String getTopic = "";
    String getMsg = "";
    if(TcpClient_Buff.length() > 15){//注意TcpClient_Buff只是个字符串，在上面开头做了初始化 String TcpClient_Buff = "";
          //此时会收到推送的指令，指令大概为 cmd=2&uid=xxx&topic=light002&msg=off
          int topicIndex = TcpClient_Buff.indexOf("&topic=")+7; //c语言字符串查找，查找&topic=位置，并移动7位
          int msgIndex = TcpClient_Buff.indexOf("&msg=");//c语言字符串查找，查找&msg=位置
          getTopic = TcpClient_Buff.substring(topicIndex,msgIndex);//c语言字符串截取，截取到topic
          getMsg = TcpClient_Buff.substring(msgIndex+5);//c语言字符串截取，截取到消息
          Serial.print("topic:------");
          Serial.println(getTopic); //打印截取到的主题值
          Serial.print("msg:--------");
          Serial.println(getMsg);   //打印截取到的消息值
   }
     if(getMsg  == "forward"){       //如果是消息==forward  
        forward();
     }else if(getMsg == "back"){ //如果是消息==back  
        back();
      }else if(getMsg == "left"){
        left();
      }else if(getMsg == "right"){
        right();
        }
        else if(getMsg == "stop"){
          Stop();
          }
          else if(getMsg == "left_loop"){
          left_loop();
          }
            else if(getMsg == "right_loop"){
            right_loop();
            }
    
   TcpClient_Buff="";
   TcpClient_BuffIndex = 0;
  }
}
/*
  *初始化wifi连接
*/
void startSTA(){
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  WiFi.begin(DEFAULT_STASSID, DEFAULT_STAPSW);
}



/**************************************************************************
                                 WIFI
***************************************************************************/
void doWiFiTick(){
  static bool startSTAFlag = false;
  static bool taskStarted = false;
  static uint32_t lastWiFiCheckTick = 0;

  if (!startSTAFlag) {
    startSTAFlag = true;
    startSTA();
  }

  //未连接1s重连
  if ( WiFi.status() != WL_CONNECTED ) {
    if (millis() - lastWiFiCheckTick > 1000) {
      lastWiFiCheckTick = millis();
    }
  }
  //连接成功建立
  else {
    if (taskStarted == false) {
      taskStarted = true;
      Serial.print("\r\nGet IP Address: ");
      Serial.println(WiFi.localIP());
      startTCPClient();
    }
  }
}
/*设置电机初始化*/
void forward(){//小车前进
   Serial.println("Turn forward");
   digitalWrite(D1,HIGH); 
   digitalWrite(D2,LOW); 
   
   digitalWrite(D3,HIGH); 
   digitalWrite(D4,LOW); 
   
   digitalWrite(D5,HIGH); 
   digitalWrite(D6,LOW); 
   
   digitalWrite(D7,HIGH); 
   digitalWrite(D8,LOW);
}
void back(){//小车后退
   Serial.println("Turn back");
   digitalWrite(D1,LOW); 
   digitalWrite(D2,HIGH);
    
   digitalWrite(D3,LOW); 
   digitalWrite(D4,HIGH); 
   
   digitalWrite(D5,LOW); 
   digitalWrite(D6,HIGH); 

   digitalWrite(D7,LOW); 
   digitalWrite(D8,HIGH); 
}
void Stop(){
    Serial.println("Turn stop");
   digitalWrite(D1,LOW); 
   digitalWrite(D2,LOW); 
   
   digitalWrite(D3,LOW); 
   digitalWrite(D4,LOW); 
   
   digitalWrite(D5,LOW); 
   digitalWrite(D6,LOW);
   
   digitalWrite(D7,LOW);
   digitalWrite(D8,LOW);
}
void left()
{    
    Serial.println("Turn left");
    digitalWrite(D1,LOW); 
    digitalWrite(D2,HIGH); 
    
    digitalWrite(D3,HIGH); 
    digitalWrite(D4,LOW); 
    
    digitalWrite(D5,HIGH);
    digitalWrite(D6,LOW);
    
    digitalWrite(D7,LOW); 
    digitalWrite(D8,HIGH);  
}
void right()
{
    Serial.println("Turn right");
    digitalWrite(D1,HIGH); 
    digitalWrite(D2,LOW); 
    
    digitalWrite(D3,LOW); 
    digitalWrite(D4,HIGH);
  
    digitalWrite(D5,LOW); 
    digitalWrite(D6,HIGH); 
    
    digitalWrite(D7,HIGH);  
    digitalWrite(D8,LOW); 
}
void right_loop()
{
  Serial.println("Turn right_loop");
   digitalWrite(D1,HIGH); 
   digitalWrite(D2,LOW); 
   
   digitalWrite(D3,HIGH); 
   digitalWrite(D4,LOW); 
   
   digitalWrite(D5,LOW); 
   digitalWrite(D6,HIGH); 

   digitalWrite(D7,LOW); 
   digitalWrite(D8,HIGH);
}
void left_loop()
{
    Serial.println("Turn left_loop");
    digitalWrite(D1,LOW); 
    digitalWrite(D2,HIGH);
    
    digitalWrite(D3,LOW); 
    digitalWrite(D4,HIGH); 
    
    digitalWrite(D5,HIGH); 
    digitalWrite(D6,LOW); 
   
    digitalWrite(D7,HIGH); 
    digitalWrite(D8,LOW);
}

void setup() {
  Serial.begin(115200);
pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);
  pinMode(D8, OUTPUT);
  Stop();
  Serial.println("start!");
}

//循环
void loop() {
  doWiFiTick();
  doTCPClientTick();
}
