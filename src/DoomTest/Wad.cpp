#include "stdafx.h"
#include "Wad.h"

#define READ_I32(buffer, offset)                                               \
((buffer)[(offset)] | ((buffer)[(offset + 1)] << 8) |                        \
((buffer)[(offset + 2)] << 16) | ((buffer)[(offset + 3)] << 24))

int wad_load_from_file(const char* filename, wad_t* wad) {
	if (wad == NULL) { return 1; }

	FILE* fp;
	errno_t err;
	err = fopen_s(&fp, filename, "rb");

	if (fp == NULL || err != 0) { return 2; }

	fseek(fp, 0, SEEK_END);
	size_t size = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	uint8_t* buffer = (uint8_t * )malloc(size);
	fread(buffer, size, 1, fp);
	fclose(fp);

	// Read header
	if (size < 12) { return 3; }
	wad->id = (char*)malloc(5);
	memcpy(wad->id, buffer, 4);
	wad->id[4] = 0; // null terminator

	wad->num_lumps = READ_I32(buffer, 4);
	wad->directory_offset = READ_I32(buffer, 8);

	wad->lumps = (lump_t*)malloc(sizeof(lump_t) * wad->num_lumps);
	for (int i = 0; i < wad->num_lumps; i++) 
	{
		uint32_t offset = wad->directory_offset + i * 16;

		wad->lumps[i].offset = READ_I32(buffer, offset);
		wad->lumps[i].size = READ_I32(buffer, offset + 4);
		wad->lumps[i].name = (char*)malloc(9);
		memcpy(wad->lumps[i].name, &buffer[offset + 8], 8);
		wad->lumps[i].name[8] = 0; // null terminator

		wad->lumps[i].data = (uint8_t*)malloc(wad->lumps[i].size);
		memcpy(wad->lumps[i].data, &buffer[wad->lumps[i].offset],
			wad->lumps[i].size);
	}

	free(buffer);
	return 0;
}

void wad_free(wad_t* wad) {
	if (wad == NULL) { return; }

	free(wad->id);
}