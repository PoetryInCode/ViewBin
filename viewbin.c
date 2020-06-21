#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>

#define BINARY "%c%c%c%c%c%c%c%c"

#define BYTE_TO_BINARY(a) \
	(a & 0x80 ? '1' : '0'), \
	(a & 0x40 ? '1' : '0'), \
	(a & 0x20 ? '1' : '0'), \
	(a & 0x10 ? '1' : '0'), \
	(a & 0x08 ? '1' : '0'), \
	(a & 0x04 ? '1' : '0'), \
	(a & 0x02 ? '1' : '0'), \
	(a & 0x01 ? '1' : '0')

#define HEX 0
#define INT 1
#define CHAR 2
#define BIN 3

int strcmp(const char *str1, const char *str2) {
	while(*str1 && *str1 == *str2) {
		++str1;
		++str2;
	}
	return (int)(unsigned char)(*str1) - (int)(unsigned char)(*str2);
}

void help() {
	printf("\nviewbin [file name]: View the contents of a file as raw data\n");
	printf("\t-i,--integer\n\t\tDisplay the contents of the file in integer form\n");
	printf("\t-c,--char\n\t\tDisplays the contents of the file as characters\n");
	printf("\t-b,--binary\n\t\tDisplays the contents of the file in binary format\n");
	printf("\t-h,--help\n\t\tDisplays this help message\n\n");
}

const char block[4] = {(char)0xe2,(char)0xac,(char)0x9c,'\0'};

int main(int argc, char **argv) {
	int print_mode=HEX,file_index=1,file_found=0;
	if(!(argc > 1)) {
		printf("[ERROR] No file provided");
		help();
		return -1;
	} else {
		int i=1;
		while(i < argc) {
			if(argv[i][0] != '-') {
				file_found=1;
			}
			if(strcmp(argv[i],"-h") == 0 || strcmp(argv[i],"--help") == 0) {
				help();
				return -1;
			} else if(strcmp(argv[i],"-i") == 0 || strcmp(argv[i],"--integer") == 0) {
				print_mode=INT;
				if(!file_found) file_index++;
			} else if(strcmp(argv[i],"-c") == 0 || strcmp(argv[i],"--char") == 0) {
				print_mode=CHAR;
				if(!file_found) file_index++;
			} else if(strcmp(argv[i],"-b") == 0 || strcmp(argv[i],"--binary") == 0) {
				print_mode=BIN;
				if(!file_found) file_index++;
			}
			++i;
		}
	}
	FILE *file;
	if(file_found) {
		file = fopen(argv[file_index],"r");
	}
	if(!file) {
		printf("[ERROR] Could not open file");
		return -1;
	}
	fseek(file,0,SEEK_END);
	int length = ftell(file);
	rewind(file);

	struct winsize w;
	ioctl(0, TIOCGWINSZ, &w);

	int width=w.ws_col;

	int chunk_size=3;
	if(print_mode == INT) {
		chunk_size=9;
	} else if(print_mode == BIN) {
		chunk_size=9;
	} else if(print_mode == CHAR) {
		chunk_size=2;
	}

	int max_per_row=(int)((width-12)/chunk_size);
	int lines = length/max_per_row;

	if(length%max_per_row != 0) {
		lines++;
	}

	char *text = (char *)malloc(length*sizeof(char));
	char *end = text+(length*sizeof(char));
	size_t original = (size_t)text;
	fread(text,sizeof(char),length,file);
	int i=0,byte_index=0,bytes_shown=0;
	while(i < lines) {
		printf("\033[32m[%08x]>\033[39m ",byte_index);
		int a=0;
		bytes_shown=0;
		while(a<max_per_row) {
			if(text <= end) {
				if(print_mode == HEX) {
					if(*text == (char)0x00) {
						printf("\033[90m00 \033[39m");
					} else {
						printf("%x%x ",(char)(*text & 0x0F),(char)(*text >> 4 & 0x0F));
					}
				} else if(print_mode == INT) {
					printf("%08i ",(int)*text);
				} else if(print_mode == CHAR) {
					if(*text == '\n') {
						printf("\\n ");
						a++;
					} else if(*text == '\t') {
						printf("\\t ");
						a++;
					} else if(*text == ' ') {
						printf("%s ",block);
					} else {
						printf("%c ",*text);
					}
				} else if(print_mode == BIN) {
					printf(BINARY" ",BYTE_TO_BINARY(*text));
				}
				bytes_shown++;
				text++;
			} else {
				break;
				break;
			}
			a++;
		}
		byte_index+=bytes_shown;
		if(i < lines-1) {
			printf("\n");
		}
		i++;
	}
	if((12+(bytes_shown*chunk_size)+11) < width) {
		printf("\033[32m<[%08x]\033[39m",byte_index);
	} else {
		printf("\n\033[32m<[%08x]\033[39m",byte_index);
	}
	free((void *)original);
	return 0;
}
