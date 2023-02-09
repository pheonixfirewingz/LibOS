ADD_FILES(
	Wayland.hpp
	Wayland.cpp 
	WaylandLibDecor.cpp
	xdg-shell-client-protocol.h
	xdg-shell-protocol.c
	Xcb.hpp
	Xcb.cpp
	CONDITION ON_LINUX AND NOT TERMINAL_MODE)

ADD_FILES(
	Win32.hpp
	Win32.cpp 
	CONDITION ON_WINDOWS AND NOT TERMINAL_MODE)

ADD_FILES(
	WinRT.hpp
	WinRT.cpp 
	CONDITION ON_UWP AND NOT TERMINAL_MODE)