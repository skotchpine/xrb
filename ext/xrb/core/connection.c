#include "core.h"

VALUE cXrbConnection;

// Frees the internal data and disconnects from XCB.
static void xrb_connection_free(xrb_connection_d *d) {
	if (d->cxn != NULL) {
		xcb_disconnect(d->cxn);
	}

	d->cxn = NULL;
	d->scr = NULL;

	free(d);
}

// Allocates the internal data.
static VALUE xrb_connection_alloc(VALUE i) {
	xrb_connection_d *d = malloc(sizeof(*d));
	return Data_Wrap_Struct(i, NULL, xrb_connection_free, d);
}

// Gets the internal data.
extern xrb_connection_d *xrb_connection_data(VALUE i) {
	xrb_connection_d *d;
	Data_Get_Struct(i, xrb_connection_d, d);
	return d;
}

// Gets the xcb connection.
extern xcb_connection_t *xrb_connection_connection(VALUE i) {
	return xrb_connection_data(i)->cxn;
}

// Gets the screen.
extern xcb_screen_t *xrb_connection_screen(VALUE i) {
	return xrb_connection_data(i)->scr;
}

// Gets the root window.
extern xcb_window_t xrb_connection_root(VALUE i) {
	return xrb_connection_data(i)->root;
}

// Creates an Xrb::Connection and connects to XCB. Raises Xrb::ConnectionError
// on failure.
//
// ISSUE (skotchpine): The first screen is the only screen honored, but there
//                     may be more than one screen, as with remote connections.
extern VALUE xrb_connection_initialize(VALUE i) {
	xrb_connection_d *d = xrb_connection_data(i);

	d->cxn = xcb_connect(NULL, NULL);
	if (xcb_connection_has_error(d->cxn)) {
		goto fail;
	}

	d->scr = xcb_setup_roots_iterator(xcb_get_setup(d->cxn)).data;
	if (d->scr == NULL) {
		xcb_disconnect(d->cxn);
		goto fail;
	}
	d->root = d->scr->root;

	return i;

fail:
	rb_raise(eXrbConnectionError, "Failed to connect to XCB.");
	return Qnil;
}

//	class Xrb::Connection
//	  def initialize; end
//	end
extern void init_xrb_connection() {
	cXrbConnection = rb_define_class_under(mXrb, "Connection", rb_cObject);
	rb_define_alloc_func(cXrbConnection, xrb_connection_alloc);
	rb_define_method(cXrbConnection, "initialize", xrb_connection_initialize, 0);
}
