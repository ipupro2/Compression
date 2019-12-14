#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <memory.h>
#include <assert.h>
#include <stdarg.h>

typedef unsigned int   uint;
typedef unsigned char uint8;
typedef unsigned int   uint32;
typedef unsigned char stbi_uc;

//static const char *failure_reason;
//
//static int e(const char *str)
//{
//	failure_reason = str;
//	return 0;
//}
//
//#ifdef STBI_NO_FAILURE_STRINGS
//#define e(x,y)  0
//#elif defined(STBI_FAILURE_USERMSG)
//#define e(x,y)  e(y)
//#else
//#define e(x,y)  e(x)
//#endif
//
//#define epuc(x,y)  ((unsigned char *) (e(x,y)?NULL:NULL))

typedef struct
{
	uint32 img_x, img_y;
	int img_n, img_out_n;

#ifndef STBI_NO_STDIO
	FILE  *img_file;
	int buflen;
	uint8 buffer_start[128];
	int from_file;
#endif
	uint8 *img_buffer, *img_buffer_end;
} stbi;

static void refill_buffer(stbi *s)
{
	int n = (int)fread(s->buffer_start, 1, s->buflen, s->img_file);
	if (n == 0) {
		s->from_file = 0;
		s->img_buffer = s->img_buffer_end - 1;
		*s->img_buffer = 0;
	}
	else {
		s->img_buffer = s->buffer_start;
		s->img_buffer_end = s->buffer_start + n;
	}
}

static int get8(stbi *s)
{
	if (s->img_buffer < s->img_buffer_end)
		return *s->img_buffer++;
#ifndef STBI_NO_STDIO
	if (s->from_file) {
		refill_buffer(s);
		return *s->img_buffer++;
	}
#endif
	return 0;
}

static uint8 get8u(stbi *s)
{
	return (uint8)get8(s);
}

static int get16le(stbi *s)
{
	int z = get8(s);
	return z + (get8(s) << 8);
}

static uint32 get32le(stbi *s)
{
	uint32 z = get16le(s);
	return z + (get16le(s) << 16);
}

static void skip(stbi *s, int n)
{
#ifndef STBI_NO_STDIO
	if (s->img_file) {
		int blen = (int)(s->img_buffer_end - s->img_buffer);
		if (blen < n) {
			s->img_buffer = s->img_buffer_end;
			fseek(s->img_file, n - blen, SEEK_CUR);
			return;
		}
	}
#endif
	s->img_buffer += n;
}

// returns 0..31 for the highest set bit
static int high_bit(unsigned int z)
{
	int n = 0;
	if (z == 0) return -1;
	if (z >= 0x10000) n += 16, z >>= 16;
	if (z >= 0x00100) n += 8, z >>= 8;
	if (z >= 0x00010) n += 4, z >>= 4;
	if (z >= 0x00004) n += 2, z >>= 2;
	if (z >= 0x00002) n += 1, z >>= 1;
	return n;
}

static int bitcount(unsigned int a)
{
	a = (a & 0x55555555) + ((a >> 1) & 0x55555555); // max 2
	a = (a & 0x33333333) + ((a >> 2) & 0x33333333); // max 4
	a = (a + (a >> 4)) & 0x0f0f0f0f; // max 8 per 4, now 8 bits
	a = (a + (a >> 8)); // max 16 per 8 bits
	a = (a + (a >> 16)); // max 32 per 8 bits
	return a & 0xff;
}

static int shiftsigned(int v, int shift, int bits)
{
	int result;
	int z = 0;

	if (shift < 0) v <<= -shift;
	else v >>= shift;
	result = v;

	z = bits;
	while (z < 8) {
		result += v >> z;
		z += bits;
	}
	return result;
}


static uint8 compute_y(int r, int g, int b)
{
	return (uint8)(((r * 77) + (g * 150) + (29 * b)) >> 8);
}

static unsigned char *convert_format(unsigned char *data, int img_n, int req_comp, uint x, uint y)
{
	int i, j;
	unsigned char *good;

	if (req_comp == img_n) return data;
	assert(req_comp >= 1 && req_comp <= 4);

	good = (unsigned char *)malloc(req_comp * x * y);
	if (good == NULL) {
		free(data);
		return 0;
	}

	for (j = 0; j < (int)y; ++j) {
		unsigned char *src = data + j * x * img_n;
		unsigned char *dest = good + j * x * req_comp;

#define COMBO(a,b)  ((a)*8+(b))
#define CASE(a,b)   case COMBO(a,b): for(i=x-1; i >= 0; --i, src += a, dest += b)
		// convert source image with img_n components to one with req_comp components;
		// avoid switch per pixel, so use switch per scanline and massive macros
		switch (COMBO(img_n, req_comp)) {
			CASE(1, 2) dest[0] = src[0], dest[1] = 255; break;
			CASE(1, 3) dest[0] = dest[1] = dest[2] = src[0]; break;
			CASE(1, 4) dest[0] = dest[1] = dest[2] = src[0], dest[3] = 255; break;
			CASE(2, 1) dest[0] = src[0]; break;
			CASE(2, 3) dest[0] = dest[1] = dest[2] = src[0]; break;
			CASE(2, 4) dest[0] = dest[1] = dest[2] = src[0], dest[3] = src[1]; break;
			CASE(3, 4) dest[0] = src[0], dest[1] = src[1], dest[2] = src[2], dest[3] = 255; break;
			CASE(3, 1) dest[0] = compute_y(src[0], src[1], src[2]); break;
			CASE(3, 2) dest[0] = compute_y(src[0], src[1], src[2]), dest[1] = 255; break;
			CASE(4, 1) dest[0] = compute_y(src[0], src[1], src[2]); break;
			CASE(4, 2) dest[0] = compute_y(src[0], src[1], src[2]), dest[1] = src[3]; break;
			CASE(4, 3) dest[0] = src[0], dest[1] = src[1], dest[2] = src[2]; break;
		default: assert(0);
		}
#undef CASE
	}

	free(data);
	return good;
}

static void start_file(stbi *s, FILE *f)
{
	s->img_file = f;
	s->buflen = sizeof(s->buffer_start);
	s->img_buffer_end = s->buffer_start + s->buflen;
	s->img_buffer = s->img_buffer_end;
	s->from_file = 1;
}

static stbi_uc *bmp_load(stbi *s, int *x, int *y, int *comp, int req_comp)
{
	uint8 *out;
	unsigned int mr = 0, mg = 0, mb = 0, ma = 0, fake_a = 0;
	stbi_uc pal[256][4];
	int psize = 0, i, j, compress = 0, width;
	int bpp, flip_vertically, pad, target, offset, hsz;
	if (get8(s) != 'B' || get8(s) != 'M') return 0;
	get32le(s); // discard filesize
	get16le(s); // discard reserved
	get16le(s); // discard reserved
	offset = get32le(s);
	hsz = get32le(s);
	if (hsz != 12 && hsz != 40 && hsz != 56 && hsz != 108) return 0;
	if (hsz == 12) {
		s->img_x = get16le(s);
		s->img_y = get16le(s);
	}
	else {
		s->img_x = get32le(s);
		s->img_y = get32le(s);
	}
	if (get16le(s) != 1) return 0;
	bpp = get16le(s);
	if (bpp == 1) return 0;
	flip_vertically = ((int)s->img_y) > 0;
	s->img_y = abs((int)s->img_y);
	if (hsz == 12) {
		if (bpp < 24)
			psize = (offset - 14 - 24) / 3;
	}
	else {
		compress = get32le(s);
		if (compress == 1 || compress == 2) return 0;
		get32le(s); // discard sizeof
		get32le(s); // discard hres
		get32le(s); // discard vres
		get32le(s); // discard colorsused
		get32le(s); // discard max important
		if (hsz == 40 || hsz == 56) {
			if (hsz == 56) {
				get32le(s);
				get32le(s);
				get32le(s);
				get32le(s);
			}
			if (bpp == 16 || bpp == 32) {
				mr = mg = mb = 0;
				if (compress == 0) {
					if (bpp == 32) {
						mr = 0xffu << 16;
						mg = 0xffu << 8;
						mb = 0xffu << 0;
						ma = 0xffu << 24;
						fake_a = 1; // @TODO: check for cases like alpha value is all 0 and switch it to 255
					}
					else {
						mr = 31u << 10;
						mg = 31u << 5;
						mb = 31u << 0;
					}
				}
				else if (compress == 3) {
					mr = get32le(s);
					mg = get32le(s);
					mb = get32le(s);
					// not documented, but generated by photoshop and handled by mspaint
					if (mr == mg && mg == mb) {
						// ?!?!?
						return 0;
					}
				}
				else
					return 0;
			}
		}
		else {
			assert(hsz == 108);
			mr = get32le(s);
			mg = get32le(s);
			mb = get32le(s);
			ma = get32le(s);
			get32le(s); // discard color space
			for (i = 0; i < 12; ++i)
				get32le(s); // discard color space parameters
		}
		if (bpp < 16)
			psize = (offset - 14 - hsz) >> 2;
	}
	s->img_n = ma ? 4 : 3;
	if (req_comp && req_comp >= 3) // we can directly decode 3 or 4
		target = req_comp;
	else
		target = s->img_n; // if they want monochrome, we'll post-convert
	out = (stbi_uc *)malloc(target * s->img_x * s->img_y);
	if (!out) return 0;
	if (bpp < 16) {
		int z = 0;
		if (psize == 0 || psize > 256) { free(out); return 0; }
		for (i = 0; i < psize; ++i) {
			pal[i][2] = get8u(s);
			pal[i][1] = get8u(s);
			pal[i][0] = get8u(s);
			if (hsz != 12) get8(s);
			pal[i][3] = 255;
		}
		skip(s, offset - 14 - hsz - psize * (hsz == 12 ? 3 : 4));
		if (bpp == 4) width = (s->img_x + 1) >> 1;
		else if (bpp == 8) width = s->img_x;
		else { free(out); return 0; }
		pad = (-width) & 3;
		for (j = 0; j < (int)s->img_y; ++j) {
			for (i = 0; i < (int)s->img_x; i += 2) {
				int v = get8(s), v2 = 0;
				if (bpp == 4) {
					v2 = v & 15;
					v >>= 4;
				}
				out[z++] = pal[v][0];
				out[z++] = pal[v][1];
				out[z++] = pal[v][2];
				if (target == 4) out[z++] = 255;
				if (i + 1 == (int)s->img_x) break;
				v = (bpp == 8) ? get8(s) : v2;
				out[z++] = pal[v][0];
				out[z++] = pal[v][1];
				out[z++] = pal[v][2];
				if (target == 4) out[z++] = 255;
			}
			skip(s, pad);
		}
	}
	else {
		int rshift = 0, gshift = 0, bshift = 0, ashift = 0, rcount = 0, gcount = 0, bcount = 0, acount = 0;
		int z = 0;
		int easy = 0;
		skip(s, offset - 14 - hsz);
		if (bpp == 24) width = 3 * s->img_x;
		else if (bpp == 16) width = 2 * s->img_x;
		else /* bpp = 32 and pad = 0 */ width = 0;
		pad = (-width) & 3;
		if (bpp == 24) {
			easy = 1;
		}
		else if (bpp == 32) {
			if (mb == 0xff && mg == 0xff00 && mr == 0xff000000 && ma == 0xff000000)
				easy = 2;
		}
		if (!easy) {
			if (!mr || !mg || !mb) return 0;
			// right shift amt to put high bit in position #7
			rshift = high_bit(mr) - 7; rcount = bitcount(mr);
			gshift = high_bit(mg) - 7; gcount = bitcount(mr);
			bshift = high_bit(mb) - 7; bcount = bitcount(mr);
			ashift = high_bit(ma) - 7; acount = bitcount(mr);
		}
		for (j = 0; j < (int)s->img_y; ++j) {
			if (easy) {
				for (i = 0; i < (int)s->img_x; ++i) {
					int a;
					out[z + 2] = get8u(s);
					out[z + 1] = get8u(s);
					out[z + 0] = get8u(s);
					z += 3;
					a = (easy == 2 ? get8(s) : 255);
					if (target == 4) out[z++] = (uint8)a;
				}
			}
			else {
				for (i = 0; i < (int)s->img_x; ++i) {
					uint32 v = (bpp == 16 ? get16le(s) : get32le(s));
					int a;
					out[z++] = (uint8)shiftsigned(v & mr, rshift, rcount);
					out[z++] = (uint8)shiftsigned(v & mg, gshift, gcount);
					out[z++] = (uint8)shiftsigned(v & mb, bshift, bcount);
					a = (ma ? shiftsigned(v & ma, ashift, acount) : 255);
					if (target == 4) out[z++] = (uint8)a;
				}
			}
			skip(s, pad);
		}
	}
	if (flip_vertically) {
		stbi_uc t;
		for (j = 0; j < (int)s->img_y >> 1; ++j) {
			stbi_uc *p1 = out + j*s->img_x*target;
			stbi_uc *p2 = out + (s->img_y - 1 - j)*s->img_x*target;
			for (i = 0; i < (int)s->img_x*target; ++i) {
				t = p1[i], p1[i] = p2[i], p2[i] = t;
			}
		}
	}

	if (req_comp && req_comp != target) {
		out = convert_format(out, target, req_comp, s->img_x, s->img_y);
		if (out == NULL) return out; // convert_format frees input on failure
	}

	*x = s->img_x;
	*y = s->img_y;
	if (comp) *comp = target;
	return out;
}

stbi_uc *stbi_bmp_load_from_file(FILE *f, int *x, int *y, int *comp, int req_comp)
{
	stbi s;
	start_file(&s, f);
	return bmp_load(&s, x, y, comp, req_comp);
}

// có rút gọn
unsigned char *stbi_load_from_file(FILE *f, int *x, int *y, int *comp, int req_comp)
{
	return stbi_bmp_load_from_file(f, x, y, comp, req_comp);
}

unsigned char *stbi_load(char const *filename, int *x, int *y, int *comp, int req_comp)
{
	FILE *f = fopen(filename, "rb");
	unsigned char *result;
	if (!f) return 0;
	result = stbi_load_from_file(f, x, y, comp, req_comp);
	fclose(f);
	return result;
}

