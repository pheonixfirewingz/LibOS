ADD_FILES(
	Wayland.hpp
	Wayland.cpp 
	WaylandLibDecor.cpp
	xdg-shell-client-protocol.h
	xdg-shell-protocol.c
	Xcb.hpp
	Xcb.cpp
	CONDITION ON_LINUX)

ADD_FILES(
	Win32.hpp
	Win32.cpp 
	CONDITION ON_WINDOWS)

ADD_FILES(
	WinRT.hpp
	WinRT.cpp 
	CONDITION ON_UWP)