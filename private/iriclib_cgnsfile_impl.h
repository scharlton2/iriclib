#ifndef IRICLIB_CGNSFILE_IMPL_H
#define IRICLIB_CGNSFILE_IMPL_H

#include "../iriclib_cgnsfile.h"
#include "iriclib_cgnsfile_baseiterativet.h"

#include <string>
#include <vector>

namespace iRICLib {

class CgnsFile::Impl
{
public:
	static const int NAME_MAXLENGTH = 200;
	static const int MAX_DIMS = 12;

	static const int VERTEX_SOLUTION_ID   = 1;
	static const int CELL_SOLUTION_ID     = 2;
	static const int IFACE_2D_SOLUTION_ID = 3;
	static const int JFACE_2D_SOLUTION_ID = 4;

	static const std::string IRICZONE;
	static const std::string BINAME;
	static const std::string RDNODE;

	Impl();
	~Impl();

	int initBaseId(bool clearResults, const char* baseName = NULL, bool skipInitZone = false);
	int initZoneId(bool clearResults);

	void optionDivideSolutions();

	int Flush();

	int clearResultData();
	int clearBaseIter(); // local_init_biter_Mul
	int clearZoneIter(); // local_init_ziter_Mul
	int clearSolution();
	int clearParticleSolution();
	int clearResultGrids();

	int loadResultData(); // local_load_sol_Mul
	int loadBcNames(); // local_init_bc_names_Mul

	int gotoBase();
	int gotoBaseIter();

	int gotoCCBase();
	int gotoCCBaseChild(const char* path); // local_gotoccb_Mul
	int gotoCC(); // local_gotocc_Mul
	int gotoCCChild(const char* path); // local_gotoccchild_Mul
	int gotoCCChildCreateIfNotExist(const char* path); // local_gotoccchild_create_Mul
	int gotoGeoData(); // local_gotord_Mul

	int gotoCCBaseIter();

	int gotoZone();
	int gotoZoneChild(const char* path); // local_gotoz_Mul
	int gotoGridCondition(); // local_gotogc_Mul
	int gotoGridConditionChild(const char* path); // local_gotogcchild_Mul
	int gotoGridConditionNewChild(const char* path); // local_gotogcnewchild_Mul
	int addGridConditionNodeIfNotExist(); // local_gc_add_gcnode_Mul

	cgsize_t gridNodeValueCount();
	cgsize_t gridCellValueCount();

	int gotoZoneIter();

	int gotoComplexGroup(const char* groupName); // local_gotocomplexgroup_Mul
	int gotoComplex(const char* groupName, int num); // local_gotocomplex_Mul
	int gotoComplexChild(const char* groupName, int num, const char* name); //local_gotocomplexchild_Mul
	int gotoComplexChildCreateIfNotExist(const char* groupName, int num, const char* name); // local_gotocomplexchild_create_Mul
	int addComplexNodeIfNotExist(); // local_complex_add_gccnode_Mul

	void getBcIndex(const char* typeName, int num, int* BC);
	int gotoBc(const char* typeName, int num); // local_gotobc_Mul
	int gotoBcChild(const char* typeName, int num, const char* name); // local_gotobcchild_Mul
	int gotoBcChildCreateIfNotExist(const char* typeName, int num, const char* name); // local_gotobcchild_create_Mul

	static int findArray(const char* name, int* index, DataType_t* dt, int* dim, cgsize_t* dimVec); //local_find_array
	static int readArray(const char* name, DataType_t dt, cgsize_t len, void* memory); // local_read_array
	static int readArrayAs(const char* name, DataType_t dt, size_t length, void* memory); // local_read_array_as
	static int readStringLen(const char* name, int* length); // local_read_string_len
	static int readString(const char* name, size_t bufferLen, char* buffer); // local_read_string

	static int writeArray(const char* name, DataType_t dt, size_t length, const void* memory); //local_write_array
	static int writeString(const char* name, const char* value); // local_write_string

	static void getComplexName(int num, char *name); // local_get_complex_name
	static void getDimensionArrayName(const char* dimName, char* name);
	static void getFunctionalDataName(int num, char* name); // local_get_gridfunctionaldata_name
	static void getBcName(const char* typeName, int num, char* name); // local_get_bc_name
	static void getSolName(int num, char* name);
	static void getCellSolName(int num, char* name);
	static void getIFaceSolName(int num, char* name);
	static void getJFaceSolName(int num, char* name);

	static void getSolGridCoordName(int num, char* name);
	static void getParticleSolName(int num, char* name);
	static void getParticleGroupSolName(int num, char* name);
	static void getPolydataSolName(int num, char* name);

	static int addSolutionNode(int fid, int bid, int zid, int sid, std::vector<std::string>* sols, std::vector<std::string>* cellsols, std::vector<std::string>* ifacesols, std::vector<std::string>* jfacesols);
	static int addSolutionGridCoordNode(int fid, int bid, int zid, int sid, std::vector<std::string>* coords);
	static int addParticleSolutionNode(int fid, int bid, int zid, int sid);
	static int addParticleGroupSolutionNode(int fid, int bid, int zid, int sid);
	static int addPolydataSolutionNode(int fid, int bid, int zid, int sid);

	static int writePointers(int fid, int bid, int zid, const char* name, const std::vector<std::string>& strs);
	static int writeFlowSolutionPointers(int fid, int bid, int zid, const std::vector<std::string>& sols);
	static int writeFlowCellSolutionPointers(int fid, int bid, int zid, const std::vector<std::string>& sols);
	static int writeFlowIFaceSolutionPointers(int fid, int bid, int zid, const std::vector<std::string>& sols);
	static int writeFlowJFaceSolutionPointers(int fid, int bid, int zid, const std::vector<std::string>& sols);
	static int writeParticleSolutionPointers(int fid, int bid, int zid, const std::vector<std::string>& sols);
	static int writeParticleGroupSolutionPointers(int fid, int bid, int zid, const std::vector<std::string>& sols);
	static int writePolydataSolutionPointers(int fid, int bid, int zid, const std::vector<std::string>& sols);
	static int writeGridCoordinatesPointers(int fid, int bid, int zid, const std::vector<std::string>& coords);

	int solIndex(CGNS_ENUMT(GridLocation_t) location, int step);

	std::string m_fileName;
	int m_fileId = 0;

	int m_baseId = 0;
	int m_ccBaseId = 0;
	int m_baseCellDim = 0;
	int m_basePhysDim = 0;

	int m_zoneId;
	cgsize_t m_zoneSize[9];

	std::vector<std::string> m_bcNames;

	int m_solId = 0;
	bool m_hasCellSols = true;
	bool m_hasFaceSols = true;
	std::vector<double> m_solTimes;
	std::vector<int> m_solIndices;
	std::vector<std::string> m_solGridCoordPointers;
	std::vector<std::string> m_solPointers;
	std::vector<std::string> m_cellSolPointers;
	std::vector<std::string> m_ifaceSolPointers;
	std::vector<std::string> m_jfaceSolPointers;

	std::vector<BaseIterativeT<int> > m_solBaseIterInts;
	std::vector<BaseIterativeT<double> > m_solBaseIterReals;
	std::vector<BaseIterativeT<std::string> > m_solBaseIterStrings;

	SolutionWriter* m_solutionWriter;

	std::string m_solLinkedFileName;
	int m_solLinkFileId;
};

} // iRICLib

#endif // IRICLIB_CGNSFILE_IMPL_H
