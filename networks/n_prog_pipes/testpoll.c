#include <fcntl.h>
#include <stdio.h>
#include <sys/poll.h>
#include <sys/time.h>
#include <unistd.h>

/* For simplicity, all error checking has been left out */
int main(int argc, char ** argv) {
   int fd;
   char buf[1024];
   int i;
   struct pollfd pfds[2];
   fd = open(argv[1], O_CREAT|O_RDONLY|O_WRONLY);
   while (1) {

       pfds[0].fd = 0;

       pfds[0].events = POLLIN;


       pfds[1].fd = fd;

       pfds[1].events = POLLIN;


       poll(pfds, 2, -1);

       if (pfds[0].revents & POLLIN) {

          i = read(0, buf, 1024);

          if (!i) {

             // printf(“stdin closed\n”);

              return 0;

          }

          write(1, buf, i);

       }


       if (pfds[1].revents & POLLIN) {

          i = read(fd, buf, 1024);

          if (!i) {

              //printf(“file closed\n”);

              return 0;

          }

          write(1, buf, i);

       }

    }

}