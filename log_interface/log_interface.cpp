
#include <stdio.h>
#include <string.h>

#include "uart_driver.h"


typedef struct 
{
    unsigned char temperature;
    unsigned char humidity;
}Telemetry_t;

typedef struct 
{
    unsigned char year;
    unsigned char month;
    unsigned char day;
}Date_t;

typedef struct 
{
    Telemetry_t telemetry;
    unsigned char n_prev;
    unsigned char n_success;
    unsigned char uc_ID;
    Date_t date;
}Data_t;

typedef struct 
{
	Date_t initial_date;
	Date_t last_date;
}Request_t;
	
class LogInterface: public UartCommunication
{
	protected:
		Data_t data_logs_buffer[256] = {0};
		int n_logs = 0;
	public:
		LogInterface(): UartCommunication(BR9600, ONE_STOP_BIT, DISABLE_PARITY_BIT, EIGHT_BITS_PER_BYTE)
		{
			printf("Log Interface initialized.\n");
		} 
		void getLogData()
		{
			int n_received_logs = 0;
			Data_t data;
			unsigned char buffer[256];
			n_received_logs = read_data(buffer)/8;
			for (int i = 0; i< n_received_logs; i++)
			{		
				data.telemetry.temperature = buffer[0 + i * 8];
				data.telemetry.humidity = buffer[1 + i * 8];
				data.n_prev = buffer[2 + i * 8];
				data.n_success = buffer[3 + i * 8];
				data.uc_ID = buffer[4 + i * 8];
				data.date.year = buffer[5 + i * 8];
				data.date.month = buffer[6 + i * 8];
				data.date.day = buffer[7 + i * 8];
				
				data_logs_buffer[n_logs++] = data;
			}
		}

		void showLogs()
		{
			for (int i = 0; i< n_logs; i++)
			{
				printf("Log %i:Year: %c, Month: %c, Day: %c, Temperature: %c, Humidity: %c, N previsions: %c, N success: %c, uc ID: %c. \n", i+1,
					data_logs_buffer[i].date.year, data_logs_buffer[i].date.month, data_logs_buffer[i].date.day, data_logs_buffer[i].telemetry.temperature,
					data_logs_buffer[i].telemetry.humidity, data_logs_buffer[i].n_prev, data_logs_buffer[i].n_success, data_logs_buffer[i].uc_ID);
			}
		}

		void requestLogs(Request_t request)
		{
			unsigned char write_buffer[8];

			write_buffer[0] = request.initial_date.year;
			write_buffer[1] = request.initial_date.month;
			write_buffer[2] = request.initial_date.day;

			write_buffer[3] = request.last_date.year;
			write_buffer[4] = request.last_date.month;
			write_buffer[5] = request.last_date.day;
			
			write_buffer[6] = '0';
			write_buffer[7] = '0';

			write_data(write_buffer, sizeof(write_buffer));
		}

};

int main(){	
	LogInterface interface;
	
	Request_t rqst;
	rqst.initial_date.year = '1';
	rqst.initial_date.month = '2';
	rqst.initial_date.day = '3';

	rqst.last_date.year = '4';
	rqst.last_date.month = '5';
	rqst.last_date.day = '6';

	interface.requestLogs(rqst);

	interface.getLogData();

	interface.showLogs();

	return 0;
}
