#include <stdio.h>
#include <stdlib.h>


int main(int argc, char* argv[]){
	unsigned int addressNumber;
	unsigned int pageNumber;
	unsigned int offSet;
	const int Page_Size = 4096;
	if(argc > 1 && argc <= 2){
		

		addressNumber = atoi(argv[1]);
		pageNumber=addressNumber / Page_Size;
		offSet=addressNumber % Page_Size;
		fprintf(stdout, "The address %d contains:\n", addressNumber); // Address number
		fprintf(stdout, "page number = %d\n", pageNumber); // Page number
		fprintf(stdout, "offset = %d\n", offSet);//  Offset

		}

		else{
			fprintf(stderr, "Invalid arguments\n");
			return 1;
		}
		return 0;
	}
