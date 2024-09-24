#include "basestation_SOH.h"

void basestation_SOH(SocketWrapper sock)
{

    // Do-while loop to send and receive data
    char buf[BUF_LEN];
    // byte array
    unsigned char b[] = { 0x7e, 0x17, 0x0e, 0x00, 0x7e, 0x0a };
    // recasting byte array as char
    const char* message = reinterpret_cast<const char*>(b);
    int i = 1;

    timeval timey;

    timey.tv_sec = SOCK_TIMEOUT_SECONDS;
    timey.tv_usec = SOCK_TIMEOUT_MICROSECONDS;

    do
    {
        int retries = RETRIES;   

        if (send_tcp(sock, message, retries, timey, buf, 6)) {
          bciout << "basestation SOH success";

          basestation_SOH_packet* data = reinterpret_cast<basestation_SOH_packet*>(buf);

          printf("begin: 0x%x \n", data->A.bgn);
          printf("op: 0x%x \n", data->A.op);
          printf("seqaddress: 0x%x \n", data->A.seqaddress);
          printf("dlen: 0x%x \n", data->A.dlen);

          printf("device type: %u \n", data->B.device_type);
          printf("batmon V: %f \n", (data->B.batmon_V) / 256.0);
          printf("batmon T: %u \n", data->B.batmon_T);
          printf("V fault: %u \n", data->B.V_fault);
          printf("T fault: %u \n", data->B.T_fault);


          printf("crc: 0x%x \n", data->C.crc);
          printf("end: 0x%x \n", data->C.end);
        }
        else {
          bciwarn << "basestation SOH failed!";
        }


        i = 0;
    } while (i > 0);
}
