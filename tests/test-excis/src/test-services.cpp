#include <tests.hpp>

#include <basis/configure.hpp>
#include <basis/sys/logger.hpp>
#include <basis/sys/traceback.hpp>
#include <basis/simstd/string>

#include <excis/exception.hpp>
#include <excis/service.hpp>

struct ServicesView:
	public sync::Observer
{
	~ServicesView()
	{
		LogTraceObjLn();
	}

	ServicesView(service::Enum* enumSvc):
		m_svcs(enumSvc)
	{
		LogTraceObjLn();
		m_svcs->register_observer(this);
	}

	void notify(const sync::Message& event) {
		UNUSED(event);
		LogReport(L"Services changed. type: 0x%X, '%s', size: %Iu\n", (unsigned)m_svcs->get_type(), m_svcs->get_host().c_str(), m_svcs->size());

		for (service::Enum::size_type i = 0; i < m_svcs->size(); ++i) {
			LogReport(L"svc[%Iu] name: '%s'\n", i, (*m_svcs)[i].name.c_str());
		}
	}

private:
	service::Enum* m_svcs;
};

void test_service()
{
	LogAtten(L"\n");

	service::Enum svcs(service::EnumerateType::SERVICES);

	LogTraceLn();
	ServicesView obs(&svcs);

	LogTraceLn();
	svcs.set_type(service::EnumerateType::SERVICES);

	LogTraceLn();
	svcs.set_type(service::EnumerateType::DRIVERS);

	svcs.start_batch();
	LogTraceLn();
	svcs.set_type(service::EnumerateType::SERVICES);
	svcs.set_host(L"localhost");
	svcs.stop_batch();

	auto name = L"!tst-Svc";
	try {
		LogTraceLn();
		service::CreateRequest create(name, L"qwerty");
		svcs.add(create);
	} catch (exception::Abstract& e) {
		auto it = svcs.find(name);
		if (it != svcs.end())
			svcs.del(it);
	}

	LogTraceLn();
}
