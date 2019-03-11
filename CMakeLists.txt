cmake_minimum_required(VERSION 2.8.10 FATAL_ERROR)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable (scratchpad main.cpp)

set (SERVICE_FILES xyz.openbmc_project.scratchpad.service)

set (SCRATCHPAD_LINK_LIBS pthread systemd sdbusplus sdeventplus inspur_dbus)

add_dependencies (scratchpad sdbusplus)
add_dependencies (scratchpad sdeventplus)
add_dependencies (scratchpad inspur-dbus-interfaces)
target_link_libraries (scratchpad ${SCRATCHPAD_LINK_LIBS})

install (TARGETS scratchpad DESTINATION sbin)
install (FILES ${SERVICE_FILES} DESTINATION /lib/systemd/system/)
