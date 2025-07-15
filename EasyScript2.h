#include <string>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <thread>
#include <atomic>
#include <memory>
#include <mutex>
#include <future>
#include <chrono>
#include <condition_variable>

std::mutex consoleMutex;
typedef std::map<std::string, std::string> routeTable;
routeTable route;
int httpPort = 80;  
int httpsPort = 443;
std::string bindon = "localhost";
std::string cert_file = "server.crt";  // 默认证书路径
std::string private_key_file = "server.key";  // 默认私钥路径
std::string dpath = "E:/w64devkit/dev/a/htdoct/default.htm";
 

class hfCommand {

public:
	
	int static strToInt(std::string str){
        std::istringstream it(str);
        int num;
        it>>num;
        return num;
    }

};

class EasyScriptsRouter {
private:
    typedef std::map<std::string, std::string> routes;
	// Note: I forgot what this line of code does
	// and what would happen if I removed it.
	// Therefore, I did not delete it.
	// If you find that it has no effect throughout the file,
	// please let me know. I will remove it in the new version.
public:
    static int strToInt(const std::string& str) {
        try {
            return std::stoi(str);
        } catch (const std::exception& e) {
            std::lock_guard<std::mutex> lock(consoleMutex);
            std::cerr << "Invalid port number: " << str << " - " << e.what()  << std::endl;
            return 0;
        }
    }
    
    static void runCommand(const std::string& file) {
        std::ifstream script(file);
        if (!script.is_open())  {
            std::lock_guard<std::mutex> lock(consoleMutex);
            std::cerr << "Failed to open config file: " << file << std::endl;
            return;
        }
        
        std::string command;
        while (script >> command) {
            if (command == "add") {
                script >> command;
                if (command == "froute") {
                    script >> command;
                    if (command == "{") {
                        while (script >> command && command != "}") {
                            std::string path;
                            if (script >> path) {
                                route[command] = path;
                                std::lock_guard<std::mutex> lock(consoleMutex);
                                std::cout << "Added route: " << command << " -> " << path << std::endl;
                            } else {
                                std::lock_guard<std::mutex> lock(consoleMutex);
                                std::cerr << "Missing path for route: " << command << std::endl;
                                break;
                            }
                        }
                    } else {
                        std::lock_guard<std::mutex> lock(consoleMutex);
                        std::cerr << "Expected '{' but found \"" << command << "\" at \"add froute >>" << command << "<<\"" << std::endl;
                    }
                }
            } else if (command == "bindon") {
                if (script >> bindon) {
                    std::lock_guard<std::mutex> lock(consoleMutex);
                    std::cout << "Set bind address to: " << bindon << std::endl;
                }
            } else if (command == "HTTPlistenon") {
                if (script >> command) {
                    httpPort = strToInt(command);
                    std::lock_guard<std::mutex> lock(consoleMutex);
                    std::cout << "Set HTTP port to: " << httpPort << std::endl;
                }
            } else if (command == "SSLlistenon") {
                if (script >> command) {
                    httpsPort = strToInt(command);
                    std::lock_guard<std::mutex> lock(consoleMutex);
                    std::cout << "Set HTTPS port to: " << httpsPort << std::endl;
                }
            } else if (command == "cert") {
                if (script >> cert_file) {
                    std::lock_guard<std::mutex> lock(consoleMutex);
                    std::cout << "Set certificate file to: " << cert_file << std::endl;
                }
            } else if (command == "privatekey") {
                if (script >> private_key_file) {
                    std::lock_guard<std::mutex> lock(consoleMutex);
                    std::cout << "Set private key file to: " << private_key_file << std::endl;
                }
            } else if (command == "set") {
                if (script >> command && command == "default") {
                    if (script >> dpath) {
                        std::lock_guard<std::mutex> lock(consoleMutex);
                        std::cout << "Set default file to: " << dpath << std::endl;
                    }
                }
            } else if (!command.empty()  && command[0] != '#') { // 忽略注释 
                std::lock_guard<std::mutex> lock(consoleMutex);
                std::cerr << "Unknown command: " << command << std::endl;
            }
        }
    }
};


class EasyScripts {

public:
	
	std::string runCommand(std::string _file){
		
		std::string response = 
		"HTTP/1.1 200 OK\r\n"
		"X-Powered-By: MtaoCharct | EasyScript\r\n"
		;
		std::ifstream script(_file.c_str());
        std::string command = "No input",last = "eof",content;
		
		do{
			last = command;
			script>>command;
			if(command == "<?") {
				while(command != "?>") {
					
					
					
				}
				script>>command;
			}else{
				script>>std::noskipws>>command;
				script>>std::skipws;
				response += command;
			}
		}while(last!=command);
		
		return response;
	}
};