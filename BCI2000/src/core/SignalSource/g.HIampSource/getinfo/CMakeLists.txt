###########################################################################
## $Id: CMakeLists.txt 7465 2023-06-30 15:23:43Z mellinger $
## Author: Alexander Belsten belsten@neurotechcenter.org
## Description: Build information for gHIampgetinfo

SET( NAME gHIampgetinfo )

UTILS_INCLUDE( frameworks/Core )
BCI2000_ADD_TARGET( INFO Tool CONSOLEAPP ${NAME}
  gHIampgetinfo.cpp
  ../lib/gHIamp.imports.cpp
)
IF( NOT FAILED )
  BCI2000_ADD_TO_INVENTORY( Tool ${NAME} )
ENDIF( NOT FAILED )
