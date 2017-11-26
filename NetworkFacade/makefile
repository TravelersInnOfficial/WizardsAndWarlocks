# Name of the executable created
Target := WizardsAndWarlocks

BinPath 			:= ./bin
BuildPath 			:= ./obj
NetworkEnginePath 	:= ./obj/NetworkEngine
SourcePath			:= $(shell find src -name '*.cpp')

CXXFLAGS 			:= -O3 -ffast-math -g -Wall
CPPFLAGS        	:= -I/usr/include/raknet
LIBS 				:= -lraknet

EXECUTABLE 			:= $(BinPath)/$(Target)
SRC 	   			:= $(wildcard $(SourcePath)/*.cpp)
OBJ					:= $(patsubst src/%.cpp,obj/%.o,$(SourcePath))

#MAKE OPTIONS
.PHONY: all clean

all: prepare $(OBJ)
	$(info Creando ejecutable $(Target)...)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(OBJ) -o $(EXECUTABLE) $(LDFLAGS) $(LIBS)

obj/%.o: src/%.cpp
	$(warning Creando el binario $@...)
	$(CC) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

prepare:
	$(warning Creando la estructura de carpetas)
	mkdir -p $(BinPath)
	mkdir -p $(BuildPath)
	mkdir -p $(NetworkEnginePath)

clean:
	$(warning Cleaning...)
	@$(RM) $(EXECUTABLE)
	@$(RM) $(OBJ)