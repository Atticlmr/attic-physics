# FindEnzyme.cmake
# 查找Enzyme自动微分库

find_path(ENZYME_INCLUDE_DIR enzyme/Enzyme.h 
    PATHS ENV ENZYME_ROOT
    PATH_SUFFIXES include)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Enzyme DEFAULT_MSG ENZYME_INCLUDE_DIR)

if(Enzyme_FOUND)
    message(STATUS "Found Enzyme: ${ENZYME_INCLUDE_DIR}")
    add_definitions(-DWITH_ENZYME)
endif()
