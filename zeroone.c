#include<stdio.h>  
#include<string.h>  
#include<stdlib.h>  
#include<unistd.h>  
#include <fcntl.h>



int main(int argc, char*argv[])
{
	char numberStr[9], chr;
	int filedes, bytes, value, count,oneCount;
	if (argc < 2)
	{
		filedes = 3;
	}
	else
	{
		if (argv[1][0] == '-'&&argv[1][1] == 0)
		{
			filedes = 3;
		}
		else
			filedes = open(argv[1], O_RDONLY, 0);
	}
	value = 0;
	count = 0;
	oneCount = 0;
	bytes=read(filedes,&chr, 1);
	printf("Original ASCII    Decimal  Parity  \n");
	printf("-------- -------- -------- ------- \n");
	while (bytes == 1)
	{
		if (chr == '0'&&count<8)
		{
			value *= 2;
			numberStr[count++] = chr;
		}
		else if (chr == '1'&&count<8)
		{
			value *= 2;
			value += 1;
			numberStr[count++] = chr;
			oneCount++;
		}
		else
		{
			numberStr[count++] = 0;
			count = 0;
			printf("%8s        %c      %3d  %-5s  \n", numberStr, (char)(value%128), value % 128,
		oneCount & 1 ? "ODD" : "EVEN");
			value = 0;
			oneCount = 0;
		}
		bytes = read(filedes, &chr, 1);
	}
	while (count < 8)
	{
		value *= 2;
		numberStr[count++] = '0';
	}
	numberStr[count++] = 0;
	printf("%8s        %c      %3d  %-5s  \n", numberStr, (char)(value%128), value % 128,
		oneCount & 1 ? "ODD" : "EVEN");
	close(filedes);
	return 0;
}
