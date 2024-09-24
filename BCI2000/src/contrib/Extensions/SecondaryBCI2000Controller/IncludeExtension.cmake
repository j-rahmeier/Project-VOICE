###########################################################################
## $Id: IncludeExtension.cmake 7465 2023-06-30 15:23:43Z mellinger $
## Authors: mellinger@neurotechcenter.org

list( APPEND BCI2000_SIGSRC_FILES
   ${BCI2000_EXTENSION_DIR}/SecondaryBCI2000Controller.cpp
   ${BCI2000_SRC_DIR}/core/Operator/BCI2000Remote/BCI2000Remote.cpp
   ${BCI2000_SRC_DIR}/core/Operator/BCI2000Remote/BCI2000Connection.cpp
   ${BCI2000_EXTENSION_DIR}/BCI2000TwiceStartup.bat
   ${BCI2000_EXTENSION_DIR}/BCI2000TwiceSecondaryStartup.bat
)


