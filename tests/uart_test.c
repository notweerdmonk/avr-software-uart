
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <signal.h>

#include <config.h>

/* this program will recieve data from /dev/ttyUSB0 */

int stop = 0;
void sigalarm_handler(int sig) {
  stop = 1;
}

#define STRINGIFY(s) #s
#define SER_DEV STRINGIFY(/dev/ttyUSB0)

char *teststring = "AAAAAAAAAAAAAAAA";

int main(int argc, char *argv[]) {

  int ser_port = -1;
  char buffer[256];
  size_t next_byte = 0;
  size_t bytes_read;

  strcpy(buffer, SER_DEV);

  int c;
  while ((c = getopt(argc, argv, ":hd:")) != -1) {
    switch (c) {
      case 'd':
        strcpy(buffer, optarg);
        break;

      case 'h':
        printf("Usage: %s [OPTION] ...\n", argv[0]);
        printf("  -h             Display this message\n");
        printf("  -d [device]    Open given device as serial port\n");
        return 0;

      case '?':
        fprintf(stderr, "Invalid option: %c\n", optopt);
        break;

      case ':':
        fprintf(stderr, "Invalid option: %c requires an argument\n", optopt);
        break;

      default:
        return -1;
    }
  }

  struct sigaction sa, old_sa;
  sa.sa_handler = sigalarm_handler;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;
  sigaction(SIGALRM, &sa, &old_sa);

  printf("Opening device: %s\n", buffer);

  ser_port = open(buffer, O_RDWR|O_NOCTTY);

  if (ser_port == -1) {
    fprintf(stderr, "Could not open device!\n");
    return -1;
  }

  struct termios ser_settings;
  tcgetattr(ser_port, &ser_settings);
#define CAT(a, b) a ## b
#define BAUDRATE(baud) CAT(B, baud)
  cfsetispeed(&ser_settings, BAUDRATE(c_UART_BAUD_RATE));
  cfsetospeed(&ser_settings, BAUDRATE(c_UART_BAUD_RATE));
  ser_settings.c_cflag &= ~PARENB;
#if c_UART_N_STOP_BITS < 2
  ser_settings.c_cflag &= ~CSTOPB;
#endif
  ser_settings.c_cflag &= ~CSIZE;
#if c_UART_CHAR_SIZE == 8
  ser_settings.c_cflag |= CS8;
#elif c_UART_CHAR_SIZE == 7
  ser_settings.c_cflag |= CS7;
#endif
  ser_settings.c_cflag &= ~CRTSCTS;
  ser_settings.c_cflag |= CREAD | CLOCAL;
  ser_settings.c_lflag &= ~(ICANON | ECHO | ECHOE | ECHONL | ISIG);
  ser_settings.c_iflag &= ~(IXON | IXOFF | IXANY);
  ser_settings.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL);
  ser_settings.c_oflag &= ~(OPOST | ONLCR);
  ser_settings.c_cc[VMIN] = 0;
  ser_settings.c_cc[VTIME] = 0;

  tcflush(ser_port, TCIFLUSH);
  if (tcsetattr(ser_port, TCSANOW, &ser_settings) == -1) {
    fprintf(stderr, "Failed to set serial port attributes!\n");
    close(ser_port);
    return -1;
  }
  else {
    printf("Serial port settings: %d Baud rate, %d-bits Character size, %d Stop bit, No Parity\n",
           c_UART_BAUD_RATE,
           c_UART_CHAR_SIZE,
           c_UART_N_STOP_BITS);
  }

  alarm(3);
  while(!stop) {
    bytes_read = read(ser_port, buffer + next_byte, 32);
    next_byte += bytes_read;
  }
  buffer[next_byte] = 0;
  printf("Received:\n%s\n", buffer);

  sigaction(SIGALRM, &old_sa, 0);

  close(ser_port);

  if (strncmp(buffer, teststring, strlen(teststring))) {
    printf("Test failed\n");
    return -1;
  }
  printf("Test passed\n");

  return 0;
}
