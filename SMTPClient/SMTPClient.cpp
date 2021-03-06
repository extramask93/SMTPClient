// SMTPClient.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <string>
#include <fstream>
#include "SMTP.h"
#include "boost/program_options.hpp" 
#include "SocketException.h"


namespace po = boost::program_options;
po::variables_map TryParseCommandLineArguments(int, char**);


int main(int argc, char **argv)
{
	const auto params = TryParseCommandLineArguments(argc, argv);
	SMTP smtp{};
	try {
		smtp.Connect(params["ip"].as<std::string>(), params["port"].as<uint16_t>());
		smtp.Login(params["login"].as<std::string>(), params["pass"].as<std::string>());
		Message message{
			params["sender"].as<std::string>(),
			{params["recv"].as<std::string>()},
			"PS LAB N2 ZIMA 2018 14A",
		};
		smtp.SendMessage(message);
		smtp.Disconnect();
	}
	catch (SocketException &e)
	{
		std::cerr << e.what();
	}
	catch(std::runtime_error er) {
		std::cerr << er.what();
	}
}
void options_required(const po::variables_map& vm, const std::vector<std::string>& required_options)
{
	for (const auto& item : required_options) {
		if (vm.count(item) == 0)
			throw std::logic_error(std::string("Option '") + item
				+ "' is required.");
	}
}
po::variables_map TryParseCommandLineArguments(int argc, char **argv)
{
	std::ifstream configFile{ "config.ini",std::ifstream::in };
	po::options_description configFileOptions("Allowed options");
	configFileOptions.add_options()
		("ip", po::value<std::string>(), "SMTP server address")
		("port", po::value<uint16_t>(), "SMTP server port")
		("login,l", po::value<std::string>(), "Email login")
		("pass,p", po::value<std::string>(), "Email login")
		("sender,s", po::value<std::string>(), "Sender Email")
		("recv,r", po::value<std::string>(), "Receiver Email")
		("help,h", "Produce help message")
		("version,v", "Prints version number")
		;
	po::variables_map vm;
	auto a = *argv;
	try {
		po::store(po::parse_command_line(argc, argv, configFileOptions), vm);
		po::store(po::parse_config_file(configFile, configFileOptions), vm);
		options_required(vm, { "ip","port","login","pass","sender","recv" });
	}
	catch(const std::logic_error &ex)
	{
		std::cerr << ex.what()<<'\n';
		std::cout << configFileOptions << "\n";
		exit(1);
	}
	catch (std::exception&)
	{
		std::cerr<<"Error: Unrecognized Option!\n";
		exit(2);
	}
	po::notify(vm);
	if (vm.count("help")) {
		std::cout << configFileOptions << "\n";
		exit(0);
	}
	if (vm.count("version"))
	{
		std::cout << "v.1.0.0\n";
		exit(0);
	}
	return vm;
}