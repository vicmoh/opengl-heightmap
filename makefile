# Framework information for older version of MACOS
#INCLUDES = -F/System/Library/Frameworks -framework OpenGL -framework GLUT -lm

# Frameworks for newer MACOS, where include files are moved 
INCLUDES = -F/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/System/Library/Frameworks/ -framework OpenGL -framework GLUT -lm -Wno-deprecated-declarations
# Mac OS opengl target
TARGET = -mmacosx-version-min=10.8

# Vic's library
LIB_INC = -I./lib/include/ -I./include
LIB_FILE = ./lib/shared/*.so

# Variable for git comment.
m = [AUTO]

# Run the program.
run: compile
	./bin/run $(FILE)

# Compile the program.
compile:
	gcc -Wall -std=c11 ./src/*.c $(LIB_FILE) -o ./bin/run $(LIB_INC) $(INCLUDES) $(TARGET)

# Run git add commit push.
git:
	git add -A
	git commit -m '$(m)'
	git push

