ifeq ($(OS),Windows_NT)
    Target				:= WizardsAndWarlocks.exe
    CXXFLAGS			:= -O3 -ffast-math -Wall -std=c++11 -m64
    CPPFLAGS        	:= -I./src/Includes -I/mingw64/include -I/mingw64/include/bullet
    LDFLAGS				:= -L/mingw64/lib
    LIBS 				:= -lopengl32 -lm -lIrrlicht -lBulletDynamics -lBulletCollision -lLinearMath -lRakNet -lfmod64 -lfmodstudio64
	ICO 				:= assets/game-icon-res.rc
	ICOOBJ				:= $(patsubst assets/%.rc,obj/%.o,$(ICO))
else
    Target				:= WizardsAndWarlocks
    CXXFLAGS			:= -O3 -ffast-math -g -Wall -std=c++11
    CPPFLAGS        	:= -I/usr/include -I/usr/include/bullet -I./src/Includes
    LDFLAGS				:= -L./libs/fmod
    LIBS 				:= -lIrrlicht -lXxf86vm -lXext -lX11 -lXcursor -lGL -lBulletDynamics -lBulletCollision -lLinearMath -lraknet -lfmod -lfmodstudio
endif

BinPath 			:= ./bin
BuildPath 			:= ./obj

SOURCE_DIRS			:= $(shell find ./src -type d -not -path "./src/.vscode" -not -path "./src")
SourcePath			:= $(shell find src -name '*.cpp')

CXX					:= clang++

EXECUTABLE 			:= $(BinPath)/$(Target)
SRC 	   			:= $(wildcard $(SourcePath)/*.cpp)
OBJ					:= $(patsubst src/%.cpp,obj/%.o,$(SourcePath))

# Assuming four folders to ignore
GAMEOBJ				 = $(shell find src \( -path "src/PhysicsEngine" -o -path "src/SoundEngine" -o -path "src/GraphicEngine" -o -path "src/NetworkEngine" \) -prune -o -name "*.cpp" -print)
SOFTCLEAN			 = $(patsubst src/%.cpp,obj/%.o,$(GAMEOBJ))
SOURCE_DIRS 		:= $(patsubst ./src/%,./obj/%,$(SOURCE_DIRS))

#MAKE OPTIONS
.PHONY: all clean cleanall ico

all: prepare ico $(OBJ)
	$(info ==============================================)
	$(info Linking executable $(Target)...)
	@$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(OBJ) -o $(EXECUTABLE) $(LDFLAGS) $(LIBS) $(ICOOBJ)
	$(info ==============================================)	
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

ico:
    ifeq ($(OS),Windows_NT)
		$(info ==============================================)
		$(info Creating ico .o file)
		$(info Compiling-> $(ICOOBJ))
		$(info ==============================================)
		$(shell windres $(ICO) -o $(ICOOBJ))
    endif

clean:
	$(info ==============================================)
	$(info Cleaning Objects (Not Engine ones), Binaries or ICO File... )
	$(info ==============================================)
	@$(RM) $(SOFTCLEAN)
	@$(RM) $(EXECUTABLE)

cleanall:
	$(info ==============================================)
	$(info Cleaning every Objects and Binaries... )
	$(info ==============================================)
	@$(RM) $(OBJ)
	@$(RM) $(ICOOBJ)
	@$(RM) $(EXECUTABLE)