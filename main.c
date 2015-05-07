#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	if(argc != 3)
	{
		printf("bmp2map [BMP_SRC] [MAP_DEST]\n");
		return 1;
	}

	FILE* src = NULL;
	FILE* dst = NULL;
	unsigned char buffer[5];
	int initOffsetBMP;

	src = fopen(argv[1], "r");
	dst = fopen(argv[2], "w");

	if(src == NULL)
	{
		printf("Unable to open the BMP file\n");
		return 2;
	}

	if(dst == NULL)
	{
		printf("Unable to open the MAP file\n");
		return 3;
	}

	//printf("Reading BMP\n");

	fseek(src, 0x0A, SEEK_SET);
	buffer[0] = getc(src);
	buffer[1] = getc(src);
	buffer[2] = getc(src);
	buffer[3] = getc(src);
	//printf("Pixel array offset %02x %02x %02x %02x\n", buffer[3], buffer[2], buffer[1], buffer[0]);
	initOffsetBMP = buffer[0] + ((int)buffer[1] * 256);

	fseek(src, 0x12, SEEK_SET);
	buffer[0] = getc(src);
	buffer[1] = getc(src);
	buffer[2] = getc(src);
	buffer[3] = getc(src);
	//printf("Width %02x %02x %02x %02x\n", buffer[3], buffer[2], buffer[1], buffer[0]);

	//fseek(src, 0x12, SEEK_SET);
	buffer[0] = getc(src);
	buffer[1] = getc(src);
	buffer[2] = getc(src);
	buffer[3] = getc(src);
	//printf("Height %02x %02x %02x %02x\n", buffer[3], buffer[2], buffer[1], buffer[0]);

	fseek(src, 0x1C, SEEK_SET);
	buffer[0] = getc(src);
	buffer[1] = getc(src);
	//printf("Bits Per Pixel %02x %02x\n", buffer[0], buffer[1]);

	if(buffer[1] != 0 || buffer[0] != 0x08)
	{
		printf("Could not read BMP as it's in %d bits per pixel\n", buffer[0]);
		return 1;
	}

	fseek(src, initOffsetBMP, SEEK_SET);

	//printf("Now reading BMP pixels and printing them to the MAP file\n");
	while(1)
	{
		buffer[0] = getc(src);
		if(feof(src))
			break;
		//printf("Pixel BMP 24 bits Grey %02X\n", buffer[0]);
		fprintf(dst, "%c", buffer[0]);
	}

	//printf("Done !\n");

	fclose(src);
	fclose(dst);

	return 0;
}
