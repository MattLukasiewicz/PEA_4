CXX ?= g++
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra
SRC := $(wildcard src/*.cpp)
TARGET := pea3.exe

ifeq ($(OS),Windows_NT)
RUN := .\$(TARGET)
else
RUN := ./$(TARGET)
endif

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

run: $(TARGET)
	$(RUN)

clean:
	-$(RM) $(TARGET)
