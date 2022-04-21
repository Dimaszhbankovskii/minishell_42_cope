#include "../includes/minishell.h"

void	open_files(t_pipexb *pipex)
{
	(void)pipex->num_cmds;




//	if (pipex->here_doc)
//		open_here_doc(pipex);
//	else
//	{
//		pipex->infile = open(pipex->argv[1], O_RDONLY);
//		if (pipex->infile < 0)
//			end_program(ERROR_OPEN_FILE_READ, pipex, 0, 0);
//	}
//	if (pipex->here_doc)
//		pipex->outfile = open(pipex->argv[pipex->argc - 1], \
//		O_WRONLY | O_CREAT | O_APPEND, 0664);
//	else
//		pipex->outfile = open(pipex->argv[pipex->argc - 1], \
//		O_WRONLY | O_CREAT | O_TRUNC, 0664);
//	if (pipex->outfile < 0)
//	{
//		close(pipex->infile);
//		end_program(ERROR_OPEN_FILE_WRITE, pipex, HERE_DOC_EXIST, 0);
//	}

}
