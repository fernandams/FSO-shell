# Trabalho de Sistemas Operacionais - Shell

Repositório com o primeiro trabalho da disciplina Fundamentos de Sistemas Operacionais da Universidade de Brasília (UnB).

# **Especificação do trabalho**

## 1 - Objetivo

Implementar um **shell de Sistema Operacional simples**, que tenha uma interface mais natural com os usuários que falam o português, o **BRshell** ou simplesmente **BRsh**.

## 2 - Sintaxe dos comandos

A linha de comando do shell deve aceitar o seguinte formato

`1. cmdo_name arg[1] arg[2] ... arg[n]`

Um comando consiste de um nome do comando seguido de um ou mais argumentos. 

Ex: `ps -elf`

`2. pipe_section[1] | pipe_section[2] | pipe_section[3] | ... | pipe_section[n]`

Um comando consiste de uma ou mais "pipe sections", sendo que cada um é um simples comando separado pelo caracter **|**. Ex: `ps -elf | grep user | more`

Cada comando aparece em uma única linha e é terminado pelo caracter **newline**. O shell deve imprimir no prompt "`BRsh - nome do usuário – diretório corrente ->`" quando estiver pronto para receber um novo comando. **Ele deve processar o comando imediatamente ao receber um caracter newline e sair quando o comando exit for encontrado.**

Não assuma nenhum tamanho limite na linha de comando, incluindo o número de caracteres de comando, o número de argumentos e o número de pipe sections. Também não limite o número de processos que podem rodar em background.

## 3 - Localização do comando

Para configurar o shell, este possui um arquivo de configuração, o **`.BRbshrc_profile`**, que deverá estar no home do usuário.

Os executáveis deverão ser procurados nos diretórios listados na variável PATH e na ordem em que aparecem neste diretório. A sintaxe deverá ser a seguinte: `PATH=path1;path2;path3; . . . ;pathN.`

O shell deve procurar o comando em cada um dos diretórios acima e tentar executá-los. Em caso de falha, informe o erro ao usuário com a seguinte mensagem:

`BRsh-user1-/home/user1>:Não achei o combando`

## 4 - Histórico

**O BRsh deve se lembrar dos últimos 10 comandos digitados.** Para visualizar os comandos basta digitar "`historico`". Com a chamada a `historico` o BRsh deve imprimir no stdout os últimos 7 comandos, sendo que o último comando digitado deve aparecer primeiro, o penúltimo em segundo e assim sucessivamente.

```bash
BRsh->historico

1 ps

2 ls -al | grep aa

3 pwd

4 who

5 mkdir tmp

BRsh->
```

No exemplo existem apenas 5 comandos desde que o BRsh entrou em funcionamento. Quando o usuário entra com o comando "`historico !\n`", onde n é um inteiro de 1 a 7, o shell executa o comando correspondente imediatamente. Por exemplo:

```bash
BRsh->history 3
BRsh->pwd
```

e este comando deve ser colocado na fila de comando executados. Se o argumento n do comando historico não existir, avisar o usuário.

`BRsh-> Comando fora do intervalo do historico`

Mantenha a lista de comandos como em uma fila FIFO.   

## 5 - Comandos em background

Normalmente o shell processa um comando criando um processo filho para cada "pipe section" e espera todos os filhos terminarem antes de aceitar um novo comando. Se o operador `&` aparecer no final da linha de comando, o BRsh deve criar o processo filho e iniciar sua execução e não esperar o término para aceitar um novo comando. Quando o comando em background for iniciado, ele deve imprimir uma mensagem:

```bash
BRsh-user1-/home/user1>textedit &

BRsh-user1-/home/user1->Processo em background [n] foi iniciado

BRsh-user1-/home/user1->
```

Quando o processo em background termina, ele deve imprimir uma linha antes do próximo prompt:

```bash
BRsh-user1-/home/user1->"Processo em background [n] [executado] "comando arg1 arg2 ... argN

BRsh-user1-/home/user1->
```

onde n é um número único gerado pelo BRsh. Ele funciona como um identificador para o programa executado em background. 

## 6 - Redirecionador de comandos

O BRsh deve ser capaz de mudar o stdin e o stdout para arquivos de entrada e saida, respectivamente.

Exemplo:

`BRsh-user1-/home/user1>  wc -l < entrada.text`

O símbolo `<` sucedendo um comando ou programa indica que sua entrada não se dará pelo teclado, mas sim de um arquivo existente.

`BRsh-> ls > saida.txt`

O símbolo `>` após um comando ou programa indica que sua saída não se dará para o terminal, mas será redirecionado para o arquivo *saida.txt*, que **deverá ser criado pelo BRsh**.

`BRsh-user1-/home/user1>ls -a >> saida.txt`

O símbolo `>>` após um comando ou programa indica que sua saída não se dará para o terminal, mas será redirecionado para o arquivo *saida.txt* que será acrescentado do novo conteúdo (***append***).

## 7 - comando ver

O BRsh deve com o comando `ver`, implementado internamente (ou seja, não é um outro programa), mostrar a versão do BRshell, sua data de atualização e seu autor.

## 8 - Gerenciamento de Recursos

O BRsh deve liberar a memória e os recursos que não precise mais usar. Os processos filhos criados devem fazer a chamada `wait` de forma apropriada de forma a liberar os recursos após o seu fim. 

## 9 - Tradução para o português

No arquivo `**.BRshrc**` devem estar os alias dos comandos e utilitários originalmente nomeados em inglês para o seu respectivo em português. 

Exemplo: 

O utilitário `cd` deverá ter o seguinte alias:

```
alias "cd" "mudar".
```

Assim como os utilitário `ps`, `rm`, `mkdir`, `mv`, `cp`, `ls`, `grep`, `wc`, `echo`, `compila`, `cat`, `find`:

```
alias "ps" "processos"
alias "rm" "remove"
alias "mkdir" "criadir"
alias "mv" "move"
alias "cp" "copia"
alias "ls" "lista"
alias "grep" "filtra"
alias "wc" "conta"
alias "echo" "ecoa"
alias "cat" "mostra"
alias "find" "acha"
alias "sleep" "dorme"
```

Os aliases deverão ser um por linha.

## 10 - Processamento de comandos e programas em lote

O BRsh deve identificar um arquivo em lote quando ele for executável e sua primeira linha tiver a seguinte sequência de caracteres:

`#!/path/BRsh`, sendo o "path" o diretório do executável.

Iniciar o shell especificado e executar as instruções do arquivo em lote.

Por exemplo, o arquivo **`teste.BRsh`** possui o seguinte conteúdo:a

```bash
#o caracter # no início da linha significa que a linha é de comentário

#! /usr/bin/BRsh

processos -elf > texto.txt

conta -l < texto.txt

acha / | filtra ime > /dev/null &

dorme 100&

pwd

criadir meu_dir

muda meu_dir

move ../texto.txt ../meu_dir/.

historico

historico 5

#nova bateria de comandos

muda ..

criadir tmp

lista -a > ./tmp/saida.txt

muda tmp

mostra saida.txt ../meu_dir/texto.txt | conta -l

uname -a

# aqui neste ponto compila um programa c qualquer (exemplo.c) que imprime “Alo UnB. Feliz 2022!”.

gcc -o exemplo exemplo.c

criadir bin

muda exemplo ./bin/.

exemplo

#fim do arquivo em lote
```

**Obs: O trabalho deve ser executado em máquinas LINUX**

# **Funcionalidades implementadas**

- [x] Sintaxe dos comandos
    - [ ] Pipe
- [x] Localização dos comandos
- [x] Histórico
- [ ] Comandos em background
- [ ] Redirecionador de comandos
- [x] Comando ver
- [x] Gerenciamento de recursos
- [x] Tradução para o português
- [ ] Processamento de comandos e programas em lote

# **Compilação e execução**
Execute ```make``` para compilar e executar usando o makefile.