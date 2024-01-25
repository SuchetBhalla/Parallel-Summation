#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char *argv[])
{

	//Error check
	if(argc < 2){
		printf(1, "ERROR: Arguments missing. This program needs both, type and filename\n");
		exit();
	}

	/*
	The file's content's are known, it has 1000 integers,
	in the range 0 till 9.
	*/
	int size = 1000,
	arr[size];

	//Reads data from the file
	char *filename = argv[2];
	int fd = open(filename, 0);

	char c;
	for(int i=0; i < size; i++){
		read(fd, &c, 1);
		arr[i] = c - '0';
		read(fd, &c, 1);
	}
	close(fd);

	//SECTION for distribution of workload over 8 processors
	int parent = getpid(),
	children = 8,
	div = size/children,
	psum = 0,
	tot_sum = 0;

	int pid = fork();

	//The parent enters here
	if ( pid > 0 ) {

		wait();
		int temp[9];

		//'recv' is a blocking-call, which was written (by the author) to xv6's kernel
		recv((void*) &temp[0]);
		recv((void*) &temp[1]);
		recv((void*) &temp[2]);
		recv((void*) &temp[3]);
		recv((void*) &temp[4]);
		recv((void*) &temp[5]);
		recv((void*) &temp[6]);
		recv((void*) &temp[7]);

		tot_sum = temp[0] + temp[1] + temp[2] + temp[3] + temp[4] + temp[5] + temp[6] + temp[7];
		printf(1, "Sum of array in the file %s is %d\n", filename, tot_sum);
	}

	//The child-process enters here
	else {

		//Creates 8 child-processes
		int f1 = fork(),
		f2 = fork(),
		f3 = fork();

		//Non-overlapping sub-arrays are alloted to each process
		if ( f1 > 0 ) {
			if ( f2 > 0 ) {
				if (f3 > 0) {
				//Child 1 enters here (111)

				for (int j = 0; j < 0 + div; j++) {
					psum += arr[j];
				}

				send( getpid() , parent, (void*) &psum);

				wait(); wait(); wait();
				exit();
				}

				//Child 2 : 110
				else {

				for (int j = div; j < div + div; j++) {
					psum += arr[j];
				}

				send( getpid() , parent, (void*) &psum);
				exit();
				}

			}

			//not f2
			else {

				//c3 : 101
				if (f3 > 0) {

					for (int j = 2*div; j < 2*div + div; j++) {
						psum += arr[j];
					}
					send( getpid() , parent, (void*) &psum);

					wait();
					exit();

				}

				//c4 : 100
				else {

					for (int j = 3*div; j < 3*div + div; j++) {
						psum += arr[j];
					}
					send( getpid() , parent, (void*) &psum);

					exit();

				}
			}
		}

		//not f1
		else {
			if ( f2 > 0 ) {
				//c5 : 011
				if (f3 > 0) {

				for (int j = 4*div; j < 4*div + div; j++) {
					psum += arr[j];
				}

				send( getpid() , parent, (void*) &psum);

				wait(); wait();
				exit();
				}

				//c6 : 010
				else {

				for (int j = 5*div; j < 5*div + div; j++) {
					psum += arr[j];
				}

				send( getpid() , parent, (void*) &psum);
				exit();
				}

			}

			//not f2
			else {

				//c7 : 001
				if (f3 > 0) {

					for (int j = 6*div; j < 6*div + div; j++) {
						psum += arr[j];
					}

					send( getpid() , parent, (void*) &psum);

					wait();
					exit();

				}

				//c8 : 000
				else {

					for (int j = 7*div; j < 7*div + div; j++) {
						psum += arr[j];
					}

					send( getpid() , parent, (void*) &psum);

					exit();
				}
			}
		}
	}

	exit();
}
