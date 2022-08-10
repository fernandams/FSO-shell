#include<stdio.h>
#include <string>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<readline/readline.h>
#include<readline/history.h>
#include <iostream> 
#include <fstream> 
#include <map>
#include <vector>
#include <deque>
using namespace std;

void command_line_execution();

string command_line = "";
map<string, string> alias_map;
vector<string> g_paths;
deque<string> history;

string current_dir()
{
	char cwd[1024];
	getcwd(cwd, sizeof(cwd));
    return cwd;
}

string current_user()
{
  string username = getenv("USER");
  return username;
}

void welcome_message()
{
    cout << "\n******************************************" << endl;
    cout << "\n\n\t**** Welcome to BRshell ****" << endl;
    cout << "\n\t- By Fernanda Macedo - " << endl;
    cout << "\n\n******************************************" << endl;
    cout << "\nCurrent user: @" << current_user() << "\n" << endl;
}


void add_history(string command)
{
  if (command == "historico")
    return;
  
  if (command.empty())
    return;

 history.push_front(command);
  
  if (history.size() > 10)
  {
    history.pop_back();
  }
}

void read_input()
{
    cout << "BRsh-" << current_user() << "-" << current_dir() << "-> ";  
    getline(cin, command_line);
    add_history(command_line);
}

vector<string> read_file_lines(string file_BRsh)
{
    ifstream file; 
    string myline;
    vector<string> lines;
   
    file.open(file_BRsh);

    if ( file.is_open() ) {

        while ( getline (file, myline)) {
            lines.push_back(myline);
        }
    } else {
        cout << "File not found." << endl;
    }

    file.close();

    return lines;

}

string string_split(string &text, string splitter)
{
    if (text.find(splitter) != string::npos)
    {
        string substring = text.substr(0, text.find(splitter));
        text.replace(0, substring.size() + splitter.size(), "");
        
        return substring;
    }

    return "";
}

void read_config_paths()
{
    vector<string> profile_lines = read_file_lines(".BRbshrc_profile");
    
    for (unsigned int i = 0; i < profile_lines.size(); i++)
    {
        string paths = profile_lines[i].replace(0, 5, "");

        while (paths.find(";") != string::npos)
        {
            string path = string_split(paths, ";");
            if (path != ""){
                g_paths.push_back(path);
            }
        }
    }
}

void read_aliases()
{   
    vector<string> alias_lines = read_file_lines(".BRshrc"); 
  
    for (int i = 0; i < 13; i++) 
    {
        string new_command = alias_lines[i].replace(0, 6, "");            
        string command = string_split(new_command, " ");
  
        command.replace(0, 1, ""); 
        command.replace(command.size()-1, 1,""); 
        new_command.replace(0, 1, ""); 
        new_command.replace(new_command.size()-1, 1,""); 
     
        alias_map.insert(pair<string, string>(new_command,command));
    }
}

void show_version()
{
    cout << "\n***************************************************" << endl;
    cout << "\n\n\t**** BRshell ****\n" << endl;
    cout << "\t - Version: 1.0" << endl;
    cout << "\t - Updated at: August 6, 2022" << endl;
    cout << "\t - Author: Fernanda Macedo de Sousa" << endl;
    cout << "\n\n***************************************************\n" << endl;
}

void show_history(vector<string> args)
{
    if(!args.empty()){
        unsigned arg_history = stoi(args[0]);
        if(arg_history <= 0 || arg_history >= history.size()){ 
            history.pop_front();
            cout << "Comando fora do intervalo do historico" << endl;
            return;
        }
        command_line = history[arg_history];
        history.pop_front();
        add_history(command_line);
        command_line_execution();

    }else{
    
        if(history.empty())
            return;

        cout << endl;

        for(unsigned i = 0; i < history.size(); i++){
            cout << i + 1 << "  " << history[i] << endl;
        }

        cout << endl;
    }
}

string get_aliases(string alias)
{
    if (alias_map.count(alias) > 0)
        return alias_map[alias];
    return alias;
}

vector<string> get_args(string command_executed)
{
    vector<string> arguments = vector<string>();
    
    if(command_executed.find(" ") == string::npos)
        return arguments;

    string_split(command_executed, " ");

    while(command_executed.find(" ") != string::npos)
    {
        string string_left = string_split(command_executed, " "); 
        arguments.push_back(string_left); 
    }

    if(!command_executed.empty())
        arguments.push_back(command_executed);

    return arguments;
}

void execute_single(string command_executed, vector<string> args)
{
    string cmd_alias = get_aliases(command_executed);

    int pid = fork();
    if (pid == 0){ // processo filho
        char *args_formated[args.size() + 2];
        string str_args = "";
        args_formated[0] = const_cast<char *>(""); 

        for (unsigned i = 0; i < args.size(); i++){
            args_formated[i + 1] = const_cast<char *>(args[i].c_str());
        }
        args_formated[args.size() + 1] = NULL;

        for(unsigned i = 0; i < g_paths.size(); i++){
            string cmd_path = g_paths[i] + cmd_alias;
            args_formated[0] = const_cast<char *>(cmd_path.c_str());
            
            execv(cmd_path.c_str(), args_formated);           
        }
        cout << "Não achei o comando: " << cmd_alias << endl;
        exit(0);

    }else{ // processo pai
        waitpid(pid, NULL, 0);
    }
}

void command_line_execution()
{
	string command_executed = command_line;

	if (command_executed != "" && command_executed != "exit")
    {
        vector<string> arguments = get_args(command_executed);
        if(command_executed.find(" ") != string::npos)
            command_executed = string_split(command_executed, " ");

		if (command_executed == "ver"){
			show_version();
		}else if(command_executed == "historico"){
			show_history(arguments);
		}else if(command_executed == "cd"){
            if (arguments.size() >= 1)
                chdir(arguments[0].c_str());
        }else{
            execute_single(command_executed, arguments);
        }
	}
}

int main()
{
    welcome_message();
    read_config_paths(); 
    read_aliases();

    while (command_line != "exit")
    {
        read_input();
        command_line_execution();
    }
 
    return 0;
}