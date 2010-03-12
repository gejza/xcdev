
#include <stdio.h>
#include <assert.h>
#include <memory.h>
#include <malloc.h>
#include <string.h>
#include <stddef.h>
#include "xc/string.h"
#include "xc/error.h"

//#define _XC_STRING_DEBUG

static xc_string_type_t xc_string_type_default;

/****** system ******/
XC_API size_t xc_strlen(const char* str)
{
	return strlen(str);
}

XC_API const char* xc_strdup(const char* str)
{
	return strdup(str);
}

XC_API void xc_strfree(const char* str)
{
	if (str) free(str);
}

XC_API int xc_stricmp(const char* s1, const char* s2)
{
#ifdef _WIN32
	return _stricmp(s1, s2);
#else
	return strcasecmp(s1, s2);
#endif
}

XC_API int xc_strcmp(const char* s1, const char* s2)
{
	return strcmp(s1, s2);
}

/****** string ******/
XC_API int xc_string_cmp(xc_string_t* left, xc_string_t* right)
{
	if (left == right)
		return 0;
	return xc_strcmp(xc_string_const_ptr(left), xc_string_const_ptr(right));
}

static __inline int on_write(struct _xc_string_t** data, size_t size, int copy)
{
	return (*data)
				? (*data)->type->_onwrite(data, size, copy)
				: xc_string_type_default._onwrite(data, size, xcfalse);
}

XC_API void xc_string_incref(xc_string_t* data)
{
	if (data && data->type->_incref)
		data->type->_incref(&data);
}

XC_API void xc_string_decref(xc_string_t** data)
{
	if (*data && (*data)->type->_decref)
	{
		(*data)->type->_decref(data);
	}
}

XC_API void xc_string_resize(xc_string_t** data, size_t size)
{
	on_write(data, size, xctrue);
}

XC_API void xc_string_reserve(xc_string_t** data, size_t size)
{
	on_write(data, size, xcfalse);

}

XC_API void xc_string_ref(xc_string_t** dest, xc_string_t* src)
{
	xc_string_decref(dest);
	*dest = src;
	xc_string_incref(*dest);
}

XC_API void xc_string_set(xc_string_t** data, const char* ptr, size_t size)
{
	if (!size)
	{
		if (!ptr || *ptr == '\0')
		{
			xc_string_decref(data);
			*data = NULL;
			return;
		}
		size = xc_strlen(ptr);
	}

	if (on_write(data, size, xcfalse))
	{
		memcpy(xc_string_ptr(*data), ptr, size+1);
	}
	else
	{
		// assert
	}
}

XC_API void xc_string_append(xc_string_t** data, const char* ptr, size_t size)
{
	const size_t origsize = xc_string_size(*data);
	//if (!ptr || !*ptr)
	//	return;
	// assert
	if (!size)
	{
		size = strlen(ptr);
	}
	if (on_write(data, origsize+size, xctrue))
	{
#ifdef _XC_STRING_DEBUG
		printf("string: append %p %s %ld.\n", *data, ptr, size);
#endif
		register char* p = xc_string_ptr(*data) + origsize;
		memcpy(p, ptr, size);
		// insert end
		p[size] = 0;
	}
}

////////////////////////////////////////////////////////
// default type
typedef struct _xc_string_def_t
{
	xc_string_type_t* type;
	int ref;
	size_t size;
	size_t reserved;
} xc_string_def_t;

static xc_string_def_t* default_new(size_t size)
{
	size_t s = xc_align(xc_string_type_default.off_ptr + size + 1, 16);
	xc_string_def_t* str = malloc(s);
#ifdef _XC_STRING_DEBUG
	printf("string: alloc %ld bytes at %p.\n", s, str);
#endif
	if (!str)
		xc_error_fatal(XCERR_OUTOFMEM, "Out of memory for allocate string.");
	memset(str, 0, xc_string_type_default.off_ptr);
	str->type = &xc_string_type_default;
	str->ref = 1;
	str->size = size;
	str->reserved = s - sizeof(xc_string_def_t);
	return str;
}

static xc_string_def_t* default_realloc(xc_string_def_t* s, size_t size)
{
	size_t a = xc_align(xc_string_type_default.off_ptr + size + 1, 16);
	s = realloc(s, a);
#ifdef _XC_STRING_DEBUG
	printf("string: realloc %ld bytes at %p.\n", a, s);
#endif
	if (!s)
		xc_error_fatal(XCERR_OUTOFMEM, "Out of memory for allocate string.");
	s->size = size;
	s->reserved = a - sizeof(xc_string_def_t);
	return s;
}

static int FASTCALL default_incref(struct _xc_string_t** data)
{
	return ++(*data)->ref;
}

static int FASTCALL default_decref(struct _xc_string_t** data)
{
	if ((*data)->ref > 1)
		return --(*data)->ref;
	// dealloc
	free(*data);
#ifdef _XC_STRING_DEBUG
	printf("string: free %p.\n", *data);
#endif
	*data = NULL;
	return 0;
}

static xcbool_t FASTCALL default_onwrite(xc_string_def_t** data, size_t size, xcbool_t copy)
{
	if (!*data)
	{
		// vytvor nove
		*data = default_new(size);
		return xctrue;
	}
	if ((*data)->ref > 1)
	{
		xc_string_def_t* s = default_new(size);
		if (copy)
		{
			memcpy(xc_string_pointer((xc_string_t*)s),
				   xc_string_pointer((xc_string_t*)*data),
				   xc_min(size,xc_string_size((xc_string_t*)*data)));
			(xc_string_pointer((xc_string_t*)s)[size]) = 0;
		}
		(*data)->ref--;
		*data = s;
		return xctrue;
	}
	if ((*data)->reserved < size)
	{
		*data = default_realloc(*data, size);
		return xctrue;
	}
	(*data)->size = size;
	return xctrue;
}

#define xc_offsetof(s, m) ((size_t)((s*)0)->m)

static xc_string_type_t xc_string_type_default = 
{
	/* name = */ "string",
	/* flags = */ XC_STRING_HAS_SIZE|XC_STRING_END,
	/* off_ptr = */ sizeof(xc_string_def_t),
	/* _ptr = */ 0, 
	/* _incref = */ default_incref, 
	/* _decref = */ default_decref, 
	/* _onwrite = */ default_onwrite,
};

