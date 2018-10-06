#ifndef _YS_RESPONSE_H_
#define _YS_RESPONSE_H_

namespace ys
{
	class response
	{
	public:
		void init();

		const std::string& msg() const;
		void msg(const std::string& val);

		const std::string& full_msg() const;
		void full_msg(const std::string& val);

		const std::uint16_t status_code() const;
		void status_code(const std::uint16_t val);

		const std::string& phrase() const;
		void phrase(const std::string& val);

		void append_buffer(send_buffer &_send_buffer, std::size_t &length);

	protected:
		std::string   _full_msg;

		std::string   _msg;
		std::uint16_t _status_code;
		std::string   _phrase;
	};

	class response_ext
	{
	public:
		response_ext(std::shared_ptr<response> _response_p);

		const std::shared_ptr<response> response_item() const;

		void build();

		void append_buffer(send_buffer &_send_buffer, std::size_t &length);

	private:
		std::shared_ptr<response> _response_p;
	};
}

#endif
