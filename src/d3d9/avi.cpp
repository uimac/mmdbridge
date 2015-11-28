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

	WriterPtr writer = archive.file_data.writer;
	if (writer) {
		archive.file_data.streamWriter = writer->AddMediaStream(512, 512, 24, 0, 512 * 512 * 3, 30);
		writer->Start();
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

std::vector<char> image(512 * 512 * 3);

static bool execute_avi_export(int currentframe)
{
	AVIArchive &archive = AVIArchive::instance();
	BridgeParameter::mutable_instance().is_exporting_without_mesh = true;

	const BridgeParameter& parameter = BridgeParameter::instance();

	AVI20::Write::MediaStreamWriter& streamWriter = archive.file_data.streamWriter;

	streamWriter.WriteFrame(image.data(), image.size(), true);

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

		for (int i = 0, isize = 512 * 512; i < isize; ++i) {
			image[i * 3 + 0] = 0x00;
			image[i * 3 + 1] = 0x00;
			image[i * 3 + 2] = 0xFF;
		}
	}
	void DisposeAVI() 
	{
		AVIArchive::instance().end();
	}
#else
	void InitAVI(){}
	void DisposeAVI() {}
#endif //WITH_AVI
