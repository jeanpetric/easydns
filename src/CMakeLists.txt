find_package (Threads)
find_package (CURL)

add_executable (server server.c)

target_link_libraries (server ${CMAKE_THREAD_LIBS_INIT} ${CURL_LIBRARY})

# install (TARGETS server DESTINATION /usr/bin)
