## Prerequisites
Ensure you have a C++ compiler (like GCC/MinGW or Clang) and **CMake 3.10+** installed on your system.

### Build Instructions
To ensure a clean environment and avoid configuration conflicts, please follow these steps:

1. **Clean previous builds** (Recommended):
   Delete the existing `build/` directory if it exists.

2. **Configure the project**:
   This command prepares the build system and gathers your dependencies.
   ```bash
   cmake -B build

3. **Compile the executable**:
   This command will compile the code into an executable in the build folder.
   ```bash
   cmake --build build