#include <stdio.h>

#include <sapi/sys.hpp>
#include <sapi/var.hpp>


int main(int argc, char * argv[]){

	String str;
	String arg1;

	if( argc >= 2 ){
		str = argv[1];

		if( str == "orphan" ){
			File f;
			if( f.create("/home/orphan.txt", true) < 0 ){
				perror("failed to open orphan");
			}
			if( f.write(str.c_str(), str.size()) < 0 ){
				perror("Failed to write orphan file");
			}
			if( f.close() < 0 ){
				perror("Failed to close orphan file");
			}
		} else if ( str == "wait" ){
			Timer::wait_msec(500);
		} else if ( str == "child" ){

			if( argc == 3 ){
				arg1 = argv[2];
				//send a signal to the parent
				kill(arg1.atoi(), SIGCHLD);
			} else {
				printf("can't get pid\n");
			}
		}
	}

	return argc;
}



