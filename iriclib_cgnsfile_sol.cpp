#include "error_macros.h"
#include "iriclib_cgnsfile.h"
#include "private/iriclib_cgnsfile_impl.h"
#include "private/iriclib_cgnsfile_baseiterativet.h"
#include "private/iriclib_cgnsfile_solutionwriter.h"

#include <assert.h>

#include <cstring>

using namespace iRICLib;

namespace {

static const std::string ECNODE = "ErrorCode";

void setupStringBuffer(const std::vector<std::string>& vals, cgsize_t* dims, std::vector<char>* buffer)
{
	cgsize_t maxLen = 0;
	for (int i = 0; i < vals.size(); ++i) {
		const std::string& s = vals.at(i);
		if (i == 0 || s.length() > maxLen) {
			maxLen = s.length();
		}
	}
	*(dims + 0) = maxLen;
	*(dims + 1) = static_cast<cgsize_t> (vals.size());
	buffer->assign(maxLen * vals.size(), ' ');
	for (int i = 0; i < vals.size(); ++i) {
		const std::string& s = vals.at(i);
		memcpy(buffer->data() + maxLen * i, s.c_str(), s.length());
	}
}

} // namespace

int CgnsFile::Sol_Read_Count(int* count)
{
	if (impl->m_hasCellSols) {
		if (impl->m_hasFaceSols) {
			assert((impl->m_solId % 4) == 0);
			*count = impl->m_solId / 4;
		}
		else {
			assert((impl->m_solId % 2) == 0);
			*count = impl->m_solId / 2;
		}
	}
	else {
		*count = impl->m_solId;
	}
	return 0;
}

int CgnsFile::Sol_Read_Time(int step, double* time)
{
	if (step > impl->m_solId) {
		return 1;
	}
	*time = impl->m_solTimes.at(step - 1);
	return 0;
}

int CgnsFile::Sol_Read_Iteration(int step, int* index)
{
	if (step > impl->m_solId) {
		return 1;
	}
	*index = impl->m_solIndices.at(step - 1);
	return 0;
}

int CgnsFile::Sol_Read_BaseIterative_Integer(int step, const char *name, int* value)
{
	if (step > impl->m_solId) {
		return 1;
	}
	for (const BaseIterativeT<int>& bit : impl->m_solBaseIterInts) {
		if (bit.name() == name) {
			*value = bit.values().at(step - 1);
			return 0;
		}
	}
	return 2;
}

int CgnsFile::Sol_Read_BaseIterative_Real(int step, const char *name, double* value)
{
	if (step > impl->m_solId) {
		return 1;
	}
	for (const BaseIterativeT<double>& bit : impl->m_solBaseIterReals) {
		if (bit.name() == name) {
			*value = bit.values().at(step - 1);
			return 0;
		}
	}
	return 2;
}

int CgnsFile::Sol_Read_BaseIterative_StringLen(int step, const char* name, int* length)
{
	if (step > impl->m_solId) {
		return 1;
	}
	for (const BaseIterativeT<std::string>& bit : impl->m_solBaseIterStrings) {
		if (bit.name() == name) {
			const std::string& v = bit.values().at(step - 1);
			*length = v.length();
			return 0;
		}
	}
	return 2;
}

int CgnsFile::Sol_Read_BaseIterative_String(int step, const char* name, char* strvalue)
{
	if (step > impl->m_solId) {
		return 1;
	}
	for (const BaseIterativeT<std::string>& bit : impl->m_solBaseIterStrings) {
		if (bit.name() == name) {
			const std::string& v = bit.values().at(step - 1);
			std::strcpy(strvalue, v.c_str());
			return 0;
		}
	}
	return 2;
}

int CgnsFile::Sol_Read_GridCoord2d(int step, double* x, double* y)
{
	int ier = cg_goto(impl->m_fileId, impl->m_baseId, "Zone_t", impl->m_zoneId,
										"GridCoordinates_t", step + 1, NULL);
	RETURN_IF_ERR;

	ier = Impl::readArray("CoordinateX", RealDouble, -1, x);
	RETURN_IF_ERR;
	ier = Impl::readArray("CoordinateY", RealDouble, -1, y);
	RETURN_IF_ERR;
	return 0;
}

int CgnsFile::Sol_Read_GridCoord3d(int step, double* x, double* y, double* z)
{
	int ier = cg_goto(impl->m_fileId, impl->m_baseId, "Zone_t", impl->m_zoneId,
										"GridCoordinates_t", step + 1, NULL);
	RETURN_IF_ERR;

	ier = Impl::readArray("CoordinateX", RealDouble, -1, x);
	RETURN_IF_ERR;
	ier = Impl::readArray("CoordinateY", RealDouble, -1, y);
	RETURN_IF_ERR;
	ier = Impl::readArray("CoordinateZ", RealDouble, -1, z);
	RETURN_IF_ERR;
	return 0;
}

int CgnsFile::Sol_Read_Integer(int step, const char *name, int* data)
{
	int idx = impl->solIndex(Vertex, step);
	int ier = cg_goto(impl->m_fileId, impl->m_baseId, "Zone_t", impl->m_zoneId,
										"FlowSolution_t", idx, NULL);
	RETURN_IF_ERR;

	return Impl::readArray(name, Integer, -1, data);
}

int CgnsFile::Sol_Read_Cell_Integer(int step, const char *name, int* data)
{
	int idx = impl->solIndex(CellCenter, step);
	int ier = cg_goto(impl->m_fileId, impl->m_baseId, "Zone_t", impl->m_zoneId,
										"FlowSolution_t", idx, NULL);
	RETURN_IF_ERR;

	return Impl::readArray(name, Integer, -1, data);
}

int CgnsFile::Sol_Read_IFace_Integer(int step, const char *name, int* data)
{
	int idx = impl->solIndex(IFaceCenter, step);
	int ier = cg_goto(impl->m_fileId, impl->m_baseId, "Zone_t", impl->m_zoneId,
		"FlowSolution_t", idx, NULL);
	RETURN_IF_ERR;

	return Impl::readArray(name, Integer, -1, data);
}

int CgnsFile::Sol_Read_JFace_Integer(int step, const char *name, int* data)
{
	int idx = impl->solIndex(JFaceCenter, step);
	int ier = cg_goto(impl->m_fileId, impl->m_baseId, "Zone_t", impl->m_zoneId,
		"FlowSolution_t", idx, NULL);
	RETURN_IF_ERR;

	return Impl::readArray(name, Integer, -1, data);
}

int CgnsFile::Sol_Read_Real(int step, const char *name, double* data)
{
	int idx = impl->solIndex(Vertex, step);
	int ier = cg_goto(impl->m_fileId, impl->m_baseId, "Zone_t", impl->m_zoneId,
		"FlowSolution_t", idx, NULL);
	RETURN_IF_ERR;

	return Impl::readArray(name, RealDouble, -1, data);
}

int CgnsFile::Sol_Read_Cell_Real(int step, const char *name, double* data)
{
	int idx = impl->solIndex(CellCenter, step);
	int ier = cg_goto(impl->m_fileId, impl->m_baseId, "Zone_t", impl->m_zoneId,
		"FlowSolution_t", idx, NULL);
	RETURN_IF_ERR;

	return Impl::readArray(name, RealDouble, -1, data);
}

int CgnsFile::Sol_Read_IFace_Real(int step, const char *name, double* data)
{
	int idx = impl->solIndex(IFaceCenter, step);
	int ier = cg_goto(impl->m_fileId, impl->m_baseId, "Zone_t", impl->m_zoneId,
		"FlowSolution_t", idx, NULL);
	RETURN_IF_ERR;

	return Impl::readArray(name, RealDouble, -1, data);
}

int CgnsFile::Sol_Read_JFace_Real(int step, const char *name, double* data)
{
	int idx = impl->solIndex(JFaceCenter, step);
	int ier = cg_goto(impl->m_fileId, impl->m_baseId, "Zone_t", impl->m_zoneId,
		"FlowSolution_t", idx, NULL);
	RETURN_IF_ERR;

	return Impl::readArray(name, RealDouble, -1, data);
}

int CgnsFile::Sol_Write_Time(double time)
{
	return impl->m_solutionWriter->Sol_Write_Time(time);
}

int CgnsFile::Sol_Write_Iteration(int index)
{
	return impl->m_solutionWriter->Sol_Write_Iteration(index);
}

int CgnsFile::Sol_Write_BaseIterative_Integer(const char *name, int value)
{
	bool found = false;
	BaseIterativeT<int> data(name);
	for (BaseIterativeT<int>& bit : impl->m_solBaseIterInts) {
		if (bit.name() == name) {
			bit.addValue(value);
			data = bit;
			found = true;
		}
	}
	if (! found) {
		BaseIterativeT<int> newData(name);
		newData.addValue(value);
		impl->m_solBaseIterInts.push_back(newData);
		data = newData;
	}
	// write the value.
	cgsize_t dimVec = static_cast<cgsize_t> (data.values().size());
	impl->gotoBaseIter();
	return cg_array_write(data.name().c_str(), Integer, 1, &dimVec, data.values().data());
}

int CgnsFile::Sol_Write_BaseIterative_Real(const char *name, double value)
{
	bool found = false;
	BaseIterativeT<double> data(name);
	for (BaseIterativeT<double>& bit : impl->m_solBaseIterReals) {
		if (bit.name() == name) {
			bit.addValue(value);
			data = bit;
			found = true;
		}
	}
	if (! found) {
		BaseIterativeT<double> newData(name);
		newData.addValue(value);
		impl->m_solBaseIterReals.push_back(newData);
		data = newData;
	}
	// write the value.
	cgsize_t dimVec = static_cast<cgsize_t> (data.values().size());
	impl->gotoBaseIter();
	return cg_array_write(data.name().c_str(), RealDouble, 1, &dimVec, data.values().data());
}

int CgnsFile::Sol_Write_BaseIterative_String(const char* name, const char* value)
{
	bool found = false;
	BaseIterativeT<std::string> data(name);
	for (BaseIterativeT<std::string>& bit : impl->m_solBaseIterStrings) {
		if (bit.name() == name) {
			bit.addValue(std::string(value));
			data = bit;
			found = true;
		}
	}
	if (! found) {
		BaseIterativeT<std::string> newData(name);
		newData.addValue(std::string(value));
		impl->m_solBaseIterStrings.push_back(newData);
		data = newData;
	}
	// write the value.
	cgsize_t dimVec[2];
	std::vector<char> buffer;
	setupStringBuffer(data.values(), dimVec, &buffer);
	impl->gotoBaseIter();
	return cg_array_write(data.name().c_str(), Character, 2, dimVec, buffer.data());
}

int CgnsFile::Sol_Write_GridCoord2d(double *x, double *y)
{
	return impl->m_solutionWriter->Sol_Write_GridCoord2d(x, y);
}

int CgnsFile::Sol_Write_GridCoord3d(double *x, double *y, double *z)
{
	return impl->m_solutionWriter->Sol_Write_GridCoord3d(x, y, z);
}

int CgnsFile::Sol_Write_Integer(const char *name, int* data)
{
	return impl->m_solutionWriter->Sol_Write_Integer(name, data);
}

int CgnsFile::Sol_Write_Cell_Integer(const char *name, int* data)
{
	return impl->m_solutionWriter->Sol_Write_Cell_Integer(name, data);
}

int CgnsFile::Sol_Write_IFace_Integer(const char *name, int* data)
{
	return impl->m_solutionWriter->Sol_Write_IFace_Integer(name, data);
}

int CgnsFile::Sol_Write_JFace_Integer(const char *name, int* data)
{
	return impl->m_solutionWriter->Sol_Write_JFace_Integer(name, data);
}

int CgnsFile::Sol_Write_Real(const char *name, double* data)
{
	return impl->m_solutionWriter->Sol_Write_Real(name, data);
}

int CgnsFile::Sol_Write_Cell_Real(const char *name, double* data)
{
	return impl->m_solutionWriter->Sol_Write_Cell_Real(name, data);
}

int CgnsFile::Sol_Write_IFace_Real(const char *name, double* data)
{
	return impl->m_solutionWriter->Sol_Write_IFace_Real(name, data);
}

int CgnsFile::Sol_Write_JFace_Real(const char *name, double* data)
{
	return impl->m_solutionWriter->Sol_Write_JFace_Real(name, data);
}

int CgnsFile::ErrorCode_Write(int errorcode)
{
	int ier = impl->gotoBase();
	RETURN_IF_ERR;

	cg_delete_node(ECNODE.c_str());
	ier = cg_user_data_write(ECNODE.c_str());
	RETURN_IF_ERR;
	ier = cg_goto(impl->m_fileId, impl->m_baseId, ECNODE.c_str(), 0, NULL);
	RETURN_IF_ERR;
	return Impl::writeArray("Value", Integer, 1, &errorcode);
}

int CgnsFile::Sol_Particle_Write_Pos2d(cgsize_t count, double* x, double* y)
{
	return impl->m_solutionWriter->Sol_Particle_Write_Pos2d(count, x, y);
}

int CgnsFile::Sol_Particle_Write_Pos3d(cgsize_t count, double* x, double* y, double* z)
{
	return impl->m_solutionWriter->Sol_Particle_Write_Pos3d(count, x, y, z);
}

int CgnsFile::Sol_Particle_Write_Real(const char* name, double* value)
{
	return impl->m_solutionWriter->Sol_Particle_Write_Real(name, value);
}

int CgnsFile::Sol_Particle_Write_Integer(const char* name, int* value)
{
	return impl->m_solutionWriter->Sol_Particle_Write_Integer(name, value);
}

int CgnsFile::Sol_ParticleGroup_Write_GroupBegin(const char* name)
{
	return impl->m_solutionWriter->Sol_ParticleGroup_Write_GroupBegin(name);
}

int CgnsFile::Sol_ParticleGroup_Write_GroupEnd()
{
	return impl->m_solutionWriter->Sol_ParticleGroup_Write_GroupEnd();
}

int CgnsFile::Sol_ParticleGroup_Write_Pos2d(double x, double y)
{
	return impl->m_solutionWriter->Sol_ParticleGroup_Write_Pos2d(x, y);
}

int CgnsFile::Sol_ParticleGroup_Write_Pos3d(double x, double y, double z)
{
	return impl->m_solutionWriter->Sol_ParticleGroup_Write_Pos3d(x, y, z);
}

int CgnsFile::Sol_ParticleGroup_Write_Integer(const char* name, int value)
{
	return impl->m_solutionWriter->Sol_ParticleGroup_Write_Integer(name, value);
}

int CgnsFile::Sol_ParticleGroup_Write_Real(const char* name, double value)
{
	return impl->m_solutionWriter->Sol_ParticleGroup_Write_Real(name, value);
}

int CgnsFile::Sol_PolyData_Write_GroupBegin(const char* name)
{
	return impl->m_solutionWriter->Sol_PolyData_Write_GroupBegin(name);
}

int CgnsFile::Sol_PolyData_Write_GroupEnd()
{
	return impl->m_solutionWriter->Sol_PolyData_Write_GroupEnd();
}

int CgnsFile::Sol_PolyData_Write_Polygon(int numPoints, double* x, double* y)
{
	return impl->m_solutionWriter->Sol_PolyData_Write_Polygon(numPoints, x, y);
}

int CgnsFile::Sol_PolyData_Write_Polyline(int numPoints, double* x, double* y)
{
	return impl->m_solutionWriter->Sol_PolyData_Write_Polyline(numPoints, x, y);
}

int CgnsFile::Sol_PolyData_Write_Integer(const char* name, int value)
{
	return impl->m_solutionWriter->Sol_PolyData_Write_Integer(name, value);
}

int CgnsFile::Sol_PolyData_Write_Real(const char* name, double value)
{
	return impl->m_solutionWriter->Sol_PolyData_Write_Real(name, value);
}
