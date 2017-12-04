# Name of the executable created
Target := WizardsAndWarlocks

BinPath 			:= ./bin
BuildPath 			:= ./obj
SOURCE_DIRS			:= $(shell find ./src -maxdepth 1 -type d -not -path "./src/.vscode" -not -path "./src" -printf '$(BuildPath)/%f ')
SourcePath			:= $(shell find src -name '*.cpp')

CXXFLAGS 			:= -O3 -ffast-math -g -Wall
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
	$(info Done)

obj/%.o: src/%.cpp
	$(info Compiling-> $@)
	@$(CC) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

prepare:
	$(info ==============================================)
	$(info Creating folder structure)
	$(info ==============================================)
	@mkdir -p $(BinPath)
	@mkdir -p $(SOURCE_DIRS)

clean:
	$(info ==============================================)
	$(info Cleaning Objects and Binaries... )
	$(info ==============================================)
	@$(RM) $(OBJ)
	@$(RM) $(EXECUTABLE)

	




