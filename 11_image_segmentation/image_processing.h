#ifndef IMAGE_PROCESSING_H
#define IMAGE_PROCESSING_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include <assert.h>

typedef enum ImageError ImageError;
enum ImageError {
    Image_Processing_error_Success = 0,
    Image_Processing_error_NullFilename,
    Image_Processing_error_FileDNE,
    Image_Processing_error_NoWriteAccess,
    Image_Processing_error_BrokenBMPHeader,
    Image_Processing_error_ReadFail,
    Image_Processing_error_max
};


/* 			Bitmap file structure 			*/
/* 	--------------------------------------- */
/* 	Name				Size 	Optional?	*/
/* 	---------------------------------------	*/
/* 	BMP File header		14B		No 			*/
/* 	DIB header			fixed	No			*/
/*	Bit masks			12/16B	Yes			*/
/*	Color Table			var		Yes			*/
/*	~~padding gap~~		var		Yes			*/
/* 	Pixel array			var		No			*/
/* 	~~padding gap~~		var		Yes			*/
/*	ICC Color Profile	var		Yes			*/

/* 			Bitmap File Header				*/
/* 	--------------------------------------- */
/* 	Offset	Size	Purpose					*/
/* 	--------------------------------------- */
/* 	00 		2B 		header field used to	*/
/* 					identify the BMP/DIB	*/
/* 					file is 0x42 0x4D.		*/
/*											*/
/* 	02 		4B 		size of BMP in bytes.	*/
/*											*/
/* 	06 		2B 		RESERVED. Depends on 	*/ 
/*					application. Can be 0.	*/
/*											*/
/* 	08 		2B 		Same as above.		 	*/ 
/*											*/
/* 	0A 		4B 		Offset for pixel array  */

/* 			DIB Header (useful info)		*/
/* 	--------------------------------------- */
/* 	Offset	Size	Purpose					*/
/* 	--------------------------------------- */
/* 	0E 		4B 		size of this header 	*/
/* 	12 		2B 		bmp width in pixels		*/
/* 	14 		2B 		bmp height in pixels	*/
/* 	16 		2B 		#color planes MUST BE 1 */
/* 	18 		2B 		#bits per pixel			*/

/* Pixel data is stored in BMP file  as BGR-pad-BGR-...*/
typedef struct RGBPixel RGBPixel;
struct RGBPixel {
	union {char blue, b;};
	union {char green, g;};
	union {char red, r;};
};

typedef struct BitmapHeader BitmapHeader;
struct BitmapHeader {
	char name[2];
	uint32_t bmph_image_size;
	uint32_t bmph_reserved;
	uint32_t bmph_image_offset;
};

typedef struct DIBHeader DIBHeader;
struct DIBHeader {
	uint32_t dibh_header_size;
	uint16_t dibh_image_width;
	uint16_t dibh_image_height;
	uint16_t dibh_color_planes;
	uint16_t dibh_bits_per_pixel;
};

typedef struct BitmapImage BitmapImage;
struct BitmapImage {
	union {BitmapHeader bitmap_file_header, bmph;};
	union {DIBHeader dib_header, dibh;};
	RGBPixel** pixel_data;
};

int get_bmp_file(BitmapImage* bmp, DIBHeader* dib_header, BitmapHeader* bmp_header, char filename[]);

int close_bmp_file(BitmapImage* bmp);

int write_bmp_file(BitmapImage* bmp, char filename[]);

size_t get_bmp_stride(uint32_t width, uint8_t alignment, size_t size_of_base_type);

size_t get_bmp_padding(uint32_t width, size_t size_of_base_type, size_t stride);

#endif