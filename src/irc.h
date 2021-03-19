#ifndef IRC_H
#define IRC_H

#include <iostream>
#include <deque>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class chat_message {
    public:
        enum { header_length = 4 };
        enum { max_body_length = 512 };

        chat_message();
        const char* data() const;
        char* data();
        std::size_t length() const;
        const char* body() const;
        char* body();
        std::size_t body_length() const;
        void body_length(std::size_t new_length);
        bool decode_header();
        void encode_header();

    private:
        //char data_[header_length + max_body_length];
        char data_[max_body_length];
        std::size_t body_length_;
};

typedef std::deque<chat_message> chat_message_queue;

class chat_client {
    public:
        chat_client(boost::asio::io_context& io_context, const tcp::resolver::results_type& endpoints);
        void write(const chat_message& msg);
        void close();
    
    private:
        void do_connect(const tcp::resolver::results_type& endpoints);
        void do_read_header();
        void do_read_body();
        void do_write();
        boost::asio::io_context& io_context_;
        tcp::socket socket_;
        chat_message read_msg_;
        chat_message_queue write_msgs_;
};

#endif
