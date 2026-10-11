cmake_minimum_required(VERSION 3.16)

# 1. Project metadata
project(VortexServer 
    VERSION 0.1.0 
    LANGUAGES CXX
)

# 2. Enforce C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 3. Enable standard compiler warnings
add_compile_options(-Wall -Wextra -Wpedantic)

# 4. Gather source files
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS "src/*.cpp")

# 5. Define the target executable
add_executable(vortex ${SRC_FILES})

# 6. Expose the include/ directory to the target
target_include_directories(vortex PRIVATE include)

# 7. Link POSIX threads library
find_package(Threads REQUIRED)
target_link_libraries(vortex PRIVATE Threads::Threads)
