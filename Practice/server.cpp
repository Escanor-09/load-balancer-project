#include<iostream>
#include "httplib.h"

void handle_client(const httplib::Request &req, httplib::Response &res){

    std::string msg_from_client = req.body;
    std::thread::id thread_id = std::this_thread::get_id();
    std::cout<<"Thread Id: "<<thread_id<<std::endl;
    std::cout<<"Message from the client "<< msg_from_client << std::endl;

    std::string msg_to_the_client = "Received Message " + msg_from_client;
    res.set_content(msg_to_the_client,"text/plain");
}

void handle_chat(const httplib::Request &req, httplib::Response &res){
    std::string msg_from_the_client = req.body;
    std::thread::id thread_id = std::this_thread::get_id();
    std::cout<<"Thread Id: "<<thread_id<<std::endl;
    std::cout<<"Message from the client "<<msg_from_the_client<<std::endl;

    std::string msg_to_the_client = "Received " + msg_from_the_client;
    res.set_content(msg_to_the_client,"text/plain"); 
}

int main(){

    httplib::Server server;

    server.Post("/chats",handle_client);
    server.Post("/chat",handle_chat);

    std::cout<<"Server is listening at port 8080\n";
    server.listen("0.0.0.0",8080);
}