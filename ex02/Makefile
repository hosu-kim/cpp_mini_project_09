CXX := c++
CXXFLAGS := -Wall -Werror -Wextra -std=c++98

NAME := PmergeMe # DOC: The name of the program is PmergeMe.
SRCS := main.cpp PmergeMe.cpp
OBJS := $(SRCS:.cpp=.o)

all: $(NAME)
$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re