#include "event.h"

VALUE cXrbKeyDownEvent;
VALUE cXrbKeyUpEvent;
VALUE cXrbKeymapChangeEvent;

void xrb_key_event_free(xrb_key_event_d *d) {
	free(d);
}

VALUE xrb_key_event_alloc(VALUE i) {
	xrb_key_event_d *d = malloc(sizeof(*d));
	return Data_Wrap_Struct(i, NULL, xrb_key_event_free, d);
}

static xrb_key_event_d *xrb_key_event_data(VALUE i) {
	xrb_key_event_d *d;
	Data_Get_Struct(i, xrb_key_event_d, d);
	return d;
}

VALUE xrb_key_event_root(VALUE i) {
	return xrb_window_new(xrb_key_event_data(i)->root);
}

VALUE xrb_key_event_event(VALUE i) {
	return xrb_window_new(xrb_key_event_data(i)->event);
}

VALUE xrb_key_event_child(VALUE i) {
	return xrb_window_new(xrb_key_event_data(i)->child);
}

void rb_define_key_event_data(VALUE *k, const char *name) {
	*k = rb_define_class_under(mXrb, name, cXrbEvent);
	rb_define_alloc_func(*k, xrb_key_event_alloc);

	rb_undef_method(rb_singleton_class(*k), "new");

	rb_define_method(*k, "root",  xrb_key_event_root,  0);
	rb_define_method(*k, "event", xrb_key_event_event, 0);
	rb_define_method(*k, "child", xrb_key_event_child, 0);
}

extern VALUE xrb_key_down_event_new(xcb_generic_event_t *g) {
	xcb_key_press_event_t *e = (xcb_key_press_event_t *)g;
	VALUE i = rb_class_new_instance(0, NULL, cXrbKeyDownEvent);

	xrb_key_event_data(i)->root  = e->root;
	xrb_key_event_data(i)->event = e->event;
	xrb_key_event_data(i)->child = e->child;

	rb_iv_set(i, "@id", INT2FIX((uint32_t)e->response_type));
	rb_iv_set(i, "@x",  INT2FIX((uint32_t)e->event_x));
	rb_iv_set(i, "@y",  INT2FIX((uint32_t)e->event_y));

	return i;
}

extern VALUE xrb_key_up_event_new(xcb_generic_event_t *g) {
	xcb_key_release_event_t *e = (xcb_key_release_event_t *)g;
	VALUE i = rb_class_new_instance(0, NULL, cXrbKeyUpEvent);

	xrb_key_event_data(i)->root  = e->root;
	xrb_key_event_data(i)->event = e->event;
	xrb_key_event_data(i)->child = e->child;

	rb_iv_set(i, "@id", INT2FIX((uint32_t)e->response_type));
	rb_iv_set(i, "@x",  INT2FIX((uint32_t)e->event_x));
	rb_iv_set(i, "@y",  INT2FIX((uint32_t)e->event_y));

	return i;
}

extern VALUE xrb_keymap_change_event_new(xcb_generic_event_t *g) {
	// xcb_keymap_notify_event_t *e = (xcb_keymap_notify_event_t *)g;
	VALUE i = rb_class_new_instance(0, NULL, cXrbKeymapChangeEvent);

	rb_iv_set(i, "@id", INT2FIX((uint32_t)g->response_type));

	return i;
}

void init_keyboard_events() {
	rb_define_key_event_data(&cXrbKeyDownEvent, "KeyDownEvent");
	rb_define_key_event_data(&cXrbKeyUpEvent, "KeyUpEvent");

	cXrbKeymapChangeEvent = rb_define_class_under(mXrb, "KeymapChangeEvent", cXrbEvent);
}
