#include "webserv.hpp"
//#include "socketServer.hpp"

int main (int ac, char **av)
{
    if (ac != 2)
        return (1);
    manyServer* servers = new manyServer(av[1]);
    std::vector<Webserver> servs;
    std::vector<int>::iterator ITports;
    std::vector<int> ports;
    for (int i = 0; i < servers->_name_server.size(); i++)
    {
        std::cout << "------------------------" << std::endl;
        std::cout << servers->_name_server[i].listen << std::endl;
        Webserver serv(servers->_name_server[i]);
        serv.CreateServer();
        serv.SelectSetsInit();
        servs.push_back(serv);
        ports.push_back(servers->_name_server[i].listen);
    }
    while (1)
    {
        for (int i = 0; i < servers->_name_server.size(); i++)
        {
            servs[i].StartServer(servers,i);
        }
    }
    return (0);
}