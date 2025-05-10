cmake_minimum_required(VERSION 3.31.5)

project(Compiler)
set(CMAKE_CXX_COMPILER "/usr/bin/g++")
#add_compile_options(-DDEBUG -g -Wall -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr)
#add_link_options(-fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr)

set(
    HEADERS
    ${Compiler_SOURCE_DIR}/driver/driver.hpp
    ${Compiler_SOURCE_DIR}/frontend/frontend.hpp
    ${Compiler_SOURCE_DIR}/frontend/parser.hpp
    ${Compiler_SOURCE_DIR}/utils/log.hpp
    ${Compiler_SOURCE_DIR}/visitors/interpreter.hpp
    ${Compiler_SOURCE_DIR}/visitors/graphDump.hpp
    ${Compiler_SOURCE_DIR}/visitors/llvmIR.hpp
    )

find_package(Boost COMPONENTS program_options REQUIRED)
message(STATUS "Found Boost::program_options ${Boost_VERSION}")

find_package(BISON)
message(STATUS "Found BISON ${BISON_VERSION}")

find_package(FLEX)
message(STATUS "Found FLEX ${FLEX_VERSION}")

find_package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

BISON_TARGET(
    MyParser
    ${Compiler_SOURCE_DIR}/frontend/parser.y
    ${Compiler_SOURCE_DIR}/frontend/parser.cpp
    COMPILE_FLAGS -Wcounterexamples
    DEFINES_FILE ${Compiler_SOURCE_DIR}/frontend/parser.hpp
)

FLEX_TARGET(
    MyScanner
    ${Compiler_SOURCE_DIR}/frontend/scanner.l
    ${Compiler_SOURCE_DIR}/frontend/scanner.cpp
)

ADD_FLEX_BISON_DEPENDENCY(MyScanner MyParser)

add_library(logging.o OBJECT ${Compiler_SOURCE_DIR}/utils/log.cpp)

add_library(
    flex.o
    OBJECT
    ${FLEX_MyScanner_OUTPUTS}
    )
target_include_directories(
    flex.o PRIVATE 
    ${Compiler_SOURCE_DIR}/visitors/ 
    ${Compiler_SOURCE_DIR}/frontend/
    ${Compiler_SOURCE_DIR}/driver/
    )

add_library(
    bison.o
    OBJECT
    ${BISON_MyParser_OUTPUTS}
    )
target_include_directories(
    bison.o PRIVATE 
    ${Compiler_SOURCE_DIR}/visitors/ 
    ${Compiler_SOURCE_DIR}/frontend/
    ${Compiler_SOURCE_DIR}/driver/
    )

add_library(
    driver.o
    OBJECT
    ${Compiler_SOURCE_DIR}/driver/driver.cpp
    )
target_include_directories(
    driver.o PRIVATE 
    ${Compiler_SOURCE_DIR}/utils/ 
    ${Compiler_SOURCE_DIR}/frontend/ 
    ${Compiler_SOURCE_DIR}/visitors/
    )

add_library(
    graphDump.o
    OBJECT
    ${Compiler_SOURCE_DIR}/visitors/graphDump.cpp
    )
target_include_directories(
    graphDump.o PRIVATE 
    ${Compiler_SOURCE_DIR}/utils/ 
    ${Compiler_SOURCE_DIR}/frontend/
    ${Compiler_SOURCE_DIR}/visitors/
    )

add_library(
    interpreter.o
    OBJECT
    ${Compiler_SOURCE_DIR}/visitors/interpreter.cpp
    )
target_include_directories(
    interpreter.o PRIVATE 
    ${Compiler_SOURCE_DIR}/utils/ 
    ${Compiler_SOURCE_DIR}/frontend/
    ${Compiler_SOURCE_DIR}/visitors/
    )

add_library(
    scope_tree.o
    OBJECT
    ${Compiler_SOURCE_DIR}/visitors/scope.cpp
    )
target_include_directories(
    scope_tree.o PRIVATE 
    ${Compiler_SOURCE_DIR}/utils/ 
    ${Compiler_SOURCE_DIR}/frontend/
    ${Compiler_SOURCE_DIR}/visitors/
    )

add_library(
    llvm_ir.o
    OBJECT
    ${Compiler_SOURCE_DIR}/visitors/llvmIR.cpp
    )
target_include_directories(
    llvm_ir.o PRIVATE 
    ${Compiler_SOURCE_DIR}/utils/ 
    ${Compiler_SOURCE_DIR}/frontend/
    ${Compiler_SOURCE_DIR}/visitors/
    )

add_library(main.o OBJECT ${Compiler_SOURCE_DIR}/main.cpp)
target_include_directories(
    main.o PRIVATE 
    ${Compiler_SOURCE_DIR}/utils/
    ${Compiler_SOURCE_DIR}/frontend/
    ${Compiler_SOURCE_DIR}/driver/
    ${Compiler_SOURCE_DIR}/visitors/
    )

add_executable(
    compiler
    $<TARGET_OBJECTS:logging.o>
    $<TARGET_OBJECTS:flex.o>
    $<TARGET_OBJECTS:bison.o>
    $<TARGET_OBJECTS:driver.o>
    $<TARGET_OBJECTS:graphDump.o>
    $<TARGET_OBJECTS:interpreter.o>
    $<TARGET_OBJECTS:llvm_ir.o>
    $<TARGET_OBJECTS:scope_tree.o>
    $<TARGET_OBJECTS:main.o>
)
target_include_directories(compiler PRIVATE ${Compiler_SOURCE_DIR} ${LLVM_INCLUDE_DIRS})

get_target_property(LLVM_LIB_PATH LLVM LOCATION)
message(STATUS "Linking againts: ${LLVM_LIB_PATH} ${Boost_LIBRARIES}")
target_link_libraries(compiler ${LLVM_LIB_PATH} ${Boost_LIBRARIES})
