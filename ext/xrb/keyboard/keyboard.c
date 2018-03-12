#include "keyboard.h"
#include "parse.c"

VALUE cXrbKeyboard;

extern keysym_mapping_t keysym_map[];

// get_keysym converts a string to an xcb_keysym_t.
//
// TODO (skotchpine): This should go into include/utils.
static xcb_keysym_t get_keysym(const char * name) {
	int n = sizeof(keysym_map) / sizeof(keysym_mapping_t);

	for (int i = 0; i < n; i ++) {
		if (strcmp(name, keysym_map[i].name) == 0) {
			return keysym_map[i].keysym;
		}
	}

	return 0;
}

// get_keycodes converts a string to an xcb_keycode_t reference.
//
// It is the user's responsiblity to free the keycodes after use.
//
// TODO (skotchpine): This should go into include/utils.
static xcb_keycode_t * get_keycodes(xcb_connection_t *cxn, const char * name) {
	xcb_key_symbols_t * keysyms;
	xcb_keycode_t     * keycode;

	if (!(keysyms = xcb_key_symbols_alloc(cxn))) {
		rb_raise(eXrbRuntimeError, "Failed to allocate symbol table.");
		return NULL;
	}

	keycode = xcb_key_symbols_get_keycode(keysyms, get_keysym(name));

	xcb_key_symbols_free(keysyms);

	return keycode;
}

// xrb_keyboard_get_keysym converts a Ruby String to a Ruby Fixnum
// representing an X keysym.
extern VALUE xrb_keyboard_get_keysym(VALUE k, VALUE rstr) {
	xcb_keysym_t keysym;

	Check_Type(rstr, T_STRING);

	keysym = get_keysym(StringValuePtr(rstr));
	if (keysym == 0) {
		return Qnil;
	}

	return INT2FIX(keysym);
}

// xrb_keyboard_get_keycodes converts a Ruby String to a Ruby Array of Fixnums
// representing all the matching X keycodes.
extern VALUE xrb_keyboard_get_keycodes(VALUE k, VALUE rstr) {
	xcb_keycode_t * keycodes;
	VALUE ary;
	int n;

	Check_Type(rstr, T_STRING);

	ary = rb_ary_new();

	keycodes = get_keycodes(xrb_klass_cxn(k), StringValuePtr(rstr));
	if (keycodes == NULL) {
		return ary;
	}

	n = sizeof(keycodes) / sizeof(xcb_keycode_t);
	for (int i = 0; i < n; i ++) {
		if (keycodes[i] != 0) {
			rb_ary_push(ary, INT2FIX(keycodes[i]));
		}
	}
	free(keycodes);

	return ary;
}

//	module Xrb::Keyboard < Xrb::Base
//	  def self.get_keysym; end
//	  def self.get_keycodes; end
//	end
void Init_keyboard() {
	VALUE s;

	cXrbKeyboard = rb_define_class_under(mXrb, "Keyboard", cXrbBase);

	s = rb_singleton_class(cXrbKeyboard);
	rb_undef_method(s, "new");

	rb_define_method(s, "get_keysym", xrb_keyboard_get_keysym, 1);
	rb_define_method(s, "get_keycodes", xrb_keyboard_get_keycodes, 1);
}
