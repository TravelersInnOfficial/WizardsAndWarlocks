# Name of the executable created
Target := WizardsAndWarlocks

IrrlichtPath 		:= ./Irrlicht
BinPath 			:= ./bin
BuildPath 			:= ./obj
PhysicsEnginePath	:= ./obj/PhysicsEngine
GraphicsEnginePath 	:= ./obj/GraphicsEngine
SourcePath			:= $(shell find src -name '*.cpp')

CXXFLAGS 		:= -O3 -ffast-math -g -Wall
CPPFLAGS        := -I/usr/include/irrlicht/ -I/usr/include/bullet
LIBS 			:= -lIrrlicht -lXxf86vm -lXext -lX11 -lXcursor -lGL -lBulletDynamics -lBulletCollision -lLinearMath -lraknet -lsfml-system -lsfml-graphics -lsfml-window

EXECUTABLE 		:= $(BinPath)/$(Target)
SRC 	   		:= $(wildcard $(SourcePath)/*.cpp)
OBJ 			:= $(subst .cpp,.o,$(SRC))
OBJ 			:= $(subst $(SourcePath),$(BuildPath),$(OBJ))

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
	mkdir -p $(PhysicsEnginePath)
	mkdir -p $(GraphicsEnginePath)

clean:
	$(warning Cleaning...)
	@$(RM) $(EXECUTABLE)
	@$(RM) $(OBJ)

	




