#include "uart_access.h"
#include "cofhe_tasks.h"

int cofhe_init(unsigned char *portname) {
    int wlen;
    unsigned char signature[4];
    if(interface_open(portname)) {
        printf("UART port is not opening.\n");
        return - 1;
    }
#if 0
    while(flag) {
    printf("Enter port number:");
    scanf("%s",portno);
    strcpy(portname, "/dev/ttyUSB");
    strcat(portname,portno);
    flag = interface_open(portname);
    }
#endif
    wlen = read_command(1,32,0x420000CC,signature);
    if((signature[0] != 0x1) && (signature[1] != 0x3) &&(signature[2] != 0xc5) &&(signature[3] != 0xc)) {
         printf("signature is not matching. There is error in the interface or FPGA.\n");
         printf("signature should be = 0cc50301 and signature value is %02x%02x%02x%02x\n",signature[3],signature[2],signature[1],signature[0]);
         return -1;
    }
    printf("signature is matched.\n");
    return 0;
}

int fhe_ntt (int poly_degree, int poly_width, unsigned char * mod, unsigned char * in, unsigned char *twdl, unsigned char *out) {
    unsigned int val0 = 0x00000000;
    unsigned int val1 = 0x0012001a;
    unsigned int val2 = 0x0010001a;
    unsigned int val3 = 0x00003001;
    unsigned int val4 = 0x00002ffe;
    unsigned int val5 = 0x24232121;
    unsigned int val6 = 0x0040000 + poly_degree;
    unsigned int val7 = 0x41242121;
    unsigned int val8 = 0x0012001a;
    unsigned int temp = 0x0;
    int wlen;

    wlen = write_command(1,32,0x42000018, (unsigned char *)&val1);
    wlen = write_command(1,32,0x42000008, (unsigned char *)&val2);
    wlen = write_command(1,32,0x42009000, &mod[0]);
    wlen = write_command(1,32,0x42009004, &mod[4]);
    wlen = write_command(1,32,0x42009008, &mod[8]);
    wlen = write_command(1,32,0x4200900c, &mod[12]);
    wlen = write_command(1,32,0x42009080, (unsigned char *)&val4);
    wlen = write_command(1,32,0x42009084, (unsigned char *)&val0);
    wlen = write_command(1,32,0x42009088, (unsigned char *)&val0);
    wlen = write_command(1,32,0x4200908c, (unsigned char *)&val0);


    printf("print temp value = %x\n",temp);
    read_command(1,32,0x42000018, (unsigned char *)&temp);
    printf("print temp value = %x\n",temp);
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

    wlen = write_command(poly_degree,poly_width,0x21000000,in);
    wlen = write_command(poly_degree,poly_width,0x23000000,twdl);
    //wlen = read_command(poly_degree,poly_width,0x21000000,in);
    wlen = write_command(1,32,0x420000a8, (unsigned char *)&val5);
    wlen = write_command(1,32,0x42008004, (unsigned char *)&val6);
    wlen = write_command(1,32,0x42008018, (unsigned char *)&val7);
    wlen = read_command(1,32,0x42009b04, (unsigned char *)&temp);
    sleep(2);
    wlen = read_command_dec(poly_degree,poly_width,0x21000000,out);

}


int fhe_poly_mul (int poly_degree, int poly_width, unsigned char * mod, unsigned char * in1, unsigned char *in2, unsigned char *twdl, unsigned char *out) {
    unsigned int val0 = 0x00000000;
    unsigned int val1 = 0x0012001a;
    unsigned int val2 = 0x0010001a;
    unsigned int val3 = 0x00003001;
    unsigned int val4 = 0x00002ffe;
    unsigned int val5 = 0x24232121;
    unsigned int val6 = 0x00400000 + poly_degree;
    unsigned int val7 = 0x41242121; //ntt on Mem 1
    unsigned int val8 = 0x41242222; //ntt on Mem 2
    unsigned int val9 = 0x43242221; //Multiplication
    unsigned int val10 = 0x46212424; //Constant Multplication
    unsigned int val11 = 0x02222121; //Inverse NTT
    int wlen;

    wlen = write_command(1,32,0x42000018, (unsigned char *)&val1);
    wlen = write_command(1,32,0x42000008, (unsigned char *)&val2);
    wlen = write_command(1,32,0x42009000, &mod[0]);
    wlen = write_command(1,32,0x42009004, &mod[4]);
    wlen = write_command(1,32,0x42009008, &mod[8]);
    wlen = write_command(1,32,0x4200900c, &mod[12]);
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

    wlen = write_command(poly_degree,poly_width,0x21000000,in1);
    wlen = write_command(poly_degree,poly_width,0x22000000,in2);
    wlen = write_command(poly_degree,poly_width,0x23000000,twdl);
    //wlen = read_command(poly_degree,poly_width,0x21000000,in);
    wlen = write_command(1,32,0x420000a8, (unsigned char *)&val5);
    wlen = write_command(1,32,0x42008004, (unsigned char *)&val6);
    wlen = write_command(1,32,0x42008018, (unsigned char *)&val7);
    wlen = read_command(1,32,0x42009b04,out);
    sleep(1);
    wlen = write_command(1,32,0x42008018, (unsigned char *)&val8);
    sleep(1);
    wlen = write_command(1,32,0x42008018, (unsigned char *)&val9);
    sleep(1);
    wlen = write_command(1,32,0x42008018, (unsigned char *)&val10);
    sleep(1);
    wlen = write_command(1,32,0x42008018, (unsigned char *)&val11);
    sleep(1);
    wlen = read_command(poly_degree,poly_width,0x21000000,out);

}
