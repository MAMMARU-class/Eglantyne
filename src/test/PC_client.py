import socket
import time

host = "192.168.4.2"  # ESPのIPアドレス
port = 12345          # ESPのポート番号

# ソケットを作成
client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client_socket.connect((host, port))

# データを送信
# message = "Hello from PC"
# client_socket.sendall(message.encode())

while True:
    # ESPからのレスポンスを受信
    response = client_socket.recv(1024)
    print("レスポンス:", response.decode())
    time.sleep(1)

# ソケットを閉じる
client_socket.close()