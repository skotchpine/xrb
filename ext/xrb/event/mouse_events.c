#include "event.h"

VALUE cXrbMouseDownEvent;
VALUE cXrbMouseUpEvent;
VALUE cXrbMouseInEvent;
VALUE cXrbMouseOutEvent;
VALUE cXrbMouseMoveEvent;

void xrb_mouse_event_free(xrb_mouse_event_d *d) {
	free(d);
}

VALUE xrb_mouse_event_alloc(VALUE i) {
	xrb_mouse_event_d *d = malloc(sizeof(*d));
	return Data_Wrap_Struct(i, NULL, xrb_mouse_event_free, d);
}

static xrb_mouse_event_d *xrb_mouse_event_data(VALUE i) {
	xrb_mouse_event_d *d;
	Data_Get_Struct(i, xrb_mouse_event_d, d);
	return d;
}

VALUE xrb_mouse_event_root_window(VALUE i) {
	return xrb_window_new(xrb_mouse_event_data(i)->root_window);
}

VALUE xrb_mouse_event_event_window(VALUE i) {
	return xrb_window_new(xrb_mouse_event_data(i)->event_window);
}

VALUE xrb_mouse_event_child_window(VALUE i) {
	return xrb_window_new(xrb_mouse_event_data(i)->child_window);
}

void rb_define_mouse_event_data(VALUE *k, const char *name) {
	*k = rb_define_class_under(mXrb, name, cXrbEvent);
	rb_define_alloc_func(*k, xrb_mouse_event_alloc);

	rb_undef_method(rb_singleton_class(*k), "new");

	rb_define_method(*k, "root_window",  xrb_mouse_event_root_window,  0);
	rb_define_method(*k, "event_window", xrb_mouse_event_event_window, 0);
	rb_define_method(*k, "child_window", xrb_mouse_event_child_window, 0);
}

extern VALUE xrb_mouse_down_event_new(xcb_generic_event_t *g) {
	xcb_button_press_event_t *e = (xcb_button_press_event_t *)g;
	VALUE i = rb_class_new_instance(0, NULL, cXrbMouseDownEvent);

	xrb_mouse_event_data(i)->root_window = e->root;
	xrb_mouse_event_data(i)->event_window = e->event;
	xrb_mouse_event_data(i)->child_window = e->child;

	rb_iv_set(i, "@id", INT2FIX((uint32_t)e->response_type));
	rb_iv_set(i, "@x", INT2FIX((uint32_t)e->event_x));
	rb_iv_set(i, "@y", INT2FIX((uint32_t)e->event_y));

	return i;
}

extern VALUE xrb_mouse_up_event_new(xcb_generic_event_t *g) {
	xcb_button_release_event_t *e = (xcb_button_release_event_t *)g;
	VALUE i = rb_class_new_instance(0, NULL, cXrbMouseUpEvent);

	xrb_mouse_event_data(i)->root_window = e->root;
	xrb_mouse_event_data(i)->event_window = e->event;
	xrb_mouse_event_data(i)->child_window = e->child;

	rb_iv_set(i, "@id", INT2FIX((uint32_t)e->response_type));
	rb_iv_set(i, "@x", INT2FIX((uint32_t)e->event_x));
	rb_iv_set(i, "@y", INT2FIX((uint32_t)e->event_y));

	return i;
}

extern VALUE xrb_mouse_in_event_new(xcb_generic_event_t *g) {
	xcb_enter_notify_event_t *e = (xcb_enter_notify_event_t *)g;
	VALUE i = rb_class_new_instance(0, NULL, cXrbMouseInEvent);

	xrb_mouse_event_data(i)->root_window = e->root;
	xrb_mouse_event_data(i)->event_window = e->event;
	xrb_mouse_event_data(i)->child_window = e->child;

	rb_iv_set(i, "@id", INT2FIX((uint32_t)e->response_type));
	rb_iv_set(i, "@x", INT2FIX((uint32_t)e->event_x));
	rb_iv_set(i, "@y", INT2FIX((uint32_t)e->event_y));

	return i;
}

extern VALUE xrb_mouse_out_event_new(xcb_generic_event_t *g) {
	xcb_leave_notify_event_t *e = (xcb_leave_notify_event_t *)g;
	VALUE i = rb_class_new_instance(0, NULL, cXrbMouseOutEvent);

	xrb_mouse_event_data(i)->root_window = e->root;
	xrb_mouse_event_data(i)->event_window = e->event;
	xrb_mouse_event_data(i)->child_window = e->child;

	rb_iv_set(i, "@id", INT2FIX((uint32_t)e->response_type));
	rb_iv_set(i, "@x", INT2FIX((uint32_t)e->event_x));
	rb_iv_set(i, "@y", INT2FIX((uint32_t)e->event_y));

	return i;
}

extern VALUE xrb_mouse_move_event_new(xcb_generic_event_t *g) {
	xcb_motion_notify_event_t *e = (xcb_motion_notify_event_t *)g;
	VALUE i = rb_class_new_instance(0, NULL, cXrbMouseMoveEvent);

	xrb_mouse_event_data(i)->root_window = e->root;
	xrb_mouse_event_data(i)->event_window = e->event;
	xrb_mouse_event_data(i)->child_window = e->child;

	rb_iv_set(i, "@id", INT2FIX((uint32_t)e->response_type));
	rb_iv_set(i, "@x", INT2FIX((uint32_t)e->event_x));
	rb_iv_set(i, "@y", INT2FIX((uint32_t)e->event_y));

	return i;
}

void init_mouse_events() {
	rb_define_mouse_event_data(&cXrbMouseDownEvent, "MouseDownEvent");
	rb_define_mouse_event_data(&cXrbMouseUpEvent,   "MouseUpEvent");
	rb_define_mouse_event_data(&cXrbMouseInEvent,   "MouseInEvent");
	rb_define_mouse_event_data(&cXrbMouseOutEvent,  "MouseOutEvent");
	rb_define_mouse_event_data(&cXrbMouseMoveEvent, "MouseMoveEvent");
}
