cmake_minimum_required(VERSION 4.3.1)
project(snake LANGUAGES C CXX)
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(${PROJECT_NAME} 
    src/main.cpp
    src/game.cpp
    src/program.cpp
    src/screen.cpp
    src/subscreen.cpp
    src/environment.cpp

    # Imgui
    vendor/imgui/imgui.cpp
    vendor/imgui/imgui_demo.cpp
    vendor/imgui/imgui_draw.cpp
    vendor/imgui/imgui_tables.cpp
    vendor/imgui/imgui_widgets.cpp
)
target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/sdl-cpp/include
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glm/include
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui
)

target_link_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glm/lib
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/sdl-cpp/lib
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    "glm"
    "sdl-cpp"
)

add_custom_command(TARGET snake POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_CURRENT_SOURCE_DIR}/vendor/sdl-cpp/lib/SDL3.dll"
    $<TARGET_FILE_DIR:snake>
)

target_compile_definitions(${PROJECT_NAME} PRIVATE
    "IMGUI_DEFINE_MATH_OPERATORS"
)

target_precompile_headers(${PROJECT_NAME} PRIVATE
    <glm/glm.hpp>
    <imgui.h>
##    Concepts & Utilities ---
#    <concepts>
#    <coroutine>
#    <utility>
   <memory>
#    <memory_resource>
#    <scoped_allocator>
#    <functional>
#    <initializer_list>
#    <tuple>
#    <any>
#    <optional>
#    <variant>
#    <type_traits>
#    <ratio>
#    <chrono>
#    <typeindex>
#    <typeinfo>
#    <debugging> // C++26

##    Strings & Text ---
   <string>
   <string_view>
   <charconv>
   <format>
#    <text_encoding> // C++26

##    Containers & Collections ---
#    <array>
   <vector>
#    <deque>
#    <list>
#    <forward_list>
#    <set>
#    <map>
#    <unordered_set>
#    <unordered_map>
#    <stack>
#    <queue>
#    <span>
#    <mdspan> // C++23
#    <flat_set> // C++23
#    <flat_map> // C++23
#    <generator> // C++23

##    Enumerators & Algorithms ---
#    <iterator>
#    <algorithm>
#    <execution>
#    <ranges>

##    Math & Numerics ---
#    <cmath>
#    <complex>
#    <numeric>
#    <random>
#    <valarray>
#    <bit>
#    <numbers>
#    <linalg> // C++26

##    Input/Output & Filesystem ---
   <iostream>
#    <istream>
#    <ostream>
#    <fstream>
#    <sstream>
#    <syncstream>
#    <strstream> // Deprecated but standard
#    <iomanip>
#    <streambuf>
#    <filesystem>
#    <print> // C++23

##    Concurrency & Threading ---
#    <thread>
#    <mutex>
#    <shared_mutex>
#    <condition_variable>
#    <future>
#    <atomic>
#    <latch>
#    <barrier>
#    <semaphore>
#    <stdatomic.h>

##    Diagnostics & Localization ---
   <exception>
   <stdexcept>
#    <cassert>
#    <system_error>
#    <stacktrace>
#    <locale>
#    <codecvt> // Deprecated but standard

##    Standard Library Wrappers ---
#    <cctype>
#    <cwctype>
#    <cstring>
#    <cwchar>
#    <cuchar>
#    <cmath>
#    <cfenv>
#    <cstdlib>
#    <cstddef>
#    <cstdint>
#    <cfloat>
#    <climits>
#    <cstdarg>
#    <csetjmp>
#    <csignal>
#    <ctime>
#    <cerrno>
#    <clocale>
#    <cinttypes>

)

## === Replicate the Visual Studio Environment ===

target_include_directories(${PROJECT_NAME} PRIVATE
    "C:/Program Files/Microsoft Visual Studio/18/Community/VC/Tools/MSVC/14.51.36231/include"
    "C:/Program Files/Microsoft Visual Studio/18/Community/VC/Tools/MSVC/14.51.36231/atlmfc/include"
    "C:/Program Files/Microsoft Visual Studio/18/Community/VC/Auxiliary/VS/include"
    "C:/Program Files (x86)/Windows Kits/10/Include/10.0.26100.0/ucrt"
    "C:/Program Files/Microsoft Visual Studio/18/Community/VC/Auxiliary/VS/UnitTest/include"
    "C:/Program Files (x86)/Windows Kits/10/Include/10.0.26100.0/um"
    "C:/Program Files (x86)/Windows Kits/10/Include/10.0.26100.0/shared"
    "C:/Program Files (x86)/Windows Kits/10/Include/10.0.26100.0/winrt"
    "C:/Program Files (x86)/Windows Kits/10/Include/10.0.26100.0/cppwinrt"
)

set(ENV{PATH}
    "C:/Program Files/Microsoft Visual Studio/18/Community/VC/Tools/MSVC/14.51.36231/bin/HostX64/x64;C:/Program Files (x86)/Windows Kits/10/bin/10.0.26100.0/x64;C:/Program Files (x86)/Windows Kits/10/bin/10.0.26100.0/x86;C:/Program Files/Microsoft Visual Studio/18/Community/Common7/tools;C:/Program Files/Microsoft Visual Studio/18/Community/Common7/ide;C:/Program Files (x86)/HTML Help Workshop;C:/Program Files/Microsoft Visual Studio/18/Community/MSBuild/Current/Bin/amd64;C:/WINDOWS/Microsoft.NET/Framework64/v4.0.30319;C:/WINDOWS/Microsoft.NET/Framework/v4.0.30319;C:/Program Files/Microsoft Visual Studio/18/Community/MSBuild/Current/Bin;C:/WINDOWS/system32;C:/WINDOWS;C:/WINDOWS/System32/Wbem;C:/WINDOWS/System32/WindowsPowerShell/v1.0;C:/WINDOWS/System32/OpenSSH;C:/Program Files/NVIDIA Corporation/NVIDIA app/NvDLISR;C:/Program Files (x86)/NVIDIA Corporation/PhysX/Common;C:/Users/arish/.local/bin;C:/Program Files/nodejs;C:/Program Files/Git/cmd;C:/Program Files/NVIDIA Corporation/Nsight Compute 2025.3.1;C:/Program Files (x86)/Windows Kits/10/Windows Performance Toolkit;C:/Program Files/Neovim/bin;C:/Program Files/Neovide;C:/Users/arish/AppData/Local/Microsoft/WindowsApps;C:/Users/arish/AppData/Local/bin/NASM;C:/Users/arish/custom_cli;C:/Program Files/Microsoft Visual Studio/18/Community/Common7/IDE/CommonExtensions/Microsoft/CMake/CMake/bin;C:/Program Files/Microsoft Visual Studio/18/Community/Common7/IDE/CommonExtensions/Microsoft/CMake/Ninja;C:/Program Files/Microsoft Visual Studio/18/Community/Common7/IDE/PrivateAssemblies/runtimes/win-x64/native;$ENV{PATH}"
)

target_link_directories(${PROJECT_NAME} PRIVATE
    "C:/Program Files/Microsoft Visual Studio/18/Community/VC/Tools/MSVC/14.51.36231/lib/x64"
    "C:/Program Files/Microsoft Visual Studio/18/Community/VC/Tools/MSVC/14.51.36231/atlmfc/lib/x64"
    "C:/Program Files/Microsoft Visual Studio/18/Community/VC/Auxiliary/VS/lib/x64"
    "C:/Program Files (x86)/Windows Kits/10/lib/10.0.26100.0/ucrt/x64"
    "C:/Program Files/Microsoft Visual Studio/18/Community/VC/Auxiliary/VS/UnitTest/lib"
    "C:/Program Files (x86)/Windows Kits/10/lib/10.0.26100.0/um/x64"
)

target_compile_options(${PROJECT_NAME} PRIVATE
    "/AI\"C:/Program Files (x86)/Windows Kits/10/References\""
)

## === Done Replicating ===
