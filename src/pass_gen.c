#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

void	help_msg(void)
{
	printf("You must provide two argument to the program:\
		\n the file to store the password.\
		\n the platform you're gonna use that password in.\
		\n Ex: ./pass_gen file.txt \"google account\"\n");
}

int	args_check(int ac, char **av)
{
	int	fd = -1;

	if (ac != 3)
	{
		help_msg();
		return (-1);
	}
	if (av[1][0] == '\0' || av[2][0] == '\n')
	{
		printf("Invalide argument: empty string provided\n");
		return (-1);
	}
	if ((fd = open(av[1], O_CREAT | O_APPEND | O_WRONLY, 0644)) == -1)
	{
		printf("Can't open the file %s\n", av[1]);
		return (-1);
	}
	return (fd);
}

char	*pass_gen(void)
{
	char	*ret;
	char	upper_case[27] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	char	lower_case[27] = "abcdefghijklmnopqrstuvwxyz";
	char	digits[10] = "0123456789";
	char	specials[14] = "!@#$%^&*()_-=+";
	srandom(time(NULL));

	ret = malloc(13 * sizeof(char));
	if (!ret)
		return (NULL);
	for (int i = 0; i < 3; i++)
	{
		ret[i] = upper_case[random() % 26];
		ret[i + 3] = lower_case[random() % 26];
		ret[i + 6] = digits[random() % 10];
		ret[i + 9] = specials[random() % 14];
	}
	return (ret);
}

void	shuffle(char **str)
{
	int		index;
	char		tmp;
	struct timeval	tv;

	for (int i = 0; i < 12; i++)
	{
		gettimeofday(&tv, NULL);
		srandom((unsigned)tv.tv_usec);
		index = random() % 12;
		tmp = (*str)[index];
		(*str)[index] = (*str)[i];
		(*str)[i] = tmp;
	}
}

void	putstr_fd(char *str, int fd)
{
	if (!str)
		return ;
	for (int i = 0; str[i]; i++)
		write(fd, &str[i], 1);
}

void	puts_in_file(char *pass, char *platform, int fd)
{
	putstr_fd("This password is for ", fd);
	putstr_fd(platform, fd);
	putstr_fd("\n", fd);
	putstr_fd(pass, fd);
	putstr_fd("\n\n", fd);
}

int	main(int ac, char **av)
{
	int	fd;
	char	*pass;

	fd = args_check(ac, av);
	if (fd == -1)
		return (-1);
	pass = pass_gen();
	shuffle(&pass);
	puts_in_file(pass, av[2], fd);
	close(fd);
	free(pass);
	return (0);
}
