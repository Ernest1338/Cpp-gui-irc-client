#include <iostream>
#include <deque>
#include <boost/asio.hpp>
#include "irc.h"

using boost::asio::ip::tcp;

int main(int argc, char* argv[]) {
    try {
        if(argc!=3) {
            std::cerr << "Usage: " << argv[0] << " <host> <port>\n";
            return 1;
        }
        boost::asio::io_context io_context;

        boost::asio::ip::tcp::resolver resolver(io_context);
        tcp::resolver::results_type endpoints = resolver.resolve(argv[1], argv[2]);
        chat_client c(io_context, endpoints);
        std::thread t([&io_context](){ io_context.run(); });
        char line[chat_message::max_body_length + 1];

        while (std::cin.getline(line, chat_message::max_body_length + 1)) {
            chat_message msg;
            msg.body_length(std::strlen(line));
            std::memcpy(msg.body(), line, msg.body_length());
            msg.encode_header();
            c.write(msg);
        }

        c.close();
        t.join();

    } catch(std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}