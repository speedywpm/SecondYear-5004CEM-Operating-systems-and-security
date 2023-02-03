//tcp-server.cc
// tcp-server that chops up the input and checks if the chunks 
// are words and number
//You need to put a fullstop at the end (.) so get last word
//For example
//Hamzepur VIP 3 2 Saturday 1 0.

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <stdlib.h>
#include <string>
#include <vector>
#include <cmath>

#define MAX_MSG 100
#define LINE_ARRAY_SIZE (MAX_MSG+1)

using namespace std;
// function that checks if the chunk  extracted from the order is a number
bool isNumber(string word)
{
    for (int character = 0; character < word.length(); character++){
        if (isdigit(word[character]) == false)
            return false;
 }
    return true;
}

int main()
{
  int listenSocket, connectSocket, i;
  unsigned short int listenPort;
  socklen_t clientAddressLength;
  struct sockaddr_in clientAddress, serverAddress;
  char line[LINE_ARRAY_SIZE];

  cout << "Enter port number to listen on (between 1500 and 65000): ";
  cin >> listenPort;

  // Create socket for listening for client connection
  // requests.
  listenSocket = socket(AF_INET, SOCK_STREAM, 0);
  if (listenSocket < 0) {
    cerr << "cannot create listen socket";
    exit(1);
  }
  
  // Bind listen socket to listen port. First set various
  // fields in the serverAddress structure, then call
  // bind().

  // htonl() and htons() convert long integers and short
  // integers (respectively) from host byte order (on x86
  // this is Least Significant Byte first) to network byte
  // order (Most Significant Byte first).
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
  serverAddress.sin_port = htons(listenPort);
  
  if (bind(listenSocket,
           (struct sockaddr *) &serverAddress,
           sizeof(serverAddress)) < 0) {
    cerr << "cannot bind socket";
    exit(1);
  }

  // Wait for connections from clients. This is a
  // non-blocking call; i.e., it registers this program with
  // the system as expecting connections on this socket, and
  // then this thread of execution continues on.
  listen(listenSocket, 5);
  
  while (1) {
    cout << "Waiting for TCP connection on port " << listenPort << " ...\n";

    // Accept a connection with a client that is requesting
    // one. The accept() call is a blocking call; i.e., this
    // thread of execution stops until a connection comes
    // in. connectSocket is a new socket that the system
    // provides, separate from listenSocket. We *could*
    // accept more connections on listenSocket, before
    // connectSocket is closed, but this program doesn't do
    // that.
    clientAddressLength = sizeof(clientAddress);
    connectSocket = accept(listenSocket,
                           (struct sockaddr *) &clientAddress,
                           &clientAddressLength);
    if (connectSocket < 0) {
      cerr << "cannot accept connection ";
      exit(1);
    }
    
    // Show the IP address of the client.
    // inet_ntoa() converts an IP address from binary form to the
    // standard "numbers and dots" notation.
    cout << "  connected to " << inet_ntoa(clientAddress.sin_addr);

    // Show the client's port number.
    // ntohs() converts a short int from network byte order (which is
    // Most Significant Byte first) to host byte order (which on x86,
    // for example, is Least Significant Byte first).
    cout << ":" << ntohs(clientAddress.sin_port) << "\n";

    // Read lines from socket, using recv(), storing them in the line
    // array.  If no messages are currently available, recv() blocks
    // until one arrives.
    // First set line to all zeroes, so we'll know where the end of
    // the string is.
    memset(line, 0x0, LINE_ARRAY_SIZE);

    //declare variables and vectors that will be used throughout the code when deciding activites and prices
    string word="";
    string notANumber="";
    string name="";
    string TicketType="";
    
    vector<float> Quantities;
    vector<string> BookingDetails;
    vector<string> StoredNames;
    
    float EntryCost = 0.0;
    float ActivityCost = 0.0;
    float TotalCost = 0.0;
    int number=0;
    int AdultQuantity=0;
    int ChildQuantity=0;
    
    bool isanum=false;
    while (recv(connectSocket, line, MAX_MSG, 0) > 0) {
      cout << "  --  " << line << "\n";

      // Convert line to upper case.
      for (i = 0; line[i] != '\0'; i++){
      //the user need to end the order with a full stop (.) to get last word/number 
      if ((line[i] != ' ') && (line[i] != '.')) {
          word=word+line[i];
          }
          // Keep adding a character until reach end of word/number
        if ((line[i] == ' ')|| (line[i]=='.')){
          //Call function to check if chunk of characters is a number
          isanum=isNumber(word);
          //if is a number convert string to number
          if (isanum){
            
            cout<<"its a number"<<endl;
            number = stoi(word);
            Quantities.push_back(number);
            
            
          }
          else {
         //If not a number   
                
            notANumber=word;
            cout<<"its not a number"<<endl;
            BookingDetails.push_back(notANumber);
            }
         //Clear word so can add next word/number
          word="";
        }
        }

        //goes through to check what type of ticket the user is booking and will create a price variable for the entry cost depending on quanity
        // will do this for all ticket types
        if (BookingDetails[1] == "saturday")
        {
        float AdultPrice = 25.0;
        float ChildPrice = 20.0;
        
        float AdultCost = Quantities[0] * AdultPrice;
        float ChildCost = Quantities[1] * ChildPrice;
        
        
        EntryCost = AdultCost+ChildCost;
        
        }
        else if (BookingDetails[1] == "VIP")
        {
        float AdultPrice = 50.0;
        float ChildPrice = 25.0;
        
        float AdultCost = Quantities[0] * AdultPrice;
        float ChildCost = Quantities[1] * ChildPrice;
        EntryCost = AdultCost+ChildCost;
        }
        else if (BookingDetails[1] == "sunday")
        {
        float AdultPrice = 10.0;
        float ChildPrice = 7.5;
        
        float AdultCost = Quantities[0] * AdultPrice;
        float ChildCost = Quantities[1] * ChildPrice;
        EntryCost = AdultCost+ChildCost;
        }
        else if (BookingDetails[1] == "weekend")
        {
        float AdultPrice = 30.0;
        float ChildPrice = 22.0;
        
        float AdultCost = Quantities[0] * AdultPrice;
        float ChildCost = Quantities[1] * ChildPrice;
        EntryCost = AdultCost+ChildCost;
        }

       //checks through what acitivies the user is booking and will apply appropriate activity costs depending on the activity and quantity of people
       //will do this for all activites 
        if (BookingDetails[2] == "baking")
        {
        float AdultPrice = 8.0;
        float ChildPrice = 5.0;
        
        float AdultCost = Quantities[2] * AdultPrice;
        float ChildCost = Quantities[3] * ChildPrice;
        ActivityCost = AdultCost+ChildCost;
        }
        else if (BookingDetails[2] == "dancing")
        {
        float AdultPrice = 15.0;
        float ChildPrice = 10.0;
        
        float AdultCost = Quantities[2] * AdultPrice;
        float ChildCost = Quantities[3] * ChildPrice;
        ActivityCost = AdultCost+ChildCost;
        }
        else if (BookingDetails[2] == "craft")
        {
        float AdultPrice = 10.0;
        float ChildPrice = 7.5;
        
        float AdultCost = Quantities[2] * AdultPrice;
        float ChildCost = Quantities[3] * ChildPrice;
        ActivityCost = AdultCost+ChildCost;
        }
        else if (BookingDetails[2] == "disco")
        {
        float AdultPrice = 15.0;
        float ChildPrice = 11.0;
        
        float AdultCost = Quantities[2] * AdultPrice;
        float ChildCost = Quantities[3] * ChildPrice;
        ActivityCost = AdultCost+ChildCost;;
        }
        else if (BookingDetails[2] == "N/A")
        {
        ActivityCost = 0.0;
        }
         
        //sends name of latest user into a vector storing names
        StoredNames.push_back(BookingDetails[0]);

        //calculates the total cost based on entry cost and acitivity cost
        TotalCost = EntryCost + ActivityCost;
        
        //loop that iterates through the stored names in a vector to check if users have made a booking before and sets cost to 0 to prevent from making anymore bookings.
        for(i=0;i<StoredNames.size()-1;i++){

            if (BookingDetails[0]==StoredNames[i]){
              TotalCost=0.0;
            }
        };

        //the vectors are cleared so there can be new input added for a different booking
        BookingDetails.clear();
       
        Quantities.clear();
        
        //prints to ensure vectors are cleared
        cout<<BookingDetails[2]<<endl;
        cout<<Quantities[3]<<endl;
        
        //prices are reset for the next user
        ActivityCost = 0.0;
        EntryCost = 0.0;
        
        
        sprintf(line," The cost of the  booking is: %.2f ", TotalCost);

      // Send converted line back to client.
      if (send(connectSocket, line, strlen(line) + 1, 0) < 0)
        cerr << "Error: cannot send modified data";

      memset(line, 0x0, LINE_ARRAY_SIZE);  // set line to all zeroes

      TotalCost= 0.0;
    }
  }
}