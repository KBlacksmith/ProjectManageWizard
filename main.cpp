#include <iostream>
#include <string>
#include <map>
#include <filesystem>
#include <fstream>

bool validateChoice(std::string question){
    std::string choice;
    std::cout << question;
    std::getline(std::cin, choice);
    while(choice.compare("y") != 0 and choice.compare("Y") != 0 and choice.compare("N") != 0 and choice.compare("n") != 0){
        std::cout << "Sorry, try again" << std::endl;
        std::cout << question;
        std::getline(std::cin, choice);
    }
    return choice.compare("y") == 0 or choice.compare("Y") == 0;
}

std::string getProjectName(){
    std::string pName;
    std::cout << "Enter the name of your new package: ";
    std::getline(std::cin, pName);
    return pName;
}

std::string getProjectLang(std::map<std::string, std::string> langs){

    std::string pLang;
    
    std::cout << "Languages available: " << std::endl;
    for(auto lang: langs){
        std::cout << " * " << lang.first << std::endl;
    }
    do{
        std::cout << "Select a lanaguage from the list: ";
        std::getline(std::cin, pLang);
    }while (!langs.contains(pLang));
    
    return pLang;
    
}

void createProject(std::string lang, std::string name, std::string extension){
    std::map<std::string, std::string> temp{
        {"python", "print(\"Hello World!\")\n"}, 
        {"cpp", "#include <iostream>\n\nint main(){\n\tstd::cout << \"Hello, world!\" << std::endl;\n\treturn 0;\n}\n"},
        {"java", "public class "+name+"{\n\tpublic static void main(String[] args){\n\t\tSystem.out.println(\"Hello, world!\");\n\t}\n}\n"},
        {"javascript", "//Run with 'deno run "+name+".js\n\nconsole.log(\"Hello World!\")"},
        {"typescript", "//Run with 'deno run "+name+".ts\n\nconsole.log(\"Hello, world!\")"}, 
        {"rust", "fn main() {\n\tprintln!(\"Hello, world!\");\n}"},
        {"sql", "//SQL Project"},
        {"ruby", ""},
        {"c", "#include <stdio.h>\n\nint main(int argc, char * argv[]){\n\tprintf(\"Hello, world!\\n\");\n\treturn 0;\n}"},
        {"dart", "//Compile with dart compile exe "+name+".dart\nvoid main(List<String> args) {\n\tprint(\"Hello, world!\");\n}"},
    };
    std::map<std::string, std::string> bash{
        {"c", "gcc "+name+".c -o "+name+"\n./"+name},
        {"cpp", "g++ -std=c++2a "+name+".cpp -o "+name+"\n./"+name},
        {"java", "javac "+name+".java\n#jar -cfe "+name+".jar "+name+" *.class */*.class\njava "+name},
        {"javascript", "deno run "+name+".js"},
        {"typescript", "deno run "+name+".ts"},
        {"python", "python3 "+name+".py"},
        {"rust", "rustc "+name+".rs\n./"+name},
        {"ruby", ""},
        {"sql", ""},
        {"dart", "dart compile exe "+name+".dart -o "+name+"\n./"+name}
    };
    std::cout << "Creating package" << std::endl;
    std::cout << "Creating directory \""+name+"\"" << std::endl;
    if(std::filesystem::create_directories(name+"/")){
        std::cout << "Directory created succesfully" << std::endl;
        std::ofstream project(name+"/"+name+"."+extension);
        project << temp[lang];
        project.close();
        std::ofstream readme(name+"/README.md");
        readme << "Package: " << name << std::endl;
        readme << "Created with Kenneth's Project Manager" << std::endl;
        readme.close();
        std::ofstream bash_script(name+"/compile.sh");
        bash_script << "echo \"Compiling and Running\"\n";
        bash_script << bash[lang] << std::endl;
        bash_script.close();
        if(!std::filesystem::exists(".wiz_settings")){
            std::ofstream settings(name+"/.wiz_settings");
            settings << extension;
            settings.close();
        }
        if(validateChoice("Do you want to open the project in VS Code? Y/n: ")){
            std::string code = "code " + name+"/";
            system(code.c_str());
        }
        return;
    }
    std::cout << "Directory already exists" << std::endl;
}

void createComponent(std::string component){
    std::string ext {""};
    std::ifstream extension(".wiz_settings");
    getline(extension, ext);
    if(std::filesystem::exists(component+"."+ext) == 0){
        std::ofstream componentFile(component+"."+ext);
        std::cout << "\""+component+"\" created succesfully.\n";
    }
    else{
        std::cout << "File \""+component+"."+ext+"\" already exists.\n";
    }
}

void help(){
    std::cout << "Usage: wizard [option]" << std::endl;
    std::cout << "Available commands: " << std::endl;
    std::cout << "\tsummon\tCreate a new package for a project" << std::endl;
    std::cout << "\tspell\tCreate new component file" << std::endl;
    std::cout << "\tomen\tDisplay list of commands and options" << std::endl;
    std::cout << "\tcurse\tLists supported programing languages" << std::endl;
    //std::cout << "\tsettings\n";
    //std::cout << "\ttree\tProject tree\n";
}

int main(int argc, char * argv[]){
    std::map<std::string, std::string> langs {
        {"python", "py"}, {"cpp", "cpp"}, {"ruby", "rb"}, {"java", "java"},
        {"c", "c"}, {"dart", "dart"}, {"javascript", "js"}, {"typescript", "ts"}, 
        {"rust", "rs"}, {"sql", "sql"}
    };
    std::string project_name, project_lang;
    switch (argc)
    {
    case 1:
        {
            help();
        }
        break;
    case 2:
        {
            std::string command = argv[1];
            if(command.compare("summon") == 0){
                std::cout << "Welcome to the Kenneth PM" << std::endl;
                project_name.assign(getProjectName());
                project_lang.assign(getProjectLang(langs));
                std::cout << std::endl << "Package name: " << project_name << std::endl << "Language: " << project_lang << std::endl;
                if(validateChoice("Create package? Y/n: ")){
                    createProject(project_lang, project_name, langs[project_lang]);
                }
                else{
                    std::cout << "Project Wizard is cancelled" << std::endl;
                }
            }
            else if(command.compare("omen") == 0){
                help();
            }
            else if (command.compare("curse") == 0){
                std::cout << "Supported languages: \n";
                for(auto lang:langs){
                    std::cout << "\t* " << lang.first << std::endl;
                }
            }
            else if(command.compare("spell") == 0){
                std::cout << "Create component\n";
                if(std::filesystem::exists(".wiz_settings")){
                    std::string component_name;
                    std::cout << "Component name: ";
                    getline(std::cin, component_name);
                    createComponent(component_name);
                }
                else{
                    std::cout << "You are not in a Wizard project directory\n";
                }
            }
            else{
                std::cout << "wizard: unrecognized option '" << command << "'\n";
                std::cout << "Try 'wizard omen' for more information and the list of available commands\n";
            }
        }
        break;
    case 3: 
        {
            std::string command = argv[1], argument = argv[2];
            if(command.compare("summon") == 0){
                if(argument.compare("omen") == 0){
                    std::cout << "wizard: option 'summon'\n";
                    std::cout << "Usage: wizard summon [name] [programming language]\n";
                }
                else {
                    project_name.assign(argument);
                    project_lang.assign(getProjectLang(langs));
                    std::cout << std::endl << "Package name: " << project_name << std::endl << "Language: " << project_lang << std::endl;
                    if(validateChoice("Create package? Y/n: ")){
                        createProject(project_lang, project_name, langs[project_lang]);
                    }
                    else{
                        std::cout << "Project Wizard is cancelled" << std::endl;
                    }
                }
            }
            else if(command.compare("spell") == 0){
                if(argument.compare("omen") == 0){
                    std::cout << "wizard: option 'spell'\n";
                    std::cout << "Usage: wizard spell [name]\n";
                }
                else{
                    std::cout << "Create component\n";
                    if(std::filesystem::exists(".wiz_settings")){
                        createComponent(argument);
                    }
                    else{
                        std::cout << "You are not in a Wizard project directory\n";
                    }
                }
            }
            else if(command.compare("omen") == 0 || command.compare("curse") == 0){
                std::cout << "wizard: too many arguments for '"+command+"' option. \n";
            }
            else{
                std::cout << "wizard: unrecognized option '" << command << "'\n";
                std::cout << "Try 'wizard omen' for more information and the list of available commands\n";
            }
        }
        break;
    case 4:
        {
            std::string command = argv[1], name = argv[2], p_lang = argv[3];
            if(command.compare("summon")==0){
                project_name.assign(name);
                project_lang.assign(p_lang);
                std::cout << std::endl << "Package name: " << project_name << std::endl << "Language: " << project_lang << std::endl;
                if(validateChoice("Create package? Y/n: ")){
                    createProject(project_lang, project_name, langs[project_lang]);
                }
                else{
                    std::cout << "Package is cancelled" << std::endl;
                }
            }
            else if(command.compare("spell")==0 || command.compare("omen")==0 || command.compare("curse")==0){
                std::cout << "wizard: Too many arguments for '"+command+"' option. \n";
            }
            else{
                std::cout << "wizard: unrecognized option '" << command << "'\n";
                std::cout << "Try 'wizard omen' for more information and the list of available commands\n";
            }
        }
        break;
    default:
        std::cout << "Too many arguments, try \"omen\" for more info" << std::endl;
        break;
    }
    return 0;
}