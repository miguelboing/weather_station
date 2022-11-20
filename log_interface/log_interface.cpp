
#include <stdio.h>
#include <string.h>

#include "uart_driver.h"

int main(){
	int i = 0;
	UartCommunication uart(BR9600, ONE_STOP_BIT, DISABLE_PARITY_BIT, EIGHT_BITS_PER_BYTE);

	unsigned char msg[] = { 'H', 'e', 'l', 'l', 'o', '\r' };

	unsigned char read_buf[256] = {0};
	int av_bytes = 0;

	uart.write_data(msg, sizeof(msg));

	av_bytes = uart.read_data(read_buf);
	printf("Received bytes: %i \n", av_bytes);
	printf("Received message: \n");
	for(i=0;i<av_bytes;i++)
	{
		printf("%c \n", read_buf[i]);
	}
	return 0;
}
