#include "cases.h"

#include <cgnsconfig.h> // for CG_BUILD_HDF5
#include <iostream>

int main(int argc, char* argv[])
{
	case_InitSuccess();
	case_InitFail();

	case_InitReadSuccess();
	case_InitReadFail();

	case_gotoRawDataTop();

	case_InitOptionCheck();

	case_InitCC();

	case_CheckLock();
	case_CheckCancel();

	case_CalcCondRead();
	case_CalcCondWrite();

	case_BcRead();
	case_BcWrite();

	case_Complex();

	case_GridRead();
#if (CG_BUILD_HDF5 != 0)
	case_GridReadFunc();
#endif
	case_GridWrite();

	case_SolStartEnd();

	case_SolWriteStd_adf();
	case_SolWriteDivide_adf();

#if (CG_BUILD_HDF5 != 0)
	case_SolWriteStd_hdf5();
	case_SolWriteDivide_hdf5();
#endif

	case_addGridAndResult();

	case_read_adf();
#if (CG_BUILD_HDF5 != 0)
	case_read_hdf5();
#endif

	case_read_adf_no_results();
#if (CG_BUILD_HDF5 != 0)
	case_read_hdf5_no_results();
#endif

	return 0;
}
