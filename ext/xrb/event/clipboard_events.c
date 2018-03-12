#include "event.h"

VALUE cXrbClipboardEvent;
VALUE cXrbClipboardClearEvent;
VALUE cXrbClipboardRequestEvent;

void xrb_clipboard_event_free(xrb_clipboard_event_d *d) {
	free(d);
}

VALUE xrb_clipboard_event_alloc(VALUE i) {
	xrb_clipboard_event_d *d = malloc(sizeof(*d));
	return Data_Wrap_Struct(i, NULL, xrb_clipboard_event_free, d);
}

static xrb_clipboard_event_d *xrb_clipboard_event_data(VALUE i) {
	xrb_clipboard_event_d *d;
	Data_Get_Struct(i, xrb_clipboard_event_d, d);
	return d;
}

VALUE xrb_clipboard_event_owner(VALUE i) {
	return xrb_window_new(xrb_clipboard_event_data(i)->owner);
}

VALUE xrb_clipboard_event_requestor(VALUE i) {
	return xrb_window_new(xrb_clipboard_event_data(i)->requestor);
}

void rb_define_clipboard_data(VALUE *k, const char *name) {
	*k = rb_define_class_under(mXrb, name, cXrbEvent);
	rb_define_alloc_func(*k, xrb_clipboard_event_alloc);

	rb_undef_method(rb_singleton_class(*k), "new");
}

extern VALUE xrb_clipboard_event_new(xcb_generic_event_t *g) {
	xcb_selection_notify_event_t *e = (xcb_selection_notify_event_t *)g;
	VALUE i = rb_class_new_instance(0, NULL, cXrbClipboardEvent);

	xrb_clipboard_event_data(i)->requestor = e->requestor;

	rb_iv_set(i, "@id", INT2FIX((uint32_t)e->response_type));

	return i;
}

extern VALUE xrb_clipboard_request_event_new(xcb_generic_event_t *g) {
	xcb_selection_request_event_t *e = (xcb_selection_request_event_t *)g;
	VALUE i = rb_class_new_instance(0, NULL, cXrbClipboardRequestEvent);

	xrb_clipboard_event_data(i)->owner = e->owner;
	xrb_clipboard_event_data(i)->requestor = e->requestor;

	rb_iv_set(i, "@id", INT2FIX((uint32_t)e->response_type));

	return i;
}

extern VALUE xrb_clipboard_clear_event_new(xcb_generic_event_t *g) {
	xcb_selection_clear_event_t *e = (xcb_selection_clear_event_t *)g;
	VALUE i = rb_class_new_instance(0, NULL, cXrbClipboardClearEvent);

	xrb_clipboard_event_data(i)->owner = e->owner;

	rb_iv_set(i, "@id", INT2FIX((uint32_t)e->response_type));

	return i;
}

void init_clipboard_events() {
	rb_define_clipboard_data(&cXrbClipboardEvent, "ClipboardEvent");
	rb_define_method(cXrbClipboardEvent, "requestor", xrb_clipboard_event_requestor, 0);

	rb_define_clipboard_data(&cXrbClipboardRequestEvent, "ClipboardRequestEvent");
	rb_define_method(cXrbClipboardRequestEvent, "owner", xrb_clipboard_event_owner, 0);
	rb_define_method(cXrbClipboardRequestEvent, "requestor", xrb_clipboard_event_requestor, 0);

	rb_define_clipboard_data(&cXrbClipboardClearEvent, "ClipboardClearEvent");
	rb_define_method(cXrbClipboardClearEvent, "owner", xrb_clipboard_event_owner, 0);
}
