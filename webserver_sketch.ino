#include <SPI.h>
#include <Ethernet.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xFE, 0xED};
byte ip[] = { };
 /*
Enter the IP address of the respective server replacing the '.' delimiter with ',', 
for example, if your IP Address is 89.002.956.400 then 
byte ip[] = { 89, 2, 956, 400}
  */
EthernetServer server(80);

void setup() 
{
  Ethernet.begin(mac, ip);
  server.begin();
}

void loop() {
  EthernetClient client = server.available();
  if (client){
    // an http request always ends with a blank line
    boolean current_line_blank = true;
    while (client.connected()){
      if (client.connected()){
        char c = client.read();
        //if we have reached the end of the line (received a newline character)
        //and the line is blank, the http request has ended,
        //so we can send a reply
        if (c == '\n' && current_line_blank){
          //send a standard http reponse header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println();

          //output the value of each analog input pin
          for (int i = 0; i < 6; i++){
            client.print("analog input ");
            client.print(i);
            client.print(" is ");
            client.print(analogRed(i));
            client.println("<br /> ");
          }
          break;
        }
        if (c == '\n'){
          //starting a new line
          current_line_blank = true;
        } else if (c !='r'){
          //we have gotten a charcter on the current line
          current_line_blank = false;
        }
      }
    }
    //give the browser some time to receive the data
    delay(1)
    client.stop();
  }
}
