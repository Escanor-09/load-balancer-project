#include<iostream>
#include"httplib.h"

int main(){

    httplib::Client cli("localhost",8080); //creating client object
    std::string message_to_server;

    std::cout<<"Enter the message to the server (type 'exit' to quit):\n";

    while(true){
        std::cout<<">>";
        std::getline(std::cin,message_to_server);

        if(message_to_server == "exit") break;

        //send POST request
        if(auto res = cli.Post("/chat",message_to_server,"text/plain")){
            std::cout<<"Serevr Replied: "<<res->body <<std::endl;
        }else{
            std::cout<<"Request failed";
        }
    }
}