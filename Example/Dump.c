//idk, finish this maybe?

#include "Include/VVVFlip.h"

int writeToFile(
	uint32_t const index,
	uint8_t const* buffer,
	size_t const size)
{

}

int main(
	int const argc,
	char const* const* argv)
{
	if (argc > 2)
	{
		blob_t blob = blob_fromAnsiPath(argv[1]);

		if (blob)
		{
			for (size_t i = 0; i < blob_getSize(blob); i++)
			{
				song_t song = blob_getSong(blob, i);
				uint8_t* buffer = song_getData(song);



				vvvflip_free(buffer);
			}

			blob_destroy(blob);
		}
		else
		{
			printf("ERROR: Could not load binary file.\n");
			return 1;
		}
	}
	else
		printf("Usage: %s [PATH TO BINARY FILE]\n", argv[0]);

	return 0;
}