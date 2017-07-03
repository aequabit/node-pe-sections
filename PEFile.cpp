/**
* @file    PEFile.cpp.
*
* Implements the pe file class.
**/

#include "PEFile.h"
#include "BinaryUtil.h"
#include <bitset>

/**
* Constructor.
*
* @author  ToxicOverflow
* @date    04.04.2017
*
* @param [in,out]  pImage  If non-null, the image.
* @param           Size    The size.
**/

PEFile::PEFile(
	char*   pImage,
	size_t  Size)
{
	nSize = Size;
	pBaseAddress = pImage;

	Parse();
}
PEFile::PEFile(
	char*   pImage)
{
	pBaseAddress = pImage;

	Parse();
}
/**
* Gets the size.
*
* @author  ToxicOverflow
* @date    04.04.2017
*
* @return  The size.
**/

size_t PEFile::GetSize() const
{
	return nSize;
}

/**
* Gets raw image.
*
* @author  ToxicOverflow
* @date    04.04.2017
*
* @return  Null if it fails, else the raw image.
**/

char* PEFile::GetRawImage() const
{
	return pBaseAddress;
}

/**
* Parses this object.
*
* @author  ToxicOverflow
* @date    04.04.2017
**/

void PEFile::Parse()
{
	pDosHeader = BinaryUtil::MakePointer< IMAGE_DOS_HEADER >(pBaseAddress, 0);

	if (pDosHeader->e_magic == IMAGE_DOS_SIGNATURE) {
		pNtHeaders = BinaryUtil::MakePointer< IMAGE_NT_HEADERS >(pBaseAddress, pDosHeader->e_lfanew);

		if (pNtHeaders->Signature == IMAGE_NT_SIGNATURE) {

			for (size_t i = 0; i < IMAGE_NUMBEROF_DIRECTORY_ENTRIES; i++) {

				DirectoryPointers[i] = reinterpret_cast< unsigned long >(
					GetPointerFromRVA< void >(pNtHeaders->OptionalHeader.DataDirectory[i].VirtualAddress));

			}

			bValidPE = true;
		}
	}
}

/**
* Query if this object is valid pe.
*
* @author  ToxicOverflow
* @date    04.04.2017
*
* @return  True if valid pe, false if not.
**/

bool PEFile::IsValidPE() const
{
	return bValidPE;
}

/**
* Gets directory size.
*
* @author  ToxicOverflow
* @date    04.04.2017
*
* @param   Directory   Pathname of the directory.
*
* @return  The directory size.
**/

size_t PEFile::GetDirectorySize(
	const int Directory) const
{
	return pNtHeaders->OptionalHeader.DataDirectory[Directory].Size;
}

/**
* Gets NT headers.
*
* @author  ToxicOverflow
* @date    04.04.2017
*
* @return  Null if it fails, else the NT headers.
**/

IMAGE_NT_HEADERS* PEFile::GetNtHeaders() const
{
	return pNtHeaders;
}

/**
* Gets dos header.
*
* @author  ToxicOverflow
* @date    04.04.2017
*
* @return  Null if it fails, else the dos header.
**/

IMAGE_DOS_HEADER* PEFile::GetDosHeader() const
{
	return pDosHeader;
}

/**
* Gets optional header.
*
* @author  ToxicOverflow
* @date    04.04.2017
*
* @return  The optional header.
**/

IMAGE_OPTIONAL_HEADER PEFile::GetOptionalHeader() const
{
	return pNtHeaders->OptionalHeader;
}

/**
* Gets the first section header.
*
* @author  ToxicOverflow
* @date    04.04.2017
*
* @return  Null if it fails, else the first section header.
**/

IMAGE_SECTION_HEADER* PEFile::GetFirstSectionHeader() const
{
	return IMAGE_FIRST_SECTION(pNtHeaders);
}

/**
* Gets import descriptor.
*
* @author  ToxicOverflow
* @date    04.04.2017
*
* @return  Null if it fails, else the import descriptor.
**/

IMAGE_IMPORT_DESCRIPTOR* PEFile::GetImportDescriptor()
{
	return GetDirectoryPointer< IMAGE_IMPORT_DESCRIPTOR >(IMAGE_DIRECTORY_ENTRY_IMPORT);
}

/**
* Gets base relocation.
*
* @author  ToxicOverflow
* @date    04.04.2017
*
* @return  Null if it fails, else the base relocation.
**/

IMAGE_BASE_RELOCATION* PEFile::GetBaseRelocation()
{
	return GetDirectoryPointer< IMAGE_BASE_RELOCATION >(IMAGE_DIRECTORY_ENTRY_BASERELOC);
}

/**
* Gets image size.
*
* @author  ToxicOverflow
* @date    04.04.2017
*
* @return  The image size.
**/

size_t PEFile::GetImageSize() const
{
	return pNtHeaders->OptionalHeader.SizeOfImage;
}

/**
* Gets entry point address.
*
* @author  ToxicOverflow
* @date    04.04.2017
*
* @return  The entry point address.
**/

unsigned long PEFile::GetEntryPointAddress() const
{
	return pNtHeaders->OptionalHeader.AddressOfEntryPoint;
}

/**
* Gets image base.
*
* @author  ToxicOverflow
* @date    04.04.2017
*
* @return  The image base.
**/

unsigned long PEFile::GetImageBase() const
{
	return pNtHeaders->OptionalHeader.ImageBase;
}

/**
* Gets section for rva.
*
* @author  ToxicOverflow
* @date    04.04.2017
*
* @param   RelativeAddress The relative address.
*
* @return  Null if it fails, else the section for rva.
**/

IMAGE_SECTION_HEADER* PEFile::GetSectionForRVA(
	const unsigned long RelativeAddress) const
{
	IMAGE_SECTION_HEADER* pSection = IMAGE_FIRST_SECTION(pNtHeaders);

	for (size_t i = 0; i < pNtHeaders->FileHeader.NumberOfSections; i++, pSection++) {

		unsigned long SectionStart = pSection->VirtualAddress;
		unsigned long SectionEnd = SectionStart;

		if (pSection->Misc.VirtualSize == 0)
			SectionEnd += pSection->SizeOfRawData;
		else
			SectionEnd += pSection->Misc.VirtualSize;

		if (RelativeAddress >= SectionStart && RelativeAddress < SectionEnd) {
			return pSection;
		}
	}

	return nullptr;
}

/**
* Gets section count.
*
* @author  ToxicOverflow
* @date    04.04.2017
*
* @return  The section count.
**/

size_t PEFile::GetSectionCount() const
{
	return pNtHeaders->FileHeader.NumberOfSections;
}