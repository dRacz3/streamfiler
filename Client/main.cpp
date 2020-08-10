// Client side C/C++ program to demonstrate Socket programming
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>
#include <sstream>
#include <thread>

#define PORT 2222

int main(int argc, char const* argv[])
{
   int sock = 0, valread;
   struct sockaddr_in serv_addr;
   int desired_char_count = 999999;
   std::stringstream ss;
   while (desired_char_count-- > 0)
   {
      if (desired_char_count % 5 == 0)
      {
         ss << "A";
      }
      else if (desired_char_count % 4 == 0)
      {
         ss << "B";
      }
      else if (desired_char_count % 3 == 0)
      {
         ss << "C";
      }
      else if (desired_char_count % 2 == 0)
      {
         ss << "D";
      }
      else
      {
         ss << "E";
      }

      ss << "A";
   }
   std::string hello = ss.str();

   std::cout << "Sending a message with size:" << sizeof(hello.c_str()) * hello.length() / 1024
             << "kb. Press enter to start!";
   // getchar();

   char buffer[1024] = {0};
   if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
   {
      printf("\n Socket creation error \n");
      return -1;
   }

   serv_addr.sin_family = AF_INET;
   serv_addr.sin_port = htons(PORT);

   // Convert IPv4 and IPv6 addresses from text to binary form
   if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
   {
      printf("\nInvalid address/ Address not supported \n");
      return -1;
   }

   if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
   {
      printf("\nConnection Failed \n");
      return -1;
   }
   send(sock, hello.c_str(), hello.size(), 0);
   printf("Hello message sent (Char count: %d)", hello.size());
   valread = read(sock, buffer, 1024);
   printf("%s\n", buffer);
   usleep(100);
   {
      using namespace std::chrono_literals;
      std::this_thread::sleep_for(20s);
   }
   return 0;
}
