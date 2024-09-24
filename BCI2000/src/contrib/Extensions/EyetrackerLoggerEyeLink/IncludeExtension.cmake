###########################################################################
## $Id: IncludeExtension.cmake 7465 2023-06-30 15:23:43Z mellinger $
## Authors: Alexander Belsten (belsten@neurotechcenter.org)

if(WIN32)

SET( BCI2000_SIGSRC_FILES
   ${BCI2000_SIGSRC_FILES}
   ${BCI2000_EXTENSION_DIR}/EyetrackerLoggerEyeLink.cpp
)

IF( CMAKE_SIZEOF_VOID_P EQUAL 8 )
	SET ( BCI2000_SIGSRC_FILES
		${BCI2000_SIGSRC_FILES}
    		${BCI2000_EXTENSION_DIR}/lib/x64/eyelink_core64.dll
		${BCI2000_EXTENSION_DIR}/lib/x64/eyelink_core_graphics64.dll
		${BCI2000_EXTENSION_DIR}/lib/x64/eyelink_w32_comp64.dll
		${BCI2000_EXTENSION_DIR}/lib/x64/SDL.dll
		${BCI2000_EXTENSION_DIR}/lib/x64/SDL_gfx.dll
		${BCI2000_EXTENSION_DIR}/lib/x64/SDL_mixer.dll
		${BCI2000_EXTENSION_DIR}/lib/x64/SDL_ttf.dll
		${BCI2000_EXTENSION_DIR}/lib/x64/libfreetype-6.dll
		${BCI2000_EXTENSION_DIR}/lib/x64/zlib1.dll
	)
  
  	SET( BCI2000_SIGSRC_LIBS
     		${BCI2000_SIGSRC_LIBS}
     		${BCI2000_EXTENSION_DIR}/lib/x64/eyelink_core64.lib
		${BCI2000_EXTENSION_DIR}/lib/x64/eyelink_core_graphics64.lib
		${BCI2000_EXTENSION_DIR}/lib/x64/eyelink_w32_comp64.lib
		${BCI2000_EXTENSION_DIR}/lib/x64/SDL.lib
		${BCI2000_EXTENSION_DIR}/lib/x64/SDL_gfx.lib
		${BCI2000_EXTENSION_DIR}/lib/x64/SDL_mixer.lib
		${BCI2000_EXTENSION_DIR}/lib/x64/SDL_ttf.lib
  	)
ELSE()
	SET ( BCI2000_SIGSRC_FILES
		${BCI2000_SIGSRC_FILES}
    		${BCI2000_EXTENSION_DIR}/lib/x86/eyelink_core.dll
		${BCI2000_EXTENSION_DIR}/lib/x86/eyelink_core_graphics.dll
		${BCI2000_EXTENSION_DIR}/lib/x86/eyelink_w32_comp.dll
		${BCI2000_EXTENSION_DIR}/lib/x86/SDL.dll
		${BCI2000_EXTENSION_DIR}/lib/x86/sdl_gfx.dll
		${BCI2000_EXTENSION_DIR}/lib/x86/SDL_mixer.dll
		${BCI2000_EXTENSION_DIR}/lib/x86/SDL_ttf.dll
		${BCI2000_EXTENSION_DIR}/lib/x86/zlib.dll
	)
  
  	SET( BCI2000_SIGSRC_LIBS
     		${BCI2000_SIGSRC_LIBS}
     		${BCI2000_EXTENSION_DIR}/lib/x86/eyelink_core.lib
		${BCI2000_EXTENSION_DIR}/lib/x86/eyelink_core_graphics.lib
		${BCI2000_EXTENSION_DIR}/lib/x86/eyelink_w32_comp.lib
		${BCI2000_EXTENSION_DIR}/lib/x86/SDL.lib
		${BCI2000_EXTENSION_DIR}/lib/x86/sdl_gfx.lib
		${BCI2000_EXTENSION_DIR}/lib/x86/SDL_mixer.lib
		${BCI2000_EXTENSION_DIR}/lib/x86/SDL_ttf.lib
  	)
ENDIF()

else()
  message("EyetrackerLoggerEyeLink: Only available on Windows")
endif()
