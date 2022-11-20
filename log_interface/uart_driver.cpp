#include <stdio.h>
#include <string.h>
#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()

#include "uart_driver.h"

UartCommunication::UartCommunication(uart_baudrate_e BR, uart_stopbit_e stop_bit, 
			uart_paritybit_e parity_bit, uart_bitsperbyte_e bits_per_byte) : serial_port(0)
{
	int error = 0;
	tcflag_t bpbytes = 0;
	speed_t baud_rate;

	serial_port = open("/dev/ttyUSB0", O_RDWR);
	struct termios tty;

	// Check for errors
	if (serial_port < 0) {
    	printf("Error %i from open: %s\n", errno, strerror(errno));
    	error = -1;
	}

	// Struct initialization 
	if(tcgetattr(serial_port, &tty) != 0) {
    	printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
      	error = -1;
	}

	// Uart Configuration 

	tty.c_cflag &= ~PARENB; // Clear parity bit, disabling parity 

	if (parity_bit == ENABLE_PARITY_BIT)
	{
		tty.c_cflag |= PARENB;
	}
	else if (parity_bit != DISABLE_PARITY_BIT)
	{
		printf("Invalid parity bit configuration, using no parity bit mode.\n");
	}  

	tty.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication 

	tty.c_cflag &= ~CSIZE; // Clear all the size bits, then use one of the statements below
	
	//TODO: Implement support for different size bytes
	tty.c_cflag |= CS8;

	tty.c_cflag &= ~CRTSCTS;

	tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)

	tty.c_lflag &= ~ICANON;

	tty.c_lflag &= ~ECHO; // Disable echo
	tty.c_lflag &= ~ECHOE; // Disable erasure
	tty.c_lflag &= ~ECHONL; // Disable new-line echo

	tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP

	tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl

	tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes

	tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
	tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed
	// tty.c_oflag &= ~OXTABS; // Prevent conversion of tabs to spaces (NOT PRESENT IN LINUX)
	// tty.c_oflag &= ~ONOEOT; // Prevent removal of C-d chars (0x004) in output (NOT PRESENT IN LINUX)

	tty.c_cc[VTIME] = 10;    // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
	tty.c_cc[VMIN] = 0;

	switch(BR)
	{
		case BR9600:	baud_rate = B9600;		break;
		case BR19200:	baud_rate = B19200;		break;
		case BR38400:	baud_rate = B38400; 	break;
		case BR115200:	baud_rate = B115200;	break;
		
		default:
			printf("Invalid Baud Rate, setting to default baud rate: 115200.\n");
			baud_rate = B115200;
			error = -1;
			break;
	}
	cfsetispeed(&tty, baud_rate);
	cfsetospeed(&tty, baud_rate);

	// Save tty settings, also checking for error
	if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
    	printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
    	error = -1;
	} 

	if (error == 0)	printf("\nUart connection initialized.\n\n");
}

UartCommunication::~UartCommunication()
{
	printf("\nClosing Uart connection. \n\n");
	close(serial_port);
}

void UartCommunication::write_data(unsigned char* write_buff, int length)
{
	write(serial_port, write_buff, length);
}

int UartCommunication::read_data(unsigned char* read_buff)
{
	int av_bytes = 0;

	av_bytes = read(serial_port, buffer, sizeof(buffer));

	//Copy the data from internal buffer to external buffer
	memcpy(read_buff, buffer, sizeof(unsigned char)*av_bytes);

	return av_bytes;
}