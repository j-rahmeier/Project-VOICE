SET( BCI2000_APP_FILES
	${BCI2000_APP_FILES}
	"${BCI2000_SRC_DIR}/contrib/Hyperscanning/HyperscanningApplicationBase/HyperscanningApplicationBase.cpp"
)
SET( BCI2000_APP_INCDIRS
	${BCI2000_APP_INCDIRS}
	"${BCI2000_SRC_DIR}/contrib/Hyperscanning/HyperscanningApplicationBase"
)
include_directories( ${BCI2000_SRC_DIR}/contrib/Hyperscanning/HyperscanningApplicationBase )
#message( "APP_INCDIRS: ${BCI2000_APP_INCDIRS}" )
#message( "APP_FILES: ${BCI2000_APP_FILES}" )
#message( "SRC_FILES: ${BCI2000_SIGSRC_FILES}" )
