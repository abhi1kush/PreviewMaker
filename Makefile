SRC=video_preview_main.cpp
SRC+=Box.cpp
SRC+=mp4_parser.cpp
SRC+=byte_buffer.cpp
SRC+=mp4_err.cpp
SRC+=stbl_handler.cpp

CPPFLAGS=-std=c++11
CXX=g++

all:
	$(CXX) $(CPPFLAGS) $(SRC)
