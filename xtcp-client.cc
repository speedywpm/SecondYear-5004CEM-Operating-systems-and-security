#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <stdlib.h>

#define MAX_LINE 100
#define LINE_ARRAY_SIZE (MAX_LINE+1)

using namespace std;

int main()
{
  int socketDescriptor;
  unsigned short int serverPort;
  struct sockaddr_in serverAddress;
  struct hostent *hostInfo;
  char buf[LINE_ARRAY_SIZE], c;

  cout << "Enter server host name or IP address: ";
  cin.get(buf, MAX_LINE, '\n');

  // gethostbyname() takes a host name or ip address in "numbers and
  // dots" notation, and returns a pointer to a hostent structure,
  // which we'll need later.  It's not important for us what this
  // structure is actually composed of.
  hostInfo = gethostbyname(buf);
  if (hostInfo == NULL) {
    cout << "problem interpreting host: " << buf << "\n";
    exit(1);
  }

  cout << "Enter server port number: ";
  cin >> serverPort;
  cin.get(c); // dispose of the newline

  // Create a socket.  "AF_INET" means it will use the IPv4 protocol.
  // "SOCK_STREAM" means it will be a reliable connection (i.e., TCP;
  // for UDP use SOCK_DGRAM), and I'm not sure what the 0 for the last
  // parameter means, but it seems to work.
  socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);
  if (socketDescriptor < 0) {
    cerr << "cannot create socket\n";
    exit(1);
  }

  // Connect to server.  First we have to set some fields in the
  // serverAddress structure.  The system will assign me an arbitrary
  // local port that is not in use.
  serverAddress.sin_family = hostInfo->h_addrtype;
  memcpy((char *) &serverAddress.sin_addr.s_addr,
         hostInfo->h_addr_list[0], hostInfo->h_length);
  serverAddress.sin_port = htons(serverPort);
				
  if (connect(socketDescriptor,
              (struct sockaddr *) &serverAddress,
              sizeof(serverAddress)) < 0) {
    cerr << "cannot connect\n";
    exit(1);
  }
  // Prints statements for the client to read so they know that format they must make sure to input
  cout << "\nHello there, Are you ready to make a booking?";
  cout << "\nThe different types of bookings you can make for entry are: \n saturday \n sunday \n VIP \n weekend.\n When typing in types of entry ticket you must make sure they are typed out as given here";
  cout << "\nTHe different types of bookings you can make for activity are \n baking \n dancing \ncraft \ndisco. \n When typing in types of acitivity make sure they are typed out as given here";
  cout << "\n Please type it in the format [Name] [Type of Ticket] [Quantity of Adult’s tickets] [Quantity of Children’s Tickets] [Name] [Type of Activity] [Quantity of Adult’s tickets] [Quantity of Children’s Tickets]";
  cout << "\n If you do not wish to book any activities then please put N/A \n Also ensure that the name of the type of ticket and type of acitivity is in lower case.";
  cout << "\n You will need to make sure there is a full stop at the end of the line as well.";
  cout << "\nOnly the first " << MAX_LINE << " characters will be used.\n\n";

  // Prompt the user for input, then read in the input, up to MAX_LINE
  // charactars, and then dispose of the rest of the line, including
  // the newline character.
  cout << "Input: ";
  cin.get(buf, MAX_LINE, '\n');
  while (cin.get(c) && c != '\n') 
    ; //Loop does nothing except consume the spare bytes


  // Stop when the user inputs a line with just a dot.
  while (strcmp(buf, ".")) { //strcmp returns 0 when the two strings
			     //are the same, so this continues when
			     //they are differenta
    // Send the line to the server.
    if (send(socketDescriptor, buf, strlen(buf) + 1, 0) < 0) {
      cerr << "cannot send data ";
      close(socketDescriptor); //Note this is just like using files...
      exit(1);
    }

    // Zero out the buffer.
    memset(buf, 0x0, LINE_ARRAY_SIZE);

    // Read the modified line back from the server.
    if (recv(socketDescriptor, buf, MAX_LINE, 0) < 0) {
      cerr << "didn't get response from server?";
      close(socketDescriptor);
      exit(1);
    }
    // Returns appropriate response and cost of booking to the client
    cout << "Hello, have booked your tickets and total cost is " << buf << "\n If your total cost is 0 that means you cannot make anymore bookings.\n This is because you have already made bookings and to stop anyone from buying too many tickets\n";

    // Prompt the user for input, then read in the input, up to MAX_LINE
    // charactars, and then dispose of the rest of the line, including
    // the newline character.  As above.
    cout << "Input: ";
    cin.get(buf, MAX_LINE, '\n');
    while (cin.get(c) && c != '\n')
      ; //Chomp chomp chomp
  }

  close(socketDescriptor);
  return 0;
}
