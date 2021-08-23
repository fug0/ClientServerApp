
set(INC_DIR ".../ACE_wrappers")       #type here your "ACE_wrappers" path
set(LINK_DIR ".../ACE_wrappers/lib")  #type here your "ACE_wrappers/lib" path
set(client_name "Client")

include_directories(${INC_DIR})
link_directories(${LINK_DIR})
link_libraries(ACEd)

add_executable (${client_name} "Client.cpp" "Client.h")

target_link_libraries(Client ACEd)
