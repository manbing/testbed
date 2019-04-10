#include <errno.h>
#include <fcntl.h> 
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>


#define TTY_INF "/dev/ttyUSB1"


int set_interface_attribs (int fd, int speed, int parity)
{
        struct termios tty;
        memset (&tty, 0, sizeof tty);
        if (tcgetattr (fd, &tty) != 0) {
                printf("error %d from tcgetattr", errno);
                return -1;
        }

        cfsetospeed (&tty, speed);
        cfsetispeed (&tty, speed);

#if 1
        tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;     // 8-bit chars
        // disable IGNBRK for mismatched speed tests; otherwise receive break
        // as \000 chars
        tty.c_iflag &= ~IGNBRK;         // disable break processing
        tty.c_lflag = 0;                // no signaling chars, no echo,
                                        // no canonical processing
        tty.c_oflag = 0;                // no remapping, no delays
        tty.c_cc[VMIN]  = 0;            // read doesn't block
        tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

        tty.c_iflag &= ~(IXON | IXOFF | IXANY); // shut off xon/xoff ctrl

        tty.c_cflag |= (CLOCAL | CREAD);// ignore modem controls,
                                        // enable reading
        tty.c_cflag &= ~(PARENB | PARODD);      // shut off parity
        tty.c_cflag |= parity;
        tty.c_cflag &= ~CSTOPB;
        tty.c_cflag &= ~CRTSCTS;
#endif

#if 0
        cfsetispeed (&tty, speed);
        cfsetospeed (&tty, speed);
        tty.c_cflag |= (CLOCAL | CREAD | CSTOPB);
        tcflush(fd, TCIOFLUSH);
#endif

        if (tcsetattr (fd, TCSANOW, &tty) != 0) {
                printf ("error %d from tcsetattr", errno);
                return -1;
        }
        return 0;
}

void set_blocking (int fd, int should_block)
{
        struct termios tty;
        memset (&tty, 0, sizeof tty);
        if (tcgetattr (fd, &tty) != 0)
        {
                printf ("error %d from tggetattr", errno);
                return;
        }

        tty.c_cc[VMIN]  = should_block ? 1 : 0;
        tty.c_cc[VTIME] = 5;            // 0.5 seconds read timeout

        if (tcsetattr (fd, TCSANOW, &tty) != 0)
                printf ("error %d setting term attributes", errno);
}

int main (void)
{
        int ret = -1;
        int fd = open (TTY_INF, O_RDWR | O_NOCTTY | O_SYNC);

        if (fd < 0) {
                printf("[%s][%d] open fail\n", __func__, __LINE__);
                return ret;
        }

                printf("send AT command\n");
                ret = write(fd, "AT", 2);
                printf("write retval = %d\n", ret);


        set_interface_attribs (fd, B19200, 0);  // set speed to 115,200 bps, 8n1 (no parity)
        set_blocking (fd, 0);                // set no blocking

        while (1) {
                fd_set rfds;
                struct timeval tv;
                int retval = 1;
                int data_len = 0;
                char buff [1024] = {0};
                
                FD_ZERO(&rfds);
                FD_SET(fd, &rfds);

                tv.tv_sec = 3;
                tv.tv_usec = 0;

#if 0
                printf("send AT command\n");
                retval = write(fd, "AT", 2);
                printf("write retval = %d\n", retval);
#endif


                sleep(1);

                printf("receive AT reply\n");
                retval = select(fd+1, &rfds, NULL, NULL, &tv);

                if (retval == -1) {
                        printf("select error\n");
                } else if (retval) {
                        if (FD_ISSET(fd, &rfds)) {
                                memset(&buff, 0, sizeof buff);
                                data_len = read(fd, buff, sizeof(buff) - 1);
                                printf("get %d bytes data:\n %s\n\n", data_len, buff);
                        }
                } else {
                        printf("Timeout\n");
                }

                printf("down\n");
                sleep(10);
        }

        return -1;
}
