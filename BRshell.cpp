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
using namespace std;

#define clear() printf("\033[H\033[J")
string command_line = "";
map<string, string> alias_map;

void init_BRshell()
{
    clear();
    printf("\n\n\n\n******************"
        "************************");
    printf("\n\n\n\t**** Welcome to BRshell ****");
    printf("\n\n\t- By Fernanda Macedo - ");
    printf("\n\n\n\n*******************"
        "***********************");
    char* username = getenv("USER");
    printf("\n\n\nUSER is: @%s", username);
    printf("\n");
}

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

void read_input()
{
    cout << "BRsh-" << current_user() << "-" << current_dir() << "->";  
    getline(cin, command_line);

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

// funcao para ler o arquivo com os paths executaveis (preciso saber qual e o path para usar na funcao que executa o comando que o usuario quer)
void read_config_paths()
{
    vector<string> profile_lines = read_file_lines(".BRbshrc_profile");
    // ler o arquivo e quebrar a string em um vetor de paths ["bin/ls", "bin/ps", .... ]
    
}

void read_aliases()
{   
    vector<string> alias_lines = read_file_lines(".BRshrc"); 
  
    for (int i = 0; i < 13; i++) 
    {
        if (alias_lines[i].find("alias ") != string::npos) 
        {
            string new_command = alias_lines[i].substr(6, alias_lines[i].size()-6);            
            string command = string_split(new_command, " ");
           
            new_command.erase(new_command.begin());
            new_command.erase(new_command.end()-1);
            command.erase(command.begin());
            command.erase(command.end()-1);
    
            alias_map.insert(pair<string, string>(new_command,command));
        }
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

int command_line_process(string command_line)
{
    // processa a linha de comando (string)
    
    // se pah é aqui que faz sentido chamar read_BRshrc_profile() e read_BRshrc()



    return 0;
}

int command_line_execution(string command_line)
{
    // executa a linha de comando (de acordo com o path do comando, apos comando ser identificado a partir do alias lido )
    // exemplo:

    // char *my_program[3] = {"/bin/ls", "-l",NULL};
    // execv(my_program[0],my_program);
    // printf("Cannot execute the command.\n");
    // return 0;

    return 0;
}

// se o usuario digitar ver eu preciso executar essa funcao que mostra a versao
void show_version()
{
    cout << "BRshell - Version: 1.0 - Updated at July 28, 2022 - Author: Fernanda Macedo de Sousa." << endl;
}


int main()
{
    init_BRshell();
    // read_config_paths(); 
    read_aliases();

    // while (command_line != "exit")
    // {
    //     read_input();
        
    //     command_line_process(command_line);

    //     command_line_execution(command_line);
        
    // }
 
    return 0;
}