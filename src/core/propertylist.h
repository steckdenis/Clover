#ifndef __PROPERTYLIST_H__
#define __PROPERTYLIST_H__

#define SIMPLE_ASSIGN(type, _value) do {    \
    value_length = sizeof(type);            \
    type##_var = _value;                    \
    value = & type##_var;                   \
} while (0);
    
#define STRING_ASSIGN(string) do {          \
    static const char str[] = string;       \
    value_length = sizeof(str);             \
    value = (void *)str;                    \
} while (0);

#define MEM_ASSIGN(size, buf) do {          \
    value_length = size;                    \
    value = (void *)buf;                    \
} while (0);

#endif