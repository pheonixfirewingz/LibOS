ADD_FILES(
	AbstractWindow.h
	FileIO.cpp
	Main.cpp
	NetIO.cpp
)

if(NOT ${TERMIANL_MODE})
	ADD_FILES(
		Window.cpp
		AbstractWindow.h
	)
endif()