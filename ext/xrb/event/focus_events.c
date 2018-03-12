#include "event.h"

VALUE cXrbFocusInEvent;
VALUE cXrbFocusOutEvent;

typedef struct xrb_focus_event_d {
	xcb_window_t window;
} xrb_focus_event_d;

void xrb_focus_event_free(xrb_focus_event_d *d) {
	free(d);
}

VALUE xrb_focus_event_alloc(VALUE i) {
	xrb_focus_event_d *d = malloc(sizeof(*d));
	return Data_Wrap_Struct(i, NULL, xrb_focus_event_free, d);
}

static xrb_focus_event_d *xrb_focus_event_data(VALUE i) {
	xrb_focus_event_d *d;
	Data_Get_Struct(i, xrb_focus_event_d, d);
	return d;
}

VALUE xrb_focus_event_window(VALUE i) {
	return xrb_window_new(xrb_focus_event_data(i)->window);
}

void rb_define_focus_event_data(VALUE *k, const char *name) {
	*k = rb_define_class_under(mXrb, name, cXrbEvent);
	rb_define_alloc_func(*k, xrb_focus_event_alloc);

	rb_undef_method(rb_singleton_class(*k), "new");

	rb_define_method(*k, "window", xrb_focus_event_window, 0);
}

extern VALUE xrb_focus_in_event_new(xcb_generic_event_t *g) {
	xcb_focus_in_event_t *e = (xcb_focus_in_event_t *)g;
	VALUE i = rb_class_new_instance(0, NULL, cXrbFocusInEvent);

	xrb_focus_event_data(i)->window = e->event;

	rb_iv_set(i, "@id", INT2FIX((uint32_t)e->response_type));

	return i;
}

extern VALUE xrb_focus_out_event_new(xcb_generic_event_t *g) {
	xcb_focus_out_event_t *e = (xcb_focus_out_event_t *)g;
	VALUE i = rb_class_new_instance(0, NULL, cXrbFocusOutEvent);

	xrb_focus_event_data(i)->window = e->event;

	rb_iv_set(i, "@id", INT2FIX((uint32_t)e->response_type));

	return i;
}

void init_focus_events() {
	rb_define_focus_event_data(&cXrbFocusInEvent,  "FocusInEvent");
	rb_define_focus_event_data(&cXrbFocusOutEvent, "FocusOutEvent");
}
