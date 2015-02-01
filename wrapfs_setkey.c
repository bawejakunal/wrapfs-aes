#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#define KEYLEN 32

int main(int argc, char **argv)
{
	int opt_char, fd, errFlag, keyFlag;
	char key[KEYLEN+1];

	errFlag = 0;
	keyFlag = 0;
	memset(key, '0', KEYLEN);
	key[KEYLEN]='\0';

	while ((opt_char = getopt(argc, argv, "k:h")) != -1){
		switch(opt_char){
		case 'k':
			if (optarg == NULL)
				errFlag = 1;
			else{
				strncpy(key, optarg, strlen(optarg));
				keyFlag = 1;
			}	
			break;
		case 'h':
			fprintf(stdout, "Usage: %s {-k KEY} [-h HELP] mount_point\n", argv[0]);
                	fprintf(stdout, "-k : Use to give key. Ex: -k \"key\"\n");
			fprintf(stdout, "-h : Use to display this help message\n");
			break;
		default: 
			fprintf(stdout, "Usage: %s {-k KEY} [-h HELP] mount_point\n", argv[0]);
                	fprintf(stdout, "-k : Use to give key. Ex: -k \"key\"\n");
			errFlag = 1;
			break;

		}
	}
	
	if ((1 == errFlag) || (argc != optind + 1)){
		fprintf(stderr, "Usage: %s {-k KEY} [-h HELP] mount_point\n", argv[0]);
		return -1;
	}else if(keyFlag == 1){
		
		fd = open(argv[optind], O_RDONLY);
		if (fd != -1)
		{
			printf("Device opened read-only\nCalling ioctl\n");
			ioctl(fd, 1, key);
			close(fd);
		} else
			printf("Device not found");
	}
	return 0;
}
