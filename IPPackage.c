//
//  main.c
//  IPPackage
//
//  Created by Stephan Warren on 1/8/15.
//  Copyright (c) 2015 Stephan Warren. All rights reserved.
//

#include <stdio.h>


int main(int argc, const char * argv[]) {

    short src[4], dst[4];
    unsigned char header[24];
    unsigned short len;
    unsigned short temp;
    unsigned short const little_endian = 1;
    
    
    FILE *file = fopen(argv[1], "r");
    while(!feof(file)) {
        fscanf(file, "%hd.%hd.%hd.%hd ", &src[0], &src[1], &src[2], &src[3]);
        fscanf(file, "%hd.%hd.%hd.%hd ", &dst[0], &dst[1], &dst[2], &dst[3]);
        
        for(char i = 0; i < 24; i++) {
            fscanf(file, "%02hx ", &temp);
            header[i] = *((unsigned char *) &temp);
//            printf("Header[%d] = %02x\n", i, header[i]);
        }
        if(*((unsigned char *) &little_endian)) {
            *((unsigned char *) &len) = header[3];
            *(1 + (unsigned char *) &len) = header[2];
        }
        else {
            *((unsigned char *) &len) = header[2];
            *(1 + (unsigned char *) &len) = header[3];
        }
        for(int i = 0; i < len - 24; i++) {
            fscanf(file, "%02hx ", &temp);
//            printf("packet[%d, %04x] = %02x\n", i, i, temp1);
        }
//        printf("Len = %d, %04x\n", len, len);
        header[12] = (unsigned char) src[0];
        header[13] = (unsigned char) src[1];
        header[14] = (unsigned char) src[2];
        header[15] = (unsigned char) src[3];

        header[16] = (unsigned char) dst[0];
        header[17] = (unsigned char) dst[1];
        header[18] = (unsigned char) dst[2];
        header[19] = (unsigned char) dst[3];
    
        unsigned int sum = 0;
        header[10] = header[11] = 0;
        for(char i = 0; i < 20; i += 2) {
            if(*((unsigned char *) &little_endian)) {
                *((unsigned char *) &temp) = header[i+1];
                *(1 + (unsigned char *) &temp) = header[i];
            }
            else {
                *((unsigned char *) &temp) = header[i];
                *(1 + (unsigned char *) &temp) = header[i+1];
            }
            sum += temp;
//            printf("sum = %04x ~%04x, temp= %04x\n", sum, ~sum, temp);
        }
        sum = ~((sum & 0xffff) + (sum >> 16));
//        printf("partial sum = %04x ~%04x, temp= %04x\n", sum, ~sum, temp);
        
//        printf("sum = %04x\n", sum);
//        printf("Want 11 cd\n");
        
        if(*((unsigned char *) &little_endian)) {
            header[10] = *(1 + (unsigned char *) &sum);
            header[11] = *(0 + (unsigned char *) &sum);
        }
        else {
            header[10] = *(3 + (unsigned char *) &sum);
            header[11] = *(2 + (unsigned char *) &sum);
        }
        
        
        char * space = "";
        for(int i = 0; i < 20; i++) {
            printf("%s%02x", space, header[i]);
            space = " ";
        }
        printf("\n");
   
    }
    
    
    
    return 0;
}
