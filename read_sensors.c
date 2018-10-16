
#include "common.h"

int main (int argc, char *argv[]){
	read_hum();
	sleep(1);
	printf("\n");
	read_opt();
	sleep(1);
	printf("\n");
	read_tmp();
}