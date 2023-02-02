#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>

int main()
{


    /*
    Si la conversion avec htonl change le resulat 
        la machine est little endian
    Si la conversion avec htonl ne change pas le resultat
        la machine est big endian
    */
    uint32_t x = 0x12345678;
    uint32_t y = htonl(x);
    if (x == y) {
        printf("Machine is big-endian\n");
    } else {
        printf("Machine is little-endian\n");
    }

    /*
    uint32_t x = 0x12345678;
    char* bytes = (char*) &x;
    for (int i = 0; i < sizeof x; i++) {
        printf("%x ", bytes[i] & 0xff);
    }
    printf("\n");
    */



   return 0;
}