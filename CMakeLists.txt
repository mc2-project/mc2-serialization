cmake_minimum_required(VERSION 3.11)

project("MC^2 Serialization" LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 11)

# Download and install Flatbuffers
include(ExternalProject)
set(FLATBUFFERS_INSTALL_LOCATION ${CMAKE_CURRENT_BINARY_DIR}/flatbuffers)
ExternalProject_Add(flatbuffers
    GIT_REPOSITORY https://github.com/google/flatbuffers.git
    GIT_TAG v1.7.0
    CMAKE_ARGS -G "Unix Makefiles"
               -DFLATBUFFERS_BUILD_TESTS=OFF
               -DFLATBUFFERS_BUILD_FLATLIB=OFF
               -DFLATBUFFERS_BUILD_FLATHASH=OFF
               -DFLATBUFFERS_BUILD_FLATC=ON
               -DCMAKE_INSTALL_PREFIX=${FLATBUFFERS_INSTALL_LOCATION})
# Generate Flatbuffers files from schemas
set(FLATC ${FLATBUFFERS_INSTALL_LOCATION}/bin/flatc)
set(flatbuffers_schemas
    ${CMAKE_CURRENT_SOURCE_DIR}/src/flatbuffers/SignedKey.fbs
    ${CMAKE_CURRENT_SOURCE_DIR}/src/flatbuffers/sql/EncryptedBlock.fbs
    ${CMAKE_CURRENT_SOURCE_DIR}/src/flatbuffers/sql/Rows.fbs)

add_custom_target(flatbuffers_cpp_generation)
set(FLATBUFFERS_GENCPP_DIR ${FLATBUFFERS_INSTALL_LOCATION}/include/flatbuffers)
foreach(schema ${flatbuffers_schemas})
    get_filename_component(filename ${schema} NAME_WE)
    # For each schema, generate the corresponding C++ file
    set(generated_include ${FLATBUFFERS_GENCPP_DIR}/${filename}_generated.h)
    add_custom_command(
        OUTPUT ${generated_include}
        DEPENDS ${schema}
        COMMAND ${FLATC} --cpp -o ${FLATBUFFERS_GENCPP_DIR} ${schema}
    )
    add_custom_target(flatbuffers_generate_${filename} DEPENDS ${generated_include})
    add_dependencies(flatbuffers_generate_${filename} flatbuffers)
    add_dependencies(flatbuffers_cpp_generation flatbuffers_generate_${filename})
endforeach()

include_directories(
    ${FLATBUFFERS_INSTALL_LOCATION}/include/flatbuffers
    ${FLATBUFFERS_INSTALL_LOCATION}/include)
add_dependencies(flatbuffers_cpp_generation flatbuffers)

# --------

# We need OpenEnclave since, when used in the enclave, we need to link to
# the OE standard library
find_package(OpenEnclave CONFIG REQUIRED)

add_library(mc2_serialization STATIC src/serialization.cpp)

target_compile_definitions(mc2_serialization PUBLIC OE_API_VERSION=2)

option(HOST "Build for an untrusted environment" OFF)
if (NOT HOST)
    if (LVI_MITIGATION MATCHES ControlFlow)
      # Helper to enable compiler options for LVI mitigation.
      apply_lvi_mitigation(enclave)
      # Link against LVI-mitigated libraries.
      target_link_libraries(
        mc2_serialization
        PRIVATE
        openenclave::oeenclave-lvi-cfg openenclave::oelibcxx-lvi-cfg)
    else()
      target_link_libraries(
        mc2_serialization
        PRIVATE
        openenclave::oeenclave openenclave::oelibcxx)
    endif()
endif()

target_include_directories(mc2_serialization PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

set_target_properties(mc2_serialization PROPERTIES
	POSITION_INDEPENDENT_CODE ON
	CXX_STANDARD 11
	CXX_STANDARD_REQUIRED ON
	VERSION 0.1)

add_dependencies(mc2_serialization flatbuffers flatbuffers_cpp_generation)
