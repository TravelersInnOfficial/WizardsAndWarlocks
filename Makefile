# Name of the executable created
Target := WizardsAndWarlocks

BinPath 			:= ./bin
BuildPath 			:= ./obj

SOURCE_DIRS			:= $(shell find ./src -type d -not -path "./src/.vscode" -not -path "./src")
SourcePath			:= $(shell find src -name '*.cpp')

CXX					:= clang++
CXXFLAGS 			:= -O3 -ffast-math -g -Wall -std=c++11
CPPFLAGS        	:= -I/usr/include -I/usr/include/bullet -I./src/Includes
LDFLAGS				:= -L./libs/fmod
LIBS 				:= -lIrrlicht -lXxf86vm -lXext -lX11 -lXcursor -lGL -lBulletDynamics -lBulletCollision -lLinearMath -lraknet -lfmod -lfmodstudio

EXECUTABLE 			:= $(BinPath)/$(Target)
SRC 	   			:= $(wildcard $(SourcePath)/*.cpp)
OBJ					:= $(patsubst src/%.cpp,obj/%.o,$(SourcePath))

# Assuming four folders to ignore
GAMEOBJ				 = $(shell find src \( -path "src/PhysicsEngine" -o -path "src/SoundEngine" -o -path "src/GraphicEngine" -o -path "src/NetworkEngine" \) -prune -o -name "*.cpp" -print)
SOFTCLEAN			 = $(patsubst src/%.cpp,obj/%.o,$(GAMEOBJ))

SOURCE_DIRS 		:= $(patsubst ./src/%,./obj/%,$(SOURCE_DIRS))

#MAKE OPTIONS
.PHONY: all clean cleanall

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

clean:
	$(info ==============================================)
	$(info Cleaning Objects (Not Engine ones) and Binaries... )
	$(info ==============================================)
	@$(RM) $(SOFTCLEAN)
	@$(RM) $(EXECUTABLE)

cleanall:
	$(info ==============================================)
	$(info Cleaning every Objects and Binaries... )
	$(info ==============================================)
	@$(RM) $(OBJ)
	@$(RM) $(EXECUTABLE)