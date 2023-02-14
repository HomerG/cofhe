#include <stdlib.h>
#include <stdio.h>
//#include <conio.h>
#include <string.h>
#include "uart_access.h"
#include "cofhe_header.h"
#define DEBUG


#define POLY_WIDTH 128
#define POLY_DEGREE 4096

int main()
{
    int addr, poly_width, poly_degree;
    unsigned char in[POLY_DEGREE*POLY_WIDTH/8];
    unsigned char out[POLY_DEGREE*POLY_WIDTH/8];
    unsigned int reg_addr[POLY_DEGREE*POLY_WIDTH/8];
    unsigned int reg_value[POLY_DEGREE*POLY_WIDTH/8];
    unsigned char mem[8][POLY_DEGREE*POLY_WIDTH/8];
    unsigned char mem1[POLY_DEGREE*POLY_WIDTH/8];
    unsigned char mem2[POLY_DEGREE*POLY_WIDTH/8];
    unsigned char mem3[POLY_DEGREE*POLY_WIDTH/8];
    unsigned char mem4[POLY_DEGREE*POLY_WIDTH/8];
    unsigned char mem5[POLY_DEGREE*POLY_WIDTH/8];
    unsigned char mem6[POLY_DEGREE*POLY_WIDTH/8];
    unsigned char mem7[POLY_DEGREE*POLY_WIDTH/8];
    unsigned char mod[16] = {0};
    unsigned char signature[4];
    unsigned char portno[4] ={'1','\0', '0', '0'},portname[100];
    unsigned int data_in, mem_addr, mem_idx, mem_value;
    unsigned char * data_in_temp;
    int i,j, wlen, reg_idx=0;
    FILE *fr;
    char c;

    data_in_temp = &data_in;

    fr = fopen("./uart_in_poly_mul.txt","r");

    if(fr == NULL) {
      printf("File open Error!");
      exit(1);
    }

    while(fscanf(fr, "%08x\n", &data_in) == 1) {
//         printf("File read = %x \n",data_in);
         if(data_in == 0x4d4d4d4d) { //Data Read
           if(fscanf(fr, "%08x\n", &data_in) == 1) {
             data_in_temp[1] = data_in_temp[1]^data_in_temp[2];
             data_in_temp[2] = data_in_temp[1]^data_in_temp[2];
             data_in_temp[1] = data_in_temp[1]^data_in_temp[2];
             data_in_temp[0] = data_in_temp[0]^data_in_temp[3];
             data_in_temp[3] = data_in_temp[0]^data_in_temp[3];
             data_in_temp[0] = data_in_temp[0]^data_in_temp[3];
             //if(data_in > 0x42000000)
             //printf("Reading Register value = %x\n",data_in);
             //else
             //printf("Reading Memory %x -> Address -> %x\n",data_in/(256*256*256)-0x20,data_in%(256*256*256));
           }
         } else if(data_in == 0x34343434) { // Data Write
           if(fscanf(fr, "%x\n", &data_in) == 1) {
             data_in_temp[1] = data_in_temp[1]^data_in_temp[2];
             data_in_temp[2] = data_in_temp[1]^data_in_temp[2];
             data_in_temp[1] = data_in_temp[1]^data_in_temp[2];
             data_in_temp[0] = data_in_temp[0]^data_in_temp[3];
             data_in_temp[3] = data_in_temp[0]^data_in_temp[3];
             data_in_temp[0] = data_in_temp[0]^data_in_temp[3];
             if(data_in > 0x42000000) {
             	reg_addr[reg_idx] = data_in;
             	if(fscanf(fr, "%x\n", &data_in) == 1) {
             		data_in_temp[1] = data_in_temp[1]^data_in_temp[2];
             		data_in_temp[2] = data_in_temp[1]^data_in_temp[2];
             		data_in_temp[1] = data_in_temp[1]^data_in_temp[2];
             		data_in_temp[0] = data_in_temp[0]^data_in_temp[3];
             		data_in_temp[3] = data_in_temp[0]^data_in_temp[3];
             		data_in_temp[0] = data_in_temp[0]^data_in_temp[3];
               		reg_value[reg_idx] = data_in;
               		// printf("%x: reg_value[%x] = %08x\n",reg_idx, reg_addr[reg_idx], reg_value[reg_idx]);
                        reg_idx++;
             	}
             } else {
             	printf("Writing Memory %d -> Address -> %x\n",data_in/(256*256*256)-0x20,data_in%(256*256*256));
             	mem_idx = data_in/(256*256*256)-0x20;
             	mem_addr = data_in%(256*256*256);
             	if(fscanf(fr, "%x\n", &data_in) == 1) {
             		data_in_temp[1] = data_in_temp[1]^data_in_temp[2];
             		data_in_temp[2] = data_in_temp[1]^data_in_temp[2];
             		data_in_temp[1] = data_in_temp[1]^data_in_temp[2];
             		data_in_temp[0] = data_in_temp[0]^data_in_temp[3];
             		data_in_temp[3] = data_in_temp[0]^data_in_temp[3];
             		data_in_temp[0] = data_in_temp[0]^data_in_temp[3];
               		mem_value = data_in;
               		mem[mem_idx][mem_addr] = mem_value;
               		mem[mem_idx][mem_addr+1] = (mem_value/256)%256;
               		mem[mem_idx][mem_addr+2] = (mem_value/(256*256))%256;
               		mem[mem_idx][mem_addr+3] = (mem_value/(256*256*256))%256;
               		// printf("mem[%d][%x] = %08x: %x %x %x %x\n",mem_idx, mem_addr, mem_value, mem[mem_idx][mem_addr], mem[mem_idx][mem_addr+1], mem[mem_idx][mem_addr+2], mem[mem_idx][mem_addr+3]);
             	}
             }
           }
         }

    }
    poly_width = POLY_WIDTH;
    poly_degree = POLY_DEGREE;

    for(i=0;i<poly_degree;i++) {
	for(j=0;j<poly_width/8;j++) {
	    in[i*poly_width/8+j] = 0x55 + i*poly_width/8+j;
	    out[i*poly_width/8+j] = 0;
	}
    }
    for(i=0;i<poly_degree;i++) {
	for(j=0;j<poly_width/8;j++) {
	    //printf("out[%03d] = %02x, in[%03d]=%02x ||   ", i*poly_width/8+j, out[i*poly_width/8+j], i*poly_width/8+j, in[i*poly_width/8+j]);
	}
    //printf("\n");
    }
#if 1
    int flag;
    strcpy(portname, "/dev/ttyUSB");
    strcat(portname,portno);
    flag = interface_open(portname);


    //write_command(1,32,8,dataPayload, dataLength);
    //flag = uart_open("/dev/ttyUSB0");
    if(cofhe_init(portname)) {
       printf("Error: UART init.\n");
       return -1;
    }
//    while(flag) {
//    printf("Enter port number:");
//    scanf("%s",portno);
//    strcpy(portname, "/dev/ttyUSB");
//    strcat(portname,portno);
//    flag = interface_open(portname);
//    }
#if 1
//    wlen = read_command(1,32,0x420000CC,signature);
//    if((signature[0] != 0x1) && (signature[1] != 0x3) &&(signature[2] != 0xc5) &&(signature[3] != 0xc)) {
//         printf("signature is not matching. There is error in the interface or FPGA.\n");
//         printf("signature should be = 0cc50301 and signature value is %02x%02x%02x%02x\n",signature[3],signature[2],signature[1],signature[0]);
//         return -1;
//    }
    printf("signature is matched.\n");
    mod[0] = 0x01;
    mod[1] = 0x30;
    // return 1;

    //fhe_ntt(poly_degree, poly_width, mod, mem[1], mem[3], mem[2]);
    fhe_poly_mul(poly_degree, poly_width, mod, mem[1], mem[2], mem[3], mem[2]);
    for(i=0;i<poly_degree;i++) {
            printf("out[%d] =%02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x %02x\n",i,mem[2][i*16],mem[2][i*16+1],mem[2][i*16+2],mem[2][i*16+3],mem[2][i*16+4],mem[2][i*16+5],mem[2][i*16+6],mem[2][i*16+7],mem[2][i*16+8],mem[2][i*16+9],mem[2][i*16+10],mem[2][i*16+11],mem[2][i*16+12],mem[2][i*16+13],mem[2][i*16+14],mem[2][i*16+15]);
    }
    // return 1;

#if 0
    unsigned int val0 = 0x00000000;
    unsigned int val1 = 0x0012001a;
    unsigned int val2 = 0x0010001a;
    unsigned int val3 = 0x00003001;
    unsigned int val4 = 0x00002ffe;
    unsigned int val5 = 0x24232121;
    unsigned int val6 = 0x00401000;
    unsigned int val7 = 0x41242121;
    unsigned int val8 = 0x0012001a;
    wlen = write_command(1,32,0x42000018, (unsigned char *)&val1);
    wlen = write_command(1,32,0x42000008, (unsigned char *)&val2);
    wlen = write_command(1,32,0x42009000, (unsigned char *)&val3);
    wlen = write_command(1,32,0x42009004, (unsigned char *)&val0);
    wlen = write_command(1,32,0x42009008, (unsigned char *)&val0);
    wlen = write_command(1,32,0x4200900c, (unsigned char *)&val0);
    wlen = write_command(1,32,0x42009080, (unsigned char *)&val4);
    wlen = write_command(1,32,0x42009084, (unsigned char *)&val0);
    wlen = write_command(1,32,0x42009088, (unsigned char *)&val0);
    wlen = write_command(1,32,0x4200908c, (unsigned char *)&val0);


//    read_command(1,32,0x42000018, (unsigned char *)&val1);
//    read_command(1,32,0x42000008, (unsigned char *)&val2);
//    read_command(1,32,0x42009000, (unsigned char *)&val3);
//    read_command(1,32,0x42009004, (unsigned char *)&val0);
//    read_command(1,32,0x42009008, (unsigned char *)&val0);
//    read_command(1,32,0x4200900c, (unsigned char *)&val0);
//    read_command(1,32,0x42009080, (unsigned char *)&val4);
//    read_command(1,32,0x42009084, (unsigned char *)&val0);
//    read_command(1,32,0x42009088, (unsigned char *)&val0);
//    read_command(1,32,0x4200908c, (unsigned char *)&val0);
//a: reg_value[420000a8] = 24232121
//b: reg_value[42008004] = 00401000
//c: reg_value[42008018] = 41242121
//Reading Register value = 42009b04

    wlen = write_command(poly_degree,poly_width,0x21000000,mem[1]);
    wlen = write_command(poly_degree,poly_width,0x23000000,mem[3]);
    wlen = read_command(poly_degree,poly_width,0x21000000,mem[2]);
    wlen = write_command(1,32,0x420000a8, (unsigned char *)&val5);
    wlen = write_command(1,32,0x42008004, (unsigned char *)&val6);
    wlen = write_command(1,32,0x42008018, (unsigned char *)&val7);
    wlen = read_command(1,32,0x42009b04,out);
    sleep(10);
    wlen = read_command_dec(poly_degree,poly_width,0x21000000,mem[2]);
#endif

#else
    wlen = write_bp_command(poly_degree,poly_width,0x21000000,in);
    wlen = read_bp_command(poly_degree,poly_width, 0x21000000,out);
#endif
    //unsigned long long *cycle_count;
    //flag = uart_open("/dev/ttyUSB1");
    //flag = uart_write(1,(unsigned char *)&in_val,16);
    //flag = uart_write(3,(unsigned char *)ss,CRYPTO_BYTES);
    //flag = uart_write(5,(unsigned char *)DRBG_ctx.V,16);
    //flag = uart_write(6,(unsigned char *)DRBG_ctx.Key,32);
    //flag = uart_write(7,(unsigned char *)zetas,128*2);
    //flag = uart_write(8,(unsigned char *)zetas_inv,128*2);
    //flag = uart_execute();
    //flag = uart_read(2,(unsigned char *)ct,CRYPTO_CIPHERTEXTBYTES);
    //flag = uart_write(3,(unsigned char *)pk,CRYPTO_PUBLICKEYBYTES);
    //flag = uart_write(4,(unsigned char *)sk,CRYPTO_SECRETKEYBYTES);
    //flag = uart_write(4,(unsigned char *)ss1,CRYPTO_BYTES);
    //flag = uart_read(0,(unsigned char *)o_data,16);
    //cycle_count = &o_data[8];
    //return_value = &o_data[0];
    //printf("cycle count = %llu\n",*cycle_count);
    uart_close();
    //return *return_value;
#else
    for(i=0;i<poly_degree;i++) {
	for(j=0;j<poly_width/8;j++) {
	    out[i*poly_width/8+j] = in[i*poly_width/8+j];
	}
    }
#endif

#if 0
    if(!memcmp(mem[1],mem[2],poly_degree*poly_width/8)) {
	    //for(i=0;i<poly_degree;i++) {
		//for(j=0;j<poly_width/8;j++) {
		  //  printf("out[%03d] = %02x, in[%03d]=%02x ||   ", i*poly_width/8+j, out[i*poly_width/8+j], i*poly_width/8+j, in[i*poly_width/8+j]);
		//}
	    //printf("\n");
	    //}
	printf("Out and in are equal.");
    } else {
	    for(i=0;i<poly_degree;i++) {
		for(j=0;j<poly_width/8;j++) {
                    if(mem[1][i*poly_width/8+j] != mem[2][i*poly_width/8+j])
		    	printf("out[%03d] = %02x, in[%03d]=%02x ||   ", i*poly_width/8+j, mem[2][i*poly_width/8+j], i*poly_width/8+j, mem[1][i*poly_width/8+j]);
		}
	    printf("\n");
	    }
	    printf("Out and in are NOT equal.\n");
    }
#endif
    return 0;
}
