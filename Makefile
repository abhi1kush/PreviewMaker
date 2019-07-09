SRCPATH=src
SRC=$(SRCPATH)/video_preview_main.cpp
SRC+=$(SRCPATH)/Box.cpp
SRC+=$(SRCPATH)/mp4_parser.cpp
SRC+=$(SRCPATH)/byte_buffer.cpp
SRC+=$(SRCPATH)/mp4_err.cpp
SRC+=$(SRCPATH)/stbl_handler.cpp

CPPFLAGS+=-std=c++11
CPPFLAGS+=-g
CXX=g++

all:
	$(CXX) -Iinclude/ $(CPPFLAGS) $(SRC)
