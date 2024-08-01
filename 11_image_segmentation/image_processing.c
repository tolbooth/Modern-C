#include "image_processing.h"

// rewrite with no pointers!

void alloc_pixel_data(RGBPixel** pixel_data, uint16_t height, uint16_t width);

void dealloc_pixel_data(RGBPixel** pixel_data, uint16_t height);

int read_pixel_data(BitmapImage* bmp, FILE* bitmap_file_pointer);

int populate_bitmap_header(BitmapHeader* bmp_header, FILE* bitmap_file_pointer);

int populate_DIB_header(DIBHeader* dib_header, FILE* bitmap_file_pointer);

int handle_error_and_close(enum ImageError err, FILE* fp);

int get_bmp_file(BitmapImage* bmp, DIBHeader* dib_header, BitmapHeader* bmp_header, char filename[]) {
	assert(bmp);
	if (!filename) return Image_Processing_error_NullFilename;

	// Open for reading as binary file
	FILE* bitmap_file_pointer = fopen(filename, "rb");
	if (!bitmap_file_pointer) return Image_Processing_error_FileDNE;

	ImageError header_read_error = populate_bitmap_header(bmp_header, bitmap_file_pointer);
	if (!handle_error_and_close(header_read_error, bitmap_file_pointer))
		return header_read_error;

	ImageError DIB_read_error = populate_DIB_header(dib_header, bitmap_file_pointer);
	if (!handle_error_and_close(DIB_read_error, bitmap_file_pointer))
		return DIB_read_error;

	bmp->bitmap_file_header = *bmp_header;
	bmp->dib_header = *dib_header;

	// With the headers read, we can interpret the pixel data
	ImageError pixel_data_read_error = !read_pixel_data(bmp, bitmap_file_pointer);
	if (!handle_error_and_close(pixel_data_read_error, bitmap_file_pointer)) {
		return pixel_data_read_error;
	}

	fclose(bitmap_file_pointer);
	return Image_Processing_error_Success;
}

int populate_bitmap_header(BitmapHeader* bmp_header, FILE* bitmap_file_pointer) {
	assert(bmp_header);
	// Read in the first 2 bytes for header name
	fread(bmp_header, 2, 1, bitmap_file_pointer);
	// Valid BMP file must have these first 2 bytes
	char expected_name[2] = {0x42, 0x4D};
	if (!memcmp(bmp_header, &expected_name, 2)) {
		return Image_Processing_error_BrokenBMPHeader;
	}
	// Read in the next 12 bytes for the rest of the members
	// of header. This section of the struct is aligned with
	// the bits in the file.
	fread(&(bmp_header->bmph_image_size), 12, 1, bitmap_file_pointer);
	return Image_Processing_error_Success;
}

int populate_DIB_header(DIBHeader* dib_header, FILE* bitmap_file_pointer) {
	// Assert on precondition
	assert(dib_header);
	assert(ftell(bitmap_file_pointer) == 0x0E);

	// First 4 bytes stores the DIB header size.
	fread(dib_header, 4, 1, bitmap_file_pointer);
	// Unaligned, so move to next member, the next 8 bytes contain the rest
	// of the info we want.
	fread(&(dib_header->dibh_image_width), 8, 1, bitmap_file_pointer);

	return Image_Processing_error_Success;
}

int handle_error_and_close(ImageError err, FILE* fp) {
	if (err != Image_Processing_error_Success) {
		fclose(fp);
		fprintf(stderr, "Failed with error code %d", err);
		return 0;
	} else return 1;
}

int read_pixel_data(BitmapImage* bmp, FILE* bitmap_file_pointer) {
	assert(bmp);

	alloc_pixel_data(bmp->pixel_data, bmp->dibh.dibh_image_height, bmp->dibh.dibh_image_width);
	if (!bmp->pixel_data) return Image_Processing_error_ReadFail;

	// Pixel data is represented bottom up, rows must be aligned to
	// multiple of 4 bytes, zero padded. Bithack rounds up to next 
	// multiple of 4, unsigned literals for portability + safety.
	fseek(bitmap_file_pointer, bmp->bmph.bmph_image_offset, SEEK_SET);
	size_t stride = get_bmp_stride(bmp->dibh.dibh_image_width, sizeof(RGBPixel), 4);
	for (size_t i = bmp->dibh.dibh_image_height - 1; i > 0; --i) {
		fread(bmp->pixel_data[i], stride, 1, bitmap_file_pointer);
	}

	return Image_Processing_error_Success;
}

void alloc_pixel_data(RGBPixel** pixel_data, uint16_t height, uint16_t width) {
	pixel_data = malloc(height * sizeof(RGBPixel*));
	if (!pixel_data) return;

	size_t stride = get_bmp_stride(width, sizeof(RGBPixel), 4);
	for (size_t i = 0; i < height; ++i) {
		pixel_data[i] = malloc(stride);
		if (!pixel_data[i]) {
			dealloc_pixel_data(pixel_data, height);
			return;
		}
	}
}

void dealloc_pixel_data(RGBPixel** pixel_data, uint16_t height) {
	assert(pixel_data);
	for (size_t i = 0; i < height; ++i) {
		if (pixel_data[i]) free(pixel_data[i]);
	}
	free(pixel_data);
}

int write_bmp_file(BitmapImage* bmp, char filename[]) {
	assert(bmp);	
	if (!filename) return Image_Processing_error_NullFilename;	

	FILE* bitmap_file_pointer = fopen(filename, "wb");
	if (!bitmap_file_pointer) return Image_Processing_error_NoWriteAccess;

	// write the bitmap file header
	fwrite(&(bmp->bmph), 2, 1, bitmap_file_pointer);
	fwrite(&(bmp->bmph.bmph_image_size), 12, 1, bitmap_file_pointer);

	// write the DIB header
	fwrite(&(bmp->dibh), 4, 1, bitmap_file_pointer);
	fwrite(&(bmp->dibh.dibh_image_width), 8, 1, bitmap_file_pointer);

	// goto offset in file and write the pixel data
	fseek(bitmap_file_pointer, bmp->bmph.bmph_image_offset, SEEK_SET);
	size_t stride = get_bmp_stride(bmp->dibh.dibh_image_width, sizeof(RGBPixel), 4);
	for (size_t i = bmp->dibh.dibh_image_height - 1; i > 0; --i) {
		fwrite(bmp->pixel_data[i], stride, 1, bitmap_file_pointer);
	}

	fclose(bitmap_file_pointer);
	return Image_Processing_error_Success;
}

size_t get_bmp_stride(uint32_t width, uint8_t alignment, size_t size_of_base_type) {
	return ((width * size_of_base_type) + (alignment - 1u)) & ~((alignment - 1u));
}

size_t get_bmp_padding(uint32_t width, size_t size_of_base_type, size_t stride) {
	return (stride - width) * size_of_base_type;
}