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
    std::cout << "Enter the name of your new project: ";
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
        {"java", "javac *.java\n#javac */*.java\n#jar -cfe "+name+".jar "+name+" *.class */*.class\njava "+name},
        {"javascript", "deno run "+name+".js"},
        {"typescript", "deno run "+name+".ts"},
        {"python", "python3 "+name+".py"},
        {"rust", "rustc "+name+".rs\n./"+name},
        {"ruby", ""},
        {"sql", ""},
        {"dart", "dart compile exe "+name+".dart -o "+name+"\n./"+name}
    };
    std::cout << "Creating project" << std::endl;
    std::cout << "Creating directory \""+name+"\"" << std::endl;
    if(std::filesystem::create_directories(name+"/")){
        std::cout << "Directory created succesfully" << std::endl;
        std::ofstream project(name+"/"+name+"."+extension);
        project << temp[lang];
        project.close();
        std::ofstream readme(name+"/README.md");
        readme << "Project: " << name << std::endl;
        readme << "Created with Kenneth's Project Manager" << std::endl;
        readme.close();
        std::ofstream bash_script(name+"/compile.sh");
        bash_script << "echo \"Compiling and Running\"\n";
        bash_script << bash[lang] << std::endl;
        bash_script.close();
        if(validateChoice("Do you want to open the project in VS Code? Y/n: ")){
            std::string code = "code " + name+"/";
            system(code.c_str());
        }
        return;
    }
    std::cout << "Directory already exists" << std::endl;
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
            std::cout << "Welcome to the Kenneth PM" << std::endl;
            project_name.assign(getProjectName());
            project_lang.assign(getProjectLang(langs));
            std::cout << std::endl << "Project name: " << project_name << std::endl << "Language: " << project_lang << std::endl;
            if(validateChoice("Create project? Y/n: ")){
                createProject(project_lang, project_name, langs[project_lang]);
            }
            else{
                std::cout << "Project is cancelled" << std::endl;
            }
        }
        break;
    case 2:
        {
           std::string argOne = argv[1];
           if(argOne.compare("--help") == 0 || argOne.compare("-h") == 0){
               std::cout << "Available commands: " << std::endl;
               std::cout << "\t--help or -h\tDisplay list of commands and options" << std::endl;
               std::cout << "\t--langs\tLists supported programing languages" << std::endl;
           }
           else if(argOne.compare("--langs") == 0 || argOne.compare("-l") == 0){
               std::cout << "Supported languages: " << std::endl;
               for(auto lang: langs){
                   std::cout << "\t" << lang.first << std::endl;
               }
           }
           /*
           else if(langs.contains(argOne)){
                project_lang.assign(argOne);
                project_name.assign(getProjectName());
                std::cout << std::endl << "Project name: " << project_name << std::endl << "Language: " << project_lang << std::endl;
                if(validateChoice("Create project? Y/n: ")){
                    createProject(project_lang, project_name, langs[project_lang]);
                }
                else{
                    std::cout << "Project is cancelled" << std::endl;
                }
           }
           */
           else{
               project_name.assign(argOne);
               project_lang.assign(getProjectLang(langs));
               std::cout << std::endl << "Project name: " << project_name << std::endl << "Language: " << project_lang << std::endl;
                if(validateChoice("Create project? Y/n: ")){
                    createProject(project_lang, project_name, langs[project_lang]);
                }
                else{
                    std::cout << "Project is cancelled" << std::endl;
                }
           }
        }
        break;
    case 3: 
        {
            if(langs.contains(argv[2])){
                project_name.assign(argv[1]);
                project_lang.assign(argv[2]);
                std::cout << std::endl << "Project name: " << project_name << std::endl << "Language: " << project_lang << std::endl;
                if(validateChoice("Create project? Y/n: ")){
                    createProject(project_lang, project_name, langs[project_lang]);
                }
                else{
                    std::cout << "Project is cancelled" << std::endl;
                }
            }
            else{
                std::cout << argv[2] << " is not a valid language"<< std::endl;
                std::cout << "Try a different language, or the subcommand \"--helps\" for the available list of supported programming languages" << std::endl;
            }
        }
        break;
    default:
        std::cout << "Too many arguments, try \"--help\" for more info" << std::endl;
        break;
    }
    return 0;
}