
MACRO( BCI2000_ADD_HYPERSCANNING_APPLICATION_MODULE )
	BCI2000_ADD_APPLICATION_MODULE( ${ARGV} ${BCI2000_SRC_DIR}/contrib/Hyperscanning/HyperscanningApplicationBase ${BCI2000_SRC_DIR}/contrib/Hyperscanning/HyperscanningApplicationBase/HyperscanningApplicationBase.cpp )

ENDMACRO()

