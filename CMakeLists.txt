cmake_minimum_required(VERSION 3.8.2 FATAL_ERROR)

set(TARGET dup_check)

set(SOURCES main.cxx scan_folders.cxx file_hash.cxx)

project(${TARGET})

add_executable(${TARGET} ${SOURCES})

target_compile_features(${TARGET} PUBLIC cxx_std_17)

target_link_libraries(${TARGET} PRIVATE boost_program_options stdc++fs crypto++)

# Add "tags" target and make ${TARGET} depending on this target.
set_source_files_properties(tags PROPERTIES GENERATED true)
add_custom_target(tags
    COMMAND ctags --c++-kinds=+p --fields=+iaS --extra=+q .
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
add_dependencies(${TARGET} tags)

# vi:set tabstop=8 softtabstop=4 shiftwidth=4 noexpandtab nowrap:
