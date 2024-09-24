// Import definitions for libmx library
// generated Thu Oct 12 16:55:47 2023 by DylibTool
#ifdef STATIC_LIBLIBMX

namespace Dylib { bool libmx_Loaded() { return true; } }


#else // STATIC_LIBLIBMX

#include "libmx.imports.h"
#include "DylibImports.h"
namespace { extern const Dylib::Import* functionImports; }

// Here you may specify a custom error message to be displayed when the library cannot be found.
static const char* notFoundMsg = "";
// Here you may specify an URL to some local or remote help resource.
static const char* notFoundURL = "";
RegisterDylibWithAliases( libmx, "libmx", functionImports, notFoundMsg, notFoundURL );

extern "C" {
decltype(&__556a7739_mxAddField_800) mxAddField_800 = 0;
decltype(&__21141b0b_mxArrayToString_800) mxArrayToString_800 = 0;
decltype(&__19414f4d_mxArrayToUTF8String_800) mxArrayToUTF8String_800 = 0;
decltype(&__1b244f5e_mxCalcSingleSubscript_800) mxCalcSingleSubscript_800 = 0;
decltype(&__5e2c1c32_mxCalloc_800) mxCalloc_800 = 0;
decltype(&__4351112a_mxCreateCellArray_800) mxCreateCellArray_800 = 0;
decltype(&__5f557031_mxCreateCellMatrix_800) mxCreateCellMatrix_800 = 0;
decltype(&__435c1c34_mxCreateCharArray_800) mxCreateCharArray_800 = 0;
decltype(&__31336146_mxCreateCharMatrixFromStrings_800) mxCreateCharMatrixFromStrings_800 = 0;
decltype(&__0b59565c_mxCreateDoubleMatrix_800) mxCreateDoubleMatrix_800 = 0;
decltype(&__1e474054_mxCreateDoubleScalar_800) mxCreateDoubleScalar_800 = 0;
decltype(&__0a5d6d76_mxCreateLogicalArray_800) mxCreateLogicalArray_800 = 0;
decltype(&__0e3c766a_mxCreateLogicalMatrix_800) mxCreateLogicalMatrix_800 = 0;
decltype(&__0629687c_mxCreateLogicalScalar_800) mxCreateLogicalScalar_800 = 0;
decltype(&__194f687a_mxCreateNumericArray_800) mxCreateNumericArray_800 = 0;
decltype(&__1d2e7366_mxCreateNumericMatrix_800) mxCreateNumericMatrix_800 = 0;
decltype(&__69715040_mxCreateSparseLogicalMatrix_800) mxCreateSparseLogicalMatrix_800 = 0;
decltype(&__183c095d_mxCreateSparse_800) mxCreateSparse_800 = 0;
decltype(&__6d3e0f0c_mxCreateStringFromNChars_800) mxCreateStringFromNChars_800 = 0;
decltype(&__053a1a46_mxCreateString_800) mxCreateString_800 = 0;
decltype(&__72484d4f_mxCreateStructArray_800) mxCreateStructArray_800 = 0;
decltype(&__1353514b_mxCreateStructMatrix_800) mxCreateStructMatrix_800 = 0;
decltype(&__6b6e4f2f_mxCreateUninitNumericArray_800) mxCreateUninitNumericArray_800 = 0;
decltype(&__70724b4e_mxCreateUninitNumericMatrix_800) mxCreateUninitNumericMatrix_800 = 0;
decltype(&__36041b40_mxDestroyArray_800) mxDestroyArray_800 = 0;
decltype(&__512a191e_mxDuplicateArray_800) mxDuplicateArray_800 = 0;
decltype(&__382d194a_mxFree_800) mxFree_800 = 0;
decltype(&__45641a39_mxGetCell_800) mxGetCell_800 = 0;
decltype(&__5b78703c_mxGetChars_800) mxGetChars_800 = 0;
decltype(&__35705270_mxGetClassID_800) mxGetClassID_800 = 0;
decltype(&__371d3a5d_mxGetClassName_800) mxGetClassName_800 = 0;
decltype(&__796d7b4d_mxGetComplexDoubles_800) mxGetComplexDoubles_800 = 0;
decltype(&__267a6679_mxGetComplexInt16s_800) mxGetComplexInt16s_800 = 0;
decltype(&__227a667b_mxGetComplexInt32s_800) mxGetComplexInt32s_800 = 0;
decltype(&__247a667e_mxGetComplexInt64s_800) mxGetComplexInt64s_800 = 0;
decltype(&__63660178_mxGetComplexInt8s_800) mxGetComplexInt8s_800 = 0;
decltype(&__6e6b6048_mxGetComplexSingles_800) mxGetComplexSingles_800 = 0;
decltype(&__3538605b_mxGetComplexUint16s_800) mxGetComplexUint16s_800 = 0;
decltype(&__373c605b_mxGetComplexUint32s_800) mxGetComplexUint32s_800 = 0;
decltype(&__323a605b_mxGetComplexUint64s_800) mxGetComplexUint64s_800 = 0;
decltype(&__347d7c3c_mxGetComplexUint8s_800) mxGetComplexUint8s_800 = 0;
decltype(&__48631e21_mxGetData_800) mxGetData_800 = 0;
decltype(&__2b0c1e3e_mxGetDimensions_800) mxGetDimensions_800 = 0;
decltype(&__24687d53_mxGetDoubles_800) mxGetDoubles_800 = 0;
decltype(&__78090f21_mxGetElementSize_800) mxGetElementSize_800 = 0;
decltype(&__46050726_mxGetEps_800) mxGetEps_800 = 0;
decltype(&__6e6d5e23_mxGetFieldByNumber_800) mxGetFieldByNumber_800 = 0;
decltype(&__03081042_mxGetFieldNameByNumber_800) mxGetFieldNameByNumber_800 = 0;
decltype(&__47003537_mxGetFieldNumber_800) mxGetFieldNumber_800 = 0;
decltype(&__456a7138_mxGetField_800) mxGetField_800 = 0;
decltype(&__46091933_mxGetInf_800) mxGetInf_800 = 0;
decltype(&__10376a4e_mxGetInt16s_800) mxGetInt16s_800 = 0;
decltype(&__12336a4e_mxGetInt32s_800) mxGetInt32s_800 = 0;
decltype(&__17356a4e_mxGetInt64s_800) mxGetInt64s_800 = 0;
decltype(&__11727629_mxGetInt8s_800) mxGetInt8s_800 = 0;
decltype(&__2101053a_mxGetIr_800) mxGetIr_800 = 0;
decltype(&__2102143a_mxGetJc_800) mxGetJc_800 = 0;
decltype(&__3308715e_mxGetLogicals_800) mxGetLogicals_800 = 0;
decltype(&__2905185d_mxGetM_800) mxGetM_800 = 0;
decltype(&__40744752_mxGetNChars_800) mxGetNChars_800 = 0;
decltype(&__2906185d_mxGetN_800) mxGetN_800 = 0;
decltype(&__460e161b_mxGetNaN_800) mxGetNaN_800 = 0;
decltype(&__2f27191c_mxGetNumberOfDimensions_800) mxGetNumberOfDimensions_800 = 0;
decltype(&__332c7a66_mxGetNumberOfElements_800) mxGetNumberOfElements_800 = 0;
decltype(&__49416a7d_mxGetNumberOfFields_800) mxGetNumberOfFields_800 = 0;
decltype(&__487e6230_mxGetNzmax_800) mxGetNzmax_800 = 0;
decltype(&__2118053a_mxGetPr_800) mxGetPr_800 = 0;
decltype(&__20127f4e_mxGetProperty_800) mxGetProperty_800 = 0;
decltype(&__4d7a665b_mxGetScalar_800) mxGetScalar_800 = 0;
decltype(&__217f7b48_mxGetSingles_800) mxGetSingles_800 = 0;
decltype(&__48756448_mxGetString_800) mxGetString_800 = 0;
decltype(&__32242848_mxGetUint16s_800) mxGetUint16s_800 = 0;
decltype(&__32262c48_mxGetUint32s_800) mxGetUint32s_800 = 0;
decltype(&__32232a48_mxGetUint64s_800) mxGetUint64s_800 = 0;
decltype(&__55256d54_mxGetUint8s_800) mxGetUint8s_800 = 0;
decltype(&__28306544_mxGetUserBits_800) mxGetUserBits_800 = 0;
decltype(&__7125152f_mxIsCell_800) mxIsCell_800 = 0;
decltype(&__71281831_mxIsChar_800) mxIsChar_800 = 0;
decltype(&__6d4b1030_mxIsClass_800) mxIsClass_800 = 0;
decltype(&__7a426c5c_mxIsComplex_800) mxIsComplex_800 = 0;
decltype(&__75426329_mxIsDouble_800) mxIsDouble_800 = 0;
decltype(&__614a0137_mxIsEmpty_800) mxIsEmpty_800 = 0;
decltype(&__6f447822_mxIsFinite_800) mxIsFinite_800 = 0;
decltype(&__52322e1f_mxIsFromGlobalWS_800) mxIsFromGlobalWS_800 = 0;
decltype(&__4b507922_mxIsFunctionHandle_800) mxIsFunctionHandle_800 = 0;
decltype(&__1c261f2c_mxIsInf_800) mxIsInf_800 = 0;
decltype(&__22490572_mxIsInt16_800) mxIsInt16_800 = 0;
decltype(&__26490570_mxIsInt32_800) mxIsInt32_800 = 0;
decltype(&__20490575_mxIsInt64_800) mxIsInt64_800 = 0;
decltype(&__7b2e0d7b_mxIsInt8_800) mxIsInt8_800 = 0;
decltype(&__061c646d_mxIsLogicalScalarTrue_800) mxIsLogicalScalarTrue_800 = 0;
decltype(&__63481618_mxIsLogicalScalar_800) mxIsLogicalScalar_800 = 0;
decltype(&__7a467245_mxIsLogical_800) mxIsLogical_800 = 0;
decltype(&__1b29372c_mxIsNaN_800) mxIsNaN_800 = 0;
decltype(&__69547749_mxIsNumeric_800) mxIsNumeric_800 = 0;
decltype(&__715e7c2e_mxIsObject_800) mxIsObject_800 = 0;
decltype(&__675d773a_mxIsOpaque_800) mxIsOpaque_800 = 0;
decltype(&__6f597727_mxIsScalar_800) mxIsScalar_800 = 0;
decltype(&__6244782c_mxIsSingle_800) mxIsSingle_800 = 0;
decltype(&__7d5d7739_mxIsSparse_800) mxIsSparse_800 = 0;
decltype(&__6d48643e_mxIsStruct_800) mxIsStruct_800 = 0;
decltype(&__3917783f_mxIsUint16_800) mxIsUint16_800 = 0;
decltype(&__3b13783f_mxIsUint32_800) mxIsUint32_800 = 0;
decltype(&__3e15783f_mxIsUint64_800) mxIsUint64_800 = 0;
decltype(&__304e1f37_mxIsUint8_800) mxIsUint8_800 = 0;
decltype(&__4e595a04_mxMakeArrayComplex_800) mxMakeArrayComplex_800 = 0;
decltype(&__292d291e_mxMakeArrayReal_800) mxMakeArrayReal_800 = 0;
decltype(&__5e2c1232_mxMalloc_800) mxMalloc_800 = 0;
decltype(&__5f4b063a_mxRealloc_800) mxRealloc_800 = 0;
decltype(&__1221795c_mxRemoveField_800) mxRemoveField_800 = 0;
decltype(&__45640e39_mxSetCell_800) mxSetCell_800 = 0;
decltype(&__371d2e5d_mxSetClassName_800) mxSetClassName_800 = 0;
decltype(&__796d6f4d_mxSetComplexDoubles_800) mxSetComplexDoubles_800 = 0;
decltype(&__267a7279_mxSetComplexInt16s_800) mxSetComplexInt16s_800 = 0;
decltype(&__227a727b_mxSetComplexInt32s_800) mxSetComplexInt32s_800 = 0;
decltype(&__247a727e_mxSetComplexInt64s_800) mxSetComplexInt64s_800 = 0;
decltype(&__63661578_mxSetComplexInt8s_800) mxSetComplexInt8s_800 = 0;
decltype(&__6e6b7448_mxSetComplexSingles_800) mxSetComplexSingles_800 = 0;
decltype(&__3538745b_mxSetComplexUint16s_800) mxSetComplexUint16s_800 = 0;
decltype(&__373c745b_mxSetComplexUint32s_800) mxSetComplexUint32s_800 = 0;
decltype(&__323a745b_mxSetComplexUint64s_800) mxSetComplexUint64s_800 = 0;
decltype(&__347d683c_mxSetComplexUint8s_800) mxSetComplexUint8s_800 = 0;
decltype(&__48630a21_mxSetData_800) mxSetData_800 = 0;
decltype(&__2b0c0a3e_mxSetDimensions_800) mxSetDimensions_800 = 0;
decltype(&__24686953_mxSetDoubles_800) mxSetDoubles_800 = 0;
decltype(&__6e6d4a23_mxSetFieldByNumber_800) mxSetFieldByNumber_800 = 0;
decltype(&__456a6538_mxSetField_800) mxSetField_800 = 0;
decltype(&__75471902_mxSetFromGlobalWS_800) mxSetFromGlobalWS_800 = 0;
decltype(&__10377e4e_mxSetInt16s_800) mxSetInt16s_800 = 0;
decltype(&__12337e4e_mxSetInt32s_800) mxSetInt32s_800 = 0;
decltype(&__17357e4e_mxSetInt64s_800) mxSetInt64s_800 = 0;
decltype(&__11726229_mxSetInt8s_800) mxSetInt8s_800 = 0;
decltype(&__2101113a_mxSetIr_800) mxSetIr_800 = 0;
decltype(&__2102003a_mxSetJc_800) mxSetJc_800 = 0;
decltype(&__29050c5d_mxSetM_800) mxSetM_800 = 0;
decltype(&__29060c5d_mxSetN_800) mxSetN_800 = 0;
decltype(&__487e7630_mxSetNzmax_800) mxSetNzmax_800 = 0;
decltype(&__2118113a_mxSetPr_800) mxSetPr_800 = 0;
decltype(&__20126b4e_mxSetProperty_800) mxSetProperty_800 = 0;
decltype(&__217f6f48_mxSetSingles_800) mxSetSingles_800 = 0;
decltype(&__32243c48_mxSetUint16s_800) mxSetUint16s_800 = 0;
decltype(&__32263848_mxSetUint32s_800) mxSetUint32s_800 = 0;
decltype(&__32233e48_mxSetUint64s_800) mxSetUint64s_800 = 0;
decltype(&__55257954_mxSetUint8s_800) mxSetUint8s_800 = 0;
decltype(&__28307144_mxSetUserBits_800) mxSetUserBits_800 = 0;
} // extern "C"

namespace {
const Dylib::Import functionImports_[] =
{
  { "mxAddField_800", (void**)&mxAddField_800, Dylib::Import::cMangled },
  { "mxArrayToString_800", (void**)&mxArrayToString_800, Dylib::Import::cMangled },
  { "mxArrayToUTF8String_800", (void**)&mxArrayToUTF8String_800, Dylib::Import::cMangled },
  { "mxCalcSingleSubscript_800", (void**)&mxCalcSingleSubscript_800, Dylib::Import::cMangled },
  { "mxCalloc_800", (void**)&mxCalloc_800, Dylib::Import::cMangled },
  { "mxCreateCellArray_800", (void**)&mxCreateCellArray_800, Dylib::Import::cMangled },
  { "mxCreateCellMatrix_800", (void**)&mxCreateCellMatrix_800, Dylib::Import::cMangled },
  { "mxCreateCharArray_800", (void**)&mxCreateCharArray_800, Dylib::Import::cMangled },
  { "mxCreateCharMatrixFromStrings_800", (void**)&mxCreateCharMatrixFromStrings_800, Dylib::Import::cMangled },
  { "mxCreateDoubleMatrix_800", (void**)&mxCreateDoubleMatrix_800, Dylib::Import::cMangled },
  { "mxCreateDoubleScalar_800", (void**)&mxCreateDoubleScalar_800, Dylib::Import::cMangled },
  { "mxCreateLogicalArray_800", (void**)&mxCreateLogicalArray_800, Dylib::Import::cMangled },
  { "mxCreateLogicalMatrix_800", (void**)&mxCreateLogicalMatrix_800, Dylib::Import::cMangled },
  { "mxCreateLogicalScalar_800", (void**)&mxCreateLogicalScalar_800, Dylib::Import::cMangled },
  { "mxCreateNumericArray_800", (void**)&mxCreateNumericArray_800, Dylib::Import::cMangled },
  { "mxCreateNumericMatrix_800", (void**)&mxCreateNumericMatrix_800, Dylib::Import::cMangled },
  { "mxCreateSparseLogicalMatrix_800", (void**)&mxCreateSparseLogicalMatrix_800, Dylib::Import::cMangled },
  { "mxCreateSparse_800", (void**)&mxCreateSparse_800, Dylib::Import::cMangled },
  { "mxCreateStringFromNChars_800", (void**)&mxCreateStringFromNChars_800, Dylib::Import::cMangled },
  { "mxCreateString_800", (void**)&mxCreateString_800, Dylib::Import::cMangled },
  { "mxCreateStructArray_800", (void**)&mxCreateStructArray_800, Dylib::Import::cMangled },
  { "mxCreateStructMatrix_800", (void**)&mxCreateStructMatrix_800, Dylib::Import::cMangled },
  { "mxCreateUninitNumericArray_800", (void**)&mxCreateUninitNumericArray_800, Dylib::Import::cMangled },
  { "mxCreateUninitNumericMatrix_800", (void**)&mxCreateUninitNumericMatrix_800, Dylib::Import::cMangled },
  { "mxDestroyArray_800", (void**)&mxDestroyArray_800, Dylib::Import::cMangled },
  { "mxDuplicateArray_800", (void**)&mxDuplicateArray_800, Dylib::Import::cMangled },
  { "mxFree_800", (void**)&mxFree_800, Dylib::Import::cMangled },
  { "mxGetCell_800", (void**)&mxGetCell_800, Dylib::Import::cMangled },
  { "mxGetChars_800", (void**)&mxGetChars_800, Dylib::Import::cMangled },
  { "mxGetClassID_800", (void**)&mxGetClassID_800, Dylib::Import::cMangled },
  { "mxGetClassName_800", (void**)&mxGetClassName_800, Dylib::Import::cMangled },
  { "mxGetComplexDoubles_800", (void**)&mxGetComplexDoubles_800, Dylib::Import::cMangled },
  { "mxGetComplexInt16s_800", (void**)&mxGetComplexInt16s_800, Dylib::Import::cMangled },
  { "mxGetComplexInt32s_800", (void**)&mxGetComplexInt32s_800, Dylib::Import::cMangled },
  { "mxGetComplexInt64s_800", (void**)&mxGetComplexInt64s_800, Dylib::Import::cMangled },
  { "mxGetComplexInt8s_800", (void**)&mxGetComplexInt8s_800, Dylib::Import::cMangled },
  { "mxGetComplexSingles_800", (void**)&mxGetComplexSingles_800, Dylib::Import::cMangled },
  { "mxGetComplexUint16s_800", (void**)&mxGetComplexUint16s_800, Dylib::Import::cMangled },
  { "mxGetComplexUint32s_800", (void**)&mxGetComplexUint32s_800, Dylib::Import::cMangled },
  { "mxGetComplexUint64s_800", (void**)&mxGetComplexUint64s_800, Dylib::Import::cMangled },
  { "mxGetComplexUint8s_800", (void**)&mxGetComplexUint8s_800, Dylib::Import::cMangled },
  { "mxGetData_800", (void**)&mxGetData_800, Dylib::Import::cMangled },
  { "mxGetDimensions_800", (void**)&mxGetDimensions_800, Dylib::Import::cMangled },
  { "mxGetDoubles_800", (void**)&mxGetDoubles_800, Dylib::Import::cMangled },
  { "mxGetElementSize_800", (void**)&mxGetElementSize_800, Dylib::Import::cMangled },
  { "mxGetEps_800", (void**)&mxGetEps_800, Dylib::Import::cMangled },
  { "mxGetFieldByNumber_800", (void**)&mxGetFieldByNumber_800, Dylib::Import::cMangled },
  { "mxGetFieldNameByNumber_800", (void**)&mxGetFieldNameByNumber_800, Dylib::Import::cMangled },
  { "mxGetFieldNumber_800", (void**)&mxGetFieldNumber_800, Dylib::Import::cMangled },
  { "mxGetField_800", (void**)&mxGetField_800, Dylib::Import::cMangled },
  { "mxGetInf_800", (void**)&mxGetInf_800, Dylib::Import::cMangled },
  { "mxGetInt16s_800", (void**)&mxGetInt16s_800, Dylib::Import::cMangled },
  { "mxGetInt32s_800", (void**)&mxGetInt32s_800, Dylib::Import::cMangled },
  { "mxGetInt64s_800", (void**)&mxGetInt64s_800, Dylib::Import::cMangled },
  { "mxGetInt8s_800", (void**)&mxGetInt8s_800, Dylib::Import::cMangled },
  { "mxGetIr_800", (void**)&mxGetIr_800, Dylib::Import::cMangled },
  { "mxGetJc_800", (void**)&mxGetJc_800, Dylib::Import::cMangled },
  { "mxGetLogicals_800", (void**)&mxGetLogicals_800, Dylib::Import::cMangled },
  { "mxGetM_800", (void**)&mxGetM_800, Dylib::Import::cMangled },
  { "mxGetNChars_800", (void**)&mxGetNChars_800, Dylib::Import::cMangled },
  { "mxGetN_800", (void**)&mxGetN_800, Dylib::Import::cMangled },
  { "mxGetNaN_800", (void**)&mxGetNaN_800, Dylib::Import::cMangled },
  { "mxGetNumberOfDimensions_800", (void**)&mxGetNumberOfDimensions_800, Dylib::Import::cMangled },
  { "mxGetNumberOfElements_800", (void**)&mxGetNumberOfElements_800, Dylib::Import::cMangled },
  { "mxGetNumberOfFields_800", (void**)&mxGetNumberOfFields_800, Dylib::Import::cMangled },
  { "mxGetNzmax_800", (void**)&mxGetNzmax_800, Dylib::Import::cMangled },
  { "mxGetPr_800", (void**)&mxGetPr_800, Dylib::Import::cMangled },
  { "mxGetProperty_800", (void**)&mxGetProperty_800, Dylib::Import::cMangled },
  { "mxGetScalar_800", (void**)&mxGetScalar_800, Dylib::Import::cMangled },
  { "mxGetSingles_800", (void**)&mxGetSingles_800, Dylib::Import::cMangled },
  { "mxGetString_800", (void**)&mxGetString_800, Dylib::Import::cMangled },
  { "mxGetUint16s_800", (void**)&mxGetUint16s_800, Dylib::Import::cMangled },
  { "mxGetUint32s_800", (void**)&mxGetUint32s_800, Dylib::Import::cMangled },
  { "mxGetUint64s_800", (void**)&mxGetUint64s_800, Dylib::Import::cMangled },
  { "mxGetUint8s_800", (void**)&mxGetUint8s_800, Dylib::Import::cMangled },
  { "mxGetUserBits_800", (void**)&mxGetUserBits_800, Dylib::Import::cMangled },
  { "mxIsCell_800", (void**)&mxIsCell_800, Dylib::Import::cMangled },
  { "mxIsChar_800", (void**)&mxIsChar_800, Dylib::Import::cMangled },
  { "mxIsClass_800", (void**)&mxIsClass_800, Dylib::Import::cMangled },
  { "mxIsComplex_800", (void**)&mxIsComplex_800, Dylib::Import::cMangled },
  { "mxIsDouble_800", (void**)&mxIsDouble_800, Dylib::Import::cMangled },
  { "mxIsEmpty_800", (void**)&mxIsEmpty_800, Dylib::Import::cMangled },
  { "mxIsFinite_800", (void**)&mxIsFinite_800, Dylib::Import::cMangled },
  { "mxIsFromGlobalWS_800", (void**)&mxIsFromGlobalWS_800, Dylib::Import::cMangled },
  { "mxIsFunctionHandle_800", (void**)&mxIsFunctionHandle_800, Dylib::Import::cMangled },
  { "mxIsInf_800", (void**)&mxIsInf_800, Dylib::Import::cMangled },
  { "mxIsInt16_800", (void**)&mxIsInt16_800, Dylib::Import::cMangled },
  { "mxIsInt32_800", (void**)&mxIsInt32_800, Dylib::Import::cMangled },
  { "mxIsInt64_800", (void**)&mxIsInt64_800, Dylib::Import::cMangled },
  { "mxIsInt8_800", (void**)&mxIsInt8_800, Dylib::Import::cMangled },
  { "mxIsLogicalScalarTrue_800", (void**)&mxIsLogicalScalarTrue_800, Dylib::Import::cMangled },
  { "mxIsLogicalScalar_800", (void**)&mxIsLogicalScalar_800, Dylib::Import::cMangled },
  { "mxIsLogical_800", (void**)&mxIsLogical_800, Dylib::Import::cMangled },
  { "mxIsNaN_800", (void**)&mxIsNaN_800, Dylib::Import::cMangled },
  { "mxIsNumeric_800", (void**)&mxIsNumeric_800, Dylib::Import::cMangled },
  { "mxIsObject_800", (void**)&mxIsObject_800, Dylib::Import::cMangled },
  { "mxIsOpaque_800", (void**)&mxIsOpaque_800, Dylib::Import::cMangled },
  { "mxIsScalar_800", (void**)&mxIsScalar_800, Dylib::Import::cMangled },
  { "mxIsSingle_800", (void**)&mxIsSingle_800, Dylib::Import::cMangled },
  { "mxIsSparse_800", (void**)&mxIsSparse_800, Dylib::Import::cMangled },
  { "mxIsStruct_800", (void**)&mxIsStruct_800, Dylib::Import::cMangled },
  { "mxIsUint16_800", (void**)&mxIsUint16_800, Dylib::Import::cMangled },
  { "mxIsUint32_800", (void**)&mxIsUint32_800, Dylib::Import::cMangled },
  { "mxIsUint64_800", (void**)&mxIsUint64_800, Dylib::Import::cMangled },
  { "mxIsUint8_800", (void**)&mxIsUint8_800, Dylib::Import::cMangled },
  { "mxMakeArrayComplex_800", (void**)&mxMakeArrayComplex_800, Dylib::Import::cMangled },
  { "mxMakeArrayReal_800", (void**)&mxMakeArrayReal_800, Dylib::Import::cMangled },
  { "mxMalloc_800", (void**)&mxMalloc_800, Dylib::Import::cMangled },
  { "mxRealloc_800", (void**)&mxRealloc_800, Dylib::Import::cMangled },
  { "mxRemoveField_800", (void**)&mxRemoveField_800, Dylib::Import::cMangled },
  { "mxSetCell_800", (void**)&mxSetCell_800, Dylib::Import::cMangled },
  { "mxSetClassName_800", (void**)&mxSetClassName_800, Dylib::Import::cMangled },
  { "mxSetComplexDoubles_800", (void**)&mxSetComplexDoubles_800, Dylib::Import::cMangled },
  { "mxSetComplexInt16s_800", (void**)&mxSetComplexInt16s_800, Dylib::Import::cMangled },
  { "mxSetComplexInt32s_800", (void**)&mxSetComplexInt32s_800, Dylib::Import::cMangled },
  { "mxSetComplexInt64s_800", (void**)&mxSetComplexInt64s_800, Dylib::Import::cMangled },
  { "mxSetComplexInt8s_800", (void**)&mxSetComplexInt8s_800, Dylib::Import::cMangled },
  { "mxSetComplexSingles_800", (void**)&mxSetComplexSingles_800, Dylib::Import::cMangled },
  { "mxSetComplexUint16s_800", (void**)&mxSetComplexUint16s_800, Dylib::Import::cMangled },
  { "mxSetComplexUint32s_800", (void**)&mxSetComplexUint32s_800, Dylib::Import::cMangled },
  { "mxSetComplexUint64s_800", (void**)&mxSetComplexUint64s_800, Dylib::Import::cMangled },
  { "mxSetComplexUint8s_800", (void**)&mxSetComplexUint8s_800, Dylib::Import::cMangled },
  { "mxSetData_800", (void**)&mxSetData_800, Dylib::Import::cMangled },
  { "mxSetDimensions_800", (void**)&mxSetDimensions_800, Dylib::Import::cMangled },
  { "mxSetDoubles_800", (void**)&mxSetDoubles_800, Dylib::Import::cMangled },
  { "mxSetFieldByNumber_800", (void**)&mxSetFieldByNumber_800, Dylib::Import::cMangled },
  { "mxSetField_800", (void**)&mxSetField_800, Dylib::Import::cMangled },
  { "mxSetFromGlobalWS_800", (void**)&mxSetFromGlobalWS_800, Dylib::Import::cMangled },
  { "mxSetInt16s_800", (void**)&mxSetInt16s_800, Dylib::Import::cMangled },
  { "mxSetInt32s_800", (void**)&mxSetInt32s_800, Dylib::Import::cMangled },
  { "mxSetInt64s_800", (void**)&mxSetInt64s_800, Dylib::Import::cMangled },
  { "mxSetInt8s_800", (void**)&mxSetInt8s_800, Dylib::Import::cMangled },
  { "mxSetIr_800", (void**)&mxSetIr_800, Dylib::Import::cMangled },
  { "mxSetJc_800", (void**)&mxSetJc_800, Dylib::Import::cMangled },
  { "mxSetM_800", (void**)&mxSetM_800, Dylib::Import::cMangled },
  { "mxSetN_800", (void**)&mxSetN_800, Dylib::Import::cMangled },
  { "mxSetNzmax_800", (void**)&mxSetNzmax_800, Dylib::Import::cMangled },
  { "mxSetPr_800", (void**)&mxSetPr_800, Dylib::Import::cMangled },
  { "mxSetProperty_800", (void**)&mxSetProperty_800, Dylib::Import::cMangled },
  { "mxSetSingles_800", (void**)&mxSetSingles_800, Dylib::Import::cMangled },
  { "mxSetUint16s_800", (void**)&mxSetUint16s_800, Dylib::Import::cMangled },
  { "mxSetUint32s_800", (void**)&mxSetUint32s_800, Dylib::Import::cMangled },
  { "mxSetUint64s_800", (void**)&mxSetUint64s_800, Dylib::Import::cMangled },
  { "mxSetUint8s_800", (void**)&mxSetUint8s_800, Dylib::Import::cMangled },
  { "mxSetUserBits_800", (void**)&mxSetUserBits_800, Dylib::Import::cMangled },
  { 0, 0, 0 }
};
const Dylib::Import* functionImports = functionImports_;
}

#endif // STATIC_LIBLIBMX

