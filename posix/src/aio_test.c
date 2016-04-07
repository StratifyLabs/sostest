/* Copyright 2011; All Rights Reserved
 * Please see http://www.coactionos.com/license.html for
 * licensing information.
 */


#include <errno.h>
#include <stdio.h>
#include <aio.h>
#include <unistd.h>
#include <fcntl.h>

#include <iface/dev/adc.h>
#include <iface/dev/dac.h>

#include "tests.h"

#define AIO_TEST_READ_DEVICE "/dev/adc0"
#define AIO_TEST_WRITE_DEVICE "/dev/dac0"

int aio_test(void){
	int fd;
	struct aiocb aio;
	char buffer[32];
	int err;
	struct aiocb * list[1];

	adc_attr_t attr;
	dac_attr_t dac_attr;

	printf("Open %s\n", AIO_TEST_READ_DEVICE);
	//Open a device to test
	fd = open(AIO_TEST_READ_DEVICE, O_RDWR);
	if ( fd < 0 ){
		perror("Failed to open test device");
		return -1;
	}

	attr.enabled_channels = (1<<0);
	attr.freq = 200;
	attr.pin_assign = 0;

	if( ioctl(fd, I_ADC_SETATTR, &attr) < 0 ){
		perror("Failed to set attributes");
		return -1;
	}

	aio.aio_fildes = fd;
	aio.aio_offset = 0;
	aio.aio_buf = buffer;
	aio.aio_nbytes = 32;
	aio.aio_reqprio = 0;
	//aio.aio_sigevent;

	printf("Test aio_read()...");
	fflush(stdout);
	if ( aio_read(&aio) == 0 ){
		while( (err = aio_return(&aio)) < 0 ){
			if ( err == -1 ){
				if ( aio_error(&aio) != EINPROGRESS ){
					fflush(stdout);
					printf("Error is not EINPROGRESS (%d)\n", aio_error(&aio));
					close(fd);
					return -1;
				}
			}
			usleep(500);
		}
	}

	if ( aio_return(&aio) != 32 ){
		printf("Failed.  Read %d bytes ( != 32)\n", aio_return(&aio));
	}

	printf("passed\n");

	aio.aio_fildes = fd;
	aio.aio_offset = 0;
	aio.aio_buf = buffer;
	aio.aio_nbytes = 32;
	aio.aio_reqprio = 0;

	printf("Test aio_suspend() (read)...");
	fflush(stdout);
	if ( aio_read(&aio) == 0 ){
		list[0] = &aio;
		aio_suspend(list, 1, NULL);
	}

	if ( aio_return(&aio) != 32 ){
		printf("Failed.  Read %d bytes ( != 32)\n", aio_return(&aio));
	}

	printf("passed\n");


	close(fd);

	printf("Open %s\n", AIO_TEST_WRITE_DEVICE);
	//Open a device to test
	fd = open(AIO_TEST_WRITE_DEVICE, O_RDWR);
	if ( fd < 0 ){
		perror("Failed to open test device");
		return -1;
	}

	dac_attr.enabled_channels = (1<<0);
	dac_attr.freq = 200;
	dac_attr.pin_assign = 0;

	if( ioctl(fd, I_DAC_SETATTR, &dac_attr) < 0 ){
		perror("Failed to set attributes");
		return -1;
	}

	aio.aio_fildes = fd;
	aio.aio_offset = 0;
	aio.aio_buf = buffer;
	aio.aio_nbytes = 32;
	aio.aio_reqprio = 0;
	printf("Test aio_write()...");
	fflush(stdout);
	if ( aio_write(&aio) == 0 ){
		while( (err = aio_return(&aio)) < 0 ){
			if ( err == -1 ){
				if ( aio_error(&aio) != EINPROGRESS ){
					fflush(stdout);
					printf("Error is not EINPROGRESS (%d)\n", aio_error(&aio));
					close(fd);
					return -1;
				}
			}
			usleep(500);
		}
	}

	if ( aio_return(&aio) != 32 ){
		printf("Failed.  Read %d bytes ( != 32)\n", aio_return(&aio));
	}
	printf("passed\n");

	aio.aio_fildes = fd;
	aio.aio_offset = 0;
	aio.aio_buf = buffer;
	aio.aio_nbytes = 32;
	aio.aio_reqprio = 0;

	printf("Test aio_suspend() (write)...");
	fflush(stdout);
	if ( aio_write(&aio) == 0 ){
		list[0] = &aio;
		aio_suspend(list, 1, NULL);
	}

	if ( aio_return(&aio) != 32 ){
		printf("Failed.  Read %d bytes ( != 32)\n", aio_return(&aio));
	}

	printf("passed\n");
	close(fd);
	return 0;
}
