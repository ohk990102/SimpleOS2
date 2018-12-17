#include <limits.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

static bool print(const char* data, size_t length) {
	const unsigned char* bytes = (const unsigned char*) data;
	for (size_t i = 0; i < length; i++)
		if (putchar(bytes[i]) == EOF)
			return false;
	return true;
}
static int printi(int i, int b, int sg, int letbase)
{
	int t, len = 0;
	unsigned int u = i;

	if (i == 0) {
		putchar('0');
		len++;
	}

	if (sg && b == 10 && i < 0) {
		u = -i;
		putchar('-');
		len++;
	}

	char printi_buf[100];
	int idx = 0;
	while (u) {
		t = u % b;
		if( t >= 10 )
			t += letbase - '0' - 10;
		printi_buf[100-++idx] = t + '0';
		u /= b;
		len++;
	}
	if(!print(&printi_buf[100-idx], idx))
		return -1;


	return len;
}
int printf(const char* restrict format, ...) {
	va_list parameters;
	va_start(parameters, format);

	int written = 0;

	while (*format != '\0') {
		size_t maxrem = INT_MAX - written;

		if (format[0] != '%' || format[1] == '%') {
			if (format[0] == '%')
				format++;
			size_t amount = 1;
			while (format[amount] && format[amount] != '%')
				amount++;
			if (maxrem < amount) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(format, amount))
				return -1;
			format += amount;
			written += amount;
			continue;
		}

		const char* format_begun_at = format++;

		if (*format == 'c') {
			format++;
			char c = (char) va_arg(parameters, int /* char promotes to int */);
			if (!maxrem) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(&c, sizeof(c)))
				return -1;
			written++;
		} else if (*format == 's') {
			format++;
			const char* str = va_arg(parameters, const char*);
			size_t len = strlen(str);
			if (maxrem < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(str, len))
				return -1;
			written += len;
		}
		else if( *format == 'd' ) {
			format++;
			int i = va_arg(parameters, int);
			int len = printi (i, 10, 1, 'a');
			if(len == -1)
				return -1;
			written += len;
			continue;
		}
		else if( *format == 'x' ) {
			format++;
			int i = va_arg(parameters, int);
			int len = printi (i, 16, 1, 'a');
			if(len == -1)
				return -1;
			written += len;
			continue;
		}
		else if( *format == 'X' ) {
			format++;
			int i = va_arg(parameters, int);
			int len = printi (i, 16, 1, 'A');
			if(len == -1)
				return -1;
			written += len;
			continue;
		}
		else if( *format == 'u' ) {
			format++;
			int i = va_arg(parameters, int);
			int len = printi (i, 16, 1, 'a');
			if(len == -1)
				return -1;
			written += len;
			continue;
		}else {
			format = format_begun_at;
			size_t len = strlen(format);
			if (maxrem < len) {
				// TODO: Set errno to EOVERFLOW.
				return -1;
			}
			if (!print(format, len))
				return -1;
			written += len;
			format += len;
		}
	}

	va_end(parameters);
	return written;
}