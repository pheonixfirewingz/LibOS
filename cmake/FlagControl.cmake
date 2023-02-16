if(CMAKE_SYSTEM_NAME MATCHES WindowsStore OR CMAKE_SYSTEM_NAME MATCHES Windows)
	set(CMAKE_CXX_FLAGS "")
	set(CMAKE_CXX_FLAGS_DEBUG "")
	set(CMAKE_CXX_FLAGS_RELEASE "")
endif()

function(flagsSetup)
	if(CMAKE_BUILD_TYPE MATCHES Debug)
		if(CMAKE_CXX_COMPILER_ID MATCHES Clang)
			target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -W -Werror -Wall -Wextra -Weverything -fno-common -Wdouble-promotion -Wshadow -Wpedantic -O0 -g3)
			if(!${STATIC_BUILD})
				target_link_options(${CMAKE_PROJECT_NAME} PRIVATE -static-libstdc++)
			 endif()
		elseif(CMAKE_CXX_COMPILER_ID MATCHES GNU)
			 target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -W -Werror -Wall -Wextra -fno-common -Wdouble-promotion -Wshadow -Wpedantic -O0 -g3)
			 if(!${STATIC_BUILD})
				target_link_options(${CMAKE_PROJECT_NAME} PRIVATE -static-libstdc++)
			 endif()
		elseif(MSVC)
			if(CMAKE_SYSTEM_NAME MATCHES Windows)
				target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE /Od /W4 /WX /RTC1 /Z7  /EHsc /sdl /JMC /external:anglebrackets /external:W0 /GS /guard:cf)
				target_link_options(${CMAKE_PROJECT_NAME} PRIVATE /DEBUG:FULL)
			endif()
			if(CMAKE_SYSTEM_NAME MATCHES WindowsStore)
				target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE /W4 /WX /ZW /EHsc /await /external:anglebrackets /external:W0)
				set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES VS_WINRT_COMPONENT TRUE OUTPUT_NAME "libos" VS_CPPWINRT true)
			endif()
		else()
			message(FATAL_ERROR "compiler not supported")
		endif()
	else()
		if(CMAKE_CXX_COMPILER_ID MATCHES Clang)
			target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -W -Werror -Wall -Wextra -fno-common -Wdouble-promotion -Wshadow -Wpedantic -O2 -g0 -flto -finline-functions)
			set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -fno-rtti)
			if(!${STATIC_BUILD})
				target_link_options(${CMAKE_PROJECT_NAME} PRIVATE -static-libstdc++)
			 endif()
		elseif(CMAKE_CXX_COMPILER_ID MATCHES GNU)
			 target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -W -Werror -Wall -Wextra -fno-common -Wdouble-promotion -Wshadow -Wpedantic -O2 -g0 -flto -finline-functions)
			 set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -fno-rtti)
			 if(!${STATIC_BUILD})
				target_link_options(${CMAKE_PROJECT_NAME} PRIVATE -static-libstdc++)
			 endif()
		elseif(MSVC)
			if(CMAKE_SYSTEM_NAME MATCHES Windows)
				target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE /O2 /sdl /EHsc /external:anglebrackets /external:W0)
				target_link_options(${CMAKE_PROJECT_NAME} PRIVATE /OPT:REF,ICF,LBR /RELEASE)
			endif()
			if(CMAKE_SYSTEM_NAME MATCHES WindowsStore)
				 target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE /W4 /WX /ZW /EHsc /await /external:anglebrackets /external:W0)
				set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES VS_WINRT_COMPONENT TRUE OUTPUT_NAME "libos"  VS_CPPWINRT true)
			endif()
		else()
			message(FATAL_ERROR "compiler not supported")
		endif()
	endif()
endfunction()