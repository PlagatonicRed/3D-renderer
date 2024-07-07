# Define variables for compiler and flags
CXX = g++
CXXFLAGS = -Isrc/include -L src/lib

# Define target for object files
OBJ = main.o

# Rule to compile .cpp files into .o files
%.o: %.cpp
	@echo "Compiling $<"
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Target to create static library
libmylibrary.a: $(OBJ)
	@echo "Creating static library $@"
	ar rcs libmylibrary.a $(OBJ)

# Final target to create the executable
main.exe: $(OBJ) libmylibrary.a
	@echo "Linking $@"
	$(CXX) $(CXXFLAGS) -o main.exe main.o -lmingw32 -lSDL2main -lSDL2 libmylibrary.a

# Default target
all: main.exe

# Clean target to remove compiled files
clean:
	@echo "Cleaning up..."
	del /F /Q *.o libmylibrary.a main.exe