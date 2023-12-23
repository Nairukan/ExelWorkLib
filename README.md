# ExelWorkLib
Tiny tool for create Exel Files
!!!!ONLY LINUX NOW!!!!

FOR INSTALL LIBARARY

1. mkdir build && cd $_
2. cmake -DCMAKE_BUILD_TYPE=Release ..
3. cmake --build . --target install
4. rm -r ./* && cmake -DCMAKE_BUILD_TYPE=Debug ..
5. cmake --build . --target install

FOR LINK TO PROJECT

1. find_package(ExelWorkLib CONFIG REQUIRED)
2. include_directories(${ExelWorkLib_INCLUDE_DIR})
3. ...
4. target_link_libraries(${PROJECT_NAME} ... ::ExelWorkLib)
5. #include <ExelWorkLib/exelworklib.h>
6. Use and enjoy
