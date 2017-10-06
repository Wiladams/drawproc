#include <string.h>
#include <stdio.h>

#include "dp_bufferio.h"

/*
* Buffered input functions to replace stdio functions
*/
void
GdImageBufferInit(buffer_t &buffer, void *startdata, size_t size)
{
	buffer.start = (unsigned char *)startdata;
	buffer.size = size;
	buffer.offset = 0;
}

void
GdImageBufferSeekTo(buffer_t &buffer, size_t offset)
{
	if (offset < buffer.size)
		buffer.offset = offset;
}

int
GdImageBufferRead(buffer_t &buffer, void *dest, size_t size)
{
	size_t copysize;

	if (buffer.offset == buffer.size)
		return 0;	// EOF

	if (buffer.offset + size > buffer.size)
		copysize = buffer.size - buffer.offset;
	else copysize = size;

	memcpy(dest, buffer.start + buffer.offset, copysize);

	buffer.offset += copysize;

	return copysize;
}

int
GdImageBufferGetChar(buffer_t &buffer)
{
	if (buffer.offset == buffer.size)
		return EOF;
	return buffer.start[buffer.offset++];
}

// Read a string from a file.  The terminator can be:
// '\r'		- carriage return alone
// '\r\n'	- carriage return followed by newline
// '\n'		- newline alone

char *
GdImageBufferGetString(buffer_t &buffer, char *dest, size_t size)
{
	if (size < 1) {
		return nullptr;
	}

	int i, o;
	size_t copysize = size - 1;

	if (buffer.offset == buffer.size)
		return 0;

	if (buffer.offset + copysize > buffer.size)
		copysize = buffer.size - buffer.offset;

	o = 0;
	i = buffer.offset;
	while (i < buffer.offset + copysize) {
		//for (o = 0, i = buffer.offset; i < buffer.offset + copysize; i++, o++) {
		if (buffer.start[i] == '\r') {
			if (buffer.start[i+1] != '\n') {
				break;
			}
			i = i + 1;

			continue;
		}

		if ((buffer.start[i] == '\n'))
			break;

		dest[o] = buffer.start[i];

		o = o + 1;
		i = i + 1;
	}

	buffer.offset = i + 1;
	dest[o] = 0;

	return dest;
}

bool
GdImageBufferEOF(buffer_t &buffer)
{
	return (buffer.offset == buffer.size);
}

