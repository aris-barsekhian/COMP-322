#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    FILE *fp = fopen("/tmp/lab6.log", "a+");
	perror("fopen");
    if (fp != 0)
    {
        char *s;
        if (argc > 1 && strcmp(argv[1], "1") == 0)
            s = "Pop mole1";
        else
            s = "Pop mole2";
        fprintf(fp, "%s\n",s);
        fclose(fp);
    }
    return 0;
}
