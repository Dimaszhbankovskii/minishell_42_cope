#ifndef MINISHELL_H
# define MINISHELL_H
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <errno.h>
# include <string.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "../libft/libft.h"
# include <fcntl.h>
# include <sys/wait.h>
# include <string.h>

# define NAME_HERE_DOC "tmp_here_doc"
# define HERE_DOC_EXIST 1
# define ERROR_NUMBER_ARGS "Error: pipex takes 4 or more arguments"
# define ERROR_INIT_PIPEX "Error: malloc 'pipex'"
# define ERROR_INIT_STOP_WORD "Error: malloc 'stop word'"
# define ERROR_PARSING_CMDS "Error: malloc in parsing commands"
# define ERROR_NO_PATHS_IN_ENVP "Error: no PATH in envp"
# define ERROR_PARSING_PATHS "Error: malloc in parsing"
# define ERROR_INIT_PATH_CMD "Error malloc in init paths of commands"
# define ERROR_SEARCH_PATH_CMD "Error: malloc in search path"
# define ERROR_NO_PATH_CMD "No path for command"
# define ERROR_CREATE_HERE_DOC "Error: create here_doc"
# define ERROR_GET_NEXT_LINE "Error: get next line"
# define ERROR_CLOSE_HERE_DOC "Error: close here_doc"
# define ERROR_UNLINK "Error: unlink"
# define ERROR_CLOSE_FD "Error: close fd"
# define ERROR_OPEN_FILE_READ "Error: open file for reading"
# define ERROR_OPEN_FILE_WRITE "Error: open file for writing"
# define ERROR_PIPE "Error: create pipe"
# define ERROR_FORK "Error: create child-process"
# define ERROR_DUP2 "Error: dup2"
# define ERROR_EXECVE "Error: execve"
# define ERROR_WAIT_CHILD "Error: waitpid child"
# define SEPARATOR 1 // ' ', '\t'
# define WORD 2
# define QUOTES 3 // ''
# define DQUOTES 4 // ""
# define RDR_IN 5 // <
# define RDR_OUT 6 // >
# define RDR_SRC 7 // <<
# define RDR_APD 8 // >>
# define PIPE 9 // |



typedef struct s_token
{
    char            *content;
    int             type;
    struct s_token  *next;
}               t_token;

typedef struct s_dict

{
    char            *key;
    char            *value;
    struct s_dict   *next;
}               t_dict;

typedef struct s_cmd
{
    int             id;
    char            **args;
    int             count;
    t_dict          *infd;
    t_dict          *outfd;
    char            *tmpname;
    struct s_cmd    *next;
}               t_cmd;

typedef struct s_main
{
    char    *input;
    char    **envp;
    int     status;
    t_token *tokens;
    t_cmd   *cmd;
}               t_main;

typedef struct s_pipexb
{
	char	**argv;
	char	*stop_word;
	int		num_cmds;
	char	***cmds;
	char	**paths;
	char	**path_cmd;
	int		num_pipes;
	int		*pipes;
	int		index;
	pid_t	child;
	t_cmd *cmd;
}				t_pipexb;


t_main  g_data;
void    lexer(void);
void    lexer_update_tokens(void);
void    open_variable(t_token *token);
void    parser(void);
t_token *new_token(char *str, int type, int *index);
void    add_token(t_token **tokens, t_token *new);
void    free_tokens(void);
t_cmd   *new_cmd(void);
t_cmd   *last_cmd(t_cmd *cmds);
void    add_cmd(t_cmd **cmds, t_cmd *new);
t_dict  *new_dict(char *key, t_token *token);
t_dict  *last_dict(t_dict *dict);
void    add_dict(t_dict **dict, t_dict *new);
void    clear_g_data(void);
int     max(int a, int b);
void    check_print(void);

char	**find_str(char **array, char *str);

//pipex
void	parsing_data(t_pipexb *pipex);
void	parsing_here_doc(t_pipexb *pipex);
void	create_paths(t_pipexb *pipex);
void	open_files(t_pipexb *pipex);
void	open_here_doc(t_pipexb *pipex);
void	close_here_doc(t_pipexb *pipex);

void	close_pipes(t_pipexb *pipex);
void	child_process(t_pipexb *pipex);

void	end_program(char *mess, t_pipexb *pipex, int mode, int code);
void	close_files(t_pipexb *pipex);

void	free_pipex(t_pipexb *pipex);
void	free_one_dim_array(char *array);
void	free_two_dim_array(char **array);
void	free_three_dim_array(char ***array);
t_pipexb	*init_pipex();
//

//functions
size_t	ft_strlen(const char *str);
int		ft_strcmp(char const *str1, char const *str2);
int		ft_strncmp(const char *str1, const char *str2, size_t n);
char	**ft_split(char const *s, char c);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_strdup(const char *s1);
void	*ft_memmove(void *dst, const void *src, size_t n);

int		get_next_line(int fd, char **line);
int		free_buffer(int read_bytes, char *buffer);
//
#endif


