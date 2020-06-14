#include<string.h>
#include<unistd.h>
int main()
{
        char buf[100] = "hello";
        char rbuf[300];
        int bytes_read;
        int fd = open("/dev/process_list",O_RDONLY);
        if(fd<0)
        {

                perror("Error");
                return 0;
        }
//      printf("%d",fd);
//      write(fd,buf,strlen(buf));
        bytes_read = read(fd,rbuf,300);
        while(bytes_read > 0)
        {
        bytes_read = read(fd,rbuf,300);
        printf("From Device: %s",rbuf);
        }
        if(bytes_read < 0)
        {
                perror("Error");
        }
        close(fd);
        return 0;
}