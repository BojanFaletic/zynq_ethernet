#include <arpa/inet.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <unistd.h>

#define BRAM_ADDR 0x40000000
#define PAGE_SIZE 4096

#define MEM_SIZE 8 * PAGE_SIZE

int main() {
  int fd = open("/dev/mem", O_RDWR);
  if (fd < 0) {
    printf("Error unable to open dev/mem\n");
    return 1;
  }

  void *map =
      mmap(NULL, MEM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, BRAM_ADDR);
  if (map == NULL) {
    printf("Mmap failed \n");
    close(fd);
    return 1;
  }

  // Write to memory
  struct sockaddr_in server, client;
  void *client_message = map;

  // create socket
  int sock_desc = socket(AF_INET, SOCK_STREAM, 0);
  if (sock_desc == -1) {
    perror("Could not create socket\n");
  }
  printf("socket created\n");

  // init server structure
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_port = htons(8888);

  // bind
  if (bind(sock_desc, (struct sockaddr *)&server, sizeof(server)) < 0) {
    perror("bind failed. Error");
    goto cleanup;
  };
  printf("bind done\n");

  // listen
  listen(sock_desc, 3);

  printf("waiting for incoming connections..");
  int c = sizeof(struct sockaddr_in);

  // accept connection form an incoming client
  int client_sock =
      accept(sock_desc, (struct sockaddr *)&client, (socklen_t *)&c);
  if (client_sock < 0) {
    perror("accept failed");
    goto cleanup;
  }
  printf("Connection accepted\n");

  // receive message from client
  int read_size;
  while ((read_size = recv(client_sock, client_message, 2000, 0)) > 0) {
    // send message back to client
    write(client_sock, client_message, strlen(client_message));
  }

  if (read_size == 0) {
    printf("Client disconnected\n");
    fflush(stdout);
  } else if (read_size == -1) {
    perror("recv failed");
  }

cleanup:
  // cleanup
  if (munmap(map, MEM_SIZE) != 0) {
    perror("Error unmap failed\n");
  }
  close(fd);
  return 0;
}