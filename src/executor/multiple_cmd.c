/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   multiple_cmd.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: jvan-kra/tosinga                             +#+                     */
/*                                                   +#+                      */
/*   Created: 2022/10/04 17:44:20 by jvan-kra      #+#    #+#                 */
/*   Updated: 2022/10/04 17:44:20 by jvan-kra      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "executor.h"
#include "parser.h"
#include "libft.h"
#include "environment.h"
#include <stdio.h>

static int	pipe_multiple_commands(t_command *cmd, t_childs *childs, \
			int last_cmd)
{
	int			last_pid;
	static int	flip = 0;

	last_pid = 0;
	if (!last_cmd)
	{
		if (pipe(childs->pipe_fd[flip]) < 0)
			perror_exit("minishell", EXIT_FAILURE);
	}
	if (childs->child_count == 0)
		first_cmd(cmd, childs->pipe_fd[flip]);
	else if (last_cmd == 1)
		last_pid = final_cmd(cmd, childs->pipe_fd[!flip][0]);
	else
		mid_cmd(cmd, childs->pipe_fd[!flip][0], childs->pipe_fd[flip]);
	flip = !flip;
	return (last_pid);
}

/**
 * @brief execute commands in seperate childs in case of multiple command input
 *
 * @param commands list
 * @return int exit code
 */
int	multiple_commands(t_list *commands)
{
	t_childs	childs;
	int			last_cmd;
	int			last_pid;
	int			ret_value;

	last_pid = 0;
	last_cmd = 0;
	childs.child_count = 0;
	while (commands)
	{
		if (!commands->next)
			last_cmd = 1;
		last_pid = pipe_multiple_commands(commands->content, &childs, last_cmd);
		commands = commands->next;
		childs.child_count++;
	}
	ret_value = wait_for_childs(childs.child_count, last_pid);
	return (ret_value);
}
