#ifndef _VVVFLIP_H
#define _VVVFLIP_H

#define VVVFLIP_EXPORT

#if defined(VVVFLIP_EXPORT)
	#if defined(_MSC_VER)
		#define VVVFLIP_DECL __declspec(dllexport)
	#else
		#define VVVFLIP_DECL __attribute__((visibility("default")))
	#endif
#else
	#define VVVFLIP_DECL
#endif


#if defined(__cplusplus)
#include <cstdio>
#include <cstdint>

namespace vvvflip
{
#else
#include <stdio.h>
#include <stdint.h>
#endif

typedef void* song_t;
typedef void* blob_t;

#if defined(__cplusplus)
	extern "C"
	{
#endif

/*
	Frees the buffer returned by a API call.
*/
VVVFLIP_DECL void vvvflip_free(void* p);

/*
	Get song informations.
	On failure, NULL is returned.
	When you're done, free the buffers using
	the right API call.
*/
VVVFLIP_DECL uint8_t* song_getData(song_t const song);
VVVFLIP_DECL size_t song_getSize(song_t const song);

/*
	Set song informations.
	On failure, NULL is returned.
*/
VVVFLIP_DECL int song_setData(
	song_t song,
	uint8_t* p,
	size_t const size);

/*
	Create an empty blob/destroy an existing one.
	On failure, the former returns NULL.
*/
VVVFLIP_DECL blob_t blob_create();
VVVFLIP_DECL void blob_destroy(blob_t blob);

/*
	Load a blob from a buffer or a file.
	On failure, NULL is returned.
*/
VVVFLIP_DECL blob_t blob_fromBuffer(
	uint8_t const* p,
	size_t const size);
VVVFLIP_DECL blob_t blob_fromFile(FILE* file);
VVVFLIP_DECL blob_t blob_fromWidePath(wchar_t const* path);
VVVFLIP_DECL blob_t blob_fromAnsiPath(char const* path);

/*
	Save a blob to a buffer or a file.
	On failure, NULL is returned.
	When you're done, free the buffers using
	the right API call.
*/
VVVFLIP_DECL uint8_t* blob_toBuffer(blob_t blob);
VVVFLIP_DECL int blob_toFile(
	blob_t const blob,
	FILE* file);
VVVFLIP_DECL int blob_toWidePath(
	blob_t const blob,
	wchar_t const* path);
VVVFLIP_DECL int blob_toAnsiPath(
	blob_t const blob,
	char const* path);

/*
	Add, remove, or get a song from a blob.
	On failure, NULL is returned.
*/
VVVFLIP_DECL song_t blob_addSong(
	blob_t blob,
	uint8_t const* p,
	size_t const size);
VVVFLIP_DECL int blob_removeSong(
	blob_t blob,
	uint32_t const index);
VVVFLIP_DECL song_t blob_getSong(
	blob_t blob,
	uint32_t const index);

/*
	Get the size of a blob (number of songs).
*/
VVVFLIP_DECL size_t blob_getSize(blob_t blob);

#if defined(__cplusplus)
	}
}
#endif

#endif /* _VVVFLIP_H */