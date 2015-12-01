#ifdef WITH_AVI

#include "d3d9.h"
#include "d3dx9.h"

#include "bridge_parameter.h"
#include "UMStringUtil.h"
#include "UMPath.h"
#include "UMMath.h"
#include "UMVector.h"
#include "UMMatrix.h"

#include <boost/python/detail/wrap_python.hpp>
#include <boost/python.hpp>
#include <boost/python/make_constructor.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/python/suite/indexing/map_indexing_suite.hpp>
#include <boost/python/copy_non_const_reference.hpp>
#include <boost/python/module.hpp>
#include <boost/python/def.hpp>
#include <boost/python/args.hpp>
#include <boost/python/tuple.hpp>
#include <boost/python/class.hpp>
#include <boost/python/overloads.hpp>
#include <boost/format.hpp>

#include <typeinfo>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <codecvt>
#include <locale>
#include <algorithm>
#include <wchar.h>

#include <AVI20/Write/StreamW.h>
#include <AVI20/Write/Writer.h>
#include <AVI20/Write/MediaStreamWriter.h>
#include <AVI20/Write/RiffStream.h>

#include "MMDExport.h"

typedef std::shared_ptr<AVI20::Write::Writer> WriterPtr;
typedef std::shared_ptr<AVI20::Write::Stream> StreamPtr;

class FileData {
public:
	FileData() : fstream(NULL) {}
	std::fstream* fstream;

	StreamPtr stream;
	WriterPtr writer;
	AVI20::Write::MediaStreamWriter streamWriter;
	std::vector<unsigned int> image;
};

class AVIArchive {
public:
	
	static AVIArchive& instance() {
		static AVIArchive instance;
		return instance; 
	}

	std::map<std::string, int> file_path_map;

	FileData file_data;

	umstring output_path;

	int export_mode;

void end()
{
	if (file_data.writer) {
		file_data.writer->Finalize();
		file_data.writer = WriterPtr();
	}
	if (file_data.stream) {
		file_data.stream = StreamPtr();
	}
	if (file_data.fstream) {
		if (file_data.fstream->is_open()) {
			file_data.fstream->close();
		}
		delete file_data.fstream;
		file_data.fstream = NULL;
	}
	file_path_map.clear();
	output_path.clear();
}

~AVIArchive() {
}
private:
	AVIArchive() {}
};

static bool start_avi_export(
	const std::string& directory_path,
	int export_mode)
{
	AVIArchive &archive = AVIArchive::instance();
	BridgeParameter::mutable_instance().is_exporting_without_mesh = true;
	const BridgeParameter& parameter = BridgeParameter::instance();

	if (directory_path.empty())
	{
		archive.output_path = umbase::UMStringUtil::utf8_to_utf16(
			umbase::UMStringUtil::wstring_to_utf8(parameter.base_path) + ("out\\"));
	}
	else
	{
		archive.output_path = umbase::UMStringUtil::utf8_to_utf16(directory_path);
	}
	umstring filename = umbase::UMStringUtil::utf8_to_utf16("out.avi");
	archive.output_path += filename;

	archive.file_data.fstream = new std::fstream();
	archive.file_data.fstream->open(archive.output_path.c_str(), std::ios::trunc | std::ios::in | std::ios::out | std::ios::binary);
	archive.file_data.stream = std::make_shared<AVI20::Write::Stream>(archive.file_data.fstream);
	archive.file_data.writer = std::make_shared<AVI20::Write::Writer>(*archive.file_data.stream);

	const VertexBufferList& finishBuffers = BridgeParameter::instance().finish_buffer_list;
	int all_size = 0;

	// header
	{
		all_size += 1; // buffersize
		for (int i = 0, isize = static_cast<int>(finishBuffers.size()); i < isize; ++i)
		{
			all_size += 3; // vsize(=nsize), uvsize, matsize
			const RenderedBuffer &renderedBuffer = parameter.render_buffer(i);
		}
	}

	// contents
	for (int i = 0, isize = static_cast<int>(finishBuffers.size()); i < isize; ++i)
	{
		const RenderedBuffer &renderedBuffer = parameter.render_buffer(i);
		const RenderedBuffer::VertexList& vertexList = renderedBuffer.vertecies;
		const RenderedBuffer::NormalList& normalList = renderedBuffer.normals;
		const RenderedBuffer::UVList &uvList = renderedBuffer.uvs;
		const int material_size = static_cast<int>(renderedBuffer.materials.size());
		// material face sizes
		all_size += material_size;
		for (int k = 0; k < material_size; ++k)
		{
			RenderedMaterial* material = renderedBuffer.materials.at(k);
			const int materialSurfaceSize = static_cast<int>(material->surface.faces.size());
			// vertex size
			all_size += materialSurfaceSize * 3 * 3;

			if (!normalList.empty()) {
				// normal size
				all_size += materialSurfaceSize * 3 * 3;
			}
			if (!uvList.empty()) {
				// uv size
				all_size += materialSurfaceSize * 2 * 3;
			}
		}
	}

	WriterPtr writer = archive.file_data.writer;
	if (writer) {
		if (all_size < (256 * 256)) {
			archive.file_data.streamWriter = writer->AddMediaStream(256, 256, 24, 0, 256 * 256 * 3, 30);
			writer->Start();
			archive.file_data.image.resize(256 * 256);
		}
		else if (all_size < (512 * 512)) {
			archive.file_data.streamWriter = writer->AddMediaStream(512, 512, 24, 0, 512 * 512 * 3, 30);
			writer->Start();
			archive.file_data.image.resize(512 * 512);
		}
		else if (all_size < 1024 * 1024) {
			archive.file_data.streamWriter = writer->AddMediaStream(1024, 1024, 24, 0, 1024 * 1024 * 3, 30);
			writer->Start();
			archive.file_data.image.resize(1024 * 1024);

		}
		else if (all_size < 2048 * 2048) {
			archive.file_data.streamWriter = writer->AddMediaStream(2048, 2048, 24, 0, 2048 * 2048 * 3, 30);
			writer->Start();
			archive.file_data.image.resize(2048 * 2048);
		}
	}

	return true;
}

static bool end_avi_export()
{
	AVIArchive &archive = AVIArchive::instance();
	BridgeParameter::mutable_instance().is_exporting_without_mesh = true;
	const BridgeParameter& parameter = BridgeParameter::instance();
	const int pmd_num = ExpGetPmdNum();

	AVIArchive::instance().end();
	return true;
}

static bool execute_avi_export(int currentframe)
{
	AVIArchive &archive = AVIArchive::instance();
	BridgeParameter::mutable_instance().is_exporting_without_mesh = true;

	const BridgeParameter& parameter = BridgeParameter::instance();

	const VertexBufferList& finishBuffers = BridgeParameter::instance().finish_buffer_list;
	int bufferpos = 0;
	std::vector<unsigned int> & image = archive.file_data.image;
	image[bufferpos++] = finishBuffers.size();
	for (int i = 0, isize = static_cast<int>(finishBuffers.size()); i < isize; ++i)
	{
		const RenderedBuffer &renderedBuffer = parameter.render_buffer(i);
		const RenderedBuffer::VertexList& vertexList = renderedBuffer.vertecies;
		const RenderedBuffer::NormalList& normalList = renderedBuffer.normals;
		const RenderedBuffer::UVList &uvList = renderedBuffer.uvs;
		const int material_size = static_cast<int>(renderedBuffer.materials.size());

		const int vertsizepos = bufferpos++;
		const int uvsizepos = bufferpos++;
		image[bufferpos++] = material_size;

		int vertex_size = 0;
		for (int k = 0; k < material_size; ++k)
		{
			RenderedMaterial* material = renderedBuffer.materials.at(k);
			const int materialSurfaceSize = static_cast<int>(material->surface.faces.size());
			image[bufferpos++] = materialSurfaceSize;
			image[vertsizepos] += materialSurfaceSize * 3;
			if (!uvList.empty()) {
				// uv size
				image[uvsizepos] += materialSurfaceSize * 2;
			}
		}
		// vert
		for (int k = 0; k < material_size; ++k)
		{
			RenderedMaterial* material = renderedBuffer.materials.at(k);
			const int materialSurfaceSize = static_cast<int>(material->surface.faces.size());
			for (int n = 0; n < materialSurfaceSize; ++n) {
				for (int m = 0; m < 3; ++m) {
					memcpy(&image[bufferpos++], &vertexList[material->surface.faces[n][m]].x, sizeof(float));
					memcpy(&image[bufferpos++], &vertexList[material->surface.faces[n][m]].y, sizeof(float));
					memcpy(&image[bufferpos++], &vertexList[material->surface.faces[n][m]].z, sizeof(float));
				}
			}
		}
		// normal
		for (int k = 0; k < material_size; ++k)
		{
			RenderedMaterial* material = renderedBuffer.materials.at(k);
			const int materialSurfaceSize = static_cast<int>(material->surface.faces.size());
			for (int n = 0; n < materialSurfaceSize; ++n) {
				for (int m = 0; m < 3; ++m) {
					memcpy(&image[bufferpos++], &normalList[material->surface.faces[n][m]].x, sizeof(float));
					memcpy(&image[bufferpos++], &normalList[material->surface.faces[n][m]].y, sizeof(float));
					memcpy(&image[bufferpos++], &normalList[material->surface.faces[n][m]].z, sizeof(float));
				}
			}
		}
		// uv
		for (int k = 0; k < material_size; ++k)
		{
			RenderedMaterial* material = renderedBuffer.materials.at(k);
			const int materialSurfaceSize = static_cast<int>(material->surface.faces.size());
			for (int n = 0; n < materialSurfaceSize; ++n) {
				for (int m = 0; m < 3; ++m) {
					memcpy(&image[bufferpos++], &uvList[material->surface.faces[n][m]].x, sizeof(float));
					memcpy(&image[bufferpos++], &uvList[material->surface.faces[n][m]].y, sizeof(float));
				}
			}
		}
	}

	AVI20::Write::MediaStreamWriter& streamWriter = archive.file_data.streamWriter;
	streamWriter.WriteFrame(archive.file_data.image.data(), archive.file_data.image.size() * 3, true);

	return true;
}

// ---------------------------------------------------------------------------
BOOST_PYTHON_MODULE( mmdbridge_avi )
{
	using namespace boost::python;
	def("start_avi_export", start_avi_export);
	def("end_avi_export", end_avi_export);
	def("execute_avi_export", execute_avi_export);
}

#endif //WITH_AVI

// ---------------------------------------------------------------------------
#ifdef WITH_AVI
void InitAVI()
	{
		PyImport_AppendInittab("mmdbridge_avi", PyInit_mmdbridge_avi);
	}
	void DisposeAVI() 
	{
		AVIArchive::instance().end();
	}
#else
	void InitAVI(){}
	void DisposeAVI() {}
#endif //WITH_AVI
