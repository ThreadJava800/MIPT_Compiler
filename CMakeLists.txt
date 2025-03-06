project(Compiler)

set(CMAKE_CXX_COMPILER "/usr/bin/g++")
add_compile_options(-g -fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr)
add_link_options(-fsanitize=address,alignment,bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,undefined,unreachable,vla-bound,vptr)

set(
    HEADERS
    ${Compiler_SOURCE_DIR}/frontend/frontend.hpp
    ${Compiler_SOURCE_DIR}/frontend/parser.hpp
    ${Compiler_SOURCE_DIR}/utils/log.hpp
    )

find_package(BISON)
find_package(FLEX)

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
    frontend.o
    OBJECT 
    ${Compiler_SOURCE_DIR}/frontend/frontend.cpp 
    ${BISON_MyParser_OUTPUTS} 
    ${FLEX_MyScanner_OUTPUTS}
    )
target_include_directories(frontend.o PRIVATE ${Compiler_SOURCE_DIR}/utils/)

add_library(main.o OBJECT ${Compiler_SOURCE_DIR}/main.cpp)
target_include_directories(
    main.o PRIVATE 
    ${Compiler_SOURCE_DIR}/utils/
    ${Compiler_SOURCE_DIR}/frontend/
)

add_executable(
    compiler
    $<TARGET_OBJECTS:logging.o>
    $<TARGET_OBJECTS:frontend.o>
    $<TARGET_OBJECTS:main.o>
)
target_include_directories(compiler PRIVATE ${Compiler_SOURCE_DIR})
