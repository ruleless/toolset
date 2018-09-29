CC:=gcc
CXX:=g++
MAKE:=make

CFLAGS:=-rdynamic -g -Wall -I.
CXXFLAGS:=-rdynamic -std=c++11 -g -Wall -I.

LDFLAGS:=

RM=-rm -rf

BIN:=toolset
SRCS+=$(wildcard lib/*.cpp)
SRCS+=$(wildcard src/*.cpp)
OBJS=$(SRCS:%.cpp=%.o)
DEPENDS=$(SRCS:%.cpp=%.d)


all:$(BIN)

toolset:$(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

$(OBJS):%.o:%.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS)

-include $(DEPENDS)

$(DEPENDS):%.d:%.cpp
	set -e; rm -f $@; \
	echo -n $(dir $<) > $@.$$$$; \
	$(CXX) -MM $(CXXFLAGS) $< >> $@.$$$$; \
	sed 's,\($*\)\.o[:]*,\1.o $@:,g' < $@.$$$$ > $@; \
	rm $@.$$$$

clean:
	$(RM) $(BIN) $(OBJS) $(DEPENDS) 

fake:
	@echo $(OBJS)
