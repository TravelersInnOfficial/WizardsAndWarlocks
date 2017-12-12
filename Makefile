# Name of the executable created
Target := WizardsAndWarlocks

BinPath 			:= ./bin
BuildPath 			:= ./obj
AIPath				:= ./obj/AI/Tasks
SteeringPath		:= ./obj/AI/SteeringBehaviour
PathfindingPath		:= ./obj/AI/Pathfinding

SOURCE_DIRS			:= $(shell find ./src -maxdepth 1 -type d -not -path "./src/.vscode" -not -path "./src" -printf '$(BuildPath)/%f ')
SourcePath			:= $(shell find src -name '*.cpp')

CXX					:= clang++
CXXFLAGS 			:= -O3 -ffast-math -g -Wall -std=c++11
CPPFLAGS        	:= -I/usr/include -I/usr/include/bullet -I./src/Includes
LDFLAGS				:= -L./libs/fmod
LIBS 				:= -lIrrlicht -lXxf86vm -lXext -lX11 -lXcursor -lGL -lBulletDynamics -lBulletCollision -lLinearMath -lraknet -lfmod -lfmodstudio

EXECUTABLE 			:= $(BinPath)/$(Target)
SRC 	   			:= $(wildcard $(SourcePath)/*.cpp)
OBJ					:= $(patsubst src/%.cpp,obj/%.o,$(SourcePath))

#MAKE OPTIONS
.PHONY: all clean

all: prepare $(OBJ)
	$(info ----------------------------------------------)
	$(info Linking executable $(Target)...)
	@$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(OBJ) -o $(EXECUTABLE) $(LDFLAGS) $(LIBS)
	$(info ----------------------------------------------)	
	$(info Compile OK)

obj/%.o: src/%.cpp
	$(info Compiling-> $@)
	@$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

prepare:
	$(info ==============================================)
	$(info Creating folder structure)
	$(info ==============================================)
	@mkdir -p $(BinPath)
	@mkdir -p $(SOURCE_DIRS)
	@mkdir -p $(AIPath)
	@mkdir -p $(SteeringPath)
	@mkdir -p $(PathfindingPath)

clean:
	$(info ==============================================)
	$(info Cleaning Objects and Binaries... )
	$(info ==============================================)
	@$(RM) $(OBJ)
	@$(RM) $(EXECUTABLE)