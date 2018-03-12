#include "event.h"

#include "mouse_events.c"
#include "keyboard_events.c"
#include "clipboard_events.c"
#include "focus_events.c"
#include "drawable_events.c"
#include "window_events.c"
#include "other_events.c"

VALUE cXrbEvent;
VALUE cXrbNoEvent;

void xrb_event_free(xrb_event_d *d) {
	free(d);
}

VALUE xrb_event_alloc(VALUE i) {
	xrb_event_d *d = malloc(sizeof(*d));
	return Data_Wrap_Struct(i, NULL, xrb_event_free, d);
}

static xrb_event_d *xrb_event_data(VALUE i) {
	xrb_event_d *d;
	Data_Get_Struct(i, xrb_event_d, d);
	return d;
}

static uint32_t xrb_event_xid(VALUE i) {
	return xrb_event_data(i)->xid;
}

static VALUE xrb_event_new(xcb_generic_event_t *e) {
	VALUE i = rb_class_new_instance(0, NULL, cXrbEvent);
	xrb_event_data(i)->m = e->response_type;

	rb_iv_set(i, "@id", INT2FIX(e->response_type));

	return i;
}

static VALUE xrb_no_event_new(xcb_generic_event_t *e) {
	VALUE i = rb_class_new_instance(0, NULL, cXrbNoEvent);
	xrb_event_data(i)->m = e->response_type;

	rb_iv_set(i, "@id", INT2FIX(e->response_type));

	return i;
}

// xrb_events_register registers for a bunch of events. This function needs
// some heavy consideration and should not be used as is.
//
// It seems that any kind of event can only be registered by any one client.
// This means that with a window manager running, sxhkd or any other instance
// of xrb, this function fails and no events can be gotten.
//
// Ruby's true or false is returned on success or failure.
VALUE xrb_event_register(VALUE k) {
	xcb_void_cookie_t     c;
	xcb_generic_error_t * error;
	uint32_t value;

	value = XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT
	      | XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY
	      | XCB_EVENT_MASK_PROPERTY_CHANGE
	      | XCB_EVENT_MASK_ENTER_WINDOW
	      | XCB_EVENT_MASK_POINTER_MOTION
	      | XCB_EVENT_MASK_BUTTON_PRESS;

	c = xcb_change_window_attributes_checked(
		xrb_klass_cxn(k),
		xrb_klass_root(k),
		XCB_CW_EVENT_MASK,
		&value
	);
	error = xcb_request_check(xrb_klass_cxn(k), c);

	xcb_flush(xrb_klass_cxn(k));

	return rbool(error != NULL);
}

VALUE xrb_event_register_on_window(VALUE i) {
	xcb_void_cookie_t     c;
	xcb_generic_error_t * error;
	uint32_t value;

	value = XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT
	      | XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY
	      | XCB_EVENT_MASK_PROPERTY_CHANGE
	      | XCB_EVENT_MASK_ENTER_WINDOW
	      | XCB_EVENT_MASK_POINTER_MOTION
	      | XCB_EVENT_MASK_BUTTON_PRESS;

	c = xcb_change_window_attributes_checked(
		xrb_inst_cxn(i),
		xrb_window_xid(i),
		XCB_CW_EVENT_MASK,
		&value
	);
	error = xcb_request_check(xrb_inst_cxn(i), c);

	xcb_flush(xrb_inst_cxn(i));

	return rbool(error != NULL);
}

VALUE xrb_event_poll(VALUE s) {
	xcb_generic_event_t * e;

	xcb_flush(xrb_klass_cxn(s));

	e = xcb_poll_for_event(xrb_klass_cxn(s));
	if (e == NULL) {
		return Qnil;
	}

	switch (e->response_type & ~0x80) {
	case XCB_GE_GENERIC:
		return xrb_event_new(e);
	case XCB_NONE:
		return xrb_no_event_new(e);

	// Keyboard
	case XCB_KEY_PRESS:
		return xrb_key_down_event_new(e);
	case XCB_KEY_RELEASE:
		return xrb_key_up_event_new(e);
	case XCB_KEYMAP_NOTIFY:
		return xrb_keymap_change_event_new(e);

	// Mouse
	case XCB_BUTTON_PRESS:
		return xrb_mouse_down_event_new(e);
	case XCB_BUTTON_RELEASE:
		return xrb_mouse_up_event_new(e);
	case XCB_ENTER_NOTIFY:
		return xrb_mouse_in_event_new(e);
	case XCB_LEAVE_NOTIFY:
		return xrb_mouse_out_event_new(e);
	case XCB_MOTION_NOTIFY:
		return xrb_mouse_move_event_new(e);

	// Clipboard
	case XCB_SELECTION_CLEAR:
		return xrb_clipboard_clear_event_new(e);
	case XCB_SELECTION_REQUEST:
		return xrb_clipboard_request_event_new(e);
	case XCB_SELECTION_NOTIFY:
		return xrb_clipboard_event_new(e);

	// Focus
	case XCB_FOCUS_IN:
		return xrb_focus_in_event_new(e);
	case XCB_FOCUS_OUT:
		return xrb_focus_out_event_new(e);

	// Drawables
	case XCB_EXPOSE:
		return xrb_expose_event_new(e);
	case XCB_GRAPHICS_EXPOSURE:
		return xrb_expose_event_new(e);
	case XCB_NO_EXPOSURE:
		return xrb_unexpose_event_new(e);

	// Windows
	case XCB_CREATE_NOTIFY:
		return xrb_create_event_new(e);
	case XCB_DESTROY_NOTIFY:
		return xrb_destroy_event_new(e);
	case XCB_UNMAP_NOTIFY:
		return xrb_unmap_event_new(e);
	case XCB_MAP_NOTIFY:
		return xrb_map_event_new(e);
	case XCB_MAP_REQUEST:
		return xrb_map_request_event_new(e);
	case XCB_VISIBILITY_NOTIFY:
		return xrb_visible_event_new(e);

	// Etc
	case XCB_CONFIGURE_NOTIFY:
		return xrb_configure_event_new(e);
	case XCB_CONFIGURE_REQUEST:
		return xrb_configure_request_event_new(e);
	case XCB_GRAVITY_NOTIFY:
		return xrb_gravity_event_new(e);
	case XCB_RESIZE_REQUEST:
		return xrb_resize_request_event_new(e);
	case XCB_CIRCULATE_NOTIFY:
		return xrb_circulate_event_new(e);
	case XCB_CIRCULATE_REQUEST:
		return xrb_circulate_request_event_new(e);
	case XCB_PROPERTY_NOTIFY:
		return xrb_property_event_new(e);
	case XCB_REPARENT_NOTIFY:
		return xrb_reparent_event_new(e);
	case XCB_CLIENT_MESSAGE:
		return xrb_client_message_event_new(e);
	case XCB_MAPPING_NOTIFY:
		return xrb_mapping_event_new(e);
	case XCB_COLORMAP_NOTIFY:
		return xrb_colormap_event_new(e);
	}

	return Qnil;
}

void Init_event() {
	VALUE s;

	cXrbEvent = rb_define_class_under(mXrb, "Event", cXrbBase);
	rb_define_alloc_func(cXrbEvent, xrb_event_alloc);

	rb_define_attr(cXrbEvent, "id", 1, 0);

	s = rb_singleton_class(cXrbEvent);
	rb_undef_method(s, "new");

	rb_define_method(s, "register", xrb_event_register, 0);
	rb_define_method(s, "poll", xrb_event_poll, 0);

	cXrbNoEvent = rb_define_class_under(mXrb, "NoEvent", cXrbEvent);

	rb_define_method(cXrbWindow, "register", xrb_event_register_on_window, 0);

	init_mouse_events();
	init_keyboard_events();
	init_clipboard_events();
	init_focus_events();
	init_drawable_events();
	init_window_events();
	init_other_events();
}
