###########################################################################
## $Id: IncludeExtension.cmake 7890 2024-03-03 21:07:55Z mellinger $
## Authors: griffin.milsap@gmail.com

list(APPEND BCI2000_SIGSRC_INCLUDE_EXTLIB
  LIBSNDFILE
  PORTAUDIO
)

SET( BCI2000_SIGSRC_FILES
  ${BCI2000_SIGSRC_FILES}
  ${BCI2000_EXTENSION_DIR}/AudioExtension.cpp
  ${BCI2000_SRC_DIR}/extlib/math/FilterDesign.cpp
)

