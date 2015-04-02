#include <excis/crypt.hpp>
#include <excis/exception.hpp>

#include <basis/sys/logger.hpp>
#include <basis/simstd/algorithm>

namespace crypt {

	Hash::~Hash()
	{
		LogTraceObj(L"begin\n");
		if (m_handle)
			::CryptDestroyHash(m_handle);
		LogTraceObj(L"end\n");
	}

	Hash::Hash(HCRYPTPROV prov, ALG_ID alg)
	{
		LogTraceObj(L"begin\n");
		CheckApi(::CryptCreateHash(prov, alg, 0, 0, &m_handle));
		LogTraceObj(L"end\n");
	}

	Hash::Hash(HCRYPTHASH hash)
	{
		LogTraceObj(L"begin\n");
		CheckApi(::CryptDuplicateHash(hash, nullptr, 0, &m_handle));
		LogTraceObj(L"end\n");
	}

//		Hash::Hash(const Fsys::File::Facade & wf, uint64_t size) :
//			m_handle((HCRYPTHASH)nullptr)
//		{
//			Hash(Fsys::File::Map(wf, size)).swap(*this);
//		}
//
//		Hash::Hash(const Fsys::File::Map & file) :
//			m_handle((HCRYPTHASH)nullptr)
//		{
//			bool ret = true;
//			for (auto it = file.begin(); ret && it != file.end(); ++it) {
//				ret = ::CryptHashData(m_handle, (CONST BYTE *)it.data(), (DWORD)it.size(), 0);
//			}
//			CheckApi(ret);
//		}

	Hash::Hash(const Hash & other)
	{
		CheckApi(::CryptDuplicateHash(other, nullptr, 0, &m_handle));
	}

	Hash & Hash::operator=(const Hash & other)
	{
		if (this != &other)
			Hash(other).swap(*this);
		return *this;
	}

	void Hash::process(const void * buf, size_t size)
	{
		CheckApi(::CryptHashData(m_handle, (PBYTE )buf, size, 0));
	}

	size_t Hash::get_size() const
	{
		DWORD ret = 0;
		DWORD ret_size = sizeof(ret);
		CheckApi(::CryptGetHashParam(m_handle, HP_HASHSIZE, (PBYTE)&ret, &ret_size, 0));
		LogDebug(L"-> %u\n", ret);
		return ret;
	}

	ALG_ID Hash::get_algorithm() const
	{
		DWORD ret = 0;
		CheckApi(::CryptGetHashParam(m_handle, HP_ALGID, nullptr, &ret, 0));
		LogDebug(L"-> %u\n", ret);
		return ret;
	}

	void Hash::get_hash(void * buf, size_t size) const
	{
		DWORD l_size = static_cast<DWORD>(size);
		CheckApi(::CryptGetHashParam(m_handle, HP_HASHVAL, (PBYTE)buf, &l_size, 0));
	}

	void Hash::swap(Hash & rhs)
	{
		using simstd::swap;
		swap(m_handle, rhs.m_handle);
	}

}
