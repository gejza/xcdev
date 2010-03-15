
# Create a library called "Hello" which includes the source file "hello.cxx".
# The extension is already found.  Any number of sources could be listed here.

ADD_EXECUTABLE(regex-comp regex_comp.cpp regex_option.h regex_option.cpp regex_config.h)
TARGET_LINK_LIBRARIES(regex-comp pcre)



