CC = g++
CFLAGS = -std=c++11 -O3 
TARGET = gobang
SRCS = operate.cpp evaluate.cpp alphaBetaSearch.cpp gobang.cpp display.cpp main.cpp

$(TARGET):$(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

clean:
	rm -rf $(TARGET)
