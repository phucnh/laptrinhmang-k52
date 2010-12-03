#include "stdafx.h"
#include "MimeCode.h"
#include "MimeChar.h"
#include "Mime.h"
#include <stdlib.h>
#include <time.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

static const char* LineFind(const char* pszString, int ch)
{
	ASSERT(pszString != NULL);
	while (*pszString != 0 && *pszString != ch && *pszString != '\r' && *pszString != '\n')
		pszString++;
	return *pszString == ch ? pszString : NULL;
}


const char* FindString(const char* pszStr1, const char* pszStr2, const char* pszEnd)
{
	pszEnd -= ::strlen(pszStr2);

	const char *s1, *s2;
	while (pszStr1 <= pszEnd)
	{
		s1 = pszStr1;
		s2 = pszStr2;
		while (*s1 == *s2 && *s2)
			s1++, s2++;
		if (!*s2)
			return pszStr1;
		pszStr1++;
	}
	return NULL;
}

const char* FindChar(const char* pszStr, char c, const char* pszEnd )
{
	for( const char *s = pszStr; s < pszEnd; ++s ) 
	{
		if ( *s == '\0' )
		{
			
			return NULL;
		}
		if ( *s == c )
		{
			return s;
		}
	}
	return NULL;
}





void CMimeField::GetValue(string& strValue) const
{
	string::size_type nEnd = m_strValue.find(';');
	if (nEnd != string::npos)
	{
		while (nEnd > 0 && CMimeChar::IsSpace((unsigned char)m_strValue[nEnd-1]))
			nEnd--;
		strValue.assign(m_strValue.c_str(), nEnd);
	}
	else
		strValue = m_strValue;
}


void CMimeField::SetParameter(const char* pszAttr, const char* pszValue)
{
	int nSize = pszValue ? (int)::strlen(pszValue) : 0;
	string strValue;
	strValue.reserve(nSize+3);
	if (!pszValue || *pszValue != '"')
		strValue = "\"";
	if (pszValue != NULL)
		strValue += pszValue;
	if (nSize < 2 || pszValue[nSize-1] != '"')
		strValue += "\"";

	int nPos;
	if (!FindParameter(pszAttr, nPos, nSize))	
	{
		m_strValue.reserve(m_strValue.size() + ::strlen(pszAttr) + strValue.size() + 5);		
		m_strValue += "; ";
		m_strValue += pszAttr;
		m_strValue += '=';
		m_strValue += strValue;
	}
	else							
		m_strValue.replace(nPos, nSize, strValue);
}


bool CMimeField::GetParameter(const char* pszAttr, string& strValue) const
{
	int nPos, nSize;
	if (!FindParameter(pszAttr, nPos, nSize))
	{
		strValue.clear();
		return false;
	}

	if (m_strValue[nPos] == '"')
	{
		nPos++;
		nSize--;
		if (nSize > 0 && m_strValue[nPos+nSize-1] == '"')
			nSize--;
	}
	strValue.assign(m_strValue.data()+nPos, nSize);
	return true;
}

int CMimeField::GetLength() const
{
	int nLength = (int) m_strName.size() + 4;
	CFieldCodeBase* pCoder = CMimeEnvironment::CreateFieldCoder(GetName());
	pCoder->SetCharset(m_strCharset.c_str());
	pCoder->SetInput(m_strValue.c_str(), (int)m_strValue.size(), true);
	nLength += pCoder->GetOutputLength();
	delete pCoder;
	return nLength;
}


int CMimeField::Store(char* pszData, int nMaxSize) const
{
	ASSERT(pszData != NULL);
	int nMinSize = (int)m_strName.size() + 4;
	if (nMaxSize < nMinSize)
		return 0;
	::strcpy_s(pszData, nMaxSize, m_strName.c_str());
	pszData += m_strName.size();
	*pszData++ = ':';
	*pszData++ = ' ';

	CFieldCodeBase* pCoder = CMimeEnvironment::CreateFieldCoder(GetName());
	pCoder->SetCharset(m_strCharset.c_str());
	pCoder->SetInput(m_strValue.c_str(), (int)m_strValue.size(), true);
	int nEncoded = pCoder->GetOutput((unsigned char*) pszData, nMaxSize-nMinSize);
	delete pCoder;
	pszData += nEncoded;

	*pszData++ = '\r';
	*pszData++ = '\n';
	return nMinSize + nEncoded;
}


int CMimeField::Load(const char* pszData, int nDataSize)
{
	Clear();
	ASSERT(pszData != NULL);
	const char *pszEnd, *pszStart = pszData;
	
	while (CMimeChar::IsSpace((unsigned char)*pszStart))
	{
		if (*pszStart == '\r')		
			return 0;
		pszStart = ::FindString(pszStart, "\r\n", pszData+nDataSize);
		if (!pszStart)
			return 0;
		pszStart += 2;
	}

	
	pszEnd = ::LineFind(pszStart, ':');
	if (pszEnd != NULL)				
	{
		m_strName.assign(pszStart, (pszEnd-pszStart));
		pszStart = pszEnd + 1;
	}

	
	while (*pszStart == ' ' || *pszStart == '\t')
		pszStart++;
	pszEnd = pszStart;
	do
	{
		pszEnd = ::FindString(pszEnd, "\r\n", pszData+nDataSize);
		if (!pszEnd)
			return 0;
		pszEnd += 2;
	} while (*pszEnd == '\t' || *pszEnd == ' ');	

	
	CFieldCodeBase* pCoder = CMimeEnvironment::CreateFieldCoder(GetName());
	pCoder->SetInput(pszStart, (int)(pszEnd-pszStart)-2, false);
	m_strValue.resize(pCoder->GetOutputLength());
	int nSize = pCoder->GetOutput((unsigned char*) m_strValue.c_str(), (int) m_strValue.capacity());
	m_strValue.resize(nSize);
	m_strCharset = pCoder->GetCharset();
	delete pCoder;
	return (int) (pszEnd - pszData);
}

bool CMimeField::FindParameter(const char* pszAttr, int& nPos, int& nSize) const
{
	ASSERT(pszAttr != NULL);
	const char* endParam = m_strValue.data() + m_strValue.length();
	const char* pszParms = FindChar( m_strValue.data(), ';', endParam );
	int nAttrSize = (int)::strlen(pszAttr);
	while (pszParms != NULL)
	{
		while (CMimeChar::IsSpace((unsigned char)*pszParms) || *pszParms == ';')
			pszParms++;

		const char* pszName = pszParms;		
		pszParms = FindChar( pszParms, '=', endParam );
		if (!pszParms)
			break;

		pszParms++;					

		while (*pszParms == ' ' || *pszParms == '\t' || *pszParms == '\r' || *pszParms == '\n')
			pszParms++;


		const char* pszParmEnd = NULL;
		if (*pszParms == '"')		
			pszParmEnd = FindChar(pszParms+1, '"', endParam );
		if (!pszParmEnd)			
		{
			pszParmEnd = pszParms;
			while (*pszParmEnd == '.' || CMimeChar::IsToken(*pszParmEnd))
				pszParmEnd++;
		}
		else  pszParmEnd++;			

		if (!::_memicmp(pszAttr, pszName, nAttrSize) &&
			(CMimeChar::IsSpace((unsigned char)pszName[nAttrSize]) || pszName[nAttrSize] == '='))
		{
			nPos = (int)(pszParms - m_strValue.data());
			nSize = (int)(pszParmEnd - pszParms);
			return true;
		}

		pszParms = pszParmEnd;
	}
	return false;
}




CMimeHeader::MediaType CMimeHeader::GetMediaType() const
{
	const char* pszType = GetContentType();
	if (!pszType)
		pszType = "text";

	int nIndex = 0;
	while (m_TypeTable[nIndex] != NULL &&
		_strnicmp( pszType, m_TypeTable[nIndex], strlen(m_TypeTable[nIndex]) ) != 0 )
		nIndex++;
	return (MediaType) nIndex;
}


string CMimeHeader::GetMainType() const
{
	string strType;
	const char* pszType = GetContentType();
	if (pszType != NULL)
	{
		const char* pszSlash = ::strchr(pszType, '/');
		if (pszSlash != NULL)
			strType.assign(pszType, pszSlash-pszType);
		else
			strType = pszType;
	}
	else
		strType = "text";
	return strType;
}


string CMimeHeader::GetSubType() const
{
	string strSubType;
	const CMimeField *pfd = GetField(CMimeConst::ContentType());
	if (pfd != NULL)
	{
		string strType;
		pfd->GetValue(strType);
		string::size_type nSlash = strType.find('/');
		if (nSlash > 0)
			strSubType = strType.substr(nSlash+1);
	}
	else
		strSubType = "plain";
	return strSubType;
}

void CMimeHeader::SetCharset(const char* pszCharset)
{
	CMimeField *pfd = GetField(CMimeConst::ContentType());
	if (!pfd)
	{
		CMimeField fd;
		fd.SetName(CMimeConst::ContentType());
		fd.SetValue("text/plain");
		fd.SetParameter(CMimeConst::Charset(), pszCharset);
		m_listFields.push_back(fd);
	}
	else
		pfd->SetParameter(CMimeConst::Charset(), pszCharset);
}


void CMimeHeader::SetName(const char* pszName)
{
	CMimeField *pfd = GetField(CMimeConst::ContentType());
	if (!pfd)
	{
		ASSERT(pszName != NULL);
		string strType;
		const char* pszType = "application/octet-stream";
		const char* pszFileExt = ::strrchr(pszName, '.');
		if (pszFileExt != NULL)
		{
			pszFileExt++;
			int nIndex = 0;
			while (m_TypeCvtTable[nIndex].nMediaType != MEDIA_UNKNOWN)
			{
				if (!::_stricmp(pszFileExt, m_TypeCvtTable[nIndex].pszFileExt))
				{
					strType = m_TypeTable[m_TypeCvtTable[nIndex].nMediaType];
					strType += '/';
					strType += m_TypeCvtTable[nIndex].pszSubType;
					pszType = strType.c_str();
					break;
				}
				nIndex++;
			}
		}

		CMimeField fd;
		fd.SetName(CMimeConst::ContentType());
		fd.SetValue(pszType);
		fd.SetParameter(CMimeConst::Name(), pszName);
		m_listFields.push_back(fd);
	}
	else
		pfd->SetParameter(CMimeConst::Name(), pszName);
}


void CMimeHeader::SetBoundary(const char* pszBoundary)
{
	static int s_nPartNumber = 0;
	char buf[80];
	if (!pszBoundary)				
	{
		::srand(((unsigned)::time(NULL)) ^ (unsigned)(intptr_t)this);
		::sprintf_s(buf, "__=_Part_Boundary_%03d_%06d.%06d", ++s_nPartNumber, rand(), rand());
		if (s_nPartNumber >= 9)
			s_nPartNumber = 0;
		pszBoundary = buf;
	}

	CMimeField *pfd = GetField(CMimeConst::ContentType());
	if (!pfd)
	{
		CMimeField fd;
		fd.SetName(CMimeConst::ContentType());
		fd.SetValue("multipart/mixed");
		fd.SetParameter(CMimeConst::Boundary(), pszBoundary);
		m_listFields.push_back(fd);
	}
	else
	{
		string val;
		pfd->GetValue( val );
		if (_stricmp( val.c_str(), "multipart" ) != 0)
			pfd->SetValue("multipart/mixed");
		pfd->SetParameter(CMimeConst::Boundary(), pszBoundary);
	}
}

void CMimeHeader::Clear()
{
	m_listFields.clear();
}


int CMimeHeader::GetLength() const
{
	int nLength = 0;
	list<CMimeField>::const_iterator it;
	for (it = m_listFields.begin(); it != m_listFields.end(); it++)
		nLength += (*it).GetLength();
	return nLength + 2;			
}


int CMimeHeader::Store(char* pszData, int nMaxSize) const
{
	ASSERT(pszData != NULL);
	int nOutput = 0;
	list<CMimeField>::const_iterator it;
	for (it = m_listFields.begin(); it != m_listFields.end(); it++)
	{
		const CMimeField& fd = *it;
		int nSize = fd.Store(pszData+nOutput, nMaxSize-nOutput);
		if (nSize <= 0)
			return nSize;
		nOutput += nSize;
	}

	pszData[nOutput++] = '\r';		
	pszData[nOutput++] = '\n';
	return nOutput;
}


int CMimeHeader::Load(const char* pszData, int nDataSize)
{
	ASSERT(pszData != NULL);
	int nInput = 0;
	while (pszData[nInput] != 0 && pszData[nInput] != '\r')
	{
		CMimeField fd;
		int nSize = fd.Load(pszData+nInput, nDataSize-nInput);
		if (nSize <= 0)
			return nSize;

		nInput += nSize;
		m_listFields.push_back(fd);	
	}

	return (pszData[nInput] == '\r') ? nInput + 2 : nInput; 
}

list<CMimeField>::const_iterator CMimeHeader::FindField(const char* pszFieldName) const
{
	list<CMimeField>::const_iterator it;
	for (it = m_listFields.begin(); it != m_listFields.end(); it++)
	{
		const CMimeField& fd = *it;
		if (!::_stricmp(fd.GetName(), pszFieldName))
			break;
	}
	return it;
}

list<CMimeField>::iterator CMimeHeader::FindField(const char* pszFieldName)
{
	list<CMimeField>::iterator it;
	for (it = m_listFields.begin(); it != m_listFields.end(); it++)
	{
		CMimeField& fd = *it;
		if (!::_stricmp(fd.GetName(), pszFieldName))
			break;
	}
	return it;
}


#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <io.h>


int CMimeBody::SetText(const char* pbText, int nLength/*=0*/)
{
	ASSERT(pbText != NULL);
	if (!nLength)
		nLength = (int)::strlen((char*)pbText);

	if (!AllocateBuffer(nLength+4))
		return -1;

	::memcpy(m_pbText, pbText, nLength);
	m_pbText[nLength] = 0;
	m_nTextSize = nLength;
	return nLength;
}

int CMimeBody::GetText(char* pbText, int nMaxSize)
{
	int nSize = min(nMaxSize, m_nTextSize);
	if (m_pbText != NULL)
		::memcpy(pbText, m_pbText, nSize);
	return nSize;
}

int CMimeBody::GetText(string& strText)
{
	if (m_pbText != NULL)
		strText.assign((const char*) m_pbText, m_nTextSize);
	return m_nTextSize;
}


bool CMimeBody::SetMessage(const CMimeMessage* pMM)
{
	ASSERT(pMM != NULL);
	int nSize = pMM->GetLength();
	if (!AllocateBuffer(nSize+4))
		return false;

	nSize = pMM->Store((char*)m_pbText, nSize);
	m_pbText[nSize] = 0;
	m_nTextSize = nSize;

	const char* pszType = GetContentType();
	if (!pszType || ::_memicmp(pszType, "message", 7) != 0)
		SetContentType("message/rfc822");
	return true;
}

void CMimeBody::GetMessage(CMimeMessage* pMM) const
{
	ASSERT(pMM != NULL);
	ASSERT(m_pbText != NULL);
	pMM->Load((const char*)m_pbText, m_nTextSize);
}



bool CMimeBody::ReadFromFile(const char* pszFilename)
{
	FILE *hFile;

	hFile = ::fopen(pszFilename, "r+b");
	if (hFile == NULL )
		return false;

	try
	{
		int nFileSize;

		::fseek(hFile, 0L, SEEK_END); 
		fpos_t pos;
		if( ::fgetpos( hFile, &pos ) == 0 )
			nFileSize =pos;
		else
			nFileSize=0;
		::fseek(hFile, 0L, SEEK_SET);

		FreeBuffer();
		if (nFileSize > 0)
		{
			AllocateBuffer(nFileSize+1);

			int nRead = ::fread(m_pbText, 1, nFileSize, hFile);
			if ( nRead == 0 )
				return false;
			*(m_pbText+nFileSize) = 0;
			m_nTextSize = nFileSize;
		}
	}
	catch(...)
	{
		::fclose(hFile);
		throw;
	}

	::fclose(hFile);
	const char* pszName = ::strrchr(pszFilename, '\\');
	if (!pszName)
		pszName = pszFilename;
	else
		pszName++;
	SetName(pszName); 

	return true;
}

bool CMimeBody::WriteToFile(const char* pszFilename)
{
	if (!m_nTextSize)
		return true;
	int hFile;
	::_sopen_s(&hFile, pszFilename, O_CREAT | O_TRUNC | O_RDWR | O_BINARY, _SH_DENYRW , S_IREAD | S_IWRITE);
	if (hFile < 0)
		return false;

	const unsigned char* pszData = m_pbText;
	int nLeft = m_nTextSize;

	try
	{
		for (;;)
		{
			int nWritten = ::_write(hFile, pszData, min(512, nLeft));
			if (nWritten <= 0)
			{
				::_close(hFile);
				return false;
			}
			pszData += nWritten;
			nLeft -= nWritten;
			if (nLeft <= 0)
				break;
		}
	}
	catch (...)
	{
		::_close(hFile);
		throw;
	}

	::_close(hFile);
	return true;
}


void CMimeBody::DeleteAll()
{
	while (!m_listBodies.empty())
	{
		CMimeBody* pBP = m_listBodies.back();
		m_listBodies.pop_back();
		ASSERT(pBP != NULL);
		delete pBP;					
	}
}


CMimeBody* CMimeBody::CreatePart(const char* pszMediaType, CMimeBody* pWhere)
{
	CMimeBody* pBP = CMimeEnvironment::CreateBodyPart(pszMediaType);
	ASSERT(pBP != NULL);
	if (pWhere != NULL)
	{
		 for (CBodyList::iterator it = m_listBodies.begin(); it != m_listBodies.end(); it++)
			if (*it == pWhere)
			{
				m_listBodies.insert(it, pBP);
				return pBP;
			}
	}
	m_listBodies.push_back(pBP);
	return pBP;
}


void CMimeBody::ErasePart(CMimeBody* pBP)
{
	ASSERT(pBP != NULL);
	m_listBodies.remove(pBP);
	delete pBP;
}


int CMimeBody::GetBodyPartList(CBodyList& rList) const
{
	int nCount = 0;
	int nMediaType = GetMediaType();

	if (MEDIA_MULTIPART != nMediaType)
	{
		rList.push_back((CMimeBody*)this);
		nCount++;
	}
	else
	{
		list<CMimeBody*>::const_iterator it;
		for (it=m_listBodies.begin(); it!=m_listBodies.end(); it++)
		{
			CMimeBody* pBP = *it;
			ASSERT(pBP != NULL);
			nCount += pBP->GetBodyPartList(rList);
		}
	}
	return nCount;
}


int CMimeBody::GetAttachmentList(CBodyList& rList) const
{
	int nCount = 0;
	int nMediaType = GetMediaType();

	if (MEDIA_MULTIPART != nMediaType)
	{
		string strName = GetName();
		if (strName.size() > 0)
		{
			rList.push_back((CMimeBody*)this);
			nCount++;
		}
	}
	else
	{
		list<CMimeBody*>::const_iterator it;
		for (it=m_listBodies.begin(); it!=m_listBodies.end(); it++)
		{
			CMimeBody* pBP = *it;
			ASSERT(pBP != NULL);
			nCount += pBP->GetAttachmentList(rList);
		}
	}
	return nCount;
}

void CMimeBody::Clear()
{
	DeleteAll();
	m_itFind = m_listBodies.end();
	FreeBuffer();
	CMimeHeader::Clear();
}


int CMimeBody::GetLength() const
{
	int nLength = CMimeHeader::GetLength();
	CMimeCodeBase* pCoder = CMimeEnvironment::CreateCoder(GetTransferEncoding());
	ASSERT(pCoder != NULL);
	pCoder->SetInput((const char*)m_pbText, m_nTextSize, true);
	nLength += pCoder->GetOutputLength();
	delete pCoder;

	if (m_listBodies.empty())
		return nLength;

	string strBoundary = GetBoundary();
	int nBoundSize = (int) strBoundary.size();
	list<CMimeBody*>::const_iterator it;
	for (it=m_listBodies.begin(); it!=m_listBodies.end(); it++)
	{
		nLength += nBoundSize + 6;	
		CMimeBody* pBP = *it;
		ASSERT(pBP != NULL);
		nLength += pBP->GetLength();
	}
	nLength += nBoundSize + 8;	
	return nLength;
}


int CMimeBody::Store(char* pszData, int nMaxSize) const
{
	
	int nSize = CMimeHeader::Store(pszData, nMaxSize);
	if (nSize <= 0)
		return nSize;

	
	char* pszDataBegin = pszData;
	pszData += nSize;
	nMaxSize -= nSize;

	CMimeCodeBase* pCoder = CMimeEnvironment::CreateCoder(GetTransferEncoding());
	ASSERT(pCoder != NULL);
	pCoder->SetInput((const char*)m_pbText, m_nTextSize, true);
	int nOutput = pCoder->GetOutput((unsigned char*)pszData, nMaxSize);
	delete pCoder;
	if (nOutput < 0)
		return nOutput;

	pszData += nOutput;
	nMaxSize -= nOutput;
	if (m_listBodies.empty())
		return (int)(pszData - pszDataBegin);
	string strBoundary = GetBoundary();
	if (strBoundary.empty())
		return -1;				

	int nBoundSize = (int)strBoundary.size() + 6;
	for (CBodyList::const_iterator it=m_listBodies.begin(); it!=m_listBodies.end(); it++)
	{
		if (nMaxSize < nBoundSize)
			break;
		if (m_listBodies.begin() == it && *(pszData-2) == '\r' && *(pszData-1) == '\n')
		{
			pszData -= 2;
			nMaxSize += 2;
		}
		::sprintf_s(pszData, nMaxSize, "\r\n--%s\r\n", strBoundary.c_str());
		pszData += nBoundSize;
		nMaxSize -= nBoundSize;

		CMimeBody* pBP = *it;
		ASSERT(pBP != NULL);
		nOutput = pBP->Store(pszData, nMaxSize);
		if (nOutput < 0)
			return nOutput;
		pszData += nOutput;
		nMaxSize -= nOutput;
	}

	if (nMaxSize >= nBoundSize+2)	
	{
		::sprintf_s(pszData, nMaxSize, "\r\n--%s--\r\n", strBoundary.c_str());
		pszData += nBoundSize + 2;
	}
	return (int)(pszData - pszDataBegin);
}


int CMimeBody::Load(const char* pszData, int nDataSize)
{
	
	int nSize = CMimeHeader::Load(pszData, nDataSize);
	if (nSize <= 0)
		return nSize;

	const char* pszDataBegin = pszData;	
	pszData += nSize;
	nDataSize -= nSize;
	FreeBuffer();

	
	const char* pszEnd = pszData + nDataSize;
	int nMediaType = GetMediaType();
	if (MEDIA_MULTIPART == nMediaType)
	{
		string strBoundary = GetBoundary();
		if (!strBoundary.empty())
		{
			strBoundary = "\r\n--" + strBoundary;
			pszEnd = ::FindString(pszData-2, strBoundary.c_str(), pszEnd);
			if (!pszEnd)
				pszEnd = pszData + nDataSize;
			else
				pszEnd += 2;
		}
	}

	nSize = (int)(pszEnd - pszData);
	if (nSize > 0)
	{
		CMimeCodeBase* pCoder = CMimeEnvironment::CreateCoder(GetTransferEncoding());
		ASSERT(pCoder != NULL);
		pCoder->SetInput(pszData, nSize, false);
		int nOutput = pCoder->GetOutputLength();
		if (AllocateBuffer(nOutput+4))
			nOutput = pCoder->GetOutput(m_pbText, nOutput);
		else
			nOutput = -1;
		delete pCoder;
		if (nOutput < 0)
			return nOutput;

		ASSERT(nOutput < m_nTextSize);
		m_pbText[nOutput] = 0;
		m_nTextSize = nOutput;
		pszData += nSize;
		nDataSize -= nSize;
	}
	if (nDataSize <= 0)
		return (int)(pszData - pszDataBegin);

	string strBoundary = GetBoundary();
	ASSERT(strBoundary.size() > 0);
	strBoundary = "\r\n--" + strBoundary;

	pszData -= 2;				
	nDataSize += 2;
	pszEnd = pszData + nDataSize;
	const char* pszBound1 = ::FindString(pszData, strBoundary.c_str(), pszEnd);
	while (pszBound1 != NULL && pszBound1 < pszEnd)
	{
		const char* pszStart = ::FindString(pszBound1+2, "\r\n", pszEnd);
		if (!pszStart)
			break;
		pszStart += 2;
		if ( pszBound1 + strBoundary.size() + 2 < pszEnd )
			if (pszBound1[strBoundary.size()] == '-' && pszBound1[strBoundary.size()+1] == '-')
				return (int)(pszStart - pszDataBegin);	// reach the closing boundary

		
		const char* pszBound2 = ::FindString(pszStart, strBoundary.c_str(), pszEnd);
		if (!pszBound2)				
			pszBound2 = pszEnd;
		int nEntitySize = (int) (pszBound2 - pszStart);

		
		CMimeHeader header;
		header.Load(pszStart, nEntitySize);
		string strMediaType = header.GetMainType();
		CMimeBody* pBP = CreatePart(strMediaType.c_str());

		int nInputSize = pBP->Load(pszStart, nEntitySize);
		if (nInputSize < 0)
		{
			ErasePart(pBP);
			return nInputSize;
		}
		pszBound1 = pszBound2;
	}
	return (int)(pszEnd - pszDataBegin);
}



void CMimeMessage::SetDate()
{
	time_t timeNow = ::time(NULL);
	struct tm localtm;
	::localtime_s(&localtm, &timeNow);
	SetDate(localtm.tm_year+1900, localtm.tm_mon+1, localtm.tm_mday, localtm.tm_hour, localtm.tm_min, localtm.tm_sec);
}

void CMimeMessage::SetDate(int nYear, int nMonth, int nDay, int nHour, int nMinute, int nSecond)
{
	static const char* s_MonthNames[] =
		{ "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
	static const char* s_DayNames[] =
		{ "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };

	struct tm tmDate;
	::memset(&tmDate, 0, sizeof(tmDate));
	tmDate.tm_year = nYear - 1900;
	tmDate.tm_mon = nMonth - 1;
	tmDate.tm_mday = nDay;
	tmDate.tm_hour = nHour;
	tmDate.tm_min = nMinute;
	tmDate.tm_sec = nSecond;
	tmDate.tm_isdst = -1;

	time_t timeDate = ::mktime(&tmDate);
	if (timeDate < 0)
	{
		ASSERT(false);
		return;
	}

	::localtime_s(&tmDate, &timeDate);			
	struct tm tmGmt;
	::gmtime_s(&tmGmt, &timeDate);	
	long nTimeDiff = tmDate.tm_mday - tmGmt.tm_mday;
	if (nTimeDiff > 1)
		nTimeDiff = -1;
	else if (nTimeDiff < -1)
		nTimeDiff = 1;
	nTimeDiff *= 60 * 24;
	nTimeDiff +=
		(tmDate.tm_hour - tmGmt.tm_hour) * 60 +
		tmDate.tm_min - tmGmt.tm_min;
	if (tmDate.tm_isdst > 0)
		nTimeDiff -= 60;

	char szDate[40];
	ASSERT(tmDate.tm_wday < 7);
	ASSERT(tmDate.tm_mon < 12);
	::sprintf_s(szDate, "%s, %d %s %d %02d:%02d:%02d %c%02d%02d",
		s_DayNames[tmDate.tm_wday],
		tmDate.tm_mday, s_MonthNames[tmDate.tm_mon], tmDate.tm_year+1900,
		tmDate.tm_hour, tmDate.tm_min, tmDate.tm_sec,
		(nTimeDiff >= 0 ? '+' : '-'), abs(nTimeDiff / 60), abs(nTimeDiff % 60));

	SetFieldValue("Date", szDate);
}
CString CMimeMessage::ConvertToString()
{
	CMimeEnvironment::SetAutoFolding(true); 
	int nSize = this->GetLength();
	char* pBuff = new char[nSize];
	nSize = this->Store(pBuff, nSize);

	return pBuff;
	//textBody = pBuff;
}