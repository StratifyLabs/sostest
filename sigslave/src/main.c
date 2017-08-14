
#include <sos/dev/uart.h>

#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

volatile int signal_received = 0;

void signal_handler(int a){
	signal_received = 1;
}

int main(int argc, char * argv[]){

	char buffer[64];
	//wait for a signal and return the signal value
	int fd;
	int ret;

	fd = open("/dev/uart0", O_RDWR);
	if( fd < 0 ){
		printf("Failed to open file\n");
	}


	/*
	uart_attr_t attr;


	memset(&attr.pin_assignment, 0xff, sizeof(uart_pin_assignment_t));
	attr.pin_assignment.rx = mcu_pin(0,2);
	attr.pin_assignment.tx = mcu_pin(0,3);
	attr.o_flags = UART_FLAG_SET_CONTROL_LINE_STATE | UART_FLAG_IS_PARITY_NONE | UART_FLAG_IS_STOP1;
	attr.width = 8;
	attr.freq = 115200;

	if( ioctl(fd, I_UART_SETATTR, &attr) < 0 ){
		printf("Failed to set uart attr\n");
		return 1;
	}
	*/

	signal(SIGUSR1, signal_handler);

	while(1){
		if( fd >= 0 ){

			errno = 0;
			ret = read(fd, buffer, 64);
			if( ret != 64 ){
				//perror("Failed to read");
				printf("Failed to write %d (%d)\n", ret, errno);
				lseek(fd, 0, SEEK_SET);
				usleep(100*1000);
			}


			if( signal_received ){
				signal_received = 0;
				printf("Received SIGUSR1...\n");
				fflush(stdout);
			}

		} else {
			printf("waiting...");
			sleep(1);
		}


		fflush(stdout);
	}

	return 0;
}
