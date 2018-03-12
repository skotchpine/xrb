#include "event.h"

VALUE cXrbConfigureEvent;
VALUE cXrbConfigureRequestEvent;
VALUE cXrbGravityEvent;
VALUE cXrbResizeRequestEvent;
VALUE cXrbCirculateEvent;
VALUE cXrbCirculateRequestEvent;
VALUE cXrbPropertyEvent;
VALUE cXrbReparentEvent;
VALUE cXrbClientMessageEvent;
VALUE cXrbMappingEvent;
VALUE cXrbColormapEvent;

extern VALUE xrb_configure_event_new(xcb_generic_event_t *g) {
	return rb_class_new_instance(0, NULL, cXrbConfigureEvent);
}

extern VALUE xrb_configure_request_event_new(xcb_generic_event_t *g) {
	return rb_class_new_instance(0, NULL, cXrbConfigureRequestEvent);
}

extern VALUE xrb_gravity_event_new(xcb_generic_event_t *g) {
	return rb_class_new_instance(0, NULL, cXrbGravityEvent);
}

extern VALUE xrb_resize_request_event_new(xcb_generic_event_t *g) {
	return rb_class_new_instance(0, NULL, cXrbResizeRequestEvent);
}

extern VALUE xrb_circulate_event_new(xcb_generic_event_t *g) {
	return rb_class_new_instance(0, NULL, cXrbCirculateEvent);
}

extern VALUE xrb_circulate_request_event_new(xcb_generic_event_t *g) {
	return rb_class_new_instance(0, NULL, cXrbCirculateRequestEvent);
}

extern VALUE xrb_property_event_new(xcb_generic_event_t *g) {
	return rb_class_new_instance(0, NULL, cXrbPropertyEvent);
}

extern VALUE xrb_reparent_event_new(xcb_generic_event_t *g) {
	return rb_class_new_instance(0, NULL, cXrbReparentEvent);
}

extern VALUE xrb_client_message_event_new(xcb_generic_event_t *g) {
	return rb_class_new_instance(0, NULL, cXrbClientMessageEvent);
}

extern VALUE xrb_mapping_event_new(xcb_generic_event_t *g) {
	return rb_class_new_instance(0, NULL, cXrbMappingEvent);
}

extern VALUE xrb_colormap_event_new(xcb_generic_event_t *g) {
	return rb_class_new_instance(0, NULL, cXrbColormapEvent);
}

void init_other_events() {
	cXrbConfigureEvent        = rb_define_class_under(mXrb, "ConfigureEvent",        cXrbEvent);
	cXrbConfigureRequestEvent = rb_define_class_under(mXrb, "ConfigureRequestEvent", cXrbEvent);
	cXrbGravityEvent          = rb_define_class_under(mXrb, "GravityEvent",          cXrbEvent);
	cXrbResizeRequestEvent    = rb_define_class_under(mXrb, "ResizeRequestEvent",    cXrbEvent);
	cXrbCirculateEvent        = rb_define_class_under(mXrb, "CirculateEvent",        cXrbEvent);
	cXrbCirculateRequestEvent = rb_define_class_under(mXrb, "CirculateRequestEvent", cXrbEvent);
	cXrbPropertyEvent         = rb_define_class_under(mXrb, "PropertyEvent",         cXrbEvent);
	cXrbReparentEvent         = rb_define_class_under(mXrb, "ReparentEvent",         cXrbEvent);
	cXrbClientMessageEvent    = rb_define_class_under(mXrb, "ClientMessageEvent",    cXrbEvent);

	cXrbMappingEvent = rb_define_class_under(mXrb, "MappingEvent", cXrbEvent);
	//rb_define_alloc_func(cXrbMappingEvent, xrb_mapping_event_alloc);

	cXrbColormapEvent = rb_define_class_under(mXrb, "ColormapEvent", cXrbEvent);
}
