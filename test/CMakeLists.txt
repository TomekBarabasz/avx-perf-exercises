# gtest/gmock 1.8.1 is visible as 1.9.0 for unknown reason
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTEST REQUIRED gtest>=1.8.1)
#pkg_check_modules(GMOCK REQUIRED gmock>=1.8.1)

#message( "gtest libraries " "${GTEST_LIBRARIES}")
#message( "gtest libraries " "${gtest_LIBRARIES}")
#message( "gtest main libraries " "${GTEST_MAIN_LIBRARIES}" )
#message( "gtest include dirs" "${gtest_INCLUDE_DIRS}" )

SET(CMAKE_CXX_FLAGS -pthread)
enable_testing()

#message("gtest cflags " ${GTEST_CFLAGS_OTHER})
#message("gmock cflags " ${GMOCK_CFLAGS_OTHER})

add_definitions(${GTEST_CFLAGS_OTHER} ${GMOCK_CFLAGS_OTHER})
add_executable(${PROJECT_NAME}-ut main.cpp main.cpp modulate_ut.cpp)
target_include_directories(${PROJECT_NAME}-ut PRIVATE
    ${gtest_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME}-ut
    stdc++
    modulate
    ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES}
)
