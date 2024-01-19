/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getMethod.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yakhay <yakhay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/26 18:40:51 by wbouwach          #+#    #+#             */
/*   Updated: 2024/01/13 14:49:31 by yakhay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "getMethod.hpp"
#include <fstream>
#include <dirent.h>
bool isDirectory(const std::string& path) {
    DIR* dir = opendir(path.c_str());
    if (dir != nullptr) {
        closedir(dir);
        return true;
    }
    return false;
}
	
// #include "one.hpp"
// #include "../config_file/server.hpp"

void send_not_found_respons(int newsockfd) {
    char resp[] = "HTTP/1.0 404 Not Found\r\n"
                  "Server: webserver-c\r\n"
                  "Content-Length: 25\r\n"
                  "Content-type: text/html\r\n\r\n"
                  "<html><h1> 404 Not Found </h1></html>\r\n"; 
    int valwrite = send(newsockfd, resp, strlen(resp), 0);

    if (valwrite < 0) {
        perror("webserver (write) failed");
    }
}

std::string determine_content(const std::string& fileExtension) {
    if (fileExtension == "html") {
        return "text/html";
    } else if (fileExtension == "pdf") {
        return "application/pdf";
    } else if (fileExtension == "png") {
        return "image/png";
    } 
    else if (fileExtension == "mp4")
        return ("video/mp4");
    else {
        return "application/octet-stream";
    }
}

int test = 0;
int test2 = 0;
std::ifstream fileStream;
size_t  max = 0;
size_t size = 0;
int bytesRead = 1;
void send_response_200(std::string new_path,std::string contentType ,manyServer *server, int newsockfd, std::string val)
{
    struct stat statbuf;
    std::string response;
   
    if (test == 0)
    {
        fileStream.open(new_path, std::ios::in | std::ios::binary);
            if (!fileStream.is_open())
            {
                send_not_found_respons(newsockfd);
                exit(404);
            }
        if (stat(new_path.c_str(), &statbuf) == -1) {
            std::cerr << "Error getting file information." << std::endl;
            exit(128) ; 
        }

        size = statbuf.st_size;
        std::cout<<"size :"<<size<<std::endl;
        if (size == 0) {
                std::cerr << "Error: File size is zero." << std::endl;
                exit(128) ; 
            }
            response = "HTTP/1.1 "+ val + "OK\r\nServer: webserver-c\r\nContent-Length: " + std::to_string(size) + "\r\nContent-type: " + contentType + "\r\n\r\n";
            int dd = send(newsockfd, response.c_str(), response.size(), 0);
                // std::cout << "send: " << dd << std::endl;
                test = 1;
    }
    else {
         std::vector<char> buffer(1024);
         fileStream.read(buffer.data(), 1024);
         
            int bytesRead = fileStream.gcount();
            if (bytesRead != 0)
                {
                   int valwrite = send(newsockfd, buffer.data(), bytesRead, 0);
                    if (valwrite < 0) {
                        perror("webserver (write) failed");
                        // fileStream.close();
                            return;
                        }
                }
             
    }
    // if (ileStream.eof())
    // {
        
       // fileStream.close();
    // }
    
    
}

void get_method(request &req, manyServer *server, int newsockfd)
{
    
    std::string new_path;
    std::string fileExtension;
    std::string contentType;
    

    
    std::vector<class Location>::iterator it;
    it = server->_name_server[0]._location.begin();
    while (it != server->_name_server[0]._location.end())
    {
        // std::cout << req.get_path() <<"--"<< std::endl;
        if (req.get_path().find(it->_name_Location) != std::string::npos)
        {
            // std::cout << "---------"<< re << std::endl;
            new_path = it->_root + req.get_path().substr(it->_name_Location.size(),req.get_path().size() - it->_name_Location.size());
            //std::cout<< "---"<< req.get_path().substr(it->_name_Location.size(),req.get_path().size() - it->_name_Location.size()) << std::endl;
            break;
        }
        it++;
    }
    
    
    if (!isDirectory(new_path))
    {
        contentType = determine_content(new_path.substr(new_path.find_last_of(".") + 1));
        send_response_200(new_path,contentType ,server,newsockfd ,"200");
    }
    // else 
    // {
        
    //     if (it->_return)
    //     {
            
    //         std::string ret =it->_return; 
    //         contentType = determine_content(ret.substr(ret .find_last_of(".") + 1));
    //         send_response_200(ret,contentType ,server,newsockfd, "301");
    //     }
    //     else 
    //     if (it->index)
    //     {
    //         std::string index = (it->index);
    //         if (it->cgi_status == 1)
    //         {
    //             if (index.find("py") != std::string::npos)
    //                     printf("cgi_py();\n");
    //             else if (index.find("php") != std::string::npos)
    //                 printf("cgi_php();\n");
    //             else if (index.find("sh") != std::string::npos)
    //                 printf("cgi_sh();\n");
    //         }
    //         else 
    //         {
    //             contentType = determine_content(index .substr(index.find_last_of(".") + 1));
    //             send_response_200(new_path + index,contentType ,server,newsockfd, "200");
    //         }
            
    //     }
    //     else if (it->_autoindex == 1)
    //     {                
    //             if (test2 == 0)
    //             {
    //                 std::remove("../Media/listing.html");
    //                 std::fstream outputFile("../Media/listing.html", std::ios::in | std::ios::out | std::ios::app);
    //                 outputFile.seekp(0, std::ios::beg);
    //                 outputFile << "";
    //                 if (!outputFile.is_open()) {
    //                     std::cerr << "Error opening the file: "  << std::endl;
    //                     return ;
    //                 }
    //                 test2 = 1;
                
    //                 std::string buffer = "<html><head>\n<title>Directory Listing</title></head><body>\n<h1>Directory Listing</h1><ul>\n";
    //                 DIR* dir = opendir(new_path.c_str());

    //                 if (dir == nullptr) {
    //                     std::cerr << "Error opening directory." << std::endl;
    //                     return;
    //                 }
    //                 struct dirent* entry;
    //                 while ((entry = readdir(dir)) != nullptr) {
    //                     if (entry->d_type == DT_REG)
    //                     {
    //                         std::string filename =  entry->d_name;
    //                         if (buffer.find(entry->d_name) == std::string::npos)
    //                             buffer = buffer + "<li> <a href=\"" + req.get_path()+"/" + filename + "\">" + filename +  "</a></li> \n";
    //                         else
    //                             break;
    //                     }
    //                 }
    //                 // buffer = buffer + "</ul></body></html>\r\n";
    //                 //std::cout << "!!!!! " << buffer << std::endl;
    //                 //write (htmlFile, (buffer  + "</ul></body></html>\r\n").c_str(), (buffer + + "</ul></body></html>\r\n").size());
    //                 outputFile << buffer + "</ul></body></html>\r\n";
    //                 //std::cout<< buffer + "</ul></body></html>\r\n";
    //                 outputFile.close();
    //                 closedir(dir);
    //                 contentType = "text/html";
    //                 send_response_200("../Media/listing.html",contentType ,server,newsockfd, "200");
    //             }
    //             else 
    //                 send_response_200("../Media/listing.html",contentType ,server,newsockfd, "200");
    //     }
    //     else
    //     {
    //         if (test2 == 0)
    //         {
    //          char resp[] = "HTTP/1.1 403 forbidden\r\n"
    //               "Server: webserver-c\r\n"
    //               "Content-Length: 25\r\n"
    //               "Content-type: text/html\r\n\r\n"
    //               "<html><h1> 403 forbidden </h1></html>\r\n"; 
    //         int valwrite = send(newsockfd, resp, strlen(resp), 0);

    //         if (valwrite < 0) {
    //             perror("webserver (write) failed");
    //             }
    //             test2 = 1;
    //         }
    //     }
    // }







       
        //     if (it->cgi.size() != 0)
        //         send_response(new_path,contentType ,server,newsockfd);
        //     else
        //         {
        //             //cgi function
        //             std::cout << "cgi function" << std::endl;
        //         }
        // }
        // else
        // {
        //         if ((it->index).c_str() != NULL)
        //         {
        //             if (it->_autoindex == 1)
        //             {
        //                 contentType = determine_content((it->index).substr((it->index).find_last_of(".") + 1));
        //                 send_response(new_path + it->index,contentType ,server,newsockfd);
        //             }
        //             else if (it->cgi.size() == 0)
        //             {
        //                 contentType = determine_content((it->index).substr((it->index).find_last_of(".") + 1));
        //                 send_response(new_path + it->index,contentType ,server,newsockfd);
        //             }
        //             else if (it->cgi.size() != 0)
        //             {
        //                 //cgi function
        //                 std::cout << "cgi function" << std::endl;
        //             }
        //         }
        //         else if (it->_autoindex == 0)
        //         {
        //             //send 403 forbidden
        //             std::cout << "send 403 forbidden" << std::endl;
        //         }
        // }

    // }
    // else
    //     send_not_found_respons(newsockfd);
    // std::cout << "end get method  \n";
}
//uplod_path : /khay/ro;