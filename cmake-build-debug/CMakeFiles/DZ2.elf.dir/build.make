# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.25

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = D:\Toolbox\apps\CLion\ch-0\231.8770.66\bin\cmake\win\x64\bin\cmake.exe

# The command to remove a file.
RM = D:\Toolbox\apps\CLion\ch-0\231.8770.66\bin\cmake\win\x64\bin\cmake.exe -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "C:\Users\xyarc\YandexDisk\Byz\Digital devices and microprocessors\DZ2"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "C:\Users\xyarc\YandexDisk\Byz\Digital devices and microprocessors\DZ2\cmake-build-debug"

# Include any dependencies generated for this target.
include CMakeFiles/DZ2.elf.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/DZ2.elf.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/DZ2.elf.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/DZ2.elf.dir/flags.make

CMakeFiles/DZ2.elf.dir/src/Startup/startup_stm32f051r8tx.s.obj: CMakeFiles/DZ2.elf.dir/flags.make
CMakeFiles/DZ2.elf.dir/src/Startup/startup_stm32f051r8tx.s.obj: C:/Users/xyarc/YandexDisk/Byz/Digital\ devices\ and\ microprocessors/DZ2/src/Startup/startup_stm32f051r8tx.s
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="C:\Users\xyarc\YandexDisk\Byz\Digital devices and microprocessors\DZ2\cmake-build-debug\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building ASM object CMakeFiles/DZ2.elf.dir/src/Startup/startup_stm32f051r8tx.s.obj"
	C:\PROGRA~2\ARMGNU~1\12977F~1.2MP\bin\AR19DD~1.EXE $(ASM_DEFINES) $(ASM_INCLUDES) $(ASM_FLAGS) -o CMakeFiles\DZ2.elf.dir\src\Startup\startup_stm32f051r8tx.s.obj -c "C:\Users\xyarc\YandexDisk\Byz\Digital devices and microprocessors\DZ2\src\Startup\startup_stm32f051r8tx.s"

CMakeFiles/DZ2.elf.dir/src/Startup/startup_stm32f051r8tx.s.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing ASM source to CMakeFiles/DZ2.elf.dir/src/Startup/startup_stm32f051r8tx.s.i"
	C:\PROGRA~2\ARMGNU~1\12977F~1.2MP\bin\AR19DD~1.EXE $(ASM_DEFINES) $(ASM_INCLUDES) $(ASM_FLAGS) -E "C:\Users\xyarc\YandexDisk\Byz\Digital devices and microprocessors\DZ2\src\Startup\startup_stm32f051r8tx.s" > CMakeFiles\DZ2.elf.dir\src\Startup\startup_stm32f051r8tx.s.i

CMakeFiles/DZ2.elf.dir/src/Startup/startup_stm32f051r8tx.s.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling ASM source to assembly CMakeFiles/DZ2.elf.dir/src/Startup/startup_stm32f051r8tx.s.s"
	C:\PROGRA~2\ARMGNU~1\12977F~1.2MP\bin\AR19DD~1.EXE $(ASM_DEFINES) $(ASM_INCLUDES) $(ASM_FLAGS) -S "C:\Users\xyarc\YandexDisk\Byz\Digital devices and microprocessors\DZ2\src\Startup\startup_stm32f051r8tx.s" -o CMakeFiles\DZ2.elf.dir\src\Startup\startup_stm32f051r8tx.s.s

CMakeFiles/DZ2.elf.dir/src/main.c.obj: CMakeFiles/DZ2.elf.dir/flags.make
CMakeFiles/DZ2.elf.dir/src/main.c.obj: C:/Users/xyarc/YandexDisk/Byz/Digital\ devices\ and\ microprocessors/DZ2/src/main.c
CMakeFiles/DZ2.elf.dir/src/main.c.obj: CMakeFiles/DZ2.elf.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="C:\Users\xyarc\YandexDisk\Byz\Digital devices and microprocessors\DZ2\cmake-build-debug\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/DZ2.elf.dir/src/main.c.obj"
	C:\PROGRA~2\ARMGNU~1\12977F~1.2MP\bin\AR19DD~1.EXE $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/DZ2.elf.dir/src/main.c.obj -MF CMakeFiles\DZ2.elf.dir\src\main.c.obj.d -o CMakeFiles\DZ2.elf.dir\src\main.c.obj -c "C:\Users\xyarc\YandexDisk\Byz\Digital devices and microprocessors\DZ2\src\main.c"

CMakeFiles/DZ2.elf.dir/src/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/DZ2.elf.dir/src/main.c.i"
	C:\PROGRA~2\ARMGNU~1\12977F~1.2MP\bin\AR19DD~1.EXE $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E "C:\Users\xyarc\YandexDisk\Byz\Digital devices and microprocessors\DZ2\src\main.c" > CMakeFiles\DZ2.elf.dir\src\main.c.i

CMakeFiles/DZ2.elf.dir/src/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/DZ2.elf.dir/src/main.c.s"
	C:\PROGRA~2\ARMGNU~1\12977F~1.2MP\bin\AR19DD~1.EXE $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S "C:\Users\xyarc\YandexDisk\Byz\Digital devices and microprocessors\DZ2\src\main.c" -o CMakeFiles\DZ2.elf.dir\src\main.c.s

# Object files for target DZ2.elf
DZ2_elf_OBJECTS = \
"CMakeFiles/DZ2.elf.dir/src/Startup/startup_stm32f051r8tx.s.obj" \
"CMakeFiles/DZ2.elf.dir/src/main.c.obj"

# External object files for target DZ2.elf
DZ2_elf_EXTERNAL_OBJECTS =

DZ2.elf: CMakeFiles/DZ2.elf.dir/src/Startup/startup_stm32f051r8tx.s.obj
DZ2.elf: CMakeFiles/DZ2.elf.dir/src/main.c.obj
DZ2.elf: CMakeFiles/DZ2.elf.dir/build.make
DZ2.elf: CMakeFiles/DZ2.elf.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="C:\Users\xyarc\YandexDisk\Byz\Digital devices and microprocessors\DZ2\cmake-build-debug\CMakeFiles" --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable DZ2.elf"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\DZ2.elf.dir\link.txt --verbose=$(VERBOSE)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Building C:/Users/xyarc/YandexDisk/Byz/Digital devices and microprocessors/DZ2/cmake-build-debug/DZ2.hex"
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Building C:/Users/xyarc/YandexDisk/Byz/Digital devices and microprocessors/DZ2/cmake-build-debug/DZ2.bin"
	arm-none-eabi-objcopy -Oihex "C:/Users/xyarc/YandexDisk/Byz/Digital devices and microprocessors/DZ2/cmake-build-debug/DZ2.elf" "C:/Users/xyarc/YandexDisk/Byz/Digital devices and microprocessors/DZ2/cmake-build-debug/DZ2.hex"
	arm-none-eabi-objcopy -Obinary "C:/Users/xyarc/YandexDisk/Byz/Digital devices and microprocessors/DZ2/cmake-build-debug/DZ2.elf" "C:/Users/xyarc/YandexDisk/Byz/Digital devices and microprocessors/DZ2/cmake-build-debug/DZ2.bin"

# Rule to build all files generated by this target.
CMakeFiles/DZ2.elf.dir/build: DZ2.elf
.PHONY : CMakeFiles/DZ2.elf.dir/build

CMakeFiles/DZ2.elf.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\DZ2.elf.dir\cmake_clean.cmake
.PHONY : CMakeFiles/DZ2.elf.dir/clean

CMakeFiles/DZ2.elf.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" "C:\Users\xyarc\YandexDisk\Byz\Digital devices and microprocessors\DZ2" "C:\Users\xyarc\YandexDisk\Byz\Digital devices and microprocessors\DZ2" "C:\Users\xyarc\YandexDisk\Byz\Digital devices and microprocessors\DZ2\cmake-build-debug" "C:\Users\xyarc\YandexDisk\Byz\Digital devices and microprocessors\DZ2\cmake-build-debug" "C:\Users\xyarc\YandexDisk\Byz\Digital devices and microprocessors\DZ2\cmake-build-debug\CMakeFiles\DZ2.elf.dir\DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/DZ2.elf.dir/depend

