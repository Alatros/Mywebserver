#include <iostream>
#include <unistd.h>
#include <string.h>
#include "src/Connection.h"
#include "src/util.h"
#include "src/Buffer.h"
#include "src/InetAddress.h"
#include "src/Socket.h"

using namespace std;

int main() {
  Socket *sock = new Socket();
  sock->connect("127.0.0.1", 8888);

  Connection *conn = new Connection(nullptr,sock);

  while (true) {
    conn->getSendBuffer()->getline();
    conn->write();
    if (conn->getState() == Connection::State::Closed) {
      conn->close();
      break;
    }
    conn->read();
    std::cout << "Message from server: " << conn->getReadBuffer()->getBuffer() << std::endl;
  }

  delete conn;
  return 0;
}
