
set(INC_DIR ".../ACE_wrappers")        #type here your "ACE_wrappers" path
set(LINK_DIR ".../ACE_wrappers/lib")   #type here your "ACE_wrappers/lib" path
set(server_name "Multithreaded_TCP_Server")

include_directories(${INC_DIR})
link_directories(${LINK_DIR})
link_libraries(ACEd)

add_executable (${server_name} "src/Multithreaded_TCP_Server.cpp" "src/Multithreaded_TCP_Server.h" "src/client_acceptor/client_acceptor.h" "src/client_acceptor/client_acceptor.cpp" "src/client_handler/client_handler.h" "src/client_handler/client_handler.cpp" "src/thread_pool/thread_pool.h" "src/thread_pool/thread_pool.cpp")

target_link_libraries(Multithreaded_TCP_Server ACEd)