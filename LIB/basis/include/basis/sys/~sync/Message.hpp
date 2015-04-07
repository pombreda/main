#ifndef BASIS_SYS_SYNC_MESSAGE_HPP_
#define BASIS_SYS_SYNC_MESSAGE_HPP_

namespace sync
{
	using Message = simstd::unique_ptr<MessageI>;

	template<typename MessageType, typename... Args>
	Message create_message(Args&&... args)
	{
		return simstd::make_unique<MessageType>(simstd::forward<Args>(args)...);
	}

	class MessageI
	{
	public:
		using param_type = size_t;

		static const param_type MASK_ALL_TYPES = ~static_cast<param_type>(0xFF);
		static const param_type MASK_TYPE = static_cast<param_type>(0xFF);

		enum Type {
			SYSTEM    = 0x01,
			SCHEDULER = 0x02,
		};

		enum Code {
			SYSTEM_EXIT      = 0x01,
			SCHEDULER_EXIT   = 0x01,
			SCHEDULER_UPDATE = 0x02,
		};

		virtual ~MessageI() = default;

		virtual Message clone() const
		{
			return create_message<MessageI>(type, a, b, c);
		}

		param_type get_type() const noexcept {return type;}
		param_type get_a() const noexcept {return a;}
		param_type get_b() const noexcept {return b;}
		param_type get_c() const noexcept {return c;}

		void set_type(const param_type& type) noexcept {this->type = type;}
		void set_a(const param_type& a) noexcept {this->a = a;}
		void set_b(const param_type& b) noexcept {this->b = b;}
		void set_c(const param_type& c) noexcept {this->c = c;}

	protected:
		MessageI(const param_type& type, const param_type& a, const param_type& b, const param_type& c) noexcept : type(type), a(a), b(b), c(c) {}

	private:
		param_type type;
		param_type a;
		param_type b;
		param_type c;

		template<typename Type, typename... Args>
		friend simstd::unique_ptr<Type> simstd::make_unique(Args&&... args);
	};

	inline Message create_message(const MessageI::param_type& type,
		const MessageI::param_type& a = MessageI::param_type(),
		const MessageI::param_type& b = MessageI::param_type(),
		const MessageI::param_type& c = MessageI::param_type())
	{
		return create_message<MessageI>(type, a, b, c);
	}
}

#endif
