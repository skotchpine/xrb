#include "event.h"

VALUE cXrbDrawableExposeEvent;
VALUE cXrbDrawableUnexposeEvent;

void xrb_drawable_event_free(xrb_drawable_event_d *d) {
	free(d);
}

VALUE xrb_drawable_event_alloc(VALUE i) {
	xrb_drawable_event_d *d = malloc(sizeof(*d));
	return Data_Wrap_Struct(i, NULL, xrb_drawable_event_free, d);
}

static xrb_drawable_event_d *xrb_drawable_event_data(VALUE i) {
	xrb_drawable_event_d *d;
	Data_Get_Struct(i, xrb_drawable_event_d, d);
	return d;
}

void rb_define_drawable_event_data(VALUE *k, const char *name) {
	*k = rb_define_class_under(mXrb, name, cXrbEvent);
	rb_define_alloc_func(*k, xrb_drawable_event_alloc);

	rb_undef_method(rb_singleton_class(*k), "new");
}

extern VALUE xrb_expose_event_new(xcb_generic_event_t *g) {
	xcb_graphics_exposure_event_t *e = (xcb_graphics_exposure_event_t *)g;
	VALUE i = rb_class_new_instance(0, NULL, cXrbDrawableExposeEvent);

	xrb_drawable_event_data(i)->drawable = e->drawable;

	rb_iv_set(i, "@id", INT2FIX((uint32_t)e->response_type));

	return i;
}

extern VALUE xrb_unexpose_event_new(xcb_generic_event_t *g) {
	xcb_no_exposure_event_t *e = (xcb_no_exposure_event_t *)g;
	VALUE i = rb_class_new_instance(0, NULL, cXrbDrawableUnexposeEvent);

	xrb_drawable_event_data(i)->drawable = e->drawable;

	rb_iv_set(i, "@id", INT2FIX((uint32_t)e->response_type));

	return i;
}

void init_drawable_events() {
	rb_define_drawable_event_data(&cXrbDrawableExposeEvent, "DrawableExposeEvent");
	rb_define_drawable_event_data(&cXrbDrawableUnexposeEvent, "DrawableExposeEvent");
}

