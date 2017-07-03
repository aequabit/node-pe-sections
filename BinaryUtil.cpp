/**
* @file    cBinaryUtil.cpp.
*
* Implements the binary utility class.
**/

#include "BinaryUtil.h"
#include <fstream>

/**
* Loads binary from disk.
*
* @author  ToxicOverflow
* @date    04.04.2017
*
* @param   strPath Full pathname of the file.
*
* @return  Null if it fails, else the binary from disk.
**/

PEFile* BinaryUtil::LoadBinaryFromDisk(
	const std::string& strPath)
{
	std::ifstream stream = std::ifstream(strPath, std::ios::binary | std::ios::ate);

	if (stream.good() && stream.is_open()) {
		std::streamoff file_size = stream.tellg();

		if (file_size) {
			stream.seekg(0, std::ios::beg);

			char* ModuleBuffer = new char[file_size];

			stream.read(ModuleBuffer, file_size);
			stream.close();

			return new PEFile(ModuleBuffer, file_size);
		}
	}

	stream.close();

	return nullptr;
}