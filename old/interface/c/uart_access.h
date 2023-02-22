int interface_open(unsigned char *portname);
int uart_execute();
int write_bp_command(int burst_size, int bitwidth, int start_address, unsigned char *data);
int read_bp_command(int burst_size, int bitwidth, int start_address, unsigned char *data);
int write_command(int burst_size, int bitwidth, int start_address, unsigned char *data);
int read_command(int burst_size, int bitwidth, int start_address, unsigned char *data);
void uart_close();
