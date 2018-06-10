#ifndef _CHATROOM_
#define _CHATROOM_


#include <cstdlib>  
#include <deque>  
#include <iostream>  
#include <boost/bind.hpp>  
#include <boost/asio.hpp>  
#include <boost/thread.hpp>  
#include "chat_message.hpp"  
#include "MagicTower.h"
using boost::asio::ip::tcp;
using namespace std;
typedef std::deque<chat_message> chat_message_queue;
USING_NS_CC;
class chat_client;
class ChatRoom : public Scene
{
public:
	ChatRoom();
	~ChatRoom();
	virtual bool init();
	CREATE_FUNC(ChatRoom);
	enum {
		kZBGLayer,
		kZMSGBox,
		kZBarrier1,
		kZBarrier2,
		kZInputBoxBGLayer,
		kZEditBox,
		kZScrollLayer
	};
	void addLabel(Label* label);
	void boot_client();
	virtual bool onTouchBegan(Touch* touch, Event* ev);
	void sendMsg();
	void createTextField();
	Label* createMSGLabel(std::string str);
	
	void rollUpWhenNewMsg();
	void createScrollLayerIfitHasNotBeenCreated();
	bool onMouseDown(Touch* touch, Event* ev);
	void onMouseUp(Touch* touch, Event* ev);
	void moveUp(float dt);
	void moveDown(float dt);
private:
	chat_client* client;
	boost::thread t;
	TextFieldTTF* textEdit;
	Layer* BGLayer;
	Layer* Barrier1;
	Layer* Barrier2;
	bool ScrollLayerHasBeenCreated = false;
	Layer* buttonLayer1;
	Layer* buttonLayer2;

	bool isTouchingScroller;//标记是否点击着滚动条
	std::list<Label*> LabelList;//存放label的List
	float chatRoomLength;//聊天室总长
	float MessageLength;//消息加起来的总长
	float buttonLength = 25;//向上及向下按钮的宽与长
	float ScrollerLength;//滚动条的长度

	tcp::resolver* resolver;
	tcp::resolver::query* query; // ip port:本机  
};

class chat_client : public Node
{
public:
	bool init();
	chat_client(boost::asio::io_service& io_service, // 1  
		tcp::resolver::iterator endpoint_iterator)
		: io_service_(io_service),
		socket_(io_service)
	{
		cout << __FUNCTION__ << endl;
		tcp::endpoint endpoint = *endpoint_iterator;
		socket_.async_connect(endpoint,
			boost::bind(&chat_client::handle_connect, this, // 2  
				boost::asio::placeholders::error, ++endpoint_iterator));
	}

	void write(const chat_message& msg) // 5  
	{
		cout << __FUNCTION__ << endl;
		io_service_.post(boost::bind(&chat_client::do_write, this, msg));
	}

	void close()
	{
		cout << __FUNCTION__ << endl;
		io_service_.post(boost::bind(&chat_client::do_close, this));
	}

private:

	void handle_connect(const boost::system::error_code& error, // 2  
		tcp::resolver::iterator endpoint_iterator)
	{
		cout << __FUNCTION__ << endl;
		if (!error)
		{
			boost::asio::async_read(socket_,
				boost::asio::buffer(read_msg_.data(), chat_message::header_length),  //copy buffer to read_msg_'s header  
				boost::bind(&chat_client::handle_read_header, this, // 3  
					boost::asio::placeholders::error));
		}
		else if (endpoint_iterator != tcp::resolver::iterator())
		{
			socket_.close();
			tcp::endpoint endpoint = *endpoint_iterator;
			socket_.async_connect(endpoint,
				boost::bind(&chat_client::handle_connect, this, // 2  
					boost::asio::placeholders::error, ++endpoint_iterator));
		}
	}

	void handle_read_header(const boost::system::error_code& error) // 3  
	{
		cout << __FUNCTION__ << endl;
		if (!error && read_msg_.decode_header())
		{
			boost::asio::async_read(socket_,
				boost::asio::buffer(read_msg_.body(), read_msg_.body_length()), //copy buffer to read_msg_'s body  
				boost::bind(&chat_client::handle_read_body, this, // 4  
					boost::asio::placeholders::error));
		}
		else
		{
			do_close();
		}
	}

	void handle_read_body(const boost::system::error_code& error);
	

	void do_write(chat_message msg) // 6  
	{
		cout << __FUNCTION__ << endl;
		bool write_in_progress = !write_msgs_.empty();
		write_msgs_.push_back(msg);
		if (!write_in_progress)
		{
			boost::asio::async_write(socket_,
				boost::asio::buffer(write_msgs_.front().data(),
					write_msgs_.front().length()), // copy write_msgs_.front() to buffer  
				boost::bind(&chat_client::handle_write, this, // 7 send message  
					boost::asio::placeholders::error));
		}
	}

	void handle_write(const boost::system::error_code& error) // 7  
	{
		cout << __FUNCTION__ << endl;
		if (!error)
		{
			write_msgs_.pop_front();
			if (!write_msgs_.empty())
			{
				boost::asio::async_write(socket_,
					boost::asio::buffer(write_msgs_.front().data(),
						write_msgs_.front().length()),
					boost::bind(&chat_client::handle_write, this, // 7  
						boost::asio::placeholders::error));
			}
		}
		else
		{
			do_close();
		}
	}

	void do_close()
	{
		cout << __FUNCTION__ << endl;
		socket_.close();
	}

private:
	boost::asio::io_service& io_service_;
	tcp::socket socket_;
	chat_message read_msg_;  // 存从buffer读出的数据  
	chat_message_queue write_msgs_; // 欲写入buffer的数据队列，deque  
};

#endif
