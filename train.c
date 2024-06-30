/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   train.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hel-asli <hel-asli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/15 21:53:16 by hel-asli          #+#    #+#             */
/*   Updated: 2024/06/30 02:20:32 by hel-asli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Mandatory/pipex.h"

void err_exit(char *str)
{
	perror(str);
	exit(EXIT_FAILURE);
}

typedef struct s_bibex
{
	int infile;
	int outfile;
} t_bibex;

// process instance of a program that is executed on the cpu bu the system .
// ===== > each proccess have it own memory space , resource and execution context 
// program is a bench of file , source code , data, the is stored on a hard drive, memory ...
 

// fork() ==> system call is used to create a childe proccess when a program call fork --- > the os create an excat copy of the calling process; called the child process including the entire memory space (stack , heap, data segment, text and code segment) , and open file descriptor table . signals handler ,
//
/*** copy-on-write == > to optimize memory usage , the modern os implement a 
technique called COW (cope-on-write) 
	1 == > when we call fork the child and parent share the same physical memory page this page marked as read only, when an enthire process atempts to write to a shared page the os create a private copy of that page for the process that initiated the write operation . ****/ // the child process start execution imdeiatly after the fork call 
/** fork are used for runing different task like background tasks or demons 
or performe some action while the parent busy doing somthing else or to protect the parent from potentiol issue . **/

// WIFEXITED == > check if the child process exited normaly or not
// WEXITSTATUS == > get the exit status of the child use the return value of the wait 
//
// execve system call is a process execution mechanism in unix-like os , is use to execut a new program in the current program image with new one 
// execve("path" => the path of the new program to execute can be absolute or relative, argv of the new program argv[0] = name, envp);
// 1. when execve is called the executable file specified by path is loaded into the current process.
// 2. setup the argv and the envp for the new program
// 3. the new program starts executing form its entery point. with the argv and envp . 
// *** => if the execve call is successful, it does not return to the calling process because . because
// the current program image has been replaced . 
// if the execve fail (executable file not found or lacks appropriate permission) it return -1;
// and the calling process continues execution with existing program image
int main(int ac, char *av[])
{
	if (ac != 5)
	{
		fprintf(stderr, "\033[0;31mbad args\n");
		exit(EXIT_FAILURE);
	}
	t_bibex bibex;
	int fds[2];
	int a = pipe(fds);
	/*** ----------- PIPE ----------
	 * pipe system call are used to create inter process communication (IPC)
	 * A pipe is a unidirectional data channel that allows data to be passed form one process to another.
	 * pipe have a read end which is fds[0] and write end which is the fds[1]
	 * the amount of data that can be write and read from a pipe its depends on the pipe buffer size and the implement of system.
	 * in the unix-like os pipe have a fixe buffer size that is determined by the os , usally from 4KB to 64 KB
	 * fds[0] fds[1] behave like regural fd but its point to newly created pipe int the kernel memory 
	 ***/
	 
	if (a == -1)
		err_exit("pipe");
	int status;
	int status1;
	pid_t pid = fork();

	if (pid < 0)
		err_exit("fork");
	if (pid == 0)
	{
		bibex.infile = open("infile", O_RDONLY);
		if (bibex.infile == -1)
			err_exit("open");
		if (dup2(bibex.infile, 0) == -1)
			err_exit("dup2");
		close(bibex.infile);
		close(fds[0]);
		dup2(fds[1], 1);
		close(fds[1]);
		char *cmd[] = {"/bin/cat" , NULL};
		execve("/bin/cat", cmd, NULL);
		err_exit("execve");
		return (42);
	}
	else
	{
		// wait and waitpid are used by the parent ps to wait for a child and obtain te exit status or termination status of its child process .
		// 1 ===> when a child process complet his execution or stoped the wait return the pid of this child to the parent
		pid_t id = fork();
		if (id < 0)
			err_exit("fork");
		if (id == 0)
		{
			bibex.outfile = open("outfile", O_CREAT | O_RDWR | O_TRUNC, 0777);
			if (bibex.outfile == -1)
				err_exit("open");
			dup2(bibex.outfile, 1);
			close(bibex.outfile);
			close(fds[1]);
			dup2(fds[0], 0);
			close(fds[0]);

			char *argv[] = {"/bin/cat", NULL};
			execve("/bin/cat", argv, NULL);
			err_exit("execve");
			// execute the second command . 
		}
		else
		{
			close(fds[0]);
			close(fds[1]);
			pid_t n = waitpid(pid, &status, 0); // the status is a 8 bit value that the child passed to the parent .
			pid_t a = waitpid(id, &status1, 0);
			if (a == -1)
				err_exit("waitpid");
			if (n == -1)
				err_exit("waitpid");
			/** ------------- pid values ---------
			 * pid > 0 : wait for the child process with the specified PID
			 * pid = -1 : wait for any child process 
			 * pid = 0 : wait for any child process in the same process group as the calling process .
			 * pid < -1  : wait for any child process whose process group ID is equal to the absolute of pid abs(pid);
			 **/
			// the status will be filled with the exit status or termination status of the child . 
			if (WIFEXITED(status1))
			{
				int exit_status = WEXITSTATUS(status1);
				printf("status : %d ---- exit_status : %d\n", status, exit_status);
			}
			else
				fprintf(stderr, "the child process did not exit normally\n");
		}
		// it allow for parent to chose which chile want to wait for . 
	}
	return (0);
}
