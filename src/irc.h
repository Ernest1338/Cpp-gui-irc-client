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

        chat_message();                                       // done
        const char* data() const;                             // done
        char* data();                                         // done
        std::size_t length() const;                           // done
        const char* body() const;                             // done
        char* body();                                         // done
        std::size_t body_length() const;                      // done
        void body_length(std::size_t new_length);             // done
        bool decode_header();                                 // done
        void encode_header();                                 // done

    private:
        char data_[header_length + max_body_length];
        std::size_t body_length_;
};

typedef std::deque<chat_message> chat_message_queue;

class chat_client {
    public:
        chat_client(boost::asio::io_context& io_context, const tcp::resolver::results_type& endpoints);                                        // done
        void write(const chat_message& msg);                  // done
        void close();                                         // done
    
    private:
        void do_connect(const tcp::resolver::results_type& endpoints);                                    // done
        void do_read_header();                                // done
        void do_read_body();                                  // done
        void do_write();                                      // done
        boost::asio::io_context& io_context_;
        tcp::socket socket_;
        chat_message read_msg_;
        chat_message_queue write_msgs_;
};

#endif
