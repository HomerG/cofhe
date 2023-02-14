#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <asm/termios.h>
#include <unistd.h>
#include <math.h>
//#include <asm/termbits.h>

#define DEBUG
//#define BUSPIRATE
#define UART
//#define SPI

#ifdef DEBUG
#define DEBUG_PRINT(x) printf x
#else
#define DEBUG_PRINT(x)
#endif

int fd;

unsigned char dataPayload[8400*128/8];
unsigned char uart_wr_command[] = {0x34, 0x34, 0x34, 0x34};
unsigned char uart_rd_command[] = {0x4D, 0x4D, 0x4D, 0x4D};
int data_length;

int prev_w_bitwidth;
int prev_w_burst_size;
int prev_w_address;
int prev_r_bitwidth;
int prev_r_burst_size;
int prev_r_address;

unsigned char uart_commands[] = {0x6D, 0x0D, 0x33, 0x0D, 0x39, 0x0D, 0x31, 0x0D, 0x31, 0x0D, 0x31, 0x0D, 0x32, 0x0D, 0x7B, 0x0D};
unsigned char spi_commands[] =  {0x6D, 0x0D, 0x35, 0x0D, 0x34, 0x0D, 0x31, 0x0D, 0x32, 0x0D, 0x31, 0x0D, 0x32, 0x0D, 0x32, 0x0D, 0x7B, 0x0D, 0x5D, 0x0D};

void read_uart(int fd, unsigned char *in_buf, int read_length, unsigned char ascii_print);

int hex2ascii(unsigned char* out, unsigned char in) {
     int len;
     if(in < 10) {
         out[0] = 0x30 + in%10;
         len = 1;
     } else if(in < 100) {
         out[0] = 0x30 + (in/10)%10;
         out[1] = 0x30 + in%10;
         len = 2;
     } else {
         out[0] = 0x30 + (in/100);
         out[1] = 0x30 + (in/10)%10;
         out[2] = 0x30 + in%10;
         len = 3;
     }
     return len;
}

int receiveBytes(int fd, unsigned char *out, int length) {
     int i,j;
     unsigned char temp[50];
     printf("in receive bytes\n.");
     for(i=0;i<length;i++) {
         read_uart(fd, temp, 14, 0);
         out[i] = ((temp[9]-64) > 0) ? (temp[9]-55)*16 : (temp[9]-48)*16;
         out[i] += ((temp[10]-64) > 0) ? (temp[10]-55) : (temp[10]-48);
         printf("out = %02X = ",out[i]);
         //for(j=0;j<14;j++) {
         //printf(".%d.",j);
         //printf(" b = %02X:%c, ",temp[j], temp[j]);
         //}
     }
}

int receiveBytes_SPI(int fd, unsigned char *out, int length) {
     int i,j, wlen, flag;
     unsigned char hexvalue = 8;
     unsigned char temp[50], asc_value[3];
     for(i=0;i<length;i++) {
         wlen = hex2ascii(asc_value, hexvalue);
         flag = write(fd, asc_value, wlen);
         flag = write(fd, &spi_commands[1], 1);
         read_uart(fd, temp, 31+wlen, 1);
         out[i] = ((temp[23]-64) > 0) ? (temp[23]-55)*16 : (temp[23]-48)*16;
         out[i] += ((temp[24]-64) > 0) ? (temp[24]-55) : (temp[24]-48);
         //printf("out = %02X = ",out[i]);
         //for(j=0;j<32;j++) {
         //printf("[%d]",j);
         //printf(" b = %02X:%c, ",temp[j], temp[j]);
         //}
         //printf("out = %02X = ",out[i]);
     }

}

// Sends a multi-byte transfer from master to slave.  Drives CS on its own.
void SendMultiByte_UART(unsigned char *data, int length) {
    int ii;
    int wlen, flag;
    unsigned char uart_data[1256];
    unsigned char uart_temp_data[3];

    for(ii=0;ii<length;ii++) {
#ifdef BUSPIRATE
        DEBUG_PRINT(("UART write data[%d] = %d.\n",ii,data[ii]));
        wlen = hex2ascii(uart_temp_data, data[ii]);
    	flag = write(fd, uart_temp_data, wlen);
    	flag = write(fd, &uart_commands[1], 1);
    	read_uart(fd, uart_data, 22, 1);
#else
    	flag = write(fd, data[ii], wlen);
#endif
    }
    /*if(wlen != length) {
        printf("UART write not successful.\n");
    }*/
}

void SendMultiByte_SPI(unsigned char *data, int length) {
    int ii;
    int wlen, flag;
    unsigned char spi_data[1256];
    unsigned char spi_temp_data[3];

    for(ii=0;ii<length;ii++) {
        DEBUG_PRINT(("SPI write data[%d] = %d.\n",ii,data[ii]));
        wlen = hex2ascii(spi_temp_data, data[ii]);
    	flag = write(fd, spi_temp_data, wlen);
    	flag = write(fd, &spi_commands[1], 1);
    	read_uart(fd, spi_data, 31+wlen, 0);
    }
    flag = write(fd, &spi_commands[18],2);
    read_uart(fd, spi_temp_data, 21, 1);
    flag = write(fd, &spi_commands[16],2);
    read_uart(fd, spi_temp_data, 20, 1);
    /*if(wlen != length) {
        printf("UART write not successful.\n");
    }*/
}

int read_bp_command(int burst_size, int bitwidth, int start_address, unsigned char *data) {
    int ii, jj, length;

    unsigned char data2[4096*128] = {0};

    length = 1;

    if(bitwidth != prev_r_bitwidth) {
	data[0] = 7;
	data[length+0] = bitwidth/256*256*256;
	data[length+1] = bitwidth/256*256;
	data[length+2] = bitwidth/256;
	data[length+3] = bitwidth;
	length = length + 4;
	data[length+0] = burst_size/256*256*256;
	data[length+1] = burst_size/256*256;
	data[length+2] = burst_size/256;
	data[length+3] = burst_size;
	length = length + 4;
        data[length+0] = start_address/256*256*256;
        data[length+1] = start_address/256*256;
        data[length+2] = start_address/256;
        data[length+3] = start_address;
        length = length + 4;
	prev_r_bitwidth = bitwidth;
	prev_r_burst_size = burst_size;
	prev_r_address = start_address + burst_size*bitwidth/8;
    } else if(burst_size != prev_r_burst_size) {
	data[0] = 6;
	data[length+0] = burst_size/256*256*256;
	data[length+1] = burst_size/256*256;
	data[length+2] = burst_size/256;
	data[length+3] = burst_size;
	length = length + 4;
        data[length+0] = start_address/256*256*256;
        data[length+1] = start_address/256*256;
        data[length+2] = start_address/256;
        data[length+3] = start_address;
        length = length + 4;
	prev_r_burst_size = burst_size;
	prev_r_address = start_address + burst_size*bitwidth/8;
    } else if(start_address != prev_r_address) {
	data[0] = 5;
        data[length+0] = start_address/256*256*256;
        data[length+1] = start_address/256*256;
        data[length+2] = start_address/256;
        data[length+3] = start_address;
        length = length + 4;
	prev_r_address = start_address + burst_size*bitwidth/8;
    } else {
        data[0] = 4;
	prev_r_address = start_address + burst_size*bitwidth/8;
    }
    printf("read payload size = %d\n",length);
    //for(ii=0;ii<length;ii++)
    //   printf("TX[%03d] = %02X || ",ii,data[ii]);
    //printf("\n");
#ifdef UART
    SendMultiByte_UART(data, length);
#elif defined(SPI)
    SendMultiByte_SPI(data, length);
#else
      printf("Select an option for interface\n.");
#endif
    //read_uart(fd, data2, 11, 1);
#ifdef UART
    receiveBytes(fd, data, burst_size*bitwidth/8);
#elif defined(SPI)
    receiveBytes_SPI(fd, data, burst_size*bitwidth/8);
#else
      printf("Select an option for interface\n.");
#endif
    //read_uart(fd, data2, 4, 1);
    //read_uart(fd, data, burst_size*bitwidth/8*15, 1);
    //for(ii=0;ii<burst_size*bitwidth/8;ii++)
    //   printf("RX[%03d] = %02X || ",ii,data[ii]);
    printf("\n");
    return burst_size*bitwidth/8;
} //Read Command

int write_command(int burst_size, int bitwidth, int start_address, unsigned char *in_payload) {
    int ii, jj;
    int length;
    int address;
    int wlen;
    if(bitwidth%32!=0) {
       printf("Error: Hardware has 32-bt Word.\n");
       return 1;
    }
    address = start_address;
    printf("Writing UART. %d\n",fd);
    for(ii=0;ii<burst_size*bitwidth/8;ii=ii+4) {
       DEBUG_PRINT(("loop %d :",ii/4));
       DEBUG_PRINT(("UART in -> %x %x %x %x", uart_wr_command[0], uart_wr_command[1], uart_wr_command[2], uart_wr_command[3]));
       DEBUG_PRINT(("- %x %x %x %x ", address%256, (address/256)%256, (address/(256*256))%256, (address/(256*256*256))%256));
       DEBUG_PRINT(("- %x %x %x %x\n", in_payload[ii], in_payload[ii+1], in_payload[ii+2], in_payload[ii+3]));
       wlen = write(fd, (unsigned char *)uart_wr_command, 4);
       wlen = write(fd, (unsigned char *)&address, 4);
       wlen = write(fd, (unsigned char *)&in_payload[ii], 4);
       address = address + 4;
    }
}

int read_command(int burst_size, int bitwidth, int start_address, unsigned char *out_payload) {
    int ii, jj;
    int length;
    int address;
    int wlen;
    address = start_address;
    printf("Reading UART. %d\n",fd);
    if(bitwidth%32!=0) {
       printf("Error: Hardware has 32-bt Word.\n");
       return 1;
    }
    for(ii=0;ii<burst_size*bitwidth/8;ii=ii+4) {
       DEBUG_PRINT(("loop %d :",ii/4));
       DEBUG_PRINT(("UART in -> %x %x %x %x", uart_rd_command[0], uart_rd_command[1], uart_rd_command[2], uart_rd_command[3]));
       DEBUG_PRINT(("- %x %x %x %x ", address%256, (address/256)%256, (address/(256*256))%256, (address/(256*256*256))%256));
       wlen = write(fd, uart_rd_command, 4);
       wlen = write(fd, (unsigned char *)&address, 4);
       read_uart(fd, &out_payload[ii], 4, 1);
       DEBUG_PRINT(("UART out <- %x %x %x %x \n", out_payload[ii], out_payload[ii+1], out_payload[ii+2], out_payload[ii+3]));
       address = address + 4;
    }
}

int read_command_dec(int burst_size, int bitwidth, int start_address, unsigned char *out_payload) {
    int ii, jj;
    int length;
    int address;
    int rev_address, temp;
    int no_of_bits = log2(burst_size);
    int wlen;
    address = start_address;
    printf("Reading UART in bit-reverse manner. %d\n",fd);
    if(bitwidth%32!=0) {
       printf("Error: Hardware has 32-bt Word.\n");
       return 1;
    }
    for(ii=0;ii<burst_size*bitwidth/8;ii=ii+4) {
       if(ii%16 == 0) {
       rev_address = 0;
       for(jj=0;jj<no_of_bits;jj++) {
          temp = ii/16 & (1<<jj);
          if(temp)
             rev_address |= (1 << (no_of_bits - 1) - jj);
       }
       rev_address *= 16;
//       printf("rev_address[%d] = %d\n",ii/16,rev_address);
       rev_address += start_address;
       } else {
          rev_address = rev_address + 4;
       }
       DEBUG_PRINT(("loop %d :",ii/4));
       DEBUG_PRINT(("UART in -> %x %x %x %x", uart_rd_command[0], uart_rd_command[1], uart_rd_command[2], uart_rd_command[3]));
       DEBUG_PRINT(("- %x %x %x %x ", rev_address%256, (rev_address/256)%256, (rev_address/(256*256))%256, (rev_address/(256*256*256))%256));
       wlen = write(fd, uart_rd_command, 4);
       wlen = write(fd, (unsigned char *)&rev_address, 4);
       read_uart(fd, &out_payload[ii], 4, 1);

       DEBUG_PRINT(("UART out <- %x %x %x %x \n", out_payload[ii], out_payload[ii+1], out_payload[ii+2], out_payload[ii+3]));
       address = address + 4;
    }
}


int write_bp_command(int burst_size, int bitwidth, int start_address, unsigned char *in_payload) {
    int ii, jj;
    int length;

    unsigned char data[8400*128/8];

    length = 1;

    if(bitwidth != prev_w_bitwidth) {
	data[0] = 3;
	data[length+0] = bitwidth/256*256*256;
	data[length+1] = bitwidth/256*256;
	data[length+2] = bitwidth/256;
	data[length+3] = bitwidth;
	length = length + 4;
	data[length+0] = burst_size/256*256*256;
	data[length+1] = burst_size/256*256;
	data[length+2] = burst_size/256;
	data[length+3] = burst_size;
	length = length + 4;
        data[length+0] = start_address/256*256*256;
        data[length+1] = start_address/256*256;
        data[length+2] = start_address/256;
        data[length+3] = start_address;
        length = length + 4;
	prev_w_bitwidth = bitwidth;
	prev_w_burst_size = burst_size;
	prev_w_address = start_address + burst_size*bitwidth/8;
    } else if(burst_size != prev_w_burst_size) {
	data[0] = 2;
	data[length+0] = burst_size/256*256*256;
	data[length+1] = burst_size/256*256;
	data[length+2] = burst_size/256;
	data[length+3] = burst_size;
	length = length + 4;
        data[length+0] = start_address/256*256*256;
        data[length+1] = start_address/256*256;
        data[length+2] = start_address/256;
        data[length+3] = start_address;
        length = length + 4;
	prev_w_burst_size = burst_size;
	prev_w_address = start_address + burst_size*bitwidth/8;
    } else if(start_address != prev_w_address) {
	data[0] = 1;
        data[length+0] = start_address/256*256*256;
        data[length+1] = start_address/256*256;
        data[length+2] = start_address/256;
        data[length+3] = start_address;
        length = length + 4;
	prev_w_address = start_address + burst_size*bitwidth/8;
    } else {
        data[0] = 0;
	prev_w_address = start_address + burst_size*bitwidth/8;
    }

    printf("write payload size = %d burst size = %d, bitwidth = %d\n",length, burst_size, bitwidth);
    for(ii=0; ii<burst_size; ii++) {
      for(jj=0; jj<bitwidth/8; jj++) {
        data[length+ii*bitwidth/8+jj] = in_payload[ii*bitwidth/8 + jj];
      }
    }
    length = length + burst_size*bitwidth/8;
    printf("write payload size = %d\n",length);
    #ifdef UART
      SendMultiByte_UART(data, length);
    #elif defined(SPI)
      SendMultiByte_SPI(data, length);
    #else
      printf("Select an option for interface\n.");
    #endif
    //for(ii=0;ii<length;ii++)
    //   printf("TX[%03d] = %02X || ",ii,data[ii]);
    printf("\n");

    return length;
} //Write command

int set_interface_attribs(int fd, int speed)
{
    struct termios tty;

    if (tcgetattr(fd, &tty) < 0) {
        DEBUG_PRINT(("Error from tcgetattr: %s\n", strerror(errno)));
        return -1;
    }

    cfsetospeed(&tty, (speed_t)speed);
    cfsetispeed(&tty, (speed_t)speed);

    tty.c_cflag |= (CLOCAL | CREAD);    /* ignore modem controls */
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;         /* 8-bit characters */
    tty.c_cflag &= ~PARENB;     /* no parity bit */
    tty.c_cflag &= ~CSTOPB;     /* only need 1 stop bit */
    tty.c_cflag &= ~CRTSCTS;    /* no hardware flowcontrol */

    /* setup for non-canonical mode */
    tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
    tty.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
    tty.c_oflag &= ~OPOST;

    /* fetch bytes as they become available */
    tty.c_cc[VMIN] = 1;
    tty.c_cc[VTIME] = 1;

    if (tcsetattr(fd, TCSANOW, &tty) != 0) {
        DEBUG_PRINT(("Error from tcsetattr: %s\n", strerror(errno)));
        return -1;
    }
    return 0;
}

void set_mincount(int fd, int mcount)
{
    struct termios tty;

    if (tcgetattr(fd, &tty) < 0) {
        DEBUG_PRINT(("Error tcgetattr: %s\n", strerror(errno)));
        return;
    }

    tty.c_cc[VMIN] = mcount ? 1 : 0;
    tty.c_cc[VTIME] = 5;        /* half second timer */

    if (tcsetattr(fd, TCSANOW, &tty) < 0)
        DEBUG_PRINT(("Error tcsetattr: %s\n", strerror(errno)));
}

int uart_execute()
{
    //int fd;
    int wlen;
    int i;
    unsigned char commands[2*13] = {0xFA, 0xFA};
    unsigned char fsm_state[17];

    /* simple output */
    wlen = write(fd, commands, 2);
    if (wlen != 2) {
        DEBUG_PRINT(("UART: Error from write: %d, %d\n", wlen, errno));
	return -1;
    }
    DEBUG_PRINT(("FSM value is read.\n"));
    tcdrain(fd);    /* delay for output */
    DEBUG_PRINT(("FSM value is read.\n"));
    //read_uart(fd, fsm_state, 16, 1); //FPGA sends FSM value.
    DEBUG_PRINT(("FSM value is read.\n"));

	// Just run the implementation.
    	//If FPGA displays some log values, read values for analysis and debugging.
	//read_uart(fd, fsm_state, 15, 1); //FPGA sends FSM value for the IDLE state.
	sleep(1);
	//read_uart(fd, fsm_state, 16, 1); //FPGA sends FSM value for the IDLE state.
	//read_uart(fd, fsm_state, 15, 1); //FPGA sends FSM value for the IDLE state.
    return 0;

}

int uart_bp_open(unsigned char *portname) {
    int wlen, i;
    unsigned char uart_data[1256];

    fd = open(portname, O_RDWR | O_NOCTTY | O_SYNC);
    if (fd < 0) {
        DEBUG_PRINT(("UART: Error opening %s: %s\n", portname, strerror(errno)));
        return -1;
    }
    DEBUG_PRINT(("UART port open : %s\n", portname));

    /*baudrate 115200, 8 bits, no parity, 1 stop bit */
    set_interface_attribs(fd, B115200);

    wlen = write(fd, uart_commands, 2);
    read_uart(fd, uart_data, 114,1);
    DEBUG_PRINT(("UART command 1 complete.\n"));

    wlen = write(fd, uart_commands+2, 2);
    read_uart(fd, uart_data, 152, 1);
    DEBUG_PRINT(("UART command 2 complete.\n"));

    wlen = write(fd, uart_commands+4, 2);
    read_uart(fd, uart_data, 93, 1);
    DEBUG_PRINT(("UART command 3 complete.\n"));

    wlen = write(fd, uart_commands+6, 2);
    read_uart(fd, uart_data, 42, 1);
    DEBUG_PRINT(("UART command 4 complete.\n"));

    wlen = write(fd, uart_commands+8, 2);
    read_uart(fd, uart_data, 59, 1);
    DEBUG_PRINT(("UART command 5 complete.\n"));

    wlen = write(fd, uart_commands+10, 2);
    read_uart(fd, uart_data, 90, 1);
    DEBUG_PRINT(("UART command 6 complete.\n"));

    wlen = write(fd, uart_commands+12, 2);
    read_uart(fd, uart_data, 15, 1);
    DEBUG_PRINT(("UART command 7 complete.\n"));

    wlen = write(fd, uart_commands+14, 2);
    read_uart(fd, uart_data, 38, 1);
    DEBUG_PRINT(("UART command 8 complete.\n"));

    //set_mincount(fd, 0);                /* set to pure timed read */

    return 0;
}

int uart_open(unsigned char *portname) {
    int wlen, i;
#if 0
/*
    struct terminos ttycfg;

    fd = open(portname, O_RDWR | O_NOCTTY);
    tcgetattr(fd, &ttycfg);
    cfsetspeed(&ttycfg, B9600);
    printf(" %d", tcsetattr(fd, TCSANOW, &ttycfg));

    printf("\n new uart open\n");*/
#endif
#if 1
    int speed;
    speed = 1000000;
    //speed = 16000;
    fd = open(portname, O_RDWR | O_NOCTTY);
    printf("PN %s\n", portname);
    struct termios2 tio;
    ioctl(fd, TCGETS2, &tio);
    tio.c_cflag &= ~CBAUD;
    tio.c_cflag |= BOTHER;
    tio.c_ispeed = speed;
    tio.c_ospeed = speed;
    int r = ioctl(fd, TCSETS2, &tio);

    if (r == 0) {
        printf("Changed successfully %d.\n",fd);
    } else {
        perror("ioctl");
    }
#else
    fd = open(portname, O_RDWR | O_NOCTTY | O_SYNC);
    if (fd < 0) {
        DEBUG_PRINT(("UART: Error opening %s: %s\n", portname, strerror(errno)));
        return -1;
    }
    DEBUG_PRINT(("UART port open : %s\n", portname));

    set_interface_attribs(fd, B19200);
    //set_mincount(fd, 0);                /* set to pure timed read */
#endif
    return 0;
}

int spi_open(unsigned char *portname) {
    int wlen, i;
    unsigned char uart_data[1256];

    fd = open(portname, O_RDWR | O_NOCTTY | O_SYNC);
    if (fd < 0) {
        DEBUG_PRINT(("SPI: Error opening %s: %s\n", portname, strerror(errno)));
        return -1;
    }
    DEBUG_PRINT(("SPI port open : %s\n", portname));

    /*baudrate 115200, 8 bits, no parity, 1 stop bit */
    set_interface_attribs(fd, B115200);

    wlen = write(fd, spi_commands, 2);
    read_uart(fd, uart_data, 114,1);
    DEBUG_PRINT(("SPI command 1 complete.\n"));

    wlen = write(fd, spi_commands+2, 2);
    read_uart(fd, uart_data, 66, 1);
    DEBUG_PRINT(("SPI command 2 complete.\n"));

    wlen = write(fd, spi_commands+4, 2);
    read_uart(fd, uart_data, 64, 1);
    DEBUG_PRINT(("SPI command 3 complete.\n"));

    wlen = write(fd, spi_commands+6, 2);
    read_uart(fd, uart_data, 78, 1);
    DEBUG_PRINT(("SPI command 4 complete.\n"));

    wlen = write(fd, spi_commands+8, 2);
    read_uart(fd, uart_data, 60, 1);
    DEBUG_PRINT(("SPI command 5 complete.\n"));

    wlen = write(fd, spi_commands+10, 2);
    read_uart(fd, uart_data, 40, 1);
    DEBUG_PRINT(("SPI command 6 complete.\n"));

    wlen = write(fd, spi_commands+12, 2);
    read_uart(fd, uart_data, 90, 1);
    DEBUG_PRINT(("SPI command 7 complete.\n"));

    wlen = write(fd, spi_commands+14, 2);
    read_uart(fd, uart_data, 14, 1);
    DEBUG_PRINT(("SPI command 8 complete.\n"));

    wlen = write(fd, spi_commands+16, 2);
    read_uart(fd, uart_data, 20, 1);
    DEBUG_PRINT(("SPI command 9 complete.\n"));

    //set_mincount(fd, 0);                /* set to pure timed read */

    return 0;
}

void uart_close() {
    close(fd);
}

#if 0
int main() {
    //unsigned char in[16]={0x7d, 0xf7, 0x6b, 0x0c, 0x1a, 0xb8, 0x99, 0xb3, 0x3e, 0x42, 0xf0, 0x47, 0xb9, 0x1b, 0x54, 0x6f};
    unsigned char in[16]={0};
    unsigned char pt[33]={0xD8, 0x1C, 0x4D, 0x8D, 0x73, 0x4F, 0xCB, 0xFB, 0xEA, 0xDE, 0x3D, 0x3F, 0x8A, 0x03, 0x9F, 0xAA, 0x2A, 0x2C, 0x99, 0x57, 0xE8, 0x35, 0xAD, 0x55, 0xB2, 0x2E, 0x75, 0xBF, 0x57, 0xBB, 0x55, 0x6A, 0xC8 };
    unsigned char key[176]={0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x62, 0x63, 0x63, 0x63, 0x62, 0x63, 0x63, 0x63, 0x62, 0x63, 0x63, 0x63, 0x62, 0x63, 0x63, 0x63, 0x9b, 0x98, 0x98, 0xc9, 0xf9, 0xfb, 0xfb, 0xaa, 0x9b, 0x98, 0x98, 0xc9, 0xf9, 0xfb, 0xfb, 0xaa, 0x90, 0x97, 0x34, 0x50, 0x69, 0x6c, 0xcf, 0xfa, 0xf2, 0xf4, 0x57, 0x33, 0xb, 0xf, 0xac, 0x99, 0xee, 0x6, 0xda, 0x7b, 0x87, 0x6a, 0x15, 0x81, 0x75, 0x9e, 0x42, 0xb2, 0x7e, 0x91, 0xee, 0x2b, 0x7f, 0x2e, 0x2b, 0x88, 0xf8, 0x44, 0x3e, 0x9, 0x8d, 0xda, 0x7c, 0xbb, 0xf3, 0x4b, 0x92, 0x90, 0xec, 0x61, 0x4b, 0x85, 0x14, 0x25, 0x75, 0x8c, 0x99, 0xff, 0x9, 0x37, 0x6a, 0xb4, 0x9b, 0xa7, 0x21, 0x75, 0x17, 0x87, 0x35, 0x50, 0x62, 0xb, 0xac, 0xaf, 0x6b, 0x3c, 0xc6, 0x1b, 0xf0, 0x9b, 0xe, 0xf9, 0x3, 0x33, 0x3b, 0xa9, 0x61, 0x38, 0x97, 0x6, 0xa, 0x4, 0x51, 0x1d, 0xfa, 0x9f, 0xb1, 0xd4, 0xd8, 0xe2, 0x8a, 0x7d, 0xb9, 0xda, 0x1d, 0x7b, 0xb3, 0xde, 0x4c, 0x66, 0x49, 0x41, 0xb4, 0xef, 0x5b, 0xcb, 0x3e, 0x92, 0xe2, 0x11, 0x23, 0xe9, 0x51, 0xcf, 0x6f, 0x8f, 0x18, 0x8e};
    unsigned char temp[2096];
    unsigned char flag;

    flag = uart_open("/dev/ttyUSB2");

    int i;

    for(i=0;i<2095;i++) temp[i] = i;
    for(i=0;i<10;i++)
        DEBUG_PRINT(("temp[%d] = %02X\n",i, temp[i]));
    DEBUG_PRINT(("UART commands starts.\n"));

    //for(i=0;i<16;i++)
    //    in[i] =1*(i+1);
    //for(i=0;i<176;i++)
    //    key[i] =17*(i+1);
#if 0
    flag = uart_command(0,in,16);
    flag = uart_command(1,key,176);
    flag = uart_command(3,in,16);
    flag = uart_command(4,key,176);
    flag = uart_command(2,in,16);
    flag = uart_command(3,in,16);
    flag = uart_command(4,key,176);
#else
    flag = uart_read(0,temp,1420);
//    flag = uart_command(6,temp,16);
    flag = uart_read(2,temp,33);
    flag = uart_read(3,temp,2096);
    flag = uart_write(2,pt,33);
    flag = uart_read(2,temp,33);
    flag = uart_execute(temp,0);
    flag = uart_read(0,temp,1420);
    flag = uart_read(4,temp,8);
    flag = uart_read(5,temp,8);
    for(i=0;i<10;i++)
        DEBUG_PRINT(("temp[%d] = %02X\n",i, temp[i]));
    //flag = uart_command(7,temp,8);
    //flag = uart_command(8,temp,33);
    //flag = uart_command(9,temp,2096);
#endif
    DEBUG_PRINT(("UART commands are over.\n"));
    uart_close();
    return 0;
}
#endif
int interface_open(unsigned char *portname) {
#if defined(UART) && defined(BUSPIRATE)
    printf("1\n");
    return uart_bp_open(portname);
#elif defined(UART)
    printf("2\n");
    return uart_open(portname);
#elif defined(SPI)
    printf("3\n");
    return spi_open(portname);
#else
      printf("Select an option for interface\n.");
#endif

}

void read_uart(int fd, unsigned char *in_buf, int read_length, unsigned char ascii_print) {
    /*View Input */
    unsigned char *buf;
    int rdlen = 0, temp, idx = 0;
    unsigned char *p;

    buf = (unsigned char *)malloc(read_length);

    do {
	//DEBUG_PRINT(("Input Read length %d : ", read_length));
	temp = 0;
        temp = read(fd, buf, read_length-rdlen);
	rdlen += temp;
        if (rdlen > 0) {
	    if(ascii_print)
		DEBUG_PRINT(("Read %d: ", rdlen));
            for (p = buf; temp-- > 0; p++) {
	        //if(idx < read_length+17 && idx > 16) {
	            in_buf[idx++] = *p;
		    //DEBUG_PRINT((" "));
	            //DEBUG_PRINT((" 0x%x", *p));
	        //}
		if(ascii_print) {
		    //DEBUG_PRINT(("%c", *p));
		    DEBUG_PRINT(("%d ", *p));
		    //DEBUG_PRINT(("%d", temp));
		}
		//else
		//    DEBUG_PRINT((" %02x", *p));
	    }
	    if(ascii_print)
		DEBUG_PRINT(("\n"));
        } else if (rdlen < 0) {
            DEBUG_PRINT(("Error from read: %d: %s\n", rdlen, strerror(errno)));
        } else {  /* rdlen == 0 */
            // DEBUG_PRINT(("Timeout from read\n"));
        }
        /* repeat read to get full message */
    } while (rdlen < read_length);

    free(buf);
}
