#include "ControlConnection.h"
#include "Server.h"

namespace ftp {
	control_connection::control_connection(socket && sock, server & srv, port_t dataport) : 
		socket_(std::move(sock)), 
		data_(dataport), 
		working_(false), 
		srv_(srv), 
		login_(server::DefaultLogin)
	{ }

	control_connection::control_connection(control_connection && rhs) : 
		socket_(std::move(rhs.socket_)), 
		thread_(std::move(rhs.thread_)), 
		data_(std::move(rhs.data_)), 
		working_(rhs.working_.load()), 
		srv_(rhs.srv_),
		login_(server::DefaultLogin)
	{ }

	control_connection::~control_connection() {
		if (is_working()) {
			stop_processing_loop();
		}
		if (thread_.joinable()) {
			thread_.join();
		}	
	}

	void control_connection::start_processing_loop() {
		working_.store(true);
		thread_ = std::thread { &control_connection::processing_loop, this };
	}

	void control_connection::stop_processing_loop() {
		working_.store(false);
	}

	void control_connection::processing_loop() {
		buffer_t buffer { };
		size_t empty_count = 0;

		while (is_working()) 
		{
			size_t received = socket_.receive(
				buffer.data(), buffer.size()
			);

			if (received == size_t(0)) {
				++empty_count;
			}

			if ((received == socket::InvalidBytesCount) || 
				(empty_count > MaxEmptyCount)) 
			{
				stop_processing_loop();
			}
			else 
			{
				process_command(buffer, received);
			}
		}
	}

	void control_connection::process_command(const buffer_t & buffer, size_t size) {
		handler_t handler = &control_connection::invalid_command;

		if (std::strcmp(buffer.data(), "QUIT") == 0) {
			handler = &control_connection::close_control;
		}
		if (std::strstr(buffer.data(), "USER") == buffer.data()) {
			handler = &control_connection::login;
		}
		if (std::strstr(buffer.data(), "PASS") == buffer.data()) {
			handler = &control_connection::password;
		}
		if (std::strcmp(buffer.data(), "PASV") == 0) {
			handler = &control_connection::open_data;
		}
		if (std::strcmp(buffer.data(), "LIST") == 0) {
			handler = &control_connection::list;
		}
		if (std::strstr(buffer.data(), "RECV") == buffer.data()) {
			handler = &control_connection::recv_file;
		}
		if (std::strstr(buffer.data(), "STOR") == buffer.data()) {
			handler = &control_connection::send_file;
		}

		resp_t responce = (this->*handler)(buffer);
		socket_.send(responce.first.data(), responce.second);
	}

	control_connection::resp_t control_connection::invalid_command(const buffer_t & arg) {
		return {
			{ "ERROR: Command was invalid." }, 28 
		};
	}

	control_connection::resp_t control_connection::close_control(const buffer_t &) {
		stop_processing_loop();

		return {
			{ "OK: Closing control connection. Goodbye." }, 41
		};
	}

	control_connection::resp_t control_connection::open_data(const buffer_t & ) {
		data_.reopen();

		socket::addr_t ip = utill::MachineIp();
		port_t port = data_.port();

		return { 
			{	
				(char)(ip >> 24) & 255, (char)(ip >> 16) & 255, (char)(ip >> 8) & 255,
				(char)ip & 255, (char)(port >> 8) & 255, (char)port & 255
			}, 6
		};
	}

	control_connection::resp_t control_connection::data_access_check(bool & valid) {
		valid = false;

		if (!data_.is_opened())
		{
			return {
				{ "ERROR: Data connection closed. PASV command must be the first." }, 63
			};
		}

		if (!authorized_) {
			return {
				{ "ERROR: Neet to be authorized." }, 30
			};
		}

		valid = true;

		return {
			{ "OK: Sending data..." }, 20
		};
	}

	control_connection::resp_t control_connection::login(const buffer_t & arg) {
		authorized_ = false;

		std::string login = std::string(
			std::find(arg.begin(), arg.end(), ' '),
			arg.end()
		);

		// Добавить \ авторизовать пользователя

		return { { "OK: User logged. Need password." }, 32 };
	}

	control_connection::resp_t control_connection::password(const buffer_t & arg) {
		std::string password = std::string(
			std::find(arg.begin(), arg.end(), ' '),
			arg.end()
		);

		// Проверить пароль

		return {
			{ "OK: Password confirmed. Authorized." }, 36
		};
	}

	control_connection::resp_t control_connection::list(const buffer_t & ) {
		bool check_passed;
		resp_t responce = data_access_check(check_passed);

		if (check_passed)
		{
			socket_.send(responce.first.data(), responce.second);

			data_.send_files(
				srv_.get_list(login_)
			);

			return {
				{ "OK: List of files was send." }, 28
			};
		}
		return responce;
	}

	control_connection::resp_t control_connection::send_file(const buffer_t & arg) {
		bool check_passed;
		resp_t responce = data_access_check(check_passed);

		if (check_passed)
		{
			socket_.send(responce.first.data(), responce.second);

			std::string filename = std::string(
				std::find(arg.begin(), arg.end(), ' '),
				arg.end()
			);

			data_.send_file(
				srv_.get_file(login_, filename)
			);

			return {
				{ "OK: File was send." }, 19
			};
		}
		return responce;
	}

	control_connection::resp_t control_connection::recv_file(const buffer_t & arg) {
		bool check_passed;
		resp_t responce = data_access_check(check_passed);

		if (check_passed) 
		{
			socket_.send(responce.first.data(), responce.second);
			
			std::string filename = std::string(
				std::find(arg.begin(), arg.end(), ' '),
				arg.end()
			);

			data_.recv_file(
				srv_.create_file(login_, filename)
			);

			return {
				{ "OK: File was uploaded." }, 23
			};
		}
		return responce;
	}
}