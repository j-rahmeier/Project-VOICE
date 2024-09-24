###########################################################################
## $Id: CMakeLists.txt 7465 2023-06-30 15:23:43Z mellinger $
## Authors: juergen.mellinger@uni-tuebingen.de
## Description: Build information for BCI2000ShellExtension

set_crt_link_type( STATIC )
bci2000_include( COMMODULE )
utils_include( frameworks/Core )

utils_set_appicon( "puzzlepiece;signal-document;text-document;generic-document" )
bci2000_add_target(
  WINDLL BCI2000ShellExtension

  BCI2000ShellExtension.cpp
  BCI2000ShellExtension.rc
  ContextMenuExtension.cpp
  QueryInfoExtension.cpp
  ExtractIconExtension.cpp
  ${BCI2000_SRC_DIR}/shared/bcistream/BCIStream_comdll.cpp

  InstallShellExtension.cmd
)
