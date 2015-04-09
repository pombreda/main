#ifndef BASIS_SYS_SYNC_MESSAGE_HPP_
#define BASIS_SYS_SYNC_MESSAGE_HPP_

namespace sync
{
	using Message = simstd::unique_ptr<message::SimpleMessage>;

	template<typename MessageType, typename... Args>
	Message create_message(Args&&... args)
	{
		return simstd::make_unique<MessageType>(simstd::forward<Args>(args)...);
	}

	inline Message create_message(const message::param_type& type,
		const message::param_type& a = message::param_type(),
		const message::param_type& b = message::param_type(),
		const message::param_type& c = message::param_type())
	{
		return create_message<message::SimpleMessage>(type, a, b, c);
	}

	namespace message
	{
		class SimpleMessage
		{
		public:
			virtual ~SimpleMessage() = default;

			virtual Message clone() const
			{
				return create_message<SimpleMessage>(type, a, b, c);
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
			SimpleMessage(const param_type& type, const param_type& a, const param_type& b, const param_type& c) noexcept : type(type), a(a), b(b), c(c) {}

		private:
			SimpleMessage(const SimpleMessage&) = delete;
			SimpleMessage& operator =(const SimpleMessage&) = delete;

			param_type type;
			param_type a;
			param_type b;
			param_type c;

			template<typename Type, typename... Args>
			friend simstd::unique_ptr<Type> simstd::make_unique(Args&&... args);
		};
	}
}

#endif
