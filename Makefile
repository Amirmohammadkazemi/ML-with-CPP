CXX = g++
CXXFLAGS = -std=c++17 -ldlib
TARGET = out
SRCS = main.cpp

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(SRCS) $(CXXFLAGS)
