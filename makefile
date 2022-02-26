CC = g++

CFLAGS = -g -Wall -std=c++11
LFLAGS = -I ./include -MMD -MP

SRCS = $(wildcard ./src/*.cpp)
OBJS = $(patsubst ./src/%.cpp,./bin/%.o, $(SRCS))
DEPS := $(patsubst %.o,%.d, $(OBJS))

#all targets 
all: ./bin/studio

# making Studio
./bin/studio: $(OBJS)
	$(CC) $(OBJS) -o $@


# build cpp files
./bin/%.o: ./src/%.cpp
	$(CC) $(LFLAGS) $(CFLAGS) -c $< -o $@

# clean files 
clean:
	@rm -f ./bin/*

-include $(DEPS)
