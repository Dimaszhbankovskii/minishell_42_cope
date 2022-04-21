#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <errno.h>
# include <string.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <fcntl.h>
# include "../libft/libft.h"

# define FT_WEXITSTATUS(x) ((x >> 8) & 0x000000ff)

# define SEPARATOR 1 // ' ', '\t'
# define WORD 2
# define QUOTES 3 // ''
# define DQUOTES 4 // ""
# define RDR_IN 5 // <
# define RDR_OUT 6 // >
# define RDR_SRC 7 // <<
# define RDR_APD 8 // >>
# define PIPE 9 // |

# define BUILTIN_ECHO 1
# define BUILTIN_PWD 2
# define BUILTIN_ENV 3
# define BUILTIN_UNSET 4


typedef struct s_pipex
{
	int		i;
	int		num;
	int		used_pipes;
	int		pipes[2][2];
	pid_t	pid;
}				t_pipex;

typedef struct s_token
{
	char			*content;
	int				type;
	struct s_token	*next;
}				t_token;

typedef struct s_dict
{
	int				key;
	char			*value;
	struct s_dict	*next;
}				t_dict;

typedef struct s_cmd
{
	char			*id;
	char			**args;
	int				count;
	t_dict			*infd;
	t_dict			*outfd;
	char			*tmpname;
	struct s_cmd	*next;
}				t_cmd;

typedef struct s_main
{
	int		pipes[3][2]; // 0 - char *envp, 1 - count, 2 - status
	char	*input;
	char	**envp;
	int		status;
	t_token	*tokens;
	t_cmd	*cmds;
}				t_main;

t_main	g_data;

void	lexer(void);
void	lexer_update_tokens(void);
void	open_variable(t_token *token);

t_token	*new_token(char *str, int type, int *index);
void	add_token(t_token **tokens, t_token *new);
void	free_tokens(void);

void	parser(void);
void	init_id_cmd(void);

t_cmd	*new_cmd(void);
t_cmd	*last_cmd(t_cmd *cmds);
void	add_cmd(t_cmd **cmds, t_cmd *new);
void	free_cmd(void);

t_dict	*new_dict(int key, t_token *token);
t_dict	*last_dict(t_dict *dict);
void	add_dict(t_dict **dict, t_dict *new);
t_dict	*free_dict(t_dict *dict);
int		count_cmds(t_cmd *cmds);

void	executor(t_cmd *cmds);
void	handling_heredoc(t_cmd *cmds);
void	child_process(t_pipex *pipex, t_cmd *cmd);
void	redirect_input(t_cmd *cmd);
void	redirect_output(t_cmd *cmd);

void	execute_echo(t_cmd *cmd);
void	execute_pwd(void);
void 	execute_env(void);
char	**execute_unset(char **envp, char **args);

void	save_envp(void);
void	get_envp(void);

int		execute_binary(t_cmd *cmd);

void	clear_data_loop(void);
void	clear_g_data(void);

int		max(int a, int b);
char	**malloc_two_array_char(int len);
void	free_two_array_char(char **array);
void	add_back_elem_two_array_char(char **array, char *elem, int len);
char	*find_str(char **array, char *str);
int		size_two_array_char(char **array);

void	error_mess(char *mess, int code);
int		warning(char *mess, int code);
// delete
void	check_print(void);

#endif