#include "termios.hpp"

namespace termios
{

bool connect(int & handler, const char * portname, int baudrate)
{
    handler = open(portname, O_RDWR | O_NOCTTY);
    if (!handler) return false;

    struct termios config;
    tcgetattr(handler, &config);

    cfsetispeed(&config, baudrate);
    cfsetospeed(&config, baudrate);

    /* 8N1 Mode */
    config.c_cflag &= ~PARENB;   /* Disables the Parity Enable bit(PARENB),So No Parity   */
    config.c_cflag &= ~CSTOPB;   /* CSTOPB = 2 Stop bits,here it is cleared so 1 Stop bit */
    config.c_cflag &= ~CSIZE;    /* Clears the mask for setting the data size             */
    config.c_cflag |=  CS8;      /* Set the data bits = 8                                 */

    config.c_cflag &= ~CRTSCTS;       /* No Hardware flow Control                         */
    config.c_cflag |= CREAD | CLOCAL; /* Enable receiver,Ignore Modem Control lines       */

    config.c_iflag &= ~(IXON | IXOFF | IXANY);          /* Disable XON/XOFF flow control both i/p and o/p */
    config.c_iflag &= ~(ICANON | ECHO | ECHOE | ISIG);  /* Non Cannonical mode                            */
    config.c_iflag &= ~INLCR; // disable Translate NL to CR on input
    config.c_iflag &= ~ICRNL; // disable Translate CR to NL on input

    config.c_oflag &= ~OCRNL; // disable Translate NL to CR on output
    config.c_oflag &= ~OPOST;/*No Output Processing*/
    config.c_oflag &- ~ONLCR; // disable Map NL to CR-NL on output

    /* Setting Time outs */
    config.c_cc[VTIME] = 0; //0; /* Wait indefinetly   */
    config.c_cc[VMIN] = 4; //3; /* Read at least 10 characters */
    config.c_lflag = 0;

    if (tcsetattr(handler, TCSANOW, &config)) /* Set the attributes to the termios structure*/
        return false;

    tcflush(handler, TCIFLUSH);   /* Discards old data in the rx buffer            */

    return true;
}

} // termios
