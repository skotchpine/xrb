#pragma once

#include "xrb.h"

#include <stdarg.h>

// This file will have unused functions and GCC won't compile them if so.
#pragma GCC diagnostic ignored "-Wunused-function"

// Provides a variadic rb_define_attr.
void rb_define_attrs(VALUE k, int get, int set, int n, const char *init, ...) {
	va_list l;
	char *cstr;
	int i;

	va_start(l, init);
	cstr = strdup(init);

	for (i = 0; i < n; i ++) {
		rb_define_attr(k, cstr, get, set);
		cstr = strdup(va_arg(l, char *));
	}

	va_end(l);
}

// Casts to Ruby's true or false.
static VALUE rbool(int x) {
	return x ? Qtrue : Qfalse;
}

// Converts an XCB/X11 ID to a Ruby String.
//
// The largest theoretical unsigned 32-bit integer is 4,294,967,295,
// which is 8 hex digits (ffffffff), so our largest string
// ("0xffffffff\0") will cost 11 characters.
static VALUE xid_to_rstr(uint32_t xid) {
	char cstr[11];
	sprintf(cstr, "0x%08x", xid);
	return rb_str_new2(cstr);
}

// Converts a Ruby String to XCB/X11 ID.
static uint32_t rstr_to_xid(VALUE rstr) {
	return (uint32_t)strtoul(StringValueCStr(rstr), NULL, 16);
}

// Gets a property as a Ruby String. Returns an empty string on
// failure or if no value is found.
static VALUE prop_to_rstr(xcb_connection_t *cxn,
                          xcb_window_t xid,
                          xcb_atom_t prop) {
	xcb_get_property_cookie_t c;
	xcb_get_property_reply_t *r;

	uint8_t *xstr;
	char *cstr;

	c = xcb_get_property(cxn, 0, xid, prop, XCB_ATOM_STRING, 0L, 32L);
	r = xcb_get_property_reply(cxn, c, NULL);
	if (r == NULL) {
		return rb_str_new2("");
	}

	xstr = xcb_get_property_value(r);
	cstr = malloc(sizeof(uint8_t) * (r->value_len + 1));

	for (size_t i = 0; i < r->value_len; i ++) {
		cstr[i] = xstr[i];
	}
	cstr[r->value_len] = '\0';

	free(r);
	return rb_str_new2(cstr);
}

// Converts an X atom to a Ruby String.
static VALUE atom_to_rstr(xcb_connection_t *cxn, xcb_atom_t prop) {
	xcb_get_atom_name_cookie_t c;
	xcb_get_atom_name_reply_t *r;
	char *cstr, *xstr;

	c = xcb_get_atom_name(cxn, prop);
	r = xcb_get_atom_name_reply(cxn, c, NULL);

	xstr = xcb_get_atom_name_name(r);
	cstr = malloc(sizeof(char *) * (r->name_len + 1));

	for (uint16_t i = 0; i < r->name_len; i ++) {
		cstr[i] = xstr[i];
	}
	cstr[r->name_len] = '\0';

	free(r);
	return rb_str_new2(cstr);
}
