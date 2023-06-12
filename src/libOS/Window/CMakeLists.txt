ADD_FILES(
	Wayland.hpp
	Wayland.cpp 
	WaylandLibDecor.cpp
	xdg-shell-client-protocol.h
	xdg-shell-protocol.c)

ADD_FILES(
	Xcb.hpp
	Xcb.cpp
	CONDITION ON_LINUX AND NOT DISABLE_XCB)

ADD_FILES(
	Win32.hpp
	Win32.cpp 
	CONDITION ON_WINDOWS)

ADD_FILES(
	WinRT.hpp
	WinRT.cpp 
	CONDITION ON_UWP)