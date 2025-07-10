#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "information.h"
#include "openssl.h"
typedef std::map<std::string, std::string> routeTable;
routeTable route;
int port = 443;  // Ĭ��HTTPS�˿ڸ�Ϊ443
std::string bindon = "localhost";
std::string cert_file = "server.crt";  // Ĭ��֤��·��
std::string private_key_file = "server.key";  // Ĭ��˽Կ·��
std::string dpath = "E:/w64devkit/dev/a/htdoct/default.htm";

class EasyScripts {
private:
    typedef std::map<std::string, std::string> routes;
public:
    int static strToInt(std::string str){
        std::istringstream it(str);
        int num;
        it>>num;
        return num;
    }
    void static runCommand(std::string file){
        std::ifstream script(file.c_str());
        std::string command;
        while(!script.eof()){
                
            script>>command;
            if(command == "add"){
                script>>command;
                if(command == "froute"){
                    script>>command;
                    if(command == "{"){
                        while(command != "}"){
                            std::string path;
                            script>>command;
                            if(command == "}"){
                                break;
                            }
                            script>>path;
                            route[command] = path;
                        }
                        
                    }else{
                        std::cerr<<"Except '{' but found \""+command+"\" at \"add froute >>"+command+"<<\""<<std::endl;
                    }
                }
            }else if(command == "bindon"){
                script>>command;
                bindon = command;
            }else if(command == "listenon"){
                script>>command;
                port = strToInt(command);
            // ���֤������֧��
            }else if(command == "cert"){
                script >> command;
                cert_file = command;
            }else if(command == "privatekey"){
                script >> command;
                private_key_file = command;
            }else if(command =="set"){
            	script >> command;
            	if(command == "default"){
            		script >> command;
            		dpath = command;
				}
			}
        }
    }
}; 

int main() {
    EasyScripts::runCommand("config.esr");
    
    // ����HTTPS������
    httplib::SSLServer svr(cert_file.c_str(), private_key_file.c_str());
    if (!svr.is_valid()) {
        std::cerr << "SSL server initialization failed!" << std::endl;
        return 1;
    }
    
    //Ĭ��ҳ��ʼ�� 
    std::string dcontent;
    std::ifstream dresponse(dpath);
    if (dresponse) {
        dresponse.seekg(0, std::ios::end);
        dcontent.reserve(dresponse.tellg());
        dresponse.seekg(0, std::ios::beg);
        dcontent.assign(
            std::istreambuf_iterator<char>(dresponse),
            std::istreambuf_iterator<char>()
        );
    } else {
        dcontent = "<h1>File not found</h1>";
        std::cerr << "Error opening: " << dpath << std::endl;
    }
    // ��·��    
    svr.Get("/", [content = std::move(dcontent)](const httplib::Request&, httplib::Response& res) {
    	res.set_content(content, "text/html");
    });
    
    
    
    // ��̬·��
    for (const auto& [path, file_path] : route) {
        std::string file_content;
        std::ifstream fresponse(file_path);
        if (fresponse) {
            fresponse.seekg(0, std::ios::end);
            file_content.reserve(fresponse.tellg());
            fresponse.seekg(0, std::ios::beg);
            file_content.assign(
                std::istreambuf_iterator<char>(fresponse),
                std::istreambuf_iterator<char>()
            );
        } else {
            file_content = "<h1>File not found</h1>";
            std::cerr << "Error opening: " << file_path << std::endl;
        }
        
        svr.Get(path.c_str(), [content = std::move(file_content)](const httplib::Request&, httplib::Response& res) {
            res.set_content(content, "text/html");
        });
    }
    
    std::cout << "HTTPS server running at https://" << bindon << ":" << port << std::endl;
    svr.listen(bindon.c_str(), port);
    
    std::string end;
    std::cin >> end;
    return 0;
}
