cmake_minimum_required(VERSION 3.22)

project(RTextJP VERSION 0.1.0 LANGUAGES C)

option(RTEXTJP_BUILD_EXAMPLES "Build the beginner examples" ON)
option(RTEXTJP_BUILD_TESTS "Build internal RTextJP tests" OFF)
option(RTEXTJP_BUILD_DOCS_GALLERY "Build the documentation screenshot generator" OFF)
option(RTEXTJP_ENABLE_SANITIZERS "Enable AddressSanitizer and UndefinedBehaviorSanitizer for tests" OFF)
option(RTEXTJP_FETCH_RAYLIB "Fetch the pinned raylib release when no installed package is found" OFF)
option(RTEXTJP_BUILD_DEMO_PACKAGE "Add the Windows demo ZIP packaging target" OFF)
set(RTEXTJP_RAYLIB_VERSION "6.0" CACHE STRING "Pinned raylib version used by FetchContent")

add_library(RTextJP INTERFACE)
target_include_directories(RTextJP INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)
target_compile_features(RTextJP INTERFACE c_std_99)

find_package(raylib ${RTEXTJP_RAYLIB_VERSION} QUIET)

if(TARGET raylib)
    set(RTEXTJP_RAYLIB_TARGET raylib)
elseif(TARGET raylib::raylib)
    set(RTEXTJP_RAYLIB_TARGET raylib::raylib)
endif()

if(NOT DEFINED RTEXTJP_RAYLIB_TARGET AND RTEXTJP_FETCH_RAYLIB)
    include(FetchContent)
    set(BUILD_EXAMPLES OFF CACHE BOOL "Build raylib examples" FORCE)
    set(BUILD_GAMES OFF CACHE BOOL "Build raylib games" FORCE)
    FetchContent_Declare(raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG ${RTEXTJP_RAYLIB_VERSION}
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE)
    FetchContent_MakeAvailable(raylib)
    FetchContent_GetProperties(raylib SOURCE_DIR rtextjp_fetched_raylib_source_dir)
    set(RTEXTJP_RAYLIB_TARGET raylib)
endif()

if(NOT DEFINED RTEXTJP_RAYLIB_TARGET)
    message(FATAL_ERROR
        "raylib ${RTEXTJP_RAYLIB_VERSION} was not found. Install raylib or configure with "
        "-DRTEXTJP_FETCH_RAYLIB=ON to download the pinned official release.")
endif()

target_link_libraries(RTextJP INTERFACE ${RTEXTJP_RAYLIB_TARGET})

function(rtextjp_configure_target target)
    target_link_libraries(${target} PRIVATE RTextJP)
    set_target_properties(${target} PROPERTIES C_STANDARD 99 C_STANDARD_REQUIRED YES)

    if(MSVC)
        target_compile_options(${target} PRIVATE /utf-8 /W4)
    else()
        target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic)
    endif()

    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/examples/resources")
        add_custom_command(TARGET ${target} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
                "${CMAKE_CURRENT_SOURCE_DIR}/examples/resources"
                "$<TARGET_FILE_DIR:${target}>/resources")
    endif()
endfunction()

function(rtextjp_enable_test_sanitizers target)
    if(NOT RTEXTJP_ENABLE_SANITIZERS)
        return()
    endif()

    if(CMAKE_C_COMPILER_ID MATCHES "Clang|GNU")
        target_compile_options(${target} PRIVATE
            -fsanitize=address,undefined
            -fno-sanitize-recover=all
            -fno-omit-frame-pointer)
        target_link_options(${target} PRIVATE -fsanitize=address,undefined)

        if(WIN32 AND CMAKE_C_COMPILER_ID MATCHES "Clang")
            execute_process(
                COMMAND "${CMAKE_C_COMPILER}" -print-resource-dir
                OUTPUT_VARIABLE rtextjp_clang_resource_dir
                OUTPUT_STRIP_TRAILING_WHITESPACE
                RESULT_VARIABLE rtextjp_clang_resource_result)
            if(NOT rtextjp_clang_resource_result EQUAL 0)
                message(FATAL_ERROR "Could not locate the Clang sanitizer runtime directory.")
            endif()
            file(GLOB rtextjp_asan_runtime_dlls
                "${rtextjp_clang_resource_dir}/lib/windows/clang_rt.asan_dynamic-*.dll")
            if(NOT rtextjp_asan_runtime_dlls)
                message(FATAL_ERROR "Could not locate the Clang AddressSanitizer runtime DLL.")
            endif()
            add_custom_command(TARGET ${target} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                    ${rtextjp_asan_runtime_dlls}
                    "$<TARGET_FILE_DIR:${target}>"
                COMMAND_EXPAND_LISTS)
        endif()
    else()
        message(FATAL_ERROR
            "RTEXTJP_ENABLE_SANITIZERS requires Clang or GCC for both ASan and UBSan.")
    endif()
endfunction()

if(RTEXTJP_BUILD_EXAMPLES)
    set(RTEXTJP_EXAMPLE_SOURCES
        00_first_rtextjp
        01_hello_japanese
        02_text_box
        03_alignment
        04_vertical_writing
        05_text_input
        06_utf8_tools
        07_character_sets)

    foreach(example IN LISTS RTEXTJP_EXAMPLE_SOURCES)
        add_executable(rtextjp_${example} "examples/${example}.c")
        rtextjp_configure_target(rtextjp_${example})
    endforeach()
endif()

if(RTEXTJP_BUILD_DEMO_PACKAGE)
    if(NOT WIN32)
        message(FATAL_ERROR "RTEXTJP_BUILD_DEMO_PACKAGE currently supports Windows only.")
    endif()
    if(NOT RTEXTJP_BUILD_EXAMPLES)
        message(FATAL_ERROR "RTEXTJP_BUILD_DEMO_PACKAGE requires RTEXTJP_BUILD_EXAMPLES=ON.")
    endif()
    if(NOT RTEXTJP_FETCH_RAYLIB OR NOT DEFINED rtextjp_fetched_raylib_source_dir)
        message(FATAL_ERROR
            "RTEXTJP_BUILD_DEMO_PACKAGE requires the pinned fetched raylib source. "
            "Configure with -DRTEXTJP_FETCH_RAYLIB=ON.")
    endif()
    get_target_property(rtextjp_raylib_target_type ${RTEXTJP_RAYLIB_TARGET} TYPE)
    if(NOT rtextjp_raylib_target_type STREQUAL "SHARED_LIBRARY")
        message(FATAL_ERROR
            "RTEXTJP_BUILD_DEMO_PACKAGE requires -DBUILD_SHARED_LIBS=ON so raylib.dll can be packaged.")
    endif()

    foreach(required_demo_file IN ITEMS
            "${CMAKE_CURRENT_SOURCE_DIR}/examples/resources/japanese.ttf"
            "${CMAKE_CURRENT_SOURCE_DIR}/examples/resources/OFL.txt"
            "${CMAKE_CURRENT_SOURCE_DIR}/examples/resources/README.txt")
        if(NOT EXISTS "${required_demo_file}")
            message(FATAL_ERROR "Required demo package file was not found: ${required_demo_file}")
        endif()
    endforeach()

    add_custom_target(rtextjp_demo_archive
        COMMAND ${CMAKE_COMMAND}
            "-DDEMO_EXECUTABLE=$<TARGET_FILE:rtextjp_01_hello_japanese>"
            "-DRAYLIB_RUNTIME=$<TARGET_FILE:${RTEXTJP_RAYLIB_TARGET}>"
            "-DFONT_FILE=${CMAKE_CURRENT_SOURCE_DIR}/examples/resources/japanese.ttf"
            "-DFONT_LICENSE=${CMAKE_CURRENT_SOURCE_DIR}/examples/resources/OFL.txt"
            "-DFONT_README=${CMAKE_CURRENT_SOURCE_DIR}/examples/resources/README.txt"
            "-DRTEXTJP_LICENSE=${CMAKE_CURRENT_SOURCE_DIR}/LICENSE"
            "-DRAYLIB_LICENSE=${rtextjp_fetched_raylib_source_dir}/LICENSE"
            "-DDEMO_README=${CMAKE_CURRENT_SOURCE_DIR}/packaging/windows-demo/README.md"
            "-DDEMO_README_JA=${CMAKE_CURRENT_SOURCE_DIR}/packaging/windows-demo/README_ja.md"
            "-DOUTPUT_DIRECTORY=${CMAKE_BINARY_DIR}/packages"
            "-DARCHIVE_NAME=RTextJP-demo-windows"
            -P "${CMAKE_CURRENT_SOURCE_DIR}/cmake/PackageDemo.cmake"
        DEPENDS rtextjp_01_hello_japanese ${RTEXTJP_RAYLIB_TARGET}
        VERBATIM)
endif()

if(RTEXTJP_BUILD_TESTS)
    include(CTest)
    add_executable(rtextjp_test_utf8 tests/test_utf8.c)
    rtextjp_configure_target(rtextjp_test_utf8)
    rtextjp_enable_test_sanitizers(rtextjp_test_utf8)
    set_target_properties(rtextjp_test_utf8 PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tests")
    add_test(NAME rtextjp_utf8 COMMAND rtextjp_test_utf8)

    add_executable(rtextjp_test_layout tests/test_layout.c)
    rtextjp_configure_target(rtextjp_test_layout)
    rtextjp_enable_test_sanitizers(rtextjp_test_layout)
    set_target_properties(rtextjp_test_layout PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tests")
    add_test(NAME rtextjp_layout COMMAND rtextjp_test_layout)

    add_executable(rtextjp_test_character_sets tests/test_character_sets.c)
    rtextjp_configure_target(rtextjp_test_character_sets)
    rtextjp_enable_test_sanitizers(rtextjp_test_character_sets)
    set_target_properties(rtextjp_test_character_sets PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/tests")
    add_test(NAME rtextjp_character_sets COMMAND rtextjp_test_character_sets)
endif()

if(RTEXTJP_BUILD_DOCS_GALLERY)
    add_executable(rtextjp_capture_gallery tools/capture_gallery.c)
    rtextjp_configure_target(rtextjp_capture_gallery)
endif()

install(FILES include/rtextjp.h DESTINATION include)
install(FILES LICENSE THIRD_PARTY_NOTICES.md DESTINATION share/licenses/RTextJP)
