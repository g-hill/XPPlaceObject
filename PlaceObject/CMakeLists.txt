cmake_minimum_required(VERSION 3.10)
project(PlaceObject)

set(CMAKE_CXX_STANDARD 17)
add_definitions(-DXPLM200=1 -DXPLM210=1 -DXPLM300=1 -DXPLM301=1)

# Path to X-Plane SDK
set(XPLANE_SDK_PATH "C:/SDK")
set(XPLANE_LIB_PATH "C:/SDK/Libraries/Win")

include_directories(
    "${XPLANE_SDK_PATH}/CHeaders/XPLM"
    "${XPLANE_SDK_PATH}/CHeaders/Widgets"
    "${XPLANE_SDK_PATH}/CHeaders/Wrappers"
)
link_directories(${XPLANE_LIB_PATH})

add_library(PlaceObject SHARED src/PlaceObject.cpp)

# Link X-Plane libraries
target_link_libraries(PlaceObject XPLM_64 XPWidgets_64)

# Platform-specific settings
if(WIN32)
    add_definitions(-DIBM=1)
    set(PLATFORM "win")
    set(ARCH "x64")
    link_directories("${XPLANE_SDK_PATH}/Libraries/Win")
endif()



# Link against XPLM
if(WIN32)
    target_link_libraries(PlaceObject "XPLM_64")
endif()

# Set output name and location
set_target_properties(PlaceObject PROPERTIES
    PREFIX ""
    SUFFIX ".xpl"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build/64"
)