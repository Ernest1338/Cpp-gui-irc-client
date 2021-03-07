#include <boost/asio.hpp>
#include "irc.h"

enum { max_length = 1024 };
char data_[max_length];

Session::Session(boost::asio::ip::tcp::socket socket): socket_(std::move(socket)) {}

void Session::start() {
    Session::do_read();
}

void Session::do_read() {
    auto self(shared_from_this());
    socket_.async_read_some(boost::asio::buffer(data_, max_length),
        [this, self](boost::system::error_code ec, std::size_t length) {
        if (!ec) {
            Session::do_write(length);
        }});
}

void Session::do_write(std::size_t length) {
    auto self(shared_from_this());
    std::string hello_str = "Hello, you typed: ";
    boost::asio::async_write(socket_, boost::asio::buffer(hello_str, hello_str.size()),
        [this, self](boost::system::error_code ec, std::size_t /*length*/) {
        if (!ec) {
            Session::do_read();
        }});
    boost::asio::async_write(socket_, boost::asio::buffer(data_, length),
        [this, self](boost::system::error_code ec, std::size_t /*length*/) {
        if (!ec) {
            Session::do_read();
        }});
}

Server::Server(boost::asio::io_context& io_context, short port): 
    acceptor_(io_context, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)) {
        Server::do_accept();
}

void Server::do_accept() {
    acceptor_.async_accept(
    [this](boost::system::error_code ec, boost::asio::ip::tcp::socket socket) {
    if (!ec) {
        std::make_shared<Session>(std::move(socket))->start();
    }

    Server::do_accept();

    });
}
