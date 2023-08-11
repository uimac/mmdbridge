#ifdef WITH_ALEMBIC

#include "d3d9.h"
#include "d3dx9.h"

#include "bridge_parameter.h"
#include "UMStringUtil.h"
#include "UMPath.h"
#include "UMMath.h"
#include "UMVector.h"


#include <pybind11/pybind11.h>
namespace py = pybind11;


#include <Alembic/Abc/All.h>
#include <Alembic/AbcGeom/All.h>
#include <Alembic/AbcCoreHDF5/All.h>
#include <Alembic/AbcCoreOgawa/All.h>


namespace AbcA = Alembic::AbcCoreAbstract;

template <class T> std::string to_string(T value)
{
	return umbase::UMStringUtil::number_to_string(value);
}

// 行列で3Dベクトルをトランスフォームする
// D3DXVec3Transformとほぼ同じ
static void d3d_vector3_dir_transform(
	D3DXVECTOR3 &dst, 
	const D3DXVECTOR3 &src, 
	const D3DXMATRIX &matrix)
{
	const float tmp[] = {
		src.x*matrix.m[0][0] + src.y*matrix.m[1][0] + src.z*matrix.m[2][0],
		src.x*matrix.m[0][1] + src.y*matrix.m[1][1] + src.z*matrix.m[2][1],
		src.x*matrix.m[0][2] + src.y*matrix.m[1][2] + src.z*matrix.m[2][2]
	};
	dst.x = tmp[0];
	dst.y = tmp[1];
	dst.z = tmp[2];
}

static void d3d_vector3_transform(
	D3DXVECTOR3 &dst, 
	const D3DXVECTOR3 &src, 
	const D3DXMATRIX &matrix)
{
	const float tmp[] = {
		src.x*matrix.m[0][0] + src.y*matrix.m[1][0] + src.z*matrix.m[2][0] + 1.0f*matrix.m[3][0],
		src.x*matrix.m[0][1] + src.y*matrix.m[1][1] + src.z*matrix.m[2][1] + 1.0f*matrix.m[3][1],
		src.x*matrix.m[0][2] + src.y*matrix.m[1][2] + src.z*matrix.m[2][2] + 1.0f*matrix.m[3][2]
	};
	dst.x = tmp[0];
	dst.y = tmp[1];
	dst.z = tmp[2];
}

class AlembicArchive {
public:
	
	static AlembicArchive& instance() { 
		static AlembicArchive instance;
		return instance; 
	}

	Alembic::Abc::OArchive * archive;
	AbcA::uint32_t timeindex;
	AbcA::TimeSamplingPtr timesampling;
	
	typedef std::map<int, Alembic::AbcGeom::OXform > XformMap;
	XformMap xform_map;

	typedef std::map<int, Alembic::AbcGeom::OXformSchema > XformSchemaMap;
	XformSchemaMap xform_schema_map;

	typedef std::map<int, Alembic::AbcGeom::OPolyMesh > MeshMap;
	MeshMap mesh_map;

	typedef std::map<int, Alembic::AbcGeom::OPolyMeshSchema > SchemaMap;
	SchemaMap schema_map;
	
	typedef std::map<int, int > SurfaceSizeMap;
	SurfaceSizeMap surface_size_map;
	
	typedef std::map<int, Alembic::AbcGeom::OCamera > CameraMap;
	CameraMap camera_map;
	
	typedef std::map<int, Alembic::AbcGeom::OCameraSchema  > CameraSchemaMap;
	CameraSchemaMap camera_schema_map;

	typedef std::map<int, int> FaceToVertexIndex;
	typedef std::map<int, FaceToVertexIndex> FaceToVertexIndexMap;
	FaceToVertexIndexMap face_to_vertex_index_map;

	typedef std::map<int, int> TriViToVertexIndex;
	typedef std::map<int, TriViToVertexIndex> TriViToVertexIndexMap;
	TriViToVertexIndexMap trivi_to_vertex_index_map;

	typedef std::map<int, int> QuadViToCountMap;
	QuadViToCountMap quadvi_count_map;

	bool is_export_normals;
	bool is_export_uvs;
	int export_mode;

	bool is_use_euler_rotation_camera;
	
	RenderedBuffer::UVList temporary_uv_list;
	RenderedBuffer::NormalList temporary_normal_list;
	RenderedBuffer::VertexList temporary_vertex_list;

	void end() { 
		xform_map.clear();
		xform_schema_map.clear();
		mesh_map.clear();
		schema_map.clear();
		camera_map.clear();
		camera_schema_map.clear();
		face_to_vertex_index_map.clear();
		trivi_to_vertex_index_map.clear();
		quadvi_count_map.clear();
		surface_size_map.clear();
		temporary_uv_list.clear();
		temporary_normal_list.clear();
		temporary_vertex_list.clear();
		{
			delete archive; archive = NULL;
		}
	}
private:
	AlembicArchive() : archive(NULL), timeindex(0), export_mode(0), is_use_euler_rotation_camera(false) {}
};

static bool start_alembic_export(
	const std::string& filepath,
	int export_mode, 
	bool isExportNomals,
	bool is_export_uvs,
	bool is_use_euler_rotation_camera,
	bool is_use_ogawa)
{
	const BridgeParameter& parameter = BridgeParameter::instance();
	if (parameter.export_fps <= 0)
	{
		return false;
	}

	if (!AlembicArchive::instance().archive)
	{
		std::string output_path(filepath);
		if (output_path.empty()) 
		{
			output_path = umbase::UMStringUtil::wstring_to_utf8(parameter.base_path) + ("out\\alembic_file.abc");
		}
		if (is_use_ogawa) {
			AlembicArchive::instance().archive =
				new Alembic::Abc::OArchive(Alembic::AbcCoreOgawa::WriteArchive(),
				output_path.c_str());
		}
		else
		{
			AlembicArchive::instance().archive =
				new Alembic::Abc::OArchive(Alembic::AbcCoreHDF5::WriteArchive(),
				output_path.c_str());
		}

		AlembicArchive &archive = AlembicArchive::instance();
		const double dt = 1.0 / parameter.export_fps;
		archive.timesampling = AbcA::TimeSamplingPtr(new AbcA::TimeSampling(dt, 0.0));
		archive.archive->addTimeSampling(*archive.timesampling);
		archive.is_export_normals = (isExportNomals != 0);
		archive.is_export_uvs = (is_export_uvs != 0);
		archive.is_use_euler_rotation_camera = (is_use_euler_rotation_camera != 0);
		archive.export_mode = export_mode;
		return true;
	}
	return false;
}

static bool end_alembic_export()
{
	if (AlembicArchive::instance().archive)
	{
		AlembicArchive::instance().end();
		return true;
	}
		
	return false;
}

static void convertToQuad(
	std::map<int, int>& viToViMap,
	std::vector<Alembic::Util::int32_t>& faceList,
	std::vector<Alembic::Util::int32_t>& faceCountList,
	const RenderedBuffer::VertexList& vertexListByMaterial)
{
	/*
	struct Triangle {
		int triIndex;
		Triangle *next;
	};
	std::vector<Triangle> triList(faceCountList.size());
	std::vector<Triangle*> triHeadList(faceCountList.size());
	int vnumber = 0;
	for (int i = 0, isize = static_cast<int>(faceCountList.size()); i < isize; ++i)
	{
		for (int k = 0; k < 3; ++k) {
			const int vnumber = i * 3 + k;
			const int vi = faceList[vnumber];
			triList[vnumber].triIndex = vnumber;
			triList[vnumber].next = triHeadList[vi];
			triHeadList[vi] = &triList[vnumber];
		}
	}
	*/

	struct Edge {
		int vertexIndex[2];
		int triangleIndex[2];
		int edgeNumber[2];
		Edge *next;
	};
	std::vector<Edge> edgeList(3 * faceCountList.size());
	std::vector<Edge*> edgeHeadList(3 * faceCountList.size());
	for (int i = 0, size = (3 * faceCountList.size()); i < size; ++i) {
		edgeHeadList[i] = NULL;
		edgeList[i].vertexIndex[0] = -1;
		edgeList[i].vertexIndex[1] = -1;
		edgeList[i].triangleIndex[0] = -1;
		edgeList[i].triangleIndex[1] = -1;
		edgeList[i].edgeNumber[0] = -1;
		edgeList[i].edgeNumber[1] = -1;
		edgeList[i].next = NULL;
	}
	std::map<std::pair<int, int>, int> edgeHashMap;
	int count = 0;
	for (int i = 0, isize = static_cast<int>(faceCountList.size()); i < isize; ++i)
	{
		for (int k = 0, j = 2; k < 2; j = k, ++k) {
			int vj = faceList[i * 3 + j];
			int vk = faceList[i * 3 + k];
			if (vj > vk) {
				int temp = vj;
				vj = vk;
				vk = temp;
			}
			std::pair<int, int> indexPair(vj, vk);
			if (edgeHashMap.find(indexPair) == edgeHashMap.end()) {
				edgeHashMap[indexPair] = edgeHashMap.size();
			}
			int hash = edgeHashMap[indexPair];
			for (Edge *edge = edgeHeadList[hash]; ; edge = edge->next) {
				if (edge == NULL) {
					edgeList[count].vertexIndex[0] = vj;
					edgeList[count].vertexIndex[1] = vk;
					edgeList[count].triangleIndex[0] = i;
					edgeList[count].edgeNumber[0] = j;
					edgeList[count].next = edgeHeadList[hash];
					edgeHeadList[hash] = &edgeList[count++];
					break;
				} else if (edge->vertexIndex[0] == vj && edge->vertexIndex[1] == vk) {
					edge->triangleIndex[1] = i;
					edge->edgeNumber[1] = j;
					break;
				}
			}
		}
	}
	
	std::vector<UMVec3f> faceNormalList(faceCountList.size());
	for (int i = 0, isize = static_cast<int>(faceCountList.size()); i < isize; ++i)
	{
		const D3DXVECTOR3& dxv1 = vertexListByMaterial[faceList[i * 3 + 0]];
		const D3DXVECTOR3& dxv2 = vertexListByMaterial[faceList[i * 3 + 1]];
		const D3DXVECTOR3& dxv3 = vertexListByMaterial[faceList[i * 3 + 2]];
		const UMVec3f v1(dxv1[0], dxv1[1], dxv1[2]);
		const UMVec3f v2(dxv2[0], dxv2[1], dxv2[2]);
		const UMVec3f v3(dxv3[0], dxv3[1], dxv3[2]);
		faceNormalList[i] = ((v2 - v1).cross(v3 - v1)).normalized();
	}

	std::vector<Alembic::Util::int32_t> quadFaceList;
	std::vector<Alembic::Util::int32_t> quadFaceCountList;
	std::map<int, int> exportedFaces;
	for (int i = 0, isize = 3 * faceCountList.size(); i < isize; ++i) {
		Edge* edge = edgeHeadList[i];
		if (edge) {
			const int t1 = edge->triangleIndex[0];
			const int t2 = edge->triangleIndex[1];
			if (t1 >= 0 && exportedFaces.find(t1) == exportedFaces.end())
			{
				const UMVec3i tri1(faceList[t1 * 3 + 0], faceList[t1 * 3 + 1], faceList[t1 * 3 + 2]);
				if (t2 >= 0 && exportedFaces.find(t2) == exportedFaces.end() && faceNormalList[t1].dot(faceNormalList[t2]) > 0.9) {
					const UMVec3i tri2(faceList[t2 * 3 + 0], faceList[t2 * 3 + 1], faceList[t2 * 3 + 2]);

					bool share[3] = { false, false, false };
					for (int k = 0; k < 3; ++k) {
						for (int n = 0; n < 3; ++n) {
							if (tri1[k] == tri2[n]) {
								share[k] = true;
							}
						}
					}
					int forthV = -1;
					for (int k = 0; k < 3; ++k) {
						if (tri2[k] != tri1[0] &&
							tri2[k] != tri1[1] &&
							tri2[k] != tri1[2])
						{
							forthV = k;
							break;
						}
					}
					if (share[0] && share[2]) {
						// 0, 1, 2, V
						viToViMap[t1 * 3 + 0] = quadFaceList.size();
						quadFaceList.push_back(tri1[0]);
						viToViMap[t1 * 3 + 1] = quadFaceList.size();
						quadFaceList.push_back(tri1[1]);
						viToViMap[t1 * 3 + 2] = quadFaceList.size();
						quadFaceList.push_back(tri1[2]);
						viToViMap[t2 * 3 + forthV] = quadFaceList.size();
						quadFaceList.push_back(tri2[forthV]);
					}
					else if (share[1] && share[2]) {
						// 0, 1, V, 2
						viToViMap[t1 * 3 + 0] = quadFaceList.size();
						quadFaceList.push_back(tri1[0]);
						viToViMap[t1 * 3 + 1] = quadFaceList.size();
						quadFaceList.push_back(tri1[1]);
						viToViMap[t2 * 3 + forthV] = quadFaceList.size();
						quadFaceList.push_back(tri2[forthV]);
						viToViMap[t1 * 3 + 2] = quadFaceList.size();
						quadFaceList.push_back(tri1[2]);
					}
					else if (share[0] && share[1]) {
						// 0, V, 1, 2
						viToViMap[t1 * 3 + 0] = quadFaceList.size();
						quadFaceList.push_back(tri1[0]);
						viToViMap[t2 * 3 + forthV] = quadFaceList.size();
						quadFaceList.push_back(tri2[forthV]);
						viToViMap[t1 * 3 + 1] = quadFaceList.size();
						quadFaceList.push_back(tri1[1]);
						viToViMap[t1 * 3 + 2] = quadFaceList.size();
						quadFaceList.push_back(tri1[2]);
					}
					
					quadFaceCountList.push_back(4);
					exportedFaces[t1] = 1;
					exportedFaces[t2] = 1;
				}
			}
		}
	}

	for (int i = 0, isize = 3 * faceCountList.size(); i < isize; ++i) {
		Edge* edge = edgeHeadList[i];
		if (edge) {
			const int t1 = edge->triangleIndex[0];
			const int t2 = edge->triangleIndex[1];
			if (t1 >= 0 && exportedFaces.find(t1) == exportedFaces.end())
			{
				const UMVec3i tri1(faceList[t1 * 3 + 0], faceList[t1 * 3 + 1], faceList[t1 * 3 + 2]);
				viToViMap[t1 * 3 + 0] = quadFaceList.size();
				quadFaceList.push_back(tri1[0]);
				viToViMap[t1 * 3 + 1] = quadFaceList.size();
				quadFaceList.push_back(tri1[1]);
				viToViMap[t1 * 3 + 2] = quadFaceList.size();
				quadFaceList.push_back(tri1[2]);
				quadFaceCountList.push_back(3);
				exportedFaces[t1] = 1;
			}
			if (t2 >= 0 && exportedFaces.find(t2) == exportedFaces.end())
			{
				const UMVec3i tri2(faceList[t2 * 3 + 0], faceList[t2 * 3 + 1], faceList[t2 * 3 + 2]);
				viToViMap[t2 * 3 + 0] = quadFaceList.size();
				quadFaceList.push_back(tri2[0]);
				viToViMap[t2 * 3 + 1] = quadFaceList.size();
				quadFaceList.push_back(tri2[1]);
				viToViMap[t2 * 3 + 2] = quadFaceList.size();
				quadFaceList.push_back(tri2[2]);
				quadFaceCountList.push_back(3);
				exportedFaces[t2] = 1;
			}
		}
	}
	quadFaceCountList.swap(faceCountList);
	quadFaceList.swap(faceList);
}

static void export_alembic_xform_by_material_fix_vindex(AlembicArchive &archive, const RenderedBuffer & renderedBuffer, int renderedBufferIndex, bool isConvertToQuad)
{
	Alembic::AbcGeom::OObject topObj(*archive.archive, Alembic::AbcGeom::kTop);

	for (int k = 0, ksize = static_cast<int>(renderedBuffer.materials.size()); k < ksize; ++k)
	{
		Alembic::AbcGeom::OPolyMesh polyMesh;
		const int key = renderedBufferIndex * 10000 + k;
				
		Alembic::AbcGeom::OXform xform;
		if (archive.xform_map.find(key) != archive.xform_map.end())
		{
			xform = archive.xform_map[key];
		}
		else
		{
			xform = Alembic::AbcGeom::OXform(topObj, "xform_" + to_string(renderedBufferIndex) + "_material_" + to_string(k) , archive.timesampling);
			archive.xform_map[key] = xform;
		}

		bool isFirstMesh = false;
		if (archive.mesh_map.find(key) != archive.mesh_map.end())
		{
			polyMesh = archive.mesh_map[key];
		}
		else
		{
			polyMesh = Alembic::AbcGeom::OPolyMesh(xform, "mesh_" + to_string(renderedBufferIndex) + "_material_" + to_string(k), archive.timesampling);
			archive.mesh_map[key] = polyMesh;
			isFirstMesh = true;

			Alembic::AbcGeom::OPolyMeshSchema &meshSchema = polyMesh.getSchema();
			archive.schema_map[key] = meshSchema;
		}

		if (archive.surface_size_map.find(key) == archive.surface_size_map.end())
		{
			archive.surface_size_map[key] = 0;
		}

		if (archive.face_to_vertex_index_map.find(key) == archive.face_to_vertex_index_map.end())
		{
			AlembicArchive::FaceToVertexIndex fiToVi;
			archive.face_to_vertex_index_map[key] = fiToVi;
		}

		if (archive.trivi_to_vertex_index_map.find(key) == archive.trivi_to_vertex_index_map.end())
		{
			AlembicArchive::TriViToVertexIndex triViToVi;
			archive.trivi_to_vertex_index_map[key] = triViToVi;
		}
			
		Alembic::AbcGeom::OPolyMeshSchema &meshSchema = archive.schema_map[key];
		meshSchema.setTimeSampling(archive.timesampling);

		Alembic::AbcGeom::OPolyMeshSchema::Sample empty;
			
		std::vector<Alembic::Util::int32_t> faceList;
		std::vector<Alembic::Util::int32_t> faceCountList;

		const RenderedBuffer::UVList &uvList = renderedBuffer.uvs;
		const RenderedBuffer::VertexList &vertexList = renderedBuffer.vertecies;
		const RenderedBuffer::NormalList &normalList =  renderedBuffer.normals;

		RenderedBuffer::VertexList vertexListByMaterial;
		RenderedBuffer::UVList uvListByMaterial;
		RenderedBuffer::NormalList normalListByMaterial;

		RenderedMaterial* material = renderedBuffer.materials.at(k);
		const int materialSurfaceSize = static_cast<int>(material->surface.faces.size());
		vertexListByMaterial.resize(materialSurfaceSize * 3);
		faceList.resize(materialSurfaceSize * 3);
		faceCountList.resize(materialSurfaceSize);

		if (!uvList.empty())
		{
			uvListByMaterial.resize(materialSurfaceSize * 3);
		}
		if (!normalList.empty())
		{
			normalListByMaterial.resize(materialSurfaceSize * 3);
		}
			
		AlembicArchive::FaceToVertexIndex& fiToVi = archive.face_to_vertex_index_map[key];
		AlembicArchive::TriViToVertexIndex& triViToVi = archive.trivi_to_vertex_index_map[key];
		int& preSurfaceSize = archive.surface_size_map[key];
		bool isFirstSurface = material->surface.faces.size() != preSurfaceSize;
		if (!isFirstMesh && isFirstSurface)
		{
			continue;
		}

		// re assign par material
		int lastIndex = 0;

		if (isFirstMesh)
		{
			for (int n = 0; n < materialSurfaceSize; ++n)
			{
				UMVec3i face = material->surface.faces[n];
				const int f1 = face.x - 1;
				const int f2 = face.y - 1;
				const int f3 = face.z - 1;
				int vi1 = 0;
				int vi2 = 0;
				int vi3 = 0;

				if (fiToVi.find(f1) == fiToVi.end()) {
					vi1 = lastIndex;
					fiToVi[f1] = vi1;
					++lastIndex;
				}
				else {
					vi1 = fiToVi[f1];
				}

				if (fiToVi.find(f2) == fiToVi.end()) {
					vi2 = lastIndex;
					fiToVi[f2] = vi2;
					++lastIndex;
				}
				else {
					vi2 = fiToVi[f2];
				}

				if (fiToVi.find(f3) == fiToVi.end()) {
					vi3 = lastIndex;
					fiToVi[f3] = vi3;
					++lastIndex;
				}
				else {
					vi3 = fiToVi[f3];
				}
			}
		}

		for (int n = 0; n < materialSurfaceSize; ++n)
		{
			UMVec3i face = material->surface.faces[n];
			const int f1 = face.x - 1;
			const int f2 = face.y - 1;
			const int f3 = face.z - 1;
			int vi1 = fiToVi[f1];
			int vi2 = fiToVi[f2];
			int vi3 = fiToVi[f3];
			vertexListByMaterial[vi1] = vertexList.at(f1);
			vertexListByMaterial[vi2] = vertexList.at(f2);
			vertexListByMaterial[vi3] = vertexList.at(f3);
			if (!normalList.empty() && archive.is_export_normals)
			{
				normalListByMaterial[vi1] = normalList.at(f1);
				normalListByMaterial[vi2] = normalList.at(f2);
				normalListByMaterial[vi3] = normalList.at(f3);
			}
			if (!uvList.empty() && archive.is_export_uvs)
			{
				uvListByMaterial[n * 3 + 0] = uvList.at(f1);
				uvListByMaterial[n * 3 + 1] = uvList.at(f2);
				uvListByMaterial[n * 3 + 2] = uvList.at(f3);
			}
			faceList[n * 3 + 0] = vi1;
			faceList[n * 3 + 1] = vi2;
			faceList[n * 3 + 2] = vi3;
			faceCountList[n] = 3;
		}

		preSurfaceSize = material->surface.faces.size();
		vertexListByMaterial.resize(fiToVi.size());
		normalListByMaterial.resize(fiToVi.size());
		for (int n = 0, nsize = vertexListByMaterial.size(); n < nsize; ++n)
		{
			vertexListByMaterial[n].z = -vertexListByMaterial[n].z;
		}

		if (isFirstMesh && isConvertToQuad) {
			convertToQuad(triViToVi, faceList, faceCountList, vertexListByMaterial);
			archive.quadvi_count_map[key] = faceList.size();
		}

		if (isConvertToQuad) {
			if (!uvList.empty() && archive.is_export_uvs)
			{
				uvListByMaterial.resize(archive.quadvi_count_map[key]);
				for (int n = 0; n < materialSurfaceSize; ++n)
				{
					UMVec3i face = material->surface.faces[n];
					const int f1 = face.x - 1;
					const int f2 = face.y - 1;
					const int f3 = face.z - 1;
					uvListByMaterial[triViToVi[n * 3 + 0]] = uvList.at(f1);
					uvListByMaterial[triViToVi[n * 3 + 1]] = uvList.at(f2);
					uvListByMaterial[triViToVi[n * 3 + 2]] = uvList.at(f3);
				}
			}
		}

		Alembic::AbcGeom::OPolyMeshSchema::Sample sample;

		// vertex
		Alembic::AbcGeom::P3fArraySample positions( (const Imath::V3f *) &vertexListByMaterial.front(), vertexListByMaterial.size());
		sample.setPositions(positions);

		if (isFirstMesh)
		{
			// face index
			Alembic::Abc::Int32ArraySample faceIndices(faceList);
			Alembic::Abc::Int32ArraySample faceCounts(faceCountList);
			sample.setFaceIndices(faceIndices);
			sample.setFaceCounts(faceCounts);

			// faceSet
			Alembic::AbcGeom::OFaceSet faceSet = meshSchema.createFaceSet("mesh_" + to_string(renderedBufferIndex) + "_material_" + to_string(k));
			Alembic::AbcGeom::OFaceSetSchema& faceSetSchema = faceSet.getSchema();
			Alembic::AbcGeom::OFaceSetSchema::Sample faceSetSample;
			faceSetSample.setFaces(faceList);
			faceSetSchema.set(faceSetSample);
		}

		// UVs
		if (!uvListByMaterial.empty() && archive.is_export_uvs)
		{
			if (!uvListByMaterial.empty() && archive.is_export_uvs)
			{
				for (int n = 0, nsize = uvListByMaterial.size(); n < nsize; ++n)
				{
					uvListByMaterial[n].y = 1.0f - uvListByMaterial[n].y;
				}
			}
			Alembic::AbcGeom::OV2fGeomParam::Sample uvSample;
			uvSample.setScope(Alembic::AbcGeom::kVertexScope );
			uvSample.setVals(Alembic::AbcGeom::V2fArraySample( ( const Imath::V2f *) &uvListByMaterial.front(), uvListByMaterial.size()));
			sample.setUVs(uvSample);
		}

		// Normals
		if (!normalListByMaterial.empty() && archive.is_export_normals)
		{
			if (!normalListByMaterial.empty() && archive.is_export_normals)
			{
				for (int n = 0, nsize = normalListByMaterial.size(); n < nsize; ++n)
				{
					normalListByMaterial[n].z = -normalListByMaterial[n].z;
				}
			}
			Alembic::AbcGeom::ON3fGeomParam::Sample normalSample;
			normalSample.setScope(Alembic::AbcGeom::kVertexScope );
			normalSample.setVals(Alembic::AbcGeom::N3fArraySample( (const Alembic::AbcGeom::N3f *) &normalListByMaterial.front(), normalListByMaterial.size()));
			sample.setNormals(normalSample);
		}

		meshSchema.set(sample);
	}
}
	
static void export_alembic_xform_by_material_direct(AlembicArchive &archive, const RenderedBuffer & renderedBuffer, int renderedBufferIndex)
{
	Alembic::AbcGeom::OObject topObj(*archive.archive, Alembic::AbcGeom::kTop);

	for (int k = 0, ksize = static_cast<int>(renderedBuffer.materials.size()); k < ksize; ++k)
	{
		Alembic::AbcGeom::OPolyMesh polyMesh;
		const int key = renderedBufferIndex * 10000 + k;
				
		Alembic::AbcGeom::OXform xform;
		if (archive.xform_map.find(key) != archive.xform_map.end())
		{
			xform = archive.xform_map[key];
		}
		else
		{
			xform = Alembic::AbcGeom::OXform(topObj, "xform_" + to_string(renderedBufferIndex) + "_material_" + to_string(k) , archive.timesampling);
			archive.xform_map[key] = xform;
		}

		bool isFirstMesh = false;
		if (archive.mesh_map.find(key) != archive.mesh_map.end())
		{
			polyMesh = archive.mesh_map[key];
		}
		else
		{
			polyMesh = Alembic::AbcGeom::OPolyMesh(xform, "mesh_" + to_string(renderedBufferIndex) + "_material_" + to_string(k), archive.timesampling);
			archive.mesh_map[key] = polyMesh;
			isFirstMesh = true;

			Alembic::AbcGeom::OPolyMeshSchema &meshSchema = polyMesh.getSchema();
			archive.schema_map[key] = meshSchema;
		}

		if (archive.surface_size_map.find(key) == archive.surface_size_map.end())
		{
			archive.surface_size_map[key] = 0;
		}
			
		Alembic::AbcGeom::OPolyMeshSchema &meshSchema = archive.schema_map[key];
		meshSchema.setTimeSampling(archive.timesampling);

		Alembic::AbcGeom::OPolyMeshSchema::Sample empty;
			
		std::vector<Alembic::Util::int32_t> faceList;
		std::vector<Alembic::Util::int32_t> faceCountList;

		const RenderedBuffer::UVList &uvList = renderedBuffer.uvs;
		const RenderedBuffer::VertexList &vertexList = renderedBuffer.vertecies;
		const RenderedBuffer::NormalList &normalList =  renderedBuffer.normals;

		RenderedBuffer::VertexList vertexListByMaterial;
		RenderedBuffer::UVList uvListByMaterial;
		RenderedBuffer::NormalList normalListByMaterial;

		RenderedMaterial* material = renderedBuffer.materials.at(k);
		const int materialSurfaceSize = static_cast<int>(material->surface.faces.size());
		vertexListByMaterial.resize(materialSurfaceSize * 3);
		faceList.resize(materialSurfaceSize * 3);
		faceCountList.resize(materialSurfaceSize);

		if (!uvList.empty())
		{
			uvListByMaterial.resize(materialSurfaceSize * 3);
		}
		if (!normalList.empty())
		{
			normalListByMaterial.resize(materialSurfaceSize * 3);
		}
			
		int& preSurfaceSize = archive.surface_size_map[key];
		bool isFirstSurface = material->surface.faces.size() != preSurfaceSize;
		if (!isFirstMesh && isFirstSurface)
		{
			continue;
		}

		// re assign par material
		int lastIndex = 0;

		for (int n = 0; n < materialSurfaceSize; ++n)
		{
			UMVec3i face = material->surface.faces[n];

			const int f1 = face.x - 1;
			const int f2 = face.y - 1;
			const int f3 = face.z - 1;
			int vi1 = n * 3 + 0;
			int vi2 = n * 3 + 1;
			int vi3 = n * 3 + 2;

			vertexListByMaterial[vi1] = vertexList.at(f1);
			vertexListByMaterial[vi2] = vertexList.at(f2);
			vertexListByMaterial[vi3] = vertexList.at(f3);
			if (!uvList.empty() && archive.is_export_uvs)
			{
				uvListByMaterial[vi1] = uvList.at(f1);
				uvListByMaterial[vi2] = uvList.at(f2);
				uvListByMaterial[vi3] = uvList.at(f3);
			}
			if (!normalList.empty() && archive.is_export_normals)
			{
				normalListByMaterial[vi1] = normalList.at(f1);
				normalListByMaterial[vi2] = normalList.at(f2);
				normalListByMaterial[vi3] = normalList.at(f3);
			}
			faceList[vi1] = vi1;
			faceList[vi2] = vi2;
			faceList[vi3] = vi3;
			faceCountList[n] = 3;
		}

		preSurfaceSize = material->surface.faces.size();
				
		for (int n = 0, nsize = vertexListByMaterial.size(); n < nsize; ++n)
		{
			vertexListByMaterial[n].z = -vertexListByMaterial[n].z;
		}

		Alembic::AbcGeom::OPolyMeshSchema::Sample sample;
				
		// vertex
		Alembic::AbcGeom::P3fArraySample positions( (const Imath::V3f *) &vertexListByMaterial.front(), vertexListByMaterial.size());
		sample.setPositions(positions);
				
		// face index
		if (isFirstMesh)
		{
			Alembic::Abc::Int32ArraySample faceIndices(faceList);
			Alembic::Abc::Int32ArraySample faceCounts(faceCountList);
			sample.setFaceIndices(faceIndices);
			sample.setFaceCounts(faceCounts);
		}

		// UVs
		if (!uvListByMaterial.empty() && archive.is_export_uvs)
		{
			for (int n = 0, nsize = uvListByMaterial.size(); n < nsize; ++n)
			{
				uvListByMaterial[n].y = 1.0f - uvListByMaterial[n].y;
			}
			Alembic::AbcGeom::OV2fGeomParam::Sample uvSample;
			uvSample.setScope(Alembic::AbcGeom::kVertexScope );
			uvSample.setVals(Alembic::AbcGeom::V2fArraySample( ( const Imath::V2f *) &uvListByMaterial.front(), uvListByMaterial.size()));
			sample.setUVs(uvSample);
		}

		// Normals
		if (!normalListByMaterial.empty() && archive.is_export_normals)
		{
			for (int n = 0, nsize = normalListByMaterial.size(); n < nsize; ++n)
			{
				normalListByMaterial[n].z = -normalListByMaterial[n].z;
			}
			Alembic::AbcGeom::ON3fGeomParam::Sample normalSample;
			normalSample.setScope(Alembic::AbcGeom::kVertexScope );
			normalSample.setVals(Alembic::AbcGeom::N3fArraySample( (const Alembic::AbcGeom::N3f *) &normalListByMaterial.front(), normalListByMaterial.size()));
			sample.setNormals(normalSample);
		}
			
		meshSchema.set(sample);
	}
}

static void export_alembic_xform_by_buffer(AlembicArchive &archive, const RenderedBuffer & renderedBuffer, int renderedBufferIndex)
{
	Alembic::AbcGeom::OObject topObj(*archive.archive, Alembic::AbcGeom::kTop);

	Alembic::AbcGeom::OXform xform;
	if (archive.xform_map.find(renderedBufferIndex) != archive.xform_map.end())
	{
		xform = archive.xform_map[renderedBufferIndex];
	}
	else
	{
		xform = Alembic::AbcGeom::OXform(topObj, "xform_" + umbase::UMStringUtil::number_to_string(renderedBufferIndex), archive.timesampling);
		archive.xform_map[renderedBufferIndex] = xform;
	}
		
	bool isFirstMesh = false;
	Alembic::AbcGeom::OPolyMesh polyMesh;
	if (archive.mesh_map.find(renderedBufferIndex) != archive.mesh_map.end())
	{
		polyMesh = archive.mesh_map[renderedBufferIndex];
	}
	else
	{
		polyMesh = Alembic::AbcGeom::OPolyMesh(xform, "mesh_" + to_string(renderedBufferIndex), archive.timesampling);
		archive.mesh_map[renderedBufferIndex] = polyMesh;
		isFirstMesh = true;

		Alembic::AbcGeom::OPolyMeshSchema &meshSchema = polyMesh.getSchema();
		archive.schema_map[renderedBufferIndex] = meshSchema;
	}

	Alembic::AbcGeom::OPolyMeshSchema &meshSchema = archive.schema_map[renderedBufferIndex];
	meshSchema.setTimeSampling(archive.timesampling);
		
	std::vector<Alembic::Util::int32_t> faceList;
	std::vector<Alembic::Util::int32_t> faceCountList;
		
	const RenderedBuffer::UVList &uvList = renderedBuffer.uvs;
	const RenderedBuffer::VertexList &vertexList = renderedBuffer.vertecies;
	const RenderedBuffer::NormalList &normalList =  renderedBuffer.normals;
	RenderedBuffer::UVList& temporary_uv = archive.temporary_uv_list;
	temporary_uv.resize(uvList.size());
	RenderedBuffer::NormalList& temporary_normal = archive.temporary_normal_list;
	temporary_normal.resize(normalList.size());
	RenderedBuffer::VertexList& temporary_vertex = archive.temporary_vertex_list;
	temporary_vertex.resize(vertexList.size());

	const int materialSize = static_cast<int>(renderedBuffer.materials.size());

	int totalFaceCount = 0;
	for (int k = 0; k < materialSize; ++k)
	{
		RenderedMaterial* material = renderedBuffer.materials.at(k);
		totalFaceCount += material->surface.faces.size();
	}
		
	if (archive.surface_size_map.find(renderedBufferIndex) == archive.surface_size_map.end())
	{
		archive.surface_size_map[renderedBufferIndex] = 0;
	}
	int& preSurfaceSize = archive.surface_size_map[renderedBufferIndex];
	bool isFirstSurface = totalFaceCount != preSurfaceSize;
	if (!isFirstMesh && isFirstSurface)
	{
		return;
	}
	preSurfaceSize = totalFaceCount;

	faceCountList.resize(totalFaceCount);
	faceList.resize(totalFaceCount * 3);

	int faceCounter = 0;
	for (int k = 0; k < materialSize; ++k)
	{
		RenderedMaterial* material = renderedBuffer.materials.at(k);
		const int faceSize = material->surface.faces.size();
		for (int n = 0; n < faceSize; ++n)
		{
			UMVec3i face = material->surface.faces[n];
			faceList[faceCounter * 3 + 0] = (face.x - 1);
			faceList[faceCounter * 3 + 1] = (face.y - 1);
			faceList[faceCounter * 3 + 2] = (face.z - 1);
			faceCountList[faceCounter] = 3;
			++faceCounter;
		}
	}

	Alembic::AbcGeom::OPolyMeshSchema::Sample sample;
				
	// vertex
	for (int n = 0, nsize = vertexList.size(); n < nsize; ++n)
	{
		temporary_vertex[n].z = -vertexList[n].z;
	}
	Alembic::AbcGeom::P3fArraySample positions( (const Imath::V3f *) &temporary_vertex.front(), temporary_vertex.size());
	sample.setPositions(positions);
				
	// face index
	if (isFirstMesh)
	{
		Alembic::Abc::Int32ArraySample faceIndices(faceList);
		Alembic::Abc::Int32ArraySample faceCounts(faceCountList);
		sample.setFaceIndices(faceIndices);
		sample.setFaceCounts(faceCounts);
	}

	// UVs
	if (!uvList.empty() && archive.is_export_uvs)
	{
		for (int n = 0, nsize = uvList.size(); n < nsize; ++n)
		{
			temporary_uv[n].y = 1.0f - uvList[n].y;
		}
		Alembic::AbcGeom::OV2fGeomParam::Sample uvSample;
		uvSample.setScope(Alembic::AbcGeom::kVertexScope );
		uvSample.setVals(Alembic::AbcGeom::V2fArraySample( ( const Imath::V2f *) &temporary_uv.front(), temporary_uv.size()));
		sample.setUVs(uvSample);
	}

	// Normals
	if (!normalList.empty() && archive.is_export_normals)
	{
		for (int n = 0, nsize = normalList.size(); n < nsize; ++n)
		{
			temporary_normal[n].z = -normalList[n].z;
		}
		Alembic::AbcGeom::ON3fGeomParam::Sample normalSample;
		normalSample.setScope(Alembic::AbcGeom::kVertexScope );
		normalSample.setVals(Alembic::AbcGeom::N3fArraySample( (const Alembic::AbcGeom::N3f *) &temporary_normal.front(), temporary_normal.size()));
		sample.setNormals(normalSample);
	}

	meshSchema.set(sample);

}

static void quatToEuler(Imath::V3d &dst, Imath::Quatd quat) {
	double xy = quat.v.x * quat.v.y;
	double zw = quat.v.z * quat.r;

	double test = xy + zw;
	if (test > 0.499) { // singularity at north pole
		double yaw = 2 * atan2(quat.v.x, quat.r);
		double pitch = M_PI/2;
		double roll = 0;
		dst = Imath::V3d(yaw, pitch, roll);
		return;
	}
	if (test < -0.499) { // singularity at south pole
		double yaw = -2 * atan2(quat.v.x, quat.r);
		double pitch = - M_PI/2;
		double roll = 0;
		dst = Imath::V3d(yaw, pitch, roll);
		return;
	}
	double xx = quat.v.x * quat.v.x;
	double yy = quat.v.y * quat.v.y;
	double zz = quat.v.z * quat.v.z;
		
	double yz = quat.v.y * quat.v.z;
	double xz = quat.v.x * quat.v.z;
	double wx = quat.r * quat.v.x;
	double wy = quat.r * quat.v.y;
	double wz = quat.r * quat.v.z;
		
	double yaw = atan2( 2*(wy - xz), 1 - 2*(yy + zz));
	double pitch = atan2( 2*(wx - yz), 1 - 2*(xx +zz));
	double roll = asin( 2*(test));
	dst = Imath::V3d(yaw, pitch, roll);
}

static py::list get_abc_angle_axis()
{
	const RenderedBuffer & renderedBuffer = BridgeParameter::instance().first_noaccessory_buffer();
	D3DXMATRIX convertMat(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, -1, 0,
		0, 0, 0, 1);

	D3DXMATRIX convertedWordInv;
	::D3DXMatrixMultiply(&convertedWordInv, &renderedBuffer.world_inv, &convertMat);
			
	D3DXVECTOR3 eye;
	{
		D3DXVECTOR3 v;
		UMGetCameraEye(&v);
		d3d_vector3_transform(eye, v, convertedWordInv);
	}

	D3DXVECTOR3 at;
	{
		D3DXVECTOR3 v;
		UMGetCameraAt(&v);
		d3d_vector3_transform(at, v, convertedWordInv);
	}

	D3DXVECTOR3 up;
	{
		D3DXVECTOR3 v;
		UMGetCameraUp(&v);
		d3d_vector3_dir_transform(up, v, convertedWordInv);
		::D3DXVec3Normalize(&up, &up);
	}

	Imath::V3d trans((double)eye.x, (double)eye.y, (double)(eye.z));

	D3DXMATRIX view;
	::D3DXMatrixLookAtLH(&view, &eye, &at, &up);

	Imath::M44d rot(
		-view.m[0][0], view.m[0][1], view.m[0][2], 0,
		-view.m[1][0], view.m[1][1], view.m[1][2], 0,
		view.m[2][0], -view.m[2][1], -view.m[2][2], 0,
		0, 0, 0, 1);

	Imath::Quatd quat = Imath::extractQuat(rot);
	quat.normalize();

	py::list result;
	result.append(quat.angle());
	result.append(quat.axis()[0]);
	result.append(quat.axis()[1]);
	result.append(quat.axis()[2]);
	return result;
}
	
static void export_alembic_camera(AlembicArchive &archive, const RenderedBuffer & renderedBuffer, bool isUseEuler)
{
	static const int cameraKey = 0xFFFFFF;
	Alembic::AbcGeom::OObject topObj(*archive.archive, Alembic::AbcGeom::kTop);

	Alembic::AbcGeom::OXform xform;
	if (archive.xform_map.find(cameraKey) != archive.xform_map.end())
	{
		xform = archive.xform_map[cameraKey];
	}
	else
	{
		xform = Alembic::AbcGeom::OXform(topObj, "camera_xform", archive.timesampling);
		archive.xform_map[cameraKey] = xform;

		Alembic::AbcGeom::OXformSchema &xformSchema = xform.getSchema();
		archive.xform_schema_map[cameraKey] = xformSchema;
	}
		
	// set camera transform
	{
		Alembic::AbcGeom::OXformSchema &xformSchema = archive.xform_schema_map[cameraKey];
		xformSchema.setTimeSampling(archive.timesampling);
		
		Alembic::AbcGeom::XformSample xformSample;

		D3DXMATRIX convertMat(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, -1, 0,
			0, 0, 0, 1);

		D3DXMATRIX convertedWordInv;
		::D3DXMatrixMultiply(&convertedWordInv, &renderedBuffer.world_inv, &convertMat);
			
		D3DXVECTOR3 eye;
		{
			D3DXVECTOR3 v;
			UMGetCameraEye(&v);
			d3d_vector3_transform(eye, v,convertedWordInv);
		}
			
		D3DXVECTOR3 at;
		{
			D3DXVECTOR3 v;
			UMGetCameraAt(&v);
			d3d_vector3_transform(at, v, convertedWordInv);
		}

		D3DXVECTOR3 up;
		{
			D3DXVECTOR3 v;
			UMGetCameraUp(&v);
			d3d_vector3_dir_transform(up, v, convertedWordInv);
			::D3DXVec3Normalize(&up, &up);
		}

		Imath::V3d trans((double)eye.x, (double)eye.y, (double)(eye.z));
		xformSample.setTranslation(trans);

		D3DXMATRIX view;
		::D3DXMatrixLookAtLH(&view, &eye, &at, &up);

		Imath::M44d rot(
			-view.m[0][0], view.m[0][1], view.m[0][2], 0,
			-view.m[1][0], view.m[1][1], view.m[1][2], 0,
			view.m[2][0], -view.m[2][1], -view.m[2][2], 0,
			0, 0, 0, 1);

		Imath::Quatd quat = Imath::extractQuat(rot);
		quat.normalize();

		if (isUseEuler)
		{
			Imath::V3d imeuler;
			quatToEuler(imeuler, quat);

			//UMMat44d umrot(
			//	-view.m[0][0], view.m[0][1], view.m[0][2], 0,
			//	-view.m[1][0], view.m[1][1], view.m[1][2], 0,
			//	view.m[2][0], -view.m[2][1], -view.m[2][2], 0,
			//	0, 0, 0, 1);
			//UMVec3d umeuler = umbase::um_matrix_to_euler_xyz(umrot.transposed());
			xformSample.setXRotation(umbase::um_to_degree(imeuler.y));
			xformSample.setYRotation(umbase::um_to_degree(imeuler.x));
			xformSample.setZRotation(-umbase::um_to_degree(imeuler.z));
		}
		else
		{
			xformSample.setRotation(quat.axis(), umbase::um_to_degree(quat.angle()));
		}

		xformSchema.set(xformSample);
	}
		
	Alembic::AbcGeom::OCamera camera;
	if (archive.camera_map.find(cameraKey) != archive.camera_map.end())
	{
		camera = archive.camera_map[cameraKey];
	}
	else
	{
		camera = Alembic::AbcGeom::OCamera(xform, "camera", archive.timesampling);
		archive.camera_map[cameraKey] = camera;
			
		Alembic::AbcGeom::OCameraSchema &cameraSchema = camera.getSchema();
		archive.camera_schema_map[cameraKey] = cameraSchema;
	}

	Alembic::AbcGeom::OCameraSchema &cameraSchema = archive.camera_schema_map[cameraKey];
	cameraSchema.setTimeSampling(archive.timesampling);
	Alembic::AbcGeom::CameraSample sample;

	D3DXVECTOR4 v;
	UMGetCameraFovLH(&v);

	sample.setNearClippingPlane(v.z);
	sample.setFarClippingPlane(v.w);

	double fovy = v.x;
	double aspect = v.y;
	double fovx = 2.0 * atan(tan(fovy / 2.0)*(aspect));
	double w = BridgeParameter::instance().frame_width / 10.0;
	double h = BridgeParameter::instance().frame_height / 10.0;
	double focalLength = w / (2.0 * tan(fovx / 2.0));

	sample.setHorizontalAperture(w / 10.0);
	sample.setVerticalAperture(h / 10.0);
	sample.setFocalLength(focalLength);

	cameraSchema.set(sample);
}

static bool execute_alembic_export(int currentframe)
{
	AlembicArchive &archive = AlembicArchive::instance();
	if (!archive.archive) { return Py_BuildValue(""); }
	
	const BridgeParameter& parameter = BridgeParameter::instance();
	const VertexBufferList& finishBuffers = BridgeParameter::instance().finish_buffer_list;
	const RenderBufferMap& renderBuffers = BridgeParameter::instance().render_buffer_map;

	bool exportedCamera = false;
	for (int i = static_cast<int>(finishBuffers.size()) - 1; i >= 0; --i)
	{
		const RenderedBuffer &renderedBuffer = parameter.render_buffer(i);

		if (!exportedCamera && !renderedBuffer.isAccessory)
		{
			export_alembic_camera(archive, renderedBuffer, archive.is_use_euler_rotation_camera);
			exportedCamera = true;
		}
	}

	for (int i = 0, isize = static_cast<int>(finishBuffers.size()); i < isize; ++i)
	{
		const RenderedBuffer &renderedBuffer = parameter.render_buffer(i);

		if (archive.export_mode == 0)
		{
			export_alembic_xform_by_material_fix_vindex(archive, renderedBuffer, i, false);
		}
		else if (archive.export_mode == 1)
		{
			export_alembic_xform_by_buffer(archive, renderedBuffer, i);
		}
		else if (archive.export_mode == 2)
		{
			export_alembic_xform_by_material_direct(archive, renderedBuffer, i);
		}
		else if (archive.export_mode == 3)
		{
			export_alembic_xform_by_material_fix_vindex(archive, renderedBuffer, i, true);
		}
	}
	return true;
}

// ---------------------------------------------------------------------------
PYBIND11_PLUGIN( mmdbridge_abc )
{
    py::module m("mmdbridge_abc");
	m.def("start_alembic_export", start_alembic_export);
    m.def("end_alembic_export", end_alembic_export);
    m.def("execute_alembic_export", execute_alembic_export);
    m.def("get_abc_angle_axis", get_abc_angle_axis);
    return m.ptr();
}

#endif //WITH_ALEMBIC

// ---------------------------------------------------------------------------
#ifdef WITH_ALEMBIC
	void InitAlembic()
	{
		PyImport_AppendInittab("mmdbridge_abc", PyInit_mmdbridge_abc);
	}
	void DisposeAlembic() 
	{
		AlembicArchive::instance().end();
	}
#else
	void InitAlembic(){}
	void DisposeAlembic() {}
#endif //WITH_ALEMBIC
