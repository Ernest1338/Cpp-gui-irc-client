#include <iostream>
#include <deque>
#include <boost/asio.hpp>
#include "irc.h"

using boost::asio::ip::tcp;

chat_message::chat_message(): body_length_(0) {}

chat_client::chat_client(boost::asio::io_context& io_context,
      const tcp::resolver::results_type& endpoints)
    : io_context_(io_context),
      socket_(io_context) {
        do_connect(endpoints);
}

const char* chat_message::data() const {
    return data_;
}

char* chat_message::data() {
    return data_;
}

std::size_t chat_message::length() const {
    //return header_length + body_length_;
    return body_length_;
}

const char* chat_message::body() const {
    //return data_ + header_length;
    return data_;
}

char* chat_message::body() {
    //return data_ + header_length;
    return data_;
}

std::size_t chat_message::body_length() const {
    return body_length_;
}

void chat_message::body_length(std::size_t new_length) {
    body_length_ = new_length;
    if(body_length_ > max_body_length) {
        body_length_ = max_body_length;
    }
}

bool chat_message::decode_header() {
    char header[header_length + 1] = "";
    std::strncat(header, data_, header_length);
    body_length_ = std::atoi(header);
    if(body_length_ > max_body_length) {
        body_length_ = 0;
        return false;
    }
    return true;
}

// Unused function providing a way to create a message header (in current state it adds message length at the beggining)
void chat_message::encode_header() {
    char header[header_length + 1] = "";
    std::sprintf(header, "%4d", static_cast<int>(body_length_));
    std::memcpy(data_, header, header_length);
}

void chat_client::write(const chat_message& msg) {
    boost::asio::post(io_context_,
        [this, msg]() {
            bool write_in_progress = !write_msgs_.empty();
            write_msgs_.push_back(msg);
            if (!write_in_progress) {
                do_write();
            }
    });
}

void chat_client::close() {
    boost::asio::post(io_context_, [this]() { socket_.close(); });
}

void chat_client::do_connect(const tcp::resolver::results_type& endpoints) {
    boost::asio::async_connect(socket_, endpoints,
        [this](boost::system::error_code ec, tcp::endpoint) {
            if (!ec) {
                do_read_header();
            }
    });
}

void chat_client::do_read_header() {
    boost::asio::async_read(socket_,
        boost::asio::buffer(read_msg_.data(), chat_message::header_length),
        [this](boost::system::error_code ec, std::size_t /*length*/) {
            if (!ec && read_msg_.decode_header()) {
                do_read_body();
            } else {
                socket_.close();
            }
    });
}

void chat_client::do_read_body() {
    boost::asio::async_read(socket_,
        boost::asio::buffer(read_msg_.body(), read_msg_.body_length()),
        [this](boost::system::error_code ec, std::size_t /*length*/) {
            if (!ec) {
                std::cout.write(read_msg_.body(), read_msg_.body_length());
                std::cout << "\n";
                do_read_header();
            } else {
                socket_.close();
            }
    });
}

void chat_client::do_write() {
    boost::asio::async_write(socket_,
        boost::asio::buffer(write_msgs_.front().data(),
          write_msgs_.front().length()),
        [this](boost::system::error_code ec, std::size_t /*length*/) {
            if (!ec) {
                write_msgs_.pop_front();
                if (!write_msgs_.empty()) {
                    do_write();
                }
            } else {
                socket_.close();
            }
    });
}
