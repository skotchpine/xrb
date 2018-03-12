#include "event.h"

VALUE cXrbCreateEvent;
VALUE cXrbDestroyEvent;
VALUE cXrbUnmapEvent;
VALUE cXrbMapEvent;
VALUE cXrbMapRequestEvent;
VALUE cXrbVisibleEvent;

void xrb_window_event_free(xrb_window_event_d *d) {
	free(d);
}

VALUE xrb_window_event_alloc(VALUE i) {
	xrb_window_event_d *d = malloc(sizeof(*d));
	return Data_Wrap_Struct(i, NULL, xrb_window_event_free, d);
}

static xrb_window_event_d *xrb_window_event_data(VALUE i) {
	xrb_window_event_d *d;
	Data_Get_Struct(i, xrb_window_event_d, d);
	return d;
}

VALUE xrb_window_event_window(VALUE i) {
	return xrb_window_new(xrb_window_event_data(i)->window);
}

VALUE xrb_window_event_event(VALUE i) {
	return xrb_window_new(xrb_window_event_data(i)->event);
}

VALUE xrb_window_event_parent(VALUE i) {
	return xrb_window_new(xrb_window_event_data(i)->parent);
}

void rb_define_window_event_data(VALUE *k, const char *name) {
	*k = rb_define_class_under(mXrb, name, cXrbEvent);
	rb_define_alloc_func(*k, xrb_window_event_alloc);

	rb_define_method(*k, "window", xrb_window_event_window, 0);
}

extern VALUE xrb_create_event_new(xcb_generic_event_t *g) {
	xcb_create_notify_event_t *e = (xcb_create_notify_event_t *)g;
	VALUE i = rb_class_new_instance(0, NULL, cXrbCreateEvent);

	xrb_window_event_data(i)->window = e->window;
	xrb_window_event_data(i)->parent = e->parent;

	rb_iv_set(i, "@id", INT2FIX((uint32_t)e->response_type));

	return i;
}

extern VALUE xrb_destroy_event_new(xcb_generic_event_t *g) {
	xcb_destroy_notify_event_t *e = (xcb_destroy_notify_event_t *)g;
	VALUE i = rb_class_new_instance(0, NULL, cXrbDestroyEvent);

	xrb_window_event_data(i)->window = e->window;
	xrb_window_event_data(i)->event = e->event;

	rb_iv_set(i, "@id", INT2FIX((uint32_t)e->response_type));

	return i;
}

extern VALUE xrb_unmap_event_new(xcb_generic_event_t *g) {
	xcb_unmap_notify_event_t *e = (xcb_unmap_notify_event_t *)g;
	VALUE i = rb_class_new_instance(0, NULL, cXrbUnmapEvent);

	xrb_window_event_data(i)->window = e->window;
	xrb_window_event_data(i)->event = e->event;

	rb_iv_set(i, "@id", INT2FIX((uint32_t)e->response_type));

	return i;
}

extern VALUE xrb_map_event_new(xcb_generic_event_t *g) {
	xcb_map_notify_event_t *e = (xcb_map_notify_event_t *)g;
	VALUE i = rb_class_new_instance(0, NULL, cXrbMapEvent);

	xrb_window_event_data(i)->window = e->window;
	xrb_window_event_data(i)->event = e->event;

	rb_iv_set(i, "@id", INT2FIX((uint32_t)e->response_type));

	return i;
}

extern VALUE xrb_map_request_event_new(xcb_generic_event_t *g) {
	xcb_map_request_event_t *e = (xcb_map_request_event_t *)g;
	VALUE i = rb_class_new_instance(0, NULL, cXrbMapRequestEvent);

	xrb_window_event_data(i)->window = e->window;
	xrb_window_event_data(i)->parent = e->parent;

	rb_iv_set(i, "@id", INT2FIX((uint32_t)e->response_type));

	return i;
}

extern VALUE xrb_visible_event_new(xcb_generic_event_t *g) {
	xcb_visibility_notify_event_t *e = (xcb_visibility_notify_event_t *)g;
	VALUE i = rb_class_new_instance(0, NULL, cXrbVisibleEvent);

	xrb_window_event_data(i)->window = e->window;

	rb_iv_set(i, "@id", INT2FIX((uint32_t)e->response_type));

	return i;
}

void init_window_events() {
	rb_define_window_event_data(&cXrbCreateEvent, "CreateEvent");
	rb_define_method(cXrbCreateEvent, "parent", xrb_window_event_parent, 0);

	rb_define_window_event_data(&cXrbDestroyEvent, "DestroyEvent");

	rb_define_window_event_data(&cXrbUnmapEvent, "UnmapEvent");
	rb_define_method(cXrbUnmapEvent, "event", xrb_window_event_event, 0);

	rb_define_window_event_data(&cXrbMapEvent, "MapEvent");
	rb_define_method(cXrbMapEvent, "event", xrb_window_event_event, 0);

	rb_define_window_event_data(&cXrbVisibleEvent, "VisibleEvent");
}
