#include "monitor.h"

#include "utils.h"

VALUE cXrbMonitor;

static VALUE xrb_monitor_new(VALUE k, xcb_randr_monitor_info_t *info) {
	VALUE i;

	i = rb_class_new_instance(0, NULL, cXrbMonitor);

	rb_iv_set(i, "@name", atom_to_rstr(xrb_klass_cxn(k), info->name));

	rb_iv_set(i, "@x",         INT2FIX((int32_t)info->x));
	rb_iv_set(i, "@y",         INT2FIX((int32_t)info->y));

	rb_iv_set(i, "@width",     INT2FIX((int32_t)info->width));
	rb_iv_set(i, "@height",    INT2FIX((int32_t)info->height));

	rb_iv_set(i, "@mm_width",  INT2FIX(info->width_in_millimeters));
	rb_iv_set(i, "@mm_height", INT2FIX(info->height_in_millimeters));

	rb_iv_set(i, "@primary?",  rbool(info->primary));

	return i;
}

static VALUE xrb_monitor_all(VALUE k) {
	xcb_randr_get_monitors_cookie_t c;
	xcb_randr_get_monitors_reply_t *r;
	xcb_randr_monitor_info_iterator_t iterator;
	VALUE ary;

	ary = rb_ary_new();

	c = xcb_randr_get_monitors(xrb_klass_cxn(k), xrb_klass_root(k), 1);
	r = xcb_randr_get_monitors_reply(xrb_klass_cxn(k), c, NULL);
	if (r == NULL) {
		return ary;
	}

	iterator = xcb_randr_get_monitors_monitors_iterator(r);
	for (; iterator.rem > 0; xcb_randr_monitor_info_next(&iterator)) {
		rb_ary_push(ary, xrb_monitor_new(k, iterator.data));
	}

	free(r);
	return ary;
}

extern void Init_monitor() {
	VALUE s;

	cXrbMonitor = rb_define_class_under(mXrb, "Monitor", cXrbBase);

	rb_define_attrs(cXrbMonitor, 1, 0, 8,
	                "name",
	                "x", "y",
	                "width", "height",
	                "mm_width", "mm_height",
	                "primary?");

	s = rb_singleton_class(cXrbMonitor);
	rb_undef_method(s, "new");
	rb_define_method(s, "all", xrb_monitor_all, 0);
}
