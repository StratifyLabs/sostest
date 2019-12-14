#include <stdio.h>

#include <sapi/sys.hpp>
#include <sapi/fs.hpp>
#include <sapi/var.hpp>
#include <sapi/chrono.hpp>


int main(int argc, char * argv[]){

	String str;
	String arg1;
	TaskManager board;

	if( argc >= 2 ){
		str = argv[1];
		if( str == "orphan" ){
			File f;
			if( f.create("/home/orphan.txt", File::IsOverwrite(true)) < 0 ){
				perror("failed to open orphan");
			}
			if( f.write(str) < 0 ){
				perror("Failed to write orphan file");
			}
			if( f.close() < 0 ){
				perror("Failed to close orphan file");
			}
		} else if ( str == "wait" ){
			wait(Milliseconds(500));
		} else if ( str == "child" ){
			if( argc == 3 ){
				arg1 = argv[2];
				//send a signal to the parent
				kill(arg1.to_integer(), SIGCHLD);
			} else {
				printf("can't get pid\n");
			}
		}
	}

	return argc;
}



