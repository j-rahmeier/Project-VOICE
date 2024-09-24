#include "stim_enable.h"

#pragma pack(1)

// Setup timeval variable

bool stim_enable(SocketWrapper socky, char* params)
{
  string ip = "192.168.1.10";
  int port = 3031;

  SocketWrapper sock;

  // Initiate socket
  sock.Create_TCP(port, ip);

  // Connect to server
  sock.Connect();
  // set selection wait window
  timeval timey;

  timey.tv_sec = SOCK_TIMEOUT_SECONDS;
  timey.tv_usec = SOCK_TIMEOUT_MICROSECONDS;

  int implant_address = int(params[8]);

  char buf[BUF_LEN];
  // message byte arrays

  unsigned char a[] = { 0x7e, 0x02, 0x0e, 0x00, 0x3c, 0x0a }; // version, probably don't need
  unsigned char b[] = { 0x7e, 0x27, 0x1e, 0x01, 0x02, 0x87, 0x0a }; // reserved, probably don't need

  unsigned char c[] = { 0x7e, 0x14, 0x2e, 0x03, 0x10, 0x01, 0x28, 0xf7, 0x0a }; // radio change to PM, target frequency channel is 0x28 (40)
  unsigned char d[] = { 0x7e, 0x14, 0x3e, 0x03, 0x10, 0x02, 0x46, 0x50, 0x0a }; // radio change to HSM, target frequency channel is 0x46 (70)

  unsigned char e[] = { 0x7e, 0x02, 0x0a, 0x00, 0x2b, 0x0a }; // version, probably don't need
  unsigned char f[] = { 0x7e, 0x27, 0x1a, 0x01, 0x02, 0x62, 0x0a }; // reserved, probably don't need

  unsigned char g[] = { 0x7e, 0x14, 0x4e, 0x03, 0x10, 0x01, 0x0a, 0x06, 0x0a }; // radio change to PM, target frequency channel is 0x0a

  unsigned char h[] = { 0x7e, 0x02, 0x09, 0x00, 0xb1, 0x0a }; // version, probably don't need
  unsigned char i[] = { 0x7e, 0x27, 0x19, 0x01, 0x02, 0x5a, 0x0a }; // reserved, probably don't need

  unsigned char j[] = { 0x7e, 0x14, 0x5e, 0x03, 0x10, 0x01, 0x0a, 0xc4, 0x0a }; // radio change to PM, target frequency channel is 0x0a (10) !!
  unsigned char k[] = { 0x7e, 0x1f, 0x29, 0x02, 0x10, 0x02, 0x44, 0x0a }; // radio change of IMPLANT to HSM/active mode !!
  unsigned char l[] = { 0x7e, 0x14, 0x6e, 0x03, 0x10, 0x02, 0x0a, 0xd7, 0x0a }; // radio change to HSM, target frequency channel is 0x0a (10) !!
  unsigned char m[] = { 0x7e, 0x01, 0x39, 0x00, 0x6d, 0x0a }; // NOP, probably don't need !!

  unsigned char n[] = { 0x7e, 0x0f, 0x49, 0x09, 0x09, 0x28, 0x23, 0x00, 0x00, 0xd8, 0xdc, 0xff, 0xff, 0xb7, 0x0a }; // Enable INTAN high-voltage rails for stim
  unsigned char o[] = { 0x7e, 0x1a, 0x59, 0x13, 0x01, params[1], 0x00, params[2], 0x00, params[3], 0x00, 0x00, 0x00, 0xff, 0xff,
     params[0], 0x00, params[4], params[5], params[6], params[7], 0x01, 0x02, 0xfc, 0x0a }; // Enable STIM, {0x04,... 0x02} are stimulation parameters to pass through with enable.

  // recasting byte array as char
  const char* aa = reinterpret_cast<const char*>(a);
  const char* bb = reinterpret_cast<const char*>(b);
  const char* cc = reinterpret_cast<const char*>(c);
  const char* dd = reinterpret_cast<const char*>(d);
  const char* ee = reinterpret_cast<const char*>(e);
  const char* ff = reinterpret_cast<const char*>(f);
  const char* gg = reinterpret_cast<const char*>(g);
  const char* hh = reinterpret_cast<const char*>(h);
  const char* ii = reinterpret_cast<const char*>(i);
  const char* jj = reinterpret_cast<const char*>(j);
  const char* kk = reinterpret_cast<const char*>(k);
  const char* ll = reinterpret_cast<const char*>(l);
  const char* mm = reinterpret_cast<const char*>(m);
  const char* nn = reinterpret_cast<const char*>(n);
  const char* oo = reinterpret_cast<const char*>(o);

  int iter = 1;

  bool success = false;

  // send messages to enable recording
  do
  {
    int retries = RETRIES;

    send_tcp(sock, aa, retries, timey, buf, 6);

    send_tcp(sock, bb, retries, timey, buf, 7);

    send_tcp(sock, cc, retries, timey, buf, 9);

    send_tcp(sock, dd, retries, timey, buf, 9);

    send_tcp(sock, ee, retries, timey, buf, 6);

    send_tcp(sock, ff, retries, timey, buf, 7);

    send_tcp(sock, gg, retries, timey, buf, 9);

    send_tcp(sock, hh, retries, timey, buf, 6);

    send_tcp(sock, ii, retries, timey, buf, 7);

    send_tcp(sock, jj, retries, timey, buf, 9);

    send_tcp(sock, kk, retries, timey, buf, 8);

    send_tcp(sock, ll, retries, timey, buf, 9);

    send_tcp(sock, mm, retries, timey, buf, 6);

    send_tcp(sock, nn, retries, timey, buf, 15);

    send_tcp(sock, oo, retries, timey, buf, 25);

    iter = 0;
  } while (iter > 0);

  return success;

}
