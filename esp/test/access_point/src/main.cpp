#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiAP.h>
const char *ssid = "ESP32C3";   // アクセスポイントのSSID
const char *password = "123456789";  // アクセスポイントのパスワード
WiFiServer server(80);  // サーバーをポート80で開始


void setup() {
  // Serial.begin(115200);  // シリアル通信を開始
  // Serial.println();
  // Serial.println("Configuring access point...");


  // アクセスポイントを設定し、接続待機
  while (!WiFi.softAP(ssid, password)) {}
  IPAddress myIP = WiFi.softAPIP();  // アクセスポイントのIPアドレスを取得
  Serial.print("AP IP address: ");
  Serial.println(myIP);


  server.begin();  // サーバーを開始
  Serial.println("Server started");
}


void loop() {
  WiFiClient client = server.available();  // クライアントの接続を待機


  if (client) {  // クライアントが接続された場合
    Serial.println("New Client.");
    String currentLine = "";


    while (client.connected()) {  // クライアントが接続されている間
      if (client.available()) {   // クライアントからのデータが利用可能な場合
        char c = client.read();    // クライアントからのデータを読み取る
        // Serial.write(c);           // シリアルモニターにデータを出力

        if (c == '\n') {           // 改行文字が受信された場合
          if (currentLine.length() == 0) {  // 空行が受信された場合
            // HTTPレスポンスを送信
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
            client.println("<h1>ESP32 Control Panel</h1>");
            client.print("<h1>Click <a href=\"/H\">here</a> to turn ON the LED.</h1><br>");
            client.print("<h1>Click <a href=\"/L\">here</a> to turn OFF the LED.</h1><br>");
            client.println();
            break;  // ループを抜ける
          } else {
            currentLine = "";  // currentLineをリセット
          }
        }
      }
    }
    client.stop();  // クライアントの接続を閉じる
    Serial.println("Client Disconnected.");
  }
}