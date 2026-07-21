cmake_minimum_required(VERSION 3.22)

foreach(required_variable IN ITEMS
        DEMO_EXECUTABLE
        RAYLIB_RUNTIME
        FONT_FILE
        FONT_LICENSE
        FONT_README
        RTEXTJP_LICENSE
        RAYLIB_LICENSE
        DEMO_README
        DEMO_README_JA
        OUTPUT_DIRECTORY
        ARCHIVE_NAME)
    if(NOT DEFINED ${required_variable} OR "${${required_variable}}" STREQUAL "")
        message(FATAL_ERROR "PackageDemo.cmake requires ${required_variable}.")
    endif()
endforeach()

foreach(required_file IN ITEMS
        "${DEMO_EXECUTABLE}"
        "${RAYLIB_RUNTIME}"
        "${FONT_FILE}"
        "${FONT_LICENSE}"
        "${FONT_README}"
        "${RTEXTJP_LICENSE}"
        "${RAYLIB_LICENSE}"
        "${DEMO_README}"
        "${DEMO_README_JA}")
    if(NOT EXISTS "${required_file}")
        message(FATAL_ERROR "Required demo package file was not found: ${required_file}")
    endif()
endforeach()

set(package_root "${OUTPUT_DIRECTORY}")
cmake_path(NORMAL_PATH package_root)
set(staging_directory "${package_root}/${ARCHIVE_NAME}")
cmake_path(IS_PREFIX package_root "${staging_directory}" NORMALIZE staging_is_inside_output)
if(NOT staging_is_inside_output)
    message(FATAL_ERROR "Demo staging directory must stay inside the package output directory.")
endif()

file(REMOVE_RECURSE "${staging_directory}")
file(MAKE_DIRECTORY "${staging_directory}")

file(COPY_FILE "${DEMO_EXECUTABLE}"
     "${staging_directory}/rtextjp_01_hello_japanese.exe" ONLY_IF_DIFFERENT)
file(COPY_FILE "${RAYLIB_RUNTIME}"
     "${staging_directory}/raylib.dll" ONLY_IF_DIFFERENT)
file(COPY_FILE "${FONT_FILE}"
     "${staging_directory}/japanese.ttf" ONLY_IF_DIFFERENT)
file(COPY_FILE "${DEMO_README}"
     "${staging_directory}/README.md" ONLY_IF_DIFFERENT)
file(COPY_FILE "${DEMO_README_JA}"
     "${staging_directory}/README_ja.md" ONLY_IF_DIFFERENT)
file(COPY_FILE "${FONT_README}"
     "${staging_directory}/README-font.txt" ONLY_IF_DIFFERENT)
file(COPY_FILE "${RTEXTJP_LICENSE}"
     "${staging_directory}/LICENSE-RTextJP.txt" ONLY_IF_DIFFERENT)
file(COPY_FILE "${RAYLIB_LICENSE}"
     "${staging_directory}/LICENSE-raylib.txt" ONLY_IF_DIFFERENT)
file(COPY_FILE "${FONT_LICENSE}"
     "${staging_directory}/LICENSE-font.txt" ONLY_IF_DIFFERENT)

set(archive_path "${package_root}/${ARCHIVE_NAME}.zip")
file(REMOVE "${archive_path}")
execute_process(
    COMMAND "${CMAKE_COMMAND}" -E tar cf "${archive_path}" --format=zip "${ARCHIVE_NAME}"
    WORKING_DIRECTORY "${package_root}"
    RESULT_VARIABLE archive_result)
if(NOT archive_result EQUAL 0)
    message(FATAL_ERROR "Could not create demo archive: ${archive_path}")
endif()

message(STATUS "Created ${archive_path}")
