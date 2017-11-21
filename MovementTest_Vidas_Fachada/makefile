# Name of the executable created
Target := WizardsAndWarlocks

BinPath 			:= ./bin
BuildPath 			:= ./obj
PhysicsEnginePath 	:= ./obj/PhysicsEngine
GraphicsEnginePath 	:= ./obj/GraphicEngine
SoundEnginePath 	:= ./obj/SoundEngine
SourcePath			:= $(shell find src -name '*.cpp')


CXXFLAGS 			:= -O3 -ffast-math -g -Wall
CPPFLAGS        	:= -I/usr/include/irrlicht/ -I/usr/include/bullet -I./libs/fmod/include
LIBS 				:= -lIrrlicht -lXxf86vm -lXext -lX11 -lXcursor -lGL -lBulletDynamics -lBulletCollision -lLinearMath -lraknet -lsfml-system -lsfml-graphics -lsfml-window 
#-lfmodex (SOLO ES LA DE BAJO NIVEL Y EN TDS TRABAJAMOS CON LA DE ALTO NIVEL, QUE HAY QUE DESCARGARSELA DE LA PAGINA OFICIAL)

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

clean:
	$(warning Cleaning...)
	@$(RM) $(EXECUTABLE)
	@$(RM) $(OBJ)