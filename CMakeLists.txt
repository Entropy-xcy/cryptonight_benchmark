cmake_minimum_required(VERSION 2.8)
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_MODULE_PATH})
include(Cryptonite)
message(STATUS "Target architectures: ${CMAKE_TARGET_ARCHITECTURES}")

project(cryptonite_hash C ASM)

# Set a default build configuration if none is specified. 'Release' produces the fastest binaries
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'Release' as none was specified.")
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
    "MinSizeRel" "RelWithDebInfo")
endif()
string(TOUPPER "${CMAKE_BUILD_TYPE}" U_CMAKE_BUILD_TYPE)
find_package(Threads REQUIRED)

# include all subdirs at source directory
include_directories(${PROJECT_SOURCE_DIR}/.)
include_directories(${PROJECT_SOURCE_DIR}/crypto)
set(CMAKE_C_FLAGS "-O3 -static")

set(CRYPTO_HEADERS
	crypto/oaes_lib.h
	crypto/c_keccak.h
	crypto/c_groestl.h
	crypto/c_blake256.h
	crypto/c_jh.h
	crypto/c_skein.h
	crypto/int-util.h
	crypto/hash-ops.h
	crypto/hash.h
)

set(CRYPTO_SOURCES
	crypto/aesb.c
	crypto/oaes_lib.c
	crypto/c_keccak.c
	crypto/c_groestl.c
	crypto/c_blake256.c
	crypto/c_jh.c
	crypto/c_skein.c
	crypto/hash.c
)

include_directories(${PROJECT_SOURCE_DIR}/compat)

set(COMPAT_HEADERS
	compat/cpuminer-config.h
	compat/inttypes.h
	compat/stdbool.h
	compat/unistd.h
	compat/winansi.h
)

# Use Assembly file for AES-IN optimization only on x64 env
# and not on Visual Studio
if( "${CMAKE_TARGET_ARCHITECTURES}" STREQUAL "x86_64" AND NOT MSVC )
    set(ASM
		asm/aesb-x64.S
	)
else()
    set(ASM
		#asm/aesb-x86.S
		#crypto/aesb-x86-impl.c
	)
endif()


# Create the binding library
add_library(cryptonite_hash
   ${CRYPTO_HEADERS}
   ${COMPAT_HEADERS}
   ${CRYPTO_SOURCES}
   ${ASM}
   compat.h
   miner.h
   cryptonite_hash.h
   cryptonite_hash.c
   cryptolite_hash.c
   sysinfos.c
  # ... extra files go here ...
)

add_executable(cryptonite_benchmark main.c)
target_link_libraries(cryptonite_benchmark cryptonite_hash pthread)

