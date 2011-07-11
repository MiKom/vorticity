IF(WIN32)
	# It's not OK to trust $ENV{PROCESSOR_ARCHITECTURE}: its value depends on the type of executable being run,
	# so a 32-bit cmake (the default binary distribution) will always say "x86" regardless of the actual target.
	IF (CMAKE_SIZEOF_VOID_P EQUAL 8)
		SET (SYSTEM_PROCESSOR "x86_64")
	ELSE(CMAKE_SIZEOF_VOID_P EQUAL 8)
		SET (SYSTEM_PROCESSOR "x86")
	ENDIF(CMAKE_SIZEOF_VOID_P EQUAL 8)
ENDIF(WIN32)