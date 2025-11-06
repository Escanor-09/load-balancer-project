#include"httplib.h"
#include<pqxx/pqxx>
#include<iostream>

int main(){
    
    try
    {
        pqxx::connection conn("host=172.24.192.1 port=5432 dbname=kvstore user=postgres password=Escanor09@");

        if(conn.is_open()){
            std::cout<<"Connected to Database: "<<conn.dbname() <<std::endl;
        }else{
            std::cout<<"Connection Failed."<<std::endl;
        }

        pqxx::work txn(conn);

        //Testing query
        pqxx::result r = txn.exec("SELECT * FROM kv_data;");
        for(auto row : r){
            std::cout<<"Key: "<<row["key"].c_str()<<", Value: "<<row["value"].c_str()<<std::endl;
        }

        txn.commit();
        conn.close();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return 1;
    }
    
}