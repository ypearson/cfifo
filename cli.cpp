#include <stdio.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <iostream>

uint8_t buffer[128];
CircularBuffer<uint8_t> cb{buffer,128};

void enable_raw_mode()
{
    termios term;
    tcgetattr(0, &term);
    term.c_lflag &= ~(ICANON | ECHO); // Disable echo as well
    tcsetattr(0, TCSANOW, &term);
}

void disable_raw_mode()
{
    termios term;
    tcgetattr(0, &term);
    term.c_lflag |= ICANON | ECHO;
    tcsetattr(0, TCSANOW, &term);
}

const char *newline = "\r\n";

int main () {

    char c;

    enable_raw_mode();

    printf("\r\nCLI");
    printf("\r\n$>");


    while(c!='0')
    {
        c = getchar()
        printf("%c",c);
    }

    disable_raw_mode();

    return 0;
}


