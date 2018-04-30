#include "vga_kernel_print.h"
#include "utils.h"
#include "vga.h"

const int VGA_CHAR_MAX_ROW = 32;
const int VGA_CHAR_MAX_COL = 80;
const int VGA_CHAR_ROW = 30;
const int VGA_CHAR_COL = 80;
int cursor_row = 0;
int cursor_col = 0;
int cursor_freq = 31;

const int num_char_per_row = 80;

unsigned int CHAR_VRAM = 0; // dummy use, deprecated

void kernel_set_cursor()
{
	vga_set_cursor_positon(num_char_per_row * cursor_col + cursor_row);
}

//@warpper function
static void put_char_linear(char c, uint32_t linear_address)
{
	put_char(c, linear_address/80 ,linear_address%80);
}



//@warpper function
static char get_char_linear(uint32_t linear_address)
{
	char ret = get_char(linear_address/80 ,linear_address%80);
	return ret;
}

void set_vram(unsigned int starting_offset,int w, int len)
{
	//int starting_offset = (int)dest;
	w = 0;//ignore the value of w
	while (len--)
		put_char_linear(w, starting_offset);//*dest++ = w;
		starting_offset++;
	return;
}


void kernel_clear_screen(int scope)
{

	unsigned int w = 0x000fff00;
	scope &= 31;
	cursor_col = 0;
	cursor_row = 0;
	kernel_set_cursor();
	set_vram(CHAR_VRAM, w, scope * VGA_CHAR_MAX_COL);
}

//void* kernel_memcpy(void* dest, void* src, int len);
void copy_vram(int dest, int src, int len)
{
	while(len--)
	{
		int value_get = get_char_linear(src);
		put_char_linear(value_get, dest);
		src++;
		dest++;
	}
	return;
}

void kernel_scroll_screen()
{
	unsigned int w = 0x000fff00;
	copy_vram(CHAR_VRAM, (CHAR_VRAM + VGA_CHAR_MAX_COL),
		      (VGA_CHAR_ROW - 2) * VGA_CHAR_MAX_COL * 4);
	set_vram((CHAR_VRAM + (VGA_CHAR_ROW - 2) * VGA_CHAR_MAX_COL), w,
		 VGA_CHAR_MAX_COL);
}

void kernel_putchar_at(int ch, int fc, int bg, int row, int col)
{
	unsigned int *p;
	row = row & 31;
	col = col & 127;
	put_char(ch, row, col);
}


int kernel_putchar(int ch, int fc, int bg)
{
	unsigned int w = 0x000fff00;
	if (ch == '\r')
		return ch;
	if (ch == '\n') {
		set_vram(CHAR_VRAM + cursor_row * VGA_CHAR_MAX_COL + cursor_col,
			 w, VGA_CHAR_COL - cursor_col);
	    //while(1);
		cursor_col = 0;
		if (cursor_row == VGA_CHAR_ROW - 2) {
			kernel_scroll_screen();
		} else {
			cursor_row++;
		}
	} else if (ch == '\t') {
		if (cursor_col >= VGA_CHAR_COL - 4) {
			kernel_putchar('\n', 0, 0);
		} else {
			set_vram(CHAR_VRAM + cursor_row * VGA_CHAR_MAX_COL
					 + cursor_col,
				 w, 4 - cursor_col & 3);
			cursor_col = (cursor_col + 4) & (-4);
		}
	} else {
		if (cursor_col == VGA_CHAR_COL) {
			kernel_putchar('\n', 0, 0);
		}
		kernel_putchar_at(ch, fc, bg, cursor_row, cursor_col);
		cursor_col++;
	}
	kernel_set_cursor();
	return ch;
}

int kernel_puts(const char *s, int fc, int bg)
{
	int ret = 0;
	while (*s) {
		ret++;
		kernel_putchar(*s++, fc, bg);
	}
	return ret;
}

int kernel_putint(int x, int fc, int bg)
{
	char buffer[12];
	char *ptr = buffer + 11;
	int neg = 0;
	buffer[11] = 0;
	if (x == 0) {
		kernel_putchar('0', fc, bg);
		return x;
	}
	if (x < 0) {
		neg = 1;
		x = -x;
	}
	while (x) {
		ptr--;
		*ptr = (x % 10) + '0';
		x /= 10;
	}
	if (neg) {
		ptr--;
		*ptr = '-';
	}
	kernel_puts(ptr, fc, bg);
	return x;
}

// static const char *HEX_MAP = "0123456789abcdef";//put inside
int kernel_putintx(unsigned int x, int fc, int bg)
{
	char HEX_MAP[] = "0123456789abcdef"; //
	char buffer[12];
	char *ptr = buffer + 11;
	buffer[11] = 0;
	if (x == 0) {
		kernel_putchar('0', fc, bg);
		return x;
	}
	while (x) {
		ptr--;
		*ptr = HEX_MAP[x & 15];
		x >>= 4;
	}
	kernel_puts(ptr, fc, bg);
	return x;
}

int kernel_vprintf(const char *format, va_list ap)
{
	int cnt = 0;
	while (*format) {
		if (*format != '%') {
			kernel_putchar(*format++, 0xfff, 0);
		} else {
			format++;
			switch (*format) {
			case 'c': {
				char valch = va_arg(ap, int);
				kernel_putchar(valch, 0xfff, 0);
				format++;
				cnt++;
				break;
			}
			case 'd': {
				int valint = va_arg(ap, int);
				kernel_putint(valint, 0xfff, 0);
				format++;
				cnt++;
				break;
			}
			case 'x': {
				int valint = va_arg(ap, int);
				kernel_putintx(valint, 0xfff, 0);
				format++;
				cnt++;
				break;
			}
			case 's': {
				char *valstr = va_arg(ap, char *);
				kernel_puts(valstr, 0xfff, 0);
				format++;
				cnt++;
				break;
			}
			default: {
				cnt = -1;
				goto exit;
			}
			}
		}
	}
exit:
	return cnt;
}

int kernel_printf(const char *format, ...)
{
	int cnt = 0;
	va_list ap;
	va_start(ap, format);
	cnt = kernel_vprintf(format, ap);
	va_end(ap);
	return cnt;
}

int test_vga_prinf()
{
	int test_num_int = 2018;
	char c = 'a';
	kernel_printf("Hello %d %c \n888", test_num_int, 'a');
	while(1==1)
	{
		kernel_printf("On that bold hill, against a broad blue stream,\n");
		kernel_printf("stood Arthur Phillip on a day of dream;\n");
		kernel_printf("what time the mists of morning westward rolled\n");
		kernel_printf("and heaven flowered on a bay of gold.\n");
		kernel_printf("Here, in the hour that shines and sounds afar,\n");
		kernel_printf("flamed first Old England's banner like a star;\n");
		kernel_printf("Here in a time august with prayer and praise,\n");
		kernel_printf("was born the nation of these splendid days,\n");
		kernel_printf("and here, this land's majestic yesterday\n");
		kernel_printf("of immemorial silence died away \n");
		int count_down=1000000000;
		while(count_down--);
	}
	// put_string("2222", 20, 2, 2);
}