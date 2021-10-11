cmake_minimum_required(VERSION 3.14)

project(uki LANGUAGES C)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)

set(SOURCES
  div64.c
  jiffies.c
  timer.c
)

set(HEADERS
	uki/bitops.h
	uki/ffs.h
	uki/kernel.h
	uki/memory.h
	uki/timer.h
	uki/bug.h
	uki/fls64.h
	uki/list.h
	uki/poison.h
	uki/typecheck.h
	uki/delay.h
	uki/fls.h
	uki/log2.h
	uki/stdlib.h
	uki/types.h
	uki/div64.h
	uki/jiffies.h
	uki/math64.h
	uki/time64.h
)

add_library(uki STATIC
  ${SOURCES}
  ${HEADERS}
)

set_target_properties(uki PROPERTIES
    PUBLIC_HEADER uki/bitops.h
    PUBLIC_HEADER uki/ffs.h
    PUBLIC_HEADER uki/kernel.h
    PUBLIC_HEADER uki/memory.h
    PUBLIC_HEADER uki/timer.h
    PUBLIC_HEADER uki/bug.h
    PUBLIC_HEADER uki/fls64.h
    PUBLIC_HEADER uki/list.h
    PUBLIC_HEADER uki/poison.h
    PUBLIC_HEADER uki/typecheck.h
    PUBLIC_HEADER uki/delay.h
    PUBLIC_HEADER uki/fls.h
    PUBLIC_HEADER uki/log2.h
    PUBLIC_HEADER uki/stdlib.h
    PUBLIC_HEADER uki/types.h
    PUBLIC_HEADER uki/div64.h
    PUBLIC_HEADER uki/jiffies.h
    PUBLIC_HEADER uki/math64.h
    PUBLIC_HEADER uki/time64.h
)

target_compile_definitions(uki PRIVATE STRINGC_LIBRARY)

install(TARGETS uki
  LIBRARY DESTINATION       "${CMAKE_BUILD_TYPE}/lib"
  ARCHIVE DESTINATION       "${CMAKE_BUILD_TYPE}/lib"
  PUBLIC_HEADER DESTINATION "${CMAKE_BUILD_TYPE}/include/uki")

set(APP_EXE uki_test)
add_executable(${APP_EXE}
    uki_test.c
    container_test.c
    list_test.c
    timer_test.c
)
target_link_libraries(${APP_EXE} uki Threads::Threads)
add_test(Test, ${APP_EXE})
