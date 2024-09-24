#include "powerpad_SOH.h"

void powerpad_SOH(SocketWrapper sock)
{
    timeval timey;

    timey.tv_sec = SOCK_TIMEOUT_SECONDS;
    timey.tv_usec = SOCK_TIMEOUT_MICROSECONDS;

    // Do-while loop to send and receive data
    char buf[BUF_LEN];
    // byte array
    unsigned char a[] = { 0x7e, 0x14, 0x0e, 0x03, 0x10, 0x02, 0x46, 0xd9, 0x0a };
    unsigned char b[] = { 0x7e, 0x17, 0x0a, 0x00, 0x69, 0x0a };
    // recasting byte array as char
    const char* p = reinterpret_cast<const char*>(a);
    const char* q = reinterpret_cast<const char*>(b);
    int i = 1;

    do
    {
        int retries = RETRIES;
        
        if (send_tcp(sock, p, retries, timey, buf, 9)) {
          if (send_tcp(sock, q, retries, timey, buf, 6)) {
            bciwarn << "powerpad SOH success";

            powerpad_SOH_packet* data = reinterpret_cast<powerpad_SOH_packet*>(buf);
            printf("begin: 0x%x \n", data->A.bgn);
            printf("op: 0x%x \n", data->A.op);
            printf("seqaddress: 0x%x \n", data->A.seqaddress);
            printf("dlen: 0x%x \n", data->A.dlen);

            printf("device type: %u \n", data->B.device_type);
            printf("batmon V: %f \n", (data->B.batmon_V) / 256.0);
            printf("batmon T: %u \n", data->B.batmon_T);
            printf("Vin: %u \n", data->B.Vin_uVadc);
            printf("T fault: %u \n", data->B.T_fault);


            printf("crc: 0x%x \n", data->C.crc);
            printf("end: 0x%x \n", data->C.end);

            
          }
          else {
            bciwarn << "second powerpad SOH send failed";
          }
        }
        else {
          bciwarn << "first powerpad SOH send failed";
        }

        i = 0;
    } while (i > 0);
}
