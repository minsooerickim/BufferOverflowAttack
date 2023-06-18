#include <stdio.h>
#include <string.h>
#include <sys/stat.h> 
#include <fcntl.h>

char filename[100];

void test(char* input)
{
    char test[17] = "abc";
    strcpy(test, input);
    
    printf("You have input: %s\n", test);
}

void log_result()
{
    int fd = open(filename, O_APPEND | O_CREAT);
    close(fd);
}


void log_result_advanced(int print)
{
    if(print == 0xefbeadde)
    {
        char filename2[100];
        int uid = getuid();
        // the file needs to be generated at a location where normal users cannot touch
        sprintf(filename2, "uid_%d_crack_advanced", uid);
        printf("file name: %s\n", filename2);
        int fd = open(filename2, O_APPEND | O_CREAT);
        close(fd);
    }
}

void main(int argc, char** args)
{
    if(argc > 1)
    {
        int uid = getuid();
        // the file needs to be generated at a location where normal users cannot touch
        sprintf(filename, "uid_%d_crack", uid);
        printf("file name: %s\n", filename);
        test(args[1]);
    }
    else
    {
        printf("Please provide at least one input\n");
    }
}
