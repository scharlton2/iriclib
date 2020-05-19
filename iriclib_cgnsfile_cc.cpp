#include "error_macros.h"
#include "iriclib_cgnsfile.h"
#include "private/iriclib_cgnsfile_impl.h"

using namespace iRICLib;

int CgnsFile::CC_Read_Integer(const char *name, int* intvalue)
{
	int ier = impl->gotoCCChild(name);
	RETURN_IF_ERR;
	return Impl::readArray("Value", Integer, 1, intvalue);
}

int CgnsFile::CC_Read_Real(const char *name, double* realvalue)
{
	int ier = impl->gotoCCChild(name);
	RETURN_IF_ERR;
	return Impl::readArray("Value", RealDouble, 1, realvalue);
}

int CgnsFile::CC_Read_RealSingle(const char* name, float* realvalue)
{
	int ier = impl->gotoCCChild(name);
	RETURN_IF_ERR;
	return Impl::readArrayAs("Value", RealSingle, 1, realvalue);
}

int CgnsFile::CC_Read_StringLen(const char *name, int* length)
{
	int ier = impl->gotoCCChild(name);
	RETURN_IF_ERR;
	return Impl::readStringLen("Value", length);
}

int CgnsFile::CC_Read_String(const char *name, char* strvalue)
{
	int ier = impl->gotoCCChild(name);
	RETURN_IF_ERR;
	return Impl::readString("Value", -1, strvalue);
}

int CgnsFile::CC_Read_FunctionalSize(const char *name, cgsize_t* size)
{
	int ier = impl->gotoCCChild(name);
	RETURN_IF_ERR;

	int narrays;
	ier = cg_narrays(&narrays);
	RETURN_IF_ERR;

	for (int i = 1; i <= narrays; ++i) {
		char arrayname[Impl::NAME_MAXLENGTH];
		DataType_t datatype;
		int dim;
		cgsize_t dimvec[Impl::MAX_DIMS];
		ier = cg_array_info(i, arrayname, &datatype, &dim, dimvec);
		*size = dimvec[0];
		return 0;
	}
	return 1;
}

int CgnsFile::CC_Read_Functional(const char *name, double* x, double* y)
{
	int ier;
	ier = CC_Read_FunctionalWithName(name, "Param", x);
	RETURN_IF_ERR;
	ier = CC_Read_FunctionalWithName(name, "Value", y);
	RETURN_IF_ERR;
	return 0;
}

int CgnsFile::CC_Read_FunctionalWithName(const char *name, const char *paramname, double* data)
{
	int ier = impl->gotoCCChild(name);
	RETURN_IF_ERR;
	return Impl::readArray(paramname, RealDouble, -1, data);
}

int CgnsFile::CC_Read_Functional_RealSingle(const char *name, float* x, float* y)
{
	int ier;
	ier = CC_Read_FunctionalWithName_RealSingle(name, "Param", x);
	RETURN_IF_ERR;
	ier = CC_Read_FunctionalWithName_RealSingle(name, "Value", y);
	RETURN_IF_ERR;
	return 0;
}

int CgnsFile::CC_Read_FunctionalWithName_RealSingle(const char* name, const char* paramname, float* data)
{
	int ier = impl->gotoCCChild(name);
	RETURN_IF_ERR;
	return Impl::readArrayAs(paramname, RealSingle, -1, data);
}

int CgnsFile::CC_Read_FunctionalWithName_String(const char *name, const char *paramname, char* strvalue)
{
	int ier = impl->gotoCCChild(name);
	RETURN_IF_ERR;
	return Impl::readString(paramname, -1, strvalue);
}

int CgnsFile::CC_Read_FunctionalWithName_StringLen(const char* name, const char* paramname, int* length)
{
	int ier = impl->gotoCCChild(name);
	RETURN_IF_ERR;
	return Impl::readStringLen(paramname, length);
}

int CgnsFile::CC_Write_Integer(const char *name, int intvalue)
{
	int ier = impl->gotoCCChildCreateIfNotExist(name);
	RETURN_IF_ERR;
	return Impl::writeArray("Value", Integer, 1, &intvalue);
}

int CgnsFile::CC_Write_Real(const char *name, double realvalue)
{
	int ier = impl->gotoCCChildCreateIfNotExist(name);
	RETURN_IF_ERR;
	return Impl::writeArray("Value", RealDouble, 1, &realvalue);
}

int CgnsFile::CC_Write_String(const char *name, const char* strvalue)
{
	int ier = impl->gotoCCChildCreateIfNotExist(name);
	RETURN_IF_ERR;
	return Impl::writeString("Value", strvalue);
}

int CgnsFile::CC_Write_Functional(const char *name, int length, double* realarray_x, double* realarray_y)
{
	int ier;
	ier = CC_Write_FunctionalWithName(name, "Param", length, realarray_x);
	RETURN_IF_ERR;
	ier = CC_Write_FunctionalWithName(name, "Value", length, realarray_y);
	RETURN_IF_ERR;
	return 0;
}

int CgnsFile::CC_Write_FunctionalWithName_String(const char* name, const char* paramname, const char* value)
{
	int ier = impl->gotoCCChildCreateIfNotExist(name);
	RETURN_IF_ERR;
	return Impl::writeString(paramname, value);
}

int CgnsFile::CC_Write_FunctionalWithName(const char *name, const char* paramname, int length, double* data)
{
	int ier = impl->gotoCCChildCreateIfNotExist(name);
	RETURN_IF_ERR;
	return Impl::writeArray(paramname, RealDouble, length, data);
}
