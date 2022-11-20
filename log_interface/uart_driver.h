#ifndef UART_DRIVER_H
#define UART_DRIVER_H

enum uart_baudrate_e
	{
		BR9600 = 0,
		BR19200,
		BR38400,
		BR115200
	};

enum uart_stopbit_e
	{
		ONE_STOP_BIT = 0,
		TWO_STOP_BIT
	};

enum uart_paritybit_e
	{
		DISABLE_PARITY_BIT = 0,
		ENABLE_PARITY_BIT
	};

enum uart_bitsperbyte_e
	{
		FIVE_BITS_PER_BYTE = 0,
		SIX_BITS_PER_BYTE,
		SEVEN_BITS_PER_BYTE,
		EIGHT_BITS_PER_BYTE
	};

class UartCommunication
{
	protected:
		int serial_port;
		unsigned char buffer[256] = {0};
	public:
		UartCommunication(uart_baudrate_e BR, uart_stopbit_e stop_bit, 
			uart_paritybit_e paritybit, uart_bitsperbyte_e bitsperbyte);
		
		~UartCommunication();
		
		/* 
		This function sends data from buffer using the UART communication.
		
		unsigned char* write_buff: pointer to the data buffer.
		int length: number of bytes to be transmited.
		
		returns: None.
		*/
		void write_data(unsigned char* write_buff, int length);

		/*
		This function reads the data received from UART communication.

		unsigned char* read_buff: buffer to store the incoming data.

		returns: the number of received bytes.
		*/
		int read_data(unsigned char* read_buff);

};

#endif /* UART_DRIVER_H */