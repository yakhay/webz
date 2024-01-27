/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getMethod.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wbouwach <wbouwach@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/26 18:40:51 by wbouwach          #+#    #+#             */
/*   Updated: 2024/01/19 17:58:23 by wbouwach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "getMethod.hpp"
#include <fstream>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
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

// void send_not_found_respons(int newsockfd) {
//     char resp[] = "HTTP/1.0 404 Not Found\r\n"
//                   "Server: webserver-c\r\n"
//                   "Content-Length: 25\r\n"
//                   "Content-type: text/html\r\n\r\n"
//                   "<html><h1>404 Not Found</h1></html>\r\n"; 
//     int valwrite = send(newsockfd, resp, strlen(resp), 0);

//     if (valwrite < 0) {
//         perror("webserver (write) failed");
//     }
// }

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
int test3 = 0;
std::ifstream fileStream;
size_t  max = 0;
size_t size = 0;
int bytesRead = 1;

void send_forbidden_response(int newsockfd) {
    const char resp[] = "HTTP/1.1 403 Forbidden\r\n"
                        "Server: webserver-c\r\n"
                        "Content-Length: 25\r\n"
                        "Content-type: text/html\r\n\r\n"
                        "<html><h1> 403 Forbidden </h1></html>\r\n";
    send(newsockfd, resp, strlen(resp), 0);
}

void send_not_found_response(int newsockfd) {
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

void send_response_200(const std::string &filename, const std::string &contentType, int newsockfd) {
    std::ifstream fileStream(filename, std::ios::binary);
    if (!fileStream.is_open()) {
        send_not_found_response(newsockfd);
        return;
    }

    struct stat statbuf;
    if (stat(filename.c_str(), &statbuf) == -1) {
        std::cerr << "Error getting file information." << std::endl;
        send_not_found_response(newsockfd);
        return;
    }

    size_t size = statbuf.st_size;
    std::cout << "size: " << size << std::endl;

    std::string response = "HTTP/1.1 200 OK\r\nServer: webserver-c\r\nContent-Length: " + std::to_string(size) + "\r\nContent-type: " + contentType + "\r\n\r\n";
    int dd = send(newsockfd, response.c_str(), response.size(), 0);

    if (dd < 0) {
        perror("webserver (write) failed");
        fileStream.close();
        return;
    }

    const size_t buffer_size = 1024;
    std::vector<char> buffer(buffer_size, 0);

    while (fileStream.read(buffer.data(), buffer_size).gcount() > 0) {
        int valwrite = send(newsockfd, buffer.data(), fileStream.gcount(), 0);
        if (valwrite < 0) {
            perror("webserver (write) failed");
            fileStream.close();
            return;
        }
    }

    fileStream.close();
}
const char *sersh_map(std::map<std::string, std::string> map, std::string key)
{
        std::map<std::string, std::string>::iterator it = map.find(key);
        if (it != map.end())
        {
            return (it->second.c_str());
        }
        else
            return (NULL);

}
// void run_cgi(std::vector<class Location>::iterator it,one_server server, std::string new_path)
// {
//     int fd = open("temp_output.html", O_CREAT | O_RDWR);

//     if (fd < 0) {
//         std::cerr << "Error creating temporary file." << std::endl;
//         return ;
//     }

//     // Fork a new process
//     pid_t pid = fork();

//     if (pid == -1) {
//         std::cerr << "Error forking process." << std::endl;
//         return ;
//     } else if (pid == 0) { // Child process
//         // Redirect child's standard output to the temporary file
//         dup2(fd, STDOUT_FILENO);

//         // Prepare arguments for execve
//         std::string str = new_path.substr(new_path.find_last_of(".") + 1);
        
//         char* phpArgs[] = {
//             (char *)str.c_str(),
//             (char *)new_path.c_str(),
//             nullptr
//         };

//         // Execute PHP script using execve
//         if (execve(sersh_map(it->cgi, "." + str), phpArgs, nullptr) == -1) {
//             std::cerr << "Error executing PHP script." << std::endl;
//             return ;
//         }
//     } else { // Parent process
//         // Wait for the child process to complete
//         int status;
//         waitpid(pid, &status, 0);
//         close(fd);
//     }
// }
void get_method(request &req, manyServer *server, int newsockfd, int idx) {
    std::string new_path;
    std::string contentType;

    std::vector<class Location>::iterator it = server->_name_server[idx]._location.begin();

    while (it != server->_name_server[idx]._location.end()) {
        if (req.get_path().find(it->_name_Location) != std::string::npos) {
            new_path = it->_root + req.get_path().substr(it->_name_Location.size());
            break;
        }
        ++it;
    }
    if (it == server->_name_server[idx]._location.end()) {
        send_not_found_response(newsockfd);
        return;
    }

    if (it->_return) {
        if (it->cgi_status == 0)
        {
            std::string ret = it->_return;
            contentType = determine_content(ret.substr(ret.find_last_of(".") + 1));
            send_response_200(ret, contentType, newsockfd);
            return;
        }
        else
        {
            if (test3 == 0)
            {
                //##run_cgi(it, server->_name_server[idx], new_path);
                test3 = 1;
            }
             std::string ret = "temp_output.html";
            contentType = determine_content(ret.substr(ret.find_last_of(".") + 1));
            send_response_200("temp_output.html", contentType, newsockfd);
        }
    }

    if (!isDirectory(new_path)) {
        if(it->cgi_status == 0)
        {
            contentType = determine_content(new_path.substr(new_path.find_last_of(".") + 1));
            send_response_200(new_path, contentType, newsockfd);
        }
        else
        {
            // ##run cgi; new_path
        }
         }
        else if (it->index) {
            std::string index = (it->index);
            if (it->cgi_status == 1) {
                //##run cgi new_path +"/" + index
            } else {
                contentType = determine_content(index.substr(index.find_last_of(".") + 1));
                send_response_200(new_path +"/" + index, contentType, newsockfd);
        }
    } else if (it->_autoindex == 1) {
        if (test2 == 0) {
            std::remove("../Media/listing.html");
            std::fstream outputFile("../Media/listing.html", std::ios::in | std::ios::out | std::ios::app);
            outputFile.seekp(0, std::ios::beg);
            outputFile << "";
            if (!outputFile.is_open()) {
                std::cerr << "Error opening the file: " << std::endl;
                return;
            }
            test2 = 1;

            std::string buffer = "<html><head>\n<title>Directory Listing</title></head><body>\n<h1>Directory Listing</h1><ul>\n";
            DIR *dir = opendir(new_path.c_str());

            if (dir == nullptr) {
                std::cerr << "Error opening directory." << std::endl;
                return;
            }
            struct dirent *entry;
            while ((entry = readdir(dir)) != nullptr) {
                if (entry->d_type == DT_REG) {
                    std::string filename = entry->d_name;
                    if (buffer.find(entry->d_name) == std::string::npos)
                        buffer = buffer + "<li> <a href=\"" + req.get_path() + "/" + filename + "\">" + filename + "</a></li> \n";
                    else
                        break;
                }
            }
            outputFile << buffer + "</ul></body></html>\r\n";
            outputFile.close();
            closedir(dir);
            contentType = "text/html";
            send_response_200("../Media/listing.html", contentType, newsockfd);
        } else
            send_response_200("../Media/listing.html", contentType, newsockfd);
    } 
    else {
        if (test2 == 0) {
            char resp[] = "HTTP/1.1 403 forbidden\r\n"
                          "Server: webserver-c\r\n"
                          "Content-Length: 25\r\n"
                          "Content-type: text/html\r\n\r\n"
                          "<html><h1> 403 forbidden </h1></html>\r\n";
            int valwrite = send(newsockfd, resp, strlen(resp), 0);

            if (valwrite < 0) {
                perror("webserver (write) failed");
            }
            test2 = 1;
        }
    }
}
