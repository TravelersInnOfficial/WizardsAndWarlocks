# Name of the executable created
Target := WizardsAndWarlocks

BinPath 			:= ./bin
BuildPath 			:= ./obj
PhysicsEnginePath 	:= ./obj/PhysicsEngine
GraphicsEnginePath 	:= ./obj/GraphicEngine
SoundEnginePath		:= ./obj/SoundEngine
SpellPath			:= ./obj/Spells
BulletPath			:= ./obj/Bullets
EffectPath			:= ./obj/Effects
SourcePath			:= $(shell find src -name '*.cpp')


CXXFLAGS 			:= -O3 -ffast-math -g -Wall
CPPFLAGS        	:= -I/usr/include -I/usr/include/bullet
LDFLAGS				:= -L./libs/fmod
LIBS 				:= -lIrrlicht -lXxf86vm -lXext -lX11 -lXcursor -lGL -lBulletDynamics -lBulletCollision -lLinearMath -lraknet -lfmod -lfmodstudio 

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
	mkdir -p $(PhysicsEnginePath)
	mkdir -p $(GraphicsEnginePath)
	mkdir -p $(SoundEnginePath)
	mkdir -p $(SpellPath)
	mkdir -p $(BulletPath)
	mkdir -p $(EffectPath)

clean:
	$(warning Cleaning...)
	@$(RM) $(EXECUTABLE)
	@$(RM) $(OBJ)

	




