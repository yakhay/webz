# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: wbouwach <wbouwach@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/12/19 22:08:42 by wbouwach          #+#    #+#              #
#    Updated: 2024/01/19 03:19:36 by wbouwach         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = server

SRC = configFile.cpp   ../get_method/main.cpp \
	../req_res_post/post.cpp ../req_res_post/request.cpp ../req_res_post/respons.cpp ../get_method/getMethod.cpp 
	
SRRC = configFile.cpp    \
	../req_res_post/post.cpp ../req_res_post/request.cpp ../req_res_post/respons.cpp ../get_method/getMethod.cpp\
	../server/webserver.cpp ../server/Handling.cpp ../server/main.cpp

OBJ = $(SRRC:.cpp=.o)

CPPFLAGS =  -std=c++98

all: $(NAME)

$(NAME): $(OBJ)
	c++ $(CPPFLAGS) $(OBJ) -o $(NAME)

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean all