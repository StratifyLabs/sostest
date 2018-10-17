#include <sapi/inet.hpp>
#include <sapi/var.hpp>
#include <sapi/sys.hpp>
#include <sapi/chrono.hpp>

#include "SocketTest.hpp"

SocketTest::SocketTest() : Test("SocketTest"){}


bool SocketTest::execute_class_api_case(){

	if( !execute_socket_address_info_case() ){
		print_case_failed("socket address info case failed");
		return case_result();
	}

	if( !execute_socket_case() ){
		print_case_failed("socket case failed");
		return case_result();
	}

	return case_result();
}
/*@brief start thread for listen and connect to them if needed
 * */
bool SocketTest::execute_socket_case(){

	//start the listener
	Thread thread;

	if( thread.create(listen_on_localhost_thread_function, this) < 0 ){
		print_case_failed("Failed to create listener thread");
		return case_result();
	}

	Timer::wait_milliseconds(1000);
/*  start tcp connection handing   */
	Vector<SocketAddressInfo> list = SocketAddressInfo().fetch_node("localhost");

	if( list.count() == 0 ){
		print_case_failed("Failed to fetch node");
		return case_result();
	}

	Socket local_host_socket;
	//SocketAddress localhost_address(SocketAddressIpv4(0, 8080));
	SocketAddress localhost_address(list.at(0), 8080);

	if( local_host_socket.create(localhost_address) < 0){
		print_case_failed("Failed to create client socket");
		return case_result();
	}

	if( local_host_socket.connect(localhost_address) < 0 ){
		print_case_failed("Failed to connect to socket");
		return case_result();
	}

	String test("Testing");
	Data reply(256);

	if( local_host_socket.write(test) < 0 ){
		print_case_failed("Failed to write client socket (%d, %d)", test.size(), local_host_socket.error_number());
		perror("failed to write");
		return case_result();
	}

	reply.fill(0);
	if( local_host_socket.read(reply) < 0 ){
		print_case_failed("Failed to read client socket");
		return case_result();
	}

	print_case_message("read '%s' from socket", reply.to_char());

	if( test != reply.to_char() ){
		print_case_failed("did not get an echo on localhost");
	}

	local_host_socket.close();
/*  end tcp connection  */
    /*if( thread.is_running() ){*///need to use on embedded
    {
/*      start udp connection handing
 *      use two sockets for write and read message
*/
        udp_server_listening = 0 ;
        SocketAddressInfo address_info(SocketAddressInfo::FAMILY_INET,SocketAddressInfo::TYPE_DGRAM,\
                                       SocketAddressInfo::PROTOCOL_UDP,AI_PASSIVE);
        list.free();
        list.clear();   //need to use before?
        list = address_info.fetch_node("localhost");
        u16 udp_port_client = 5002;
        u16 udp_port_server = 5003;
        Socket local_host_socket_udp_client;
        Socket local_host_socket_udp_server;

        SocketAddress localhost_udp_address_client(list.at(0), udp_port_client);
        SocketAddress localhost_udp_address_server(list.at(0), udp_port_server);
        if(localhost_udp_address_client.port()!=udp_port_client){
            print_case_failed("port mismatch");
            return case_result();
        }
        if(localhost_udp_address_server.port()!=udp_port_server){
            print_case_failed("port mismatch");
            return case_result();
        }
        if(!localhost_udp_address_client.is_ipv4()||\
           !localhost_udp_address_server.is_ipv4()){
            print_case_failed("ip version mismatch");
            return case_result();
        }
        if(local_host_socket_udp_client.create(localhost_udp_address_client)!=0){
            print_case_failed("create error socket");
            return case_result();
        }
        if(local_host_socket_udp_server.create(localhost_udp_address_server)!=0){
            print_case_failed("create error socket");
            return case_result();
        }

        //for udp not need it but? without write_to use connect before
        if( local_host_socket_udp_client.connect(localhost_udp_address_client) < 0 ){
            print_case_failed("Failed to connect to socket");
            return case_result();
        }

        int len_writed;
        len_writed = local_host_socket_udp_client.write(test);
        if( len_writed < 0 ){
            print_case_failed("Failed to write client socket (%d, %d)", test.size(), local_host_socket_udp_client.error_number());
            perror("failed to write");
            return case_result();
        }
  /*      local_host_socket_udp_server.clear_error_number();
        local_host_socket_udp_server << SocketOption().reuse_address() << SocketOption().reuse_port();
*/
        local_host_socket_udp_client.close();
        reply.fill(0);

        if( local_host_socket_udp_server.bind_and_listen(localhost_udp_address_server) < 0 ){
            print_case_failed("Failed to bind to localhost (%d)", local_host_socket_udp_server.error_number());
            return 0;
        }
        if( local_host_socket_udp_server.read(reply) < 0 ){
            print_case_failed("Failed to read client socket");
            return case_result();
        }

        print_case_message("read '%s' from udp socket ", reply.to_char());

        if( test != reply.to_char() ){
            print_case_failed("did not get an echo on localhost");
        }

        local_host_socket_udp_server.close();
    }
    /*}else{    //need to use on embedded
        print_case_failed("thread is stoped");
    }*/
/*  end udp connection handing    */

	return case_result();
}

void * SocketTest::listen_on_localhost(){
	Vector<SocketAddressInfo> list = SocketAddressInfo().fetch_node("localhost");

	if( list.count() == 0 ){
		print_case_failed("Failed to fetch node");
		return 0;
	}

	Socket local_host_listen_socket;
	SocketAddress localhost_address(list.at(0), 8080);
	localhost_address = SocketAddressIpv4(0, 8080);

	print_case_message("create socket at %s", localhost_address.address_to_string().str());
	if( local_host_listen_socket.create(localhost_address) < 0 ){
		print_case_failed("failed to create socket");
		return 0;
	}

	//listen on port 80 of localhost
	local_host_listen_socket.clear_error_number();
	local_host_listen_socket << SocketOption().reuse_address() << SocketOption().reuse_port();

	if( local_host_listen_socket.error_number() != 0 ){
		print_case_failed("failed to set socket options (%d)", local_host_listen_socket.error_number());
	}

	//connect to port 80 of localhost
	if( local_host_listen_socket.bind_and_listen(localhost_address) < 0 ){
		print_case_failed("Failed to bind to localhost (%d)", local_host_listen_socket.error_number());
		return 0;
	}


	print_case_message("Listening on localhost:%d", localhost_address.port());
	//now accept -- this will block until a request arrives
	SocketAddress accepted_address;
	Socket local_host_session_socket = local_host_listen_socket.accept(accepted_address);

	print_case_message("Accepted connection from %s:%d", accepted_address.address_to_string().str(), accepted_address.port());

	Data incoming(256);
	local_host_session_socket.read(incoming);
	local_host_session_socket.write(incoming);

    if(local_host_session_socket.close()<0){
        print_case_failed("failed to close socket");
        return 0;
    }

	if( local_host_listen_socket.close() < 0 ){
		print_case_failed("failed to close socket");
		return 0;
	}
    {
        /*  udp type
         * */
        SocketAddressInfo address_info(SocketAddressInfo::FAMILY_INET,SocketAddressInfo::TYPE_DGRAM,\
                                       SocketAddressInfo::PROTOCOL_UDP,AI_PASSIVE);
        list.free();
        list.clear();   //need to use before?
        list = address_info.fetch_node("localhost");
        //
        u16 udp_port_client = 5003;
        u16 udp_port_server = 5002;
        SocketAddress localhost_udp_address_client(list.at(0), udp_port_client);
        Socket local_host_listen_udp_socket_client;

        SocketAddress localhost_udp_address_server(list.at(0), udp_port_server);
        Socket local_host_listen_udp_socket_server;

        if( local_host_listen_udp_socket_server.create(localhost_udp_address_server) < 0 ){
            print_case_failed("failed to create socket");
            return 0;
        }

        //listen on port 80 of localhost
    /*    local_host_listen_udp_socket_server.clear_error_number();
        local_host_listen_udp_socket_client.clear_error_number();
        local_host_listen_udp_socket_server << SocketOption().reuse_address() << SocketOption().reuse_port();
        local_host_listen_udp_socket_client << SocketOption().reuse_address() << SocketOption().reuse_port();
    */
        if( local_host_listen_udp_socket_server.error_number() != 0 ){
            print_case_failed("failed to set socket options (%d)", local_host_listen_udp_socket_server.error_number());
        }
        //connect to port 80 of localhost
        if( local_host_listen_udp_socket_server.bind_and_listen(localhost_udp_address_server) < 0 ){
            print_case_failed("Failed to bind to localhost (%d)", local_host_listen_udp_socket_server.error_number());
            return 0;
        }
        int len_handl;
        incoming.fill(0);
        len_handl = local_host_listen_udp_socket_server.read(incoming);

        if( local_host_listen_udp_socket_server.close() < 0 ){
            print_case_failed("failed to close socket");
            return 0;
        }
        Timer::wait_milliseconds(500);

        if( local_host_listen_udp_socket_client.create(localhost_udp_address_client) < 0 ){
            print_case_failed("failed to create socket");
            return 0;
        }
        //for udp not need it but? without write_to use connect before
        if( local_host_listen_udp_socket_client.connect(localhost_udp_address_client) < 0 ){
            print_case_failed("Failed to connect to socket");
            return 0;
        }

        len_handl = local_host_listen_udp_socket_client.write(incoming);
        if( len_handl < 0 ){
            print_case_failed("Failed to write client socket (%d, %d)", len_handl, local_host_listen_udp_socket_client.error_number());
            perror("failed to write");
            return 0;
        }

        if( local_host_listen_udp_socket_client.close() < 0 ){
            print_case_failed("failed to close socket");
            return 0;
        }
    }

	return 0;
}
/*@brief test address info section (getaddrinfo)
 * */
bool SocketTest::execute_socket_address_info_case(){
    bool result;
	SocketAddressInfo address_info;
    result = true;
	print_case_message("get list from stratifylabs.co");
	Vector<SocketAddressInfo> list = address_info.fetch_node("stratifylabs.co");

	print_case_message("got %d entries", list.count());
	for(u32 i=0; i < list.count(); i++){
		open_case(String().format("entry-%d", i));
		SocketAddress address(list.at(i), 80);
		if( address.family() == SocketAddressInfo::FAMILY_INET ){
			print_case_message("family is ipv4");
		} else {
			print_case_message("family is other:%d", address.family());
		}

		if( address.type() == SocketAddressInfo::TYPE_STREAM ){
			print_case_message("type is stream");
		} else {
			print_case_message("type is other:%d", address.type());
		}

		if( address.protocol() == SocketAddressInfo::PROTOCOL_TCP ){
			print_case_message("protocol is tcp");
		} else {
			print_case_message("protocol is other:%d", address.protocol());
		}

		print_case_message("address: %s", address.address_to_string().str());
		print_case_message("port is %d", address.port());
		close_case();
	}
    list.free();
    list.clear();
    list = address_info.fetch("stratifylabs.co","http");
    print_case_message("got http %d entries", list.count());
    if(list.count()==0){
        print_case_failed("get address http failed ");
        result = false;
    }
    list.free();
    list.clear();
    list = address_info.fetch_service("http");
    print_case_message("got only service %d entries", list.count());
    if(list.count()==0){
        print_case_failed("dont have self http service ");
        result = false;
    }
    SocketAddressInfo address_info_dos(SocketAddressInfo::FAMILY_INET6, SocketAddressInfo::TYPE_DGRAM, SocketAddressInfo::PROTOCOL_UDP, AI_PASSIVE);
    address_info_dos.set_flags(AI_PASSIVE);
    address_info_dos.set_family(SocketAddressInfo::FAMILY_NONE);
    address_info_dos.set_type(SocketAddressInfo::TYPE_STREAM);
    address_info_dos.set_protocol(SocketAddressInfo::PROTOCOL_TCP);
    if(address_info_dos.flags()!=AI_PASSIVE||
       address_info_dos.family()!=SocketAddressInfo::FAMILY_NONE||
       address_info_dos.type()!=SocketAddressInfo::TYPE_STREAM||
       address_info_dos.protocol()!=SocketAddressInfo::PROTOCOL_TCP){
        print_case_failed("don't set some property");
        result = false;
    }
    list.free();
    list.clear();
    list = address_info_dos.fetch_node("localhost");
    if(list.count()==0){
        print_case_failed("dont have self addr info ");
        result = false;
    }else{
        print_case_message("local host stream tcp list count %d",list.count());
    }

    SocketAddressInfo address_info_tres(SocketAddressInfo::FAMILY_INET, SocketAddressInfo::TYPE_DGRAM, SocketAddressInfo::PROTOCOL_UDP, AI_PASSIVE);
    if(address_info_tres.flags()!=AI_PASSIVE||
       address_info_tres.family()!=SocketAddressInfo::FAMILY_INET||
       address_info_tres.type()!=SocketAddressInfo::TYPE_DGRAM||
       address_info_tres.protocol()!=SocketAddressInfo::PROTOCOL_UDP){
        print_case_failed("don't set some property");
        result = false;
    }
    list.free();
    list.clear();
    list = address_info_tres.fetch_node("localhost");
    if(list.count()==0){
        print_case_failed("dont have self addr info");
        result = false;
    }else{
        print_case_message("local host stream udp list count %d",list.count());
    }


    return result;
}
bool SocketTest::execute_socket_property_case(){
    bool result;
    result = true;
    return result;
}
