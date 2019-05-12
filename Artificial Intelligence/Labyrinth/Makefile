CC = g++
CFLAGS = -std=c++11 -O3 
TARGET1 = Labyrinth_A
SRCS1 = Labyrinth_A.cpp
TARGET2 = Labyrinth_IDA
SRCS2 = Labyrinth_IDA.cpp


$(TARGET1):$(SRCS1)
	$(CC) $(CFLAGS) -o $(TARGET1) $(SRCS1)
$(TARGET2):$(SRCS2)
	$(CC) $(CFLAGS) -o $(TARGET2) $(SRCS2)

clean:
	rm -rf $(TARGET1) $(TARGET2)
