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

string command_line = "";
map<string, string> alias_map;
vector<string> g_paths;
deque<string> history;
// vector<string> parsed_commands;

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
    // for (unsigned int i = 0; i < g_paths.size(); i++)
    // {
    //     cout << "G Paths:\n" << g_paths[i] << endl;  
    // }

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
    
    // map<string, string>::iterator it;

    // for (it = alias_map.begin(); it != alias_map.end(); it++)
    // {
    //     std::cout << it->first    // string (key)
    //             << ':'
    //             << it->second   // string's value 
    //             << std::endl;
    // }
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

void command_line_process(string command_line) 
{ // le a linha de comando e faz o parsing dos comandos colocando em um vetor
	
	// processa a string:
	// parsing -> divisão de comandos em palavras e strings individuais (Um comando consiste de um nome do comando seguido de um ou mais argumentos. )
    //  cmdo_name arg[1] arg[2] ... arg[n] -> ex: ps -elf

	// verificar se ha pipes (Ex: ps -elf | grep user | more)
	// verificar se & aparece no final da linha -> para fazer tratamento de comandos em background
	// verificar se é o caso de redirecionador de comandos -> lidar com leitura e escrita em arquivos

}

void show_history()
{
    if (history.empty())
        return;

    cout << endl;

    for(unsigned i = 0; i < history.size(); i++){
        cout << i + 1 << "  " << history[i] << endl;
    }

    cout << endl;
}

void execute_single(string command_executed)
{
    int pid = fork();
    if (pid == 0){ // processo filho
        char *args_formated[3];
        args_formated[0] = const_cast<char *>("");
        args_formated[1] = NULL;
        args_formated[2] = NULL;

        for(unsigned i = 0; i < g_paths.size(); i++){
            string cmd_path = g_paths[i] + command_executed;
            args_formated[0] = const_cast<char *>(cmd_path.c_str());
            
            execv(cmd_path.c_str(), args_formated);           
        }
        
    }else{ // processo pai
        waitpid(pid, NULL, 0);
    }
}

int command_line_execution()
{
	string command_executed = command_line;
   
	if (command_executed != "" && command_executed != "exit")
    {
		if (command_executed == "ver"){
			show_version();
		}else if(command_executed == "historico"){
			show_history();
		}else if(command_executed == "cd"){
            // if (args.size() >= 1)
            //     chdir(args[0].c_str());
        }else{
            execute_single(command_executed);
        }
	}

	// executa a linha de comando (de acordo com o path do comando, apos comando ser identificado a partir do alias lido )
    // exemplo:
    // char *my_program[3] = {"/bin/ls", "-l",NULL};
    // execv(my_program[0],my_program);
    // printf("Cannot execute the command.\n");
    // return 0;

	// tratamento de pipes, se houver
	// tratamento de comandos em background (se houver):
	//  o BRsh deve criar o processo filho e iniciar sua execução e não esperar o término para aceitar um novo comando. Quando o comando em background for iniciado, ele deve imprimir uma mensagem: (...); Quando o processo em background termina, ele deve imprimir uma linha antes do próximo prompt (...) onde n é um número único gerado pelo BRsh. Ele funciona como um identificador para o programa executado em background. 

	// Redirecionador de comandos ->  mudar o stdin e o stdout para arquivos de entrada e saida, respectivamente.

    return 0;
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