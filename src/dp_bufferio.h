#pragma once
#include <stdint.h>

// Buffered input functions to replace stdio functions
typedef struct {  /* structure for reading images from buffer   */
	unsigned char *start;	/* The pointer to the beginning of the buffer */
	size_t offset;	/* The current offset within the buffer       */
	size_t size;	/* The total size of the buffer               */
} buffer_t;

void	GdImageBufferInit(buffer_t &buffer, void *startdata, size_t size);
void	GdImageBufferSeekTo(buffer_t &buffer, size_t offset);
int		GdImageBufferRead(buffer_t &buffer, void *dest, size_t size);
int		GdImageBufferGetChar(buffer_t &buffer);
char *	GdImageBufferGetString(buffer_t &buffer, char *dest, size_t size);
bool	GdImageBufferEOF(buffer_t &buffer);
