GESRCPATH				=src/Engines/TravelersOcularEngine
INCLUDE_FOLDERS			=-I./src/Includes -I./src/Engines/TravelersOcularEngine/src/Common -I./src/Engines/ -I./$(GESRCPATH)/src/Common
USER_FLAGS				=
USER_FLAGS_C			=

# DONT EDIT ---------------------------------------------------

CPPFLAGS        	:= $(INCLUDE_FOLDERS)
CXXFLAGS			:= -O3 -Wall -std=c++14 $(USER_FLAGS)
CCFLAGS				:= -O3 -Wall $(USER_FLAGS_C)
LIBS 				:= -lIrrlicht -lBulletDynamics -lBulletCollision -lLinearMath -lassimp

ifeq ($(OS),Windows_NT)
    Target				:= WizardsAndWarlocks.exe
    CPPFLAGS        	+= -I/mingw64/include -I/mingw64/include/bullet
    LDFLAGS				:= -L/mingw64/lib -L/mingw64/lib/SFML
    LIBS 				+= -lRakNet -lfmod64 -lfmodstudio64 -lopengl32 -lglew32 -lm 
	ICO 				:= assets/game-icon-res.rc
	ICOOBJ				:= $(patsubst assets/%.rc,obj/%.o,$(ICO))
else
    Target				:= WizardsAndWarlocks
    CPPFLAGS        	+= -I/usr/include -I/usr/include/bullet
    LDFLAGS				:= -L./libs/Linux
    LIBS 				+= -lraknet -lfmod -lfmodstudio -lXxf86vm -lXext -lX11 -lXcursor -lGL -lGLEW -lglfw
endif

BinPath 			:= ./bin
BuildPath 			:= ./obj

SOURCE_DIRS			:= $(shell find "./$(GESRCPATH)/src" -type d)
SOURCE_DIRS			+= $(shell find ./src -type d -not -path "./src" -not -path "./$(GESRCPATH)/*")
SourcePath			:= $(shell find src -name '*.c')
SourcePath			+= $(shell find src -name '*.cpp' -not -path '$(GESRCPATH)/src/main.cpp')

CXX					:= clang++
CC					:= clang

EXECUTABLE 			:= $(BinPath)/$(Target)
OBJ					:= $(patsubst src/%.cpp,obj/%.o,$(SourcePath))			# Subst src path by obj
OBJ					:= $(patsubst src/%.c,obj/%.o,$(OBJ))					# Subst src path by obj

# Assuming four folders to ignore
GAMEOBJ				 = $(shell find src \( -path "src/Engines" \) -prune -o -name "*.cpp" -print)
SOFTCLEAN			 = $(patsubst src/%.cpp,obj/%.o,$(GAMEOBJ))
SOURCE_DIRS 		:= $(patsubst ./src/%,./obj/%,$(SOURCE_DIRS))

#MAKE OPTIONS
.PHONY: all clean cleanall ico

all: prepare ico compile
	$(info Compile OK)

compile: $(OBJ)
	$(info ==============================================)
	$(info Linking executable $(Target)...)
	@$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(OBJ) -o $(EXECUTABLE) $(LDFLAGS) $(LIBS) $(ICOOBJ)
	$(info ==============================================)	

# Compile .cpp files
obj/%.o: src/%.cpp
	$(info Compiling-> $@)
	@$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c $< -o $@

# Compile .c files
obj/%.o: src/%.c
	$(info Compiling-> $@)
	@$(CC) $(CCFLAGS) $(CPPFLAGS) -c $< -o $@

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