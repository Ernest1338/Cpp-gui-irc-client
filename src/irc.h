#ifndef IRC_H
#define IRC_H

#include <boost/asio.hpp>

class Session : public std::enable_shared_from_this<Session> {
    public:
        boost::asio::ip::tcp::socket socket_;
        Session(boost::asio::ip::tcp::socket socket);

    void start();

    private:
        void do_read();

    void do_write(std::size_t length);
};

class Server {
    public:
        boost::asio::ip::tcp::acceptor acceptor_;
        Server(boost::asio::io_context& io_context, short port);

    private:
        void do_accept();
};

#endif
