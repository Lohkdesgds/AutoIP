#undef UNICODE

#include "socket.h"

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <string>
#include <thread>

const std::string version = "2.0.0MP";

const std::string url = "ipv6-test.com";// "dynv6.com/api/update\0";
const std::string urlget = "/api/myip.php";
const std::string urlhost = "ipv6-test.com";
const std::string rawautodns = "ipv6.dynv6.com";
const std::string rawautodns4 = "ipv4.dynv6.com";
const std::string rawautodns_pos = "/api/update";
/*const std::string autodns[] = {
	"ipv6.dynv6.com/api/update?hostname=", // blackmotion.dynv6.net
	"&token=", //tnAk8yEH3qAVq23yQT3xtXWHWvxj-4
	"&ipv6=" // that ivp6
};*/

std::vector<char> read_timed(Lunaris::TCP_client&);
void delay(const double);
std::string GetTodayDayNow();
bool downloadAsString(std::string&);
std::string getPAGE(const std::string, std::string, std::string, std::string, const bool = false);

int main(int argc, char* argv[])
{
	std::string temp;

	//std::string to_send;

	double seconds = 300.0;
	std::string path_fp;

	bool full = false;

	bool using_dynv6 = false;
	bool use_6to4_instead = false;
	bool also_update_ipv4 = false;
	std::string token;
	std::string hostname;

	if (argc > 1)
	{
		for (int argc_c = 1; argc_c < argc; argc_c++)
		{
			std::string temp_arg = argv[argc_c];

			if (temp_arg == "-t")
			{
				argc_c++;
				if (argc_c >= argc) continue;
				temp_arg = argv[argc_c];

				seconds = atof(temp_arg.c_str());

				if (seconds < 60.0)
				{
					std::cout << GetTodayDayNow() << "[INIT_ARG_TIME] You should not use less than 60.0 seconds for update time. Default value being used instead (300.0 sec)." << std::endl;
				}
				else {
					std::cout << GetTodayDayNow() << "[INIT_ARG_TIME] Successfully defined update time of " << std::to_string(seconds) << " seconds." << std::endl;
				}
			}

			if (temp_arg == "--token")
			{
				argc_c++;
				if (argc_c >= argc) continue;
				token = argv[argc_c];

				std::cout << GetTodayDayNow() << "[INIT_ARG_TOKEN] Defined." << std::endl;
			}

			if (temp_arg == "--hostname")
			{
				argc_c++;
				if (argc_c >= argc) continue;
				hostname = argv[argc_c];

				std::cout << GetTodayDayNow() << "[INIT_ARG_HOSTNAME] Defined." << std::endl;
			}

			if (temp_arg == "--ipv6to4")
			{
				std::cout << GetTodayDayNow() << "[INIT_ARG_IPV6TO4DYN] Defined 6to4." << std::endl;
				use_6to4_instead = true;
			}

			if (temp_arg == "--enableipv4")
			{
				std::cout << GetTodayDayNow() << "[INIT_ARG_IPV4] Enabled IPV4 too." << std::endl;
				also_update_ipv4 = true;
			}

			if (temp_arg == "--showalllog")
			{
				std::cout << GetTodayDayNow() << "[INIT_ARG_COUT] ALL LOG ACTIVATED." << std::endl;
				full = true;
			}

			if (temp_arg == "-v" || temp_arg == "--version")
			{
				std::cout << GetTodayDayNow() << "[INIT_ARG_VERS] Showing current version: " << version << std::endl;
			}

			if (temp_arg == "-h" || temp_arg == "--help")
			{
				std::cout << GetTodayDayNow() << "Help:" << std::endl
					<< "This is mainly for IPV6 redirects. IPV4 is optional. These are the arguments available:" << std::endl
					<< "-t <time> = define update time" << std::endl
					<< "--usedynv6 = enable dynv6 auto updater (optional)" << std::endl
					<< "--token <token> = define token for dynv6" << std::endl
					<< "--ipv6to4 = enable dynv6 6to4 instead of auto" << std::endl
					<< "--enableipv4 = also updates ipv4 info" << std::endl
					<< "--hostname <full hostname> = define hostname for dynv6" << std::endl
					<< "--showalllog = show all the log all the time (if disabled, shows only errors or ip change)" << std::endl
					<< "-v = shows the version" << std::endl
					<< "-h = this page" << std::endl;
				return 0;
			}
		}
	}

    if (token.empty()){
        std::cout << GetTodayDayNow() << "[WARN] No token set!" << std::endl;
    }
    if (hostname.empty()){
        std::cout << GetTodayDayNow() << "[WARN] No hostname set!" << std::endl;
    }

    using_dynv6 = token.size() > 0 && hostname.size() > 0;

	while (1)
	{
		std::string newtemp;
		if (!downloadAsString(newtemp))
		{
			std::cout << GetTodayDayNow() << "[ERROR] (GETIP): " << newtemp << std::endl;
			delay(2);
		}
		else {
            {
                size_t ntemppos = newtemp.find(':');
                if (ntemppos < newtemp.size()) {
                    while(ntemppos != 0){
                        if (newtemp[ntemppos] == '\n') break;
                        --ntemppos;
                    }
                    newtemp = newtemp.substr(ntemppos + 1);
                    if ((ntemppos = newtemp.find('\r')) != std::string::npos){
                        newtemp = newtemp.substr(0, ntemppos);
                    }
                }
            }

            if (newtemp.empty()) {
                std::cout << GetTodayDayNow() << "[WARN] Local IP change tracking failed once. Trying again in 10 seconds..." << std::endl;
			    delay(10);
                continue;
            }

            const bool was_diff = newtemp != temp && temp.size();
            const bool was_null = temp.empty();
            temp = newtemp;

            if (full) {
                if (was_diff || was_null) std::cout << GetTodayDayNow() << "[INFO] IP changed/updated!" << std::endl;
                std::cout << GetTodayDayNow() << "[INFO] Current IP: " << newtemp << std::endl;
            }
            else {
                if (was_null) {
                    std::cout << GetTodayDayNow() << "[INFO] Found your IP (for your debugging)" << std::endl;
                    std::cout << GetTodayDayNow() << "[INFO] Updated IP: " << newtemp << std::endl;
                }
                else if (was_diff) {
                    std::cout << GetTodayDayNow() << "[INFO] Updated IP: " << newtemp << std::endl;
                }
            }

			if (using_dynv6)
			{
				newtemp = getPAGE(rawautodns, hostname, token, (use_6to4_instead ? "6to4" : ""/*auto*/));
                if (newtemp.find("HTTP/1.1 200 OK") != std::string::npos) {
                    if (full || was_diff || was_null) std::cout << GetTodayDayNow() << "[INFO] HTTP/1.1 200 OK!" << std::endl;
                }
                else std::cout << GetTodayDayNow() << "[WARN] " << (newtemp.empty() ? "Dynv6 didn't return a thing!" : newtemp) << std::endl;

				if (also_update_ipv4) {
                    newtemp = getPAGE(rawautodns4, hostname, token, "", true);
                    if (newtemp.find("HTTP/1.1 200 OK") != std::string::npos) {
                        if (full || was_diff || was_null) std::cout << GetTodayDayNow() << "[INFO][IPV4 PART] HTTP/1.1 200 OK!" << std::endl;
                    }
                    else std::cout << GetTodayDayNow() << "[WARN][IPV4 PART] " << (newtemp.empty() ? "Dynv6 didn't return a thing!" : newtemp)  << std::endl;
                }
			}
            else {
				std::cout << std::endl;
            }

			delay(seconds);
		}

	}
}

void delay(const double v)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<long long>(v * 1000.0)));
}

std::vector<char> read_timed(Lunaris::TCP_client& cli)
{
    std::vector<char> vec;    
    for(size_t tr = 0; tr < 10; tr++){
        const auto _v = cli.recv(static_cast<size_t>(-1), false);
        if (_v.size()) {
            vec.insert(vec.end(), _v.begin(), _v.end());
            tr = 0;
        }
        else std::this_thread::sleep_for(std::chrono::milliseconds(150));
    }
    return vec;
}

std::string GetTodayDayNow()
{
	std::time_t now_c = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    auto tm = std::localtime(&now_c);
    char buffer[32];
    std::strftime(buffer, 32, "[%Y-%m-%d %H:%M:%S%z]", tm);
    return std::string(buffer);
}

bool downloadAsString(std::string& wheree)
{
	Lunaris::TCP_client cli;
    if (!cli.setup(Lunaris::socket_config().set_ip_address(url).set_port(80))) {
        if (!cli.setup(Lunaris::socket_config().set_ip_address(url).set_port(8080))) {
            return false;
        }
    }

    const std::string sending = 
        "GET " + urlget + " HTTP/1.1\r\n"
        "Host: " + urlhost + "\r\n"
        "Accept: */*\r\n\r\n";

    cli.send(sending.data(), sending.size());

    std::this_thread::sleep_for(std::chrono::milliseconds(250));

    std::vector<char> vec = read_timed(cli);

    wheree.clear();    

    for(const auto& it : vec) {
        wheree += it;
    }

    size_t pos = wheree.find("\r\n\r\n");
    if (pos != std::string::npos){
        wheree = wheree.substr(pos + 4);
    }

    return wheree.size() > 0;
}

std::string getPAGE(const std::string durl, std::string hostname, std::string token, std::string ipA, const bool ipv4now)
{
	Lunaris::TCP_client cli;
    if (!cli.setup(Lunaris::socket_config().set_ip_address(durl).set_port(80))) {
        if (!cli.setup(Lunaris::socket_config().set_ip_address(durl).set_port(8080))){            
            std::cout << "[ERROR] Failed to connect to host." << std::endl;
            return {};
        }
    }

	std::string sending =
		"GET " + rawautodns_pos + "?" + (ipv4now ? "ipv4" : "ipv6") + "=" + std::string((ipA.length() < 2) ? "auto" : ipA) + "&token=" + token + "&hostname=" + hostname + " HTTP/1.1\r\n"
		"Host: " + durl + "\r\n"
		"Connection: close\r\n\r\n";


    if(!cli.send(sending.data(), sending.size())){
        std::cout << "[ERROR] Failed to request HTTP GET" << std::endl;
        return {};
    }    

	std::string getting;

    std::this_thread::sleep_for(std::chrono::milliseconds(250));

    const auto __gett = read_timed(cli);

    for(const auto& it : __gett) getting += it;

    //size_t pos = getting.rfind('\n');
    //if (pos != std::string::npos){
    //    return getting.substr(0, pos-1);
    //}
	//else return getting;
    return getting;
}