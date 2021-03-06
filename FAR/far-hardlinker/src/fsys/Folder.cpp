#include <fsys.hpp>
#include <global.hpp>

#include <basis/sys/logger.hpp>

namespace fsys {

	Folder::~Folder()
	{
		LogTraceObjLn();
		LogTrace(L"'%s'\n", m_name.c_str());
		LogTrace2If(m_parent, L"'%s'\n", get_full_path().c_str());
		++global::statistics().folderObjectsDestroyed;
	}

	Folder::Folder(const ustring& name) :
		Node(name, Node_t(nullptr))
	{
		LogTraceObjLn();
		LogTrace(L"'%s'\n", m_name.c_str());
		++global::statistics().folderObjectsCreated;
	}

	Folder::Folder(const ustring& name, Node_t parent) :
		Node(name, parent)
	{
		LogTraceObjLn();
		LogTrace(L"'%s'\n", m_name.c_str());
		++global::statistics().folderObjectsCreated;
	}

}
