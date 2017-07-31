#include "dp_win32.h"

/*
* Calculate size and pitch of memory gc.
* If bpp or planes is 0, use passed psd's bpp/planes.
* Pitch is calculated to be DWORD right aligned for speed.
*/
int
GdCalcMemGCAlloc(PSD psd, int width, int height, int planes, int bpp,
	unsigned int *psize, unsigned int *ppitch)
{
	unsigned int pitch;

	if (!planes)
		planes = psd->planes;
	if (!bpp)
		bpp = psd->bpp;
	/*
	* swap width and height in left/right portrait modes,
	* so imagesize is calculated properly
	*/
	if (psd->portrait & (DPPORTRAIT_LEFT | DPPORTRAIT_RIGHT)) {
		int tmp = width;
		width = height;
		height = tmp;
	}

	/* use 4bpp linear for VGA 4 planes memdc format*/
	if (planes == 4)
		bpp = 4;

	/* compute pitch: bytes per line*/
	switch (bpp) {
	case 1:
		pitch = (width + 7) / 8;
		break;
	case 2:
		pitch = (width + 3) / 4;
		break;
	case 4:
		pitch = (width + 1) / 2;
		break;
	case 8:
		pitch = width;
		break;
	case 16:
		pitch = width * 2;
		break;
	case 18:
	case 24:
		pitch = width * 3;
		break;
	case 32:
		pitch = width * 4;
		break;
	default:
		*ppitch = *psize = 0;
		return 0;
	}

	/* right align pitch to DWORD boundary*/
	pitch = (pitch + 3) & ~3;

	*psize = pitch * height;
	*ppitch = pitch;
	return 1;
}


/* alloc and initialize a new memory drawing surface (memgc)*/
PSD
GdCreatePixmap(PSD rootpsd, DPCOORD width, DPCOORD height, int format, void *pixels, int palsize)
{
	PSD		pmd;
	int 	bpp, planes, data_format, pixtype;
	unsigned int size, pitch;

	if (width <= 0 || height <= 0)
		return NULL;

	bpp = rootpsd->bpp;
	data_format = rootpsd->data_format;
	pixtype = rootpsd->pixtype;
	planes = rootpsd->planes;

	/* check if format supported*/
	switch (format) {
	case 0:			/* default, return framebuffer compatible pixmap*/
		break;
	case 32:		/* match framebuffer format if running 32bpp, else RGBA*/
		if (bpp == 32)
			break;
		/* else fall through - create RGBA8888 pixmap*/
	case DPIF_RGBA8888:
		bpp = 32;
		data_format = format;
		pixtype = DPPF_TRUECOLORABGR;
		break;
	case DPIF_BGRA8888:
		bpp = 32;
		data_format = format;
		pixtype = DPPF_TRUECOLOR8888;
		break;
		/*case DPIF_PAL1:*/				/* DPIF_PAL1 is DPIF_MONOBYTEMSB*/
	case DPIF_MONOBYTEMSB:			/* ft2 non-alias*/
	case DPIF_MONOBYTELSB:			/* t1lib non-alias*/
	case DPIF_MONOWORDMSB:			/* core mwcfont, pcf*/
		bpp = 1;
		data_format = format;
		pixtype = DPPF_PALETTE;
		break;
	case DPIF_PAL2:
		bpp = 2;
		data_format = format;
		pixtype = DPPF_PALETTE;
		break;
	case DPIF_PAL4:
		bpp = 4;
		data_format = format;
		pixtype = DPPF_PALETTE;
		break;
	case DPIF_PAL8:
		bpp = 8;
		data_format = format;
		pixtype = DPPF_PALETTE;
		break;
	case DPIF_RGB1555:
		bpp = 16;
		data_format = format;
		pixtype = DPPF_TRUECOLOR1555;
		break;
	case DPIF_RGB555:
		bpp = 16;
		data_format = format;
		pixtype = DPPF_TRUECOLOR555;
		break;
	case DPIF_RGB565:
		bpp = 16;
		data_format = format;
		pixtype = DPPF_TRUECOLOR565;
		break;
	case DPIF_RGB888:
		bpp = 24;
		data_format = format;
		pixtype = DPPF_TRUECOLOR888;
		break;
	default:
		DPRINTF("GdCreatePixmap: unsupported format %08x\n", format);
		return NULL;	/* fail*/
	}

	/*
	* Allocate offscreen drawing surface.  If screen driver doesn't
	* support blitting, this will fail.  Use root window screen
	* device for compatibility.
	*/
	pmd = rootpsd->AllocateMemGC(rootpsd);
	if (!pmd)
		return NULL;

	GdCalcMemGCAlloc(pmd, width, height, planes, bpp, &size, &pitch);

	/* Allocate space for pixel values */
	if (!pixels) {
		pixels = calloc(size, 1);
		pmd->flags |= PSF_ADDRMALLOC;
	}
	if (!pixels) {
	err:
		pmd->FreeMemGC(pmd);
		return NULL;
	}

	/* allocate palette*/
	if (palsize && (pmd->palette = (DPPALENTRY *)calloc(palsize * sizeof(DPPALENTRY), 1)) == NULL)
		goto err;
	pmd->palsize = palsize;

	pmd->MapMemGC(pmd, width, height, planes, bpp, data_format, pitch, size, pixels);
	pmd->pixtype = pixtype;		/* save pixtype for proper colorval creation*/
	pmd->ncolors = (pmd->bpp >= 24) ? (1 << 24) : (1 << pmd->bpp);

	return pmd;
}



void
GdFreePixmap(PSD pmd)
{
	pmd->FreeMemGC(pmd);
}

