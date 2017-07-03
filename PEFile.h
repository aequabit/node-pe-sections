/**
* @file    PEFile.h.
*
* Declares the pe file class.
**/

#pragma once
#include <Windows.h>

class PEFile
{
public:

	/**
	* Constructor.
	*
	* @author  ToxicOverflow
	* @date    04.04.2017
	*
	* @param [in,out]  pImage  If non-null, the image.
	* @param           Size    The size.
	**/

	explicit PEFile(char* pImage, size_t Size);

	/**
	* Constructor.
	*
	* @author  ToxicOverflow
	* @date    21.04.2017
	*
	* @param [in,out]  pImage  If non-null, the image.
	**/

	explicit PEFile(char* pImage);

	/**
	* Gets NT headers.
	*
	* @author  ToxicOverflow
	* @date    04.04.2017
	*
	* @return  Null if it fails, else the NT headers.
	**/

	IMAGE_NT_HEADERS*               GetNtHeaders() const;

	/**
	* Gets dos header.
	*
	* @author  ToxicOverflow
	* @date    04.04.2017
	*
	* @return  Null if it fails, else the dos header.
	**/

	IMAGE_DOS_HEADER*               GetDosHeader() const;

	/**
	* Gets optional header.
	*
	* @author  ToxicOverflow
	* @date    04.04.2017
	*
	* @return  The optional header.
	**/

	IMAGE_OPTIONAL_HEADER           GetOptionalHeader() const;

	/**
	* Gets the first section header.
	*
	* @author  ToxicOverflow
	* @date    04.04.2017
	*
	* @return  Null if it fails, else the first section header.
	**/

	IMAGE_SECTION_HEADER*           GetFirstSectionHeader() const;

	/**
	* Gets import descriptor.
	*
	* @author  ToxicOverflow
	* @date    04.04.2017
	*
	* @return  Null if it fails, else the import descriptor.
	**/

	IMAGE_IMPORT_DESCRIPTOR*        GetImportDescriptor();

	/**
	* Gets base relocation.
	*
	* @author  ToxicOverflow
	* @date    04.04.2017
	*
	* @return  Null if it fails, else the base relocation.
	**/

	IMAGE_BASE_RELOCATION*          GetBaseRelocation();

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

	IMAGE_SECTION_HEADER*           GetSectionForRVA(const unsigned long RelativeAddress) const;

	/**
	* Gets section count.
	*
	* @author  ToxicOverflow
	* @date    04.04.2017
	*
	* @return  The section count.
	**/

	size_t                          GetSectionCount() const;

	/**
	* Gets the size.
	*
	* @author  ToxicOverflow
	* @date    04.04.2017
	*
	* @return  The size.
	**/

	size_t                          GetSize() const;

	/**
	* Gets image size.
	*
	* @author  ToxicOverflow
	* @date    04.04.2017
	*
	* @return  The image size.
	**/

	size_t                          GetImageSize() const;

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

	size_t                          GetDirectorySize(const int Directory) const;

	/**
	* Gets raw image.
	*
	* @author  ToxicOverflow
	* @date    04.04.2017
	*
	* @return  Null if it fails, else the raw image.
	**/

	char*                           GetRawImage() const;

	/**
	* Gets entry point address.
	*
	* @author  ToxicOverflow
	* @date    04.04.2017
	*
	* @return  The entry point address.
	**/

	unsigned long                   GetEntryPointAddress() const;

	/**
	* Gets image base.
	*
	* @author  ToxicOverflow
	* @date    04.04.2017
	*
	* @return  The image base.
	**/

	unsigned long                   GetImageBase() const;

	/**
	* Query if this object is valid pe.
	*
	* @author  ToxicOverflow
	* @date    04.04.2017
	*
	* @return  True if valid pe, false if not.
	**/

	bool                            IsValidPE() const;

	/**
	* Gets directory pointer.
	*
	* @author  ToxicOverflow
	* @date    04.04.2017
	*
	* @tparam  T   Generic type parameter.
	* @param   iType   Zero-based index of the type.
	*
	* @return  Null if it fails, else the directory pointer.
	**/

	template < typename T >
	T* GetDirectoryPointer(const int iType)
	{
		return reinterpret_cast< T* >(DirectoryPointers[iType]);
	}

	/**
	* Gets pointer from rva.
	*
	* @author  ToxicOverflow
	* @date    04.04.2017
	*
	* @tparam  T   Generic type parameter.
	* @param   RelativeAddress The relative address.
	*
	* @return  Null if it fails, else the pointer from rva.
	**/

	template < typename T >
	T* GetPointerFromRVA(const unsigned long RelativeAddress)
	{
		IMAGE_SECTION_HEADER* pSectionHeader = GetSectionForRVA(RelativeAddress);

		if (pSectionHeader) {
			ptrdiff_t diff = static_cast< ptrdiff_t >(
				pSectionHeader->VirtualAddress - pSectionHeader->PointerToRawData);

			return reinterpret_cast< T* >(pBaseAddress + RelativeAddress - pSectionHeader->VirtualAddress + pSectionHeader->PointerToRawData);
		}

		return nullptr;
	}

	~PEFile()
	{
		if (pBaseAddress)
			delete[] pBaseAddress;
	}

private:

	/**
	* Parses this object.
	*
	* @author  ToxicOverflow
	* @date    04.04.2017
	**/

	void Parse();

private:

	char*               pBaseAddress;   /* The base address */
	size_t              nSize;  /* The size */
	IMAGE_DOS_HEADER*   pDosHeader; /* The dos header */
	IMAGE_NT_HEADERS*   pNtHeaders; /* The NT headers */
	bool                bValidPE = true;
	unsigned long       DirectoryPointers[IMAGE_NUMBEROF_DIRECTORY_ENTRIES];    /* The directory pointers[ image numberof directory entries] */
};