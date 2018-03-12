#include "pointer.h"

VALUE cXrbPointer;

// xrb_pointer_get returns a Ruby Xrb::Pointer with the current position.
//
// On failure, a Ruby Xrb::Pointer with no instance variables is returned.
static VALUE xrb_pointer_get(VALUE k) {
	xcb_query_pointer_cookie_t c;
	xcb_query_pointer_reply_t * r;
	VALUE i;

	// TODO (skotchpine): This should be refactored (#1).
	i = rb_class_new_instance(0, NULL, cXrbPointer);

	c = xcb_query_pointer(xrb_klass_cxn(k), xrb_klass_root(k));
	r = xcb_query_pointer_reply(xrb_klass_cxn(k), c, NULL);
	if (r == NULL) {
		return i;
	}

	// TODO (skotchpine): What is this switch doing?
	if (r->child == XCB_NONE) {
		rb_iv_set(i, "@x", INT2FIX(r->root_x));
		rb_iv_set(i, "@y", INT2FIX(r->root_y));
	} else {
		rb_iv_set(i, "@x", INT2FIX(r->win_x));
		rb_iv_set(i, "@y", INT2FIX(r->win_y));
	}

	return i;
}

// Moves the pointer to coordinates
// given as a Ruby Array. A Ruby Xrb::Pointer is returned
// with the new coordinates.
static VALUE xrb_pointer_set(VALUE k, VALUE x, VALUE y) {
	VALUE i;

	Check_Type(x, T_FIXNUM);
	Check_Type(y, T_FIXNUM);

	xcb_warp_pointer(xrb_klass_cxn(k), XCB_NONE, xrb_klass_root(k),
	                 0, 0, 0, 0,
	                 FIX2INT(x), FIX2INT(y));

	xcb_aux_sync(xrb_klass_cxn(k));

	// TODO (skotchpine): This should be refactored (#1).
	i = rb_class_new_instance(0, NULL, cXrbPointer);

	rb_iv_set(i, "@x", x);
	rb_iv_set(i, "@y", y);

	return i;
}

//	class Xrb::Pointer < Xrb::Base
//	  attr_reader :x, :y
//
//	  def self.get; end
//	  def self.set; end
//	end
void Init_pointer() {
	VALUE s;

	cXrbPointer = rb_define_class_under(mXrb, "Pointer", cXrbBase);

	rb_define_attrs(cXrbPointer, 1, 0, 2, "x", "y");

	s = rb_singleton_class(cXrbPointer);
	rb_undef_method(s, "new");

	rb_define_method(s, "get", xrb_pointer_get, 0);
	rb_define_method(s, "set", xrb_pointer_set, 2);
}
