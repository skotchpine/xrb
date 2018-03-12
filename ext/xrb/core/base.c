#include "core.h"

VALUE cXrbBase;

// Get this class's connection or base's singleton class's connection
extern VALUE xrb_klass_connection(VALUE k) {
	VALUE connection;

	connection = rb_attr_get(k, rb_intern("@connection"));
	if (connection != Qnil) {
		return connection;
	}

	return rb_attr_get(cXrbBase, rb_intern("@connection"));
}

// Get this instance's connection, its class's connection
// or its superclass's connection.
extern VALUE xrb_inst_connection(VALUE i) {
	VALUE connection;

	connection = rb_attr_get(i, rb_intern("@connection"));
	if (connection != Qnil) {
		return connection;
	}

	return xrb_klass_connection(rb_obj_class(i));
}

// Gets the xcb connection.
extern xcb_connection_t *xrb_inst_cxn(VALUE i) {
	return xrb_connection_data(xrb_inst_connection(i))->cxn;
}

// Gets the screen.
extern xcb_screen_t *xrb_inst_scr(VALUE i) {
	return xrb_connection_data(xrb_inst_connection(i))->scr;
}

// Gets the root window.
extern xcb_window_t xrb_inst_root(VALUE i) {
	return xrb_connection_data(xrb_inst_connection(i))->root;
}

// Gets the xcb connection.
extern xcb_connection_t *xrb_klass_cxn(VALUE i) {
	return xrb_connection_data(xrb_klass_connection(i))->cxn;
}

// Gets the screen.
extern xcb_screen_t *xrb_klass_scr(VALUE i) {
	return xrb_connection_data(xrb_klass_connection(i))->scr;
}

// Gets the root window.
extern xcb_window_t xrb_klass_root(VALUE i) {
	return xrb_connection_data(xrb_klass_connection(i))->root;
}

// Defines new and connection on a subclass of Xrb::Base.
static VALUE xrb_base_inherit(VALUE k, VALUE s) {
	rb_define_singleton_method(s, "new", rb_class_new_instance, -1);
	rb_define_singleton_method(s, "connection", xrb_klass_connection, 0);
	return Qnil;
}

//	class Xrb::Base
//	  attr_accessor :connection
//	  def connection; end
//
//	  class << self
//	    attr_accessor :connection
//	    undef :new
//	    def inherited; end
//	  end
//	end
extern void init_xrb_base() {
	cXrbBase = rb_define_class_under(mXrb, "Base", rb_cObject);
	rb_define_method(cXrbBase, "connection", xrb_inst_connection, 0);

	rb_define_attr(rb_singleton_class(cXrbBase), "connection", 1, 1);
	rb_undef_method(rb_singleton_class(cXrbBase), "new");

	rb_define_singleton_method(cXrbBase, "inherited", xrb_base_inherit, 1);
}
