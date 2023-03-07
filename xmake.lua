target("server")
    set_kind("binary")
    add_files("server.cpp","src/*.cpp")
    add_syslinks("pthread")

target("client")
    set_kind("binary")
    add_files("client.cpp","src/*.cpp")
    add_syslinks("pthread")