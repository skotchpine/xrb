#include "window.h"

VALUE cXrbWindow;

// Frees the internal data.
void xrb_window_free(xrb_window_d *d) {
	free(d);
}

// Allocates the internal data.
VALUE xrb_window_alloc(VALUE i) {
	xrb_window_d *d = malloc(sizeof(*d));
	return Data_Wrap_Struct(i, NULL, xrb_window_free, d);
}

// Gets the internal data.
static xrb_window_d *xrb_window_data(VALUE i) {
	xrb_window_d *d;
	Data_Get_Struct(i, xrb_window_d, d);
	return d;
}

// Gets the XCB/X11 ID.
extern xcb_window_t xrb_window_xid(VALUE i) {
	return xrb_window_data(i)->xid;
}

// Sets @tite. An empty String is set on failure or if no title is found.
// Returns boolean.
static VALUE xrb_window_get_title(VALUE i) {
	VALUE rstr;

	rstr = prop_to_rstr(xrb_inst_cxn(i), xrb_window_xid(i), XCB_ATOM_WM_NAME);

	return rb_iv_set(i, "@title", rstr);
}

// Sets @wm_class. An empty string is set on failure or if no wm_class is found.
// Returns boolean.
static VALUE xrb_window_get_wm_class(VALUE i) {
	VALUE rstr;

	rstr = prop_to_rstr(xrb_inst_cxn(i), xrb_window_xid(i), XCB_ATOM_WM_CLASS);

	return rb_iv_set(i, "@wm_class", rstr);
}

// Sets @x, @y, @width, @height and @border_width and returns Ruby's true on
// success. Returns false on failure.
static VALUE xrb_window_get_geometry(VALUE i) {
	xcb_get_geometry_cookie_t c;
	xcb_get_geometry_reply_t *r;

	c = xcb_get_geometry(xrb_inst_cxn(i), xrb_window_xid(i));
	r = xcb_get_geometry_reply(xrb_inst_cxn(i), c, NULL);
	if (r == NULL) {
		return Qfalse;
	}

	rb_iv_set(i, "@x",            INT2FIX(r->x));
	rb_iv_set(i, "@y",            INT2FIX(r->y));
	rb_iv_set(i, "@width",        INT2FIX(r->width));
	rb_iv_set(i, "@height",       INT2FIX(r->height));
	rb_iv_set(i, "@border_width", INT2FIX(r->border_width));

	return Qtrue;
}

// Sets an @ignored? and @mapped? and returns Ruby's true on success.
// Returns false on failure.
static VALUE xrb_window_get_attributes(VALUE i) {
	xcb_get_window_attributes_cookie_t c;
	xcb_get_window_attributes_reply_t *r;

	c = xcb_get_window_attributes(xrb_inst_cxn(i), xrb_window_xid(i));
	r = xcb_get_window_attributes_reply(xrb_inst_cxn(i), c, NULL);
	if (r == NULL) {
		return Qfalse;
	}

	rb_iv_set(i, "@mapped?", rbool(r->map_state == XCB_MAP_STATE_VIEWABLE));
	rb_iv_set(i, "@ignored?", rbool(r->override_redirect));

	return Qtrue;
}

// Creates a new Xrb::Window
extern VALUE xrb_window_new(xcb_window_t xid) {
	VALUE i;

	i = rb_class_new_instance(0, NULL, cXrbWindow);

	xrb_window_data(i)->xid = xid;
	rb_iv_set(i, "@id", xid_to_rstr(xid));

	if (xrb_window_get_attributes(i) == Qfalse) {
		goto dead_window;
	}

	if (xrb_window_get_geometry(i) == Qfalse) {
		goto dead_window;
	}

	if (xrb_window_get_title(i) == Qfalse) {
		goto dead_window;
	}

	if (xrb_window_get_wm_class(i) == Qfalse) {
		goto dead_window;
	}

	rb_iv_set(i, "@dead?", Qfalse);

	return i;

dead_window:
	rb_iv_set(i, "@dead?", Qtrue);
	return i;
}

// Focuses a window, returns the window
static VALUE xrb_window_focus(VALUE i) {
	xcb_set_input_focus(xrb_inst_cxn(i), XCB_INPUT_FOCUS_POINTER_ROOT,
	                    xrb_window_xid(i), XCB_CURRENT_TIME);
	xcb_aux_sync(xrb_inst_cxn(i));

	return xrb_window_new(xrb_window_xid(i));
}

// Kills a window, returns the window
static VALUE xrb_window_kill(VALUE i) {
	xcb_destroy_window(xrb_inst_cxn(i), xrb_window_xid(i));
	xcb_aux_sync(xrb_inst_cxn(i));

	return xrb_window_new(xrb_window_xid(i));
}

// Kills a window and its client, returns the window
static VALUE xrb_window_kill2(VALUE i) {
	xcb_kill_client(xrb_inst_cxn(i), xrb_window_xid(i));
	xcb_aux_sync(xrb_inst_cxn(i));
	return i;
}

// Maps a window, returns the window
static VALUE xrb_window_map(VALUE i) {
	xcb_map_window(xrb_inst_cxn(i), xrb_window_xid(i));
	xcb_aux_sync(xrb_inst_cxn(i));
	return xrb_window_new(xrb_window_xid(i));
}

// Unmaps a window, returns the window
static VALUE xrb_window_unmap(VALUE i) {
	xcb_unmap_window(xrb_inst_cxn(i), xrb_window_xid(i));
	xcb_aux_sync(xrb_inst_cxn(i));
	return xrb_window_new(xrb_window_xid(i));
}

// Ignores a window, returns the window
static VALUE xrb_window_ignore(VALUE i) {
	uint32_t m, v[1];

	m = XCB_CW_OVERRIDE_REDIRECT;
	v[0] = 1;

	xcb_change_window_attributes(xrb_inst_cxn(i), xrb_window_xid(i), m, v);
	xcb_aux_sync(xrb_inst_cxn(i));

	return xrb_window_new(xrb_window_xid(i));
}

// Unignores a window, returns the window
static VALUE xrb_window_unignore(VALUE i) {
	uint32_t m, v[1];

	m = XCB_CW_OVERRIDE_REDIRECT;
	v[0] = 0;

	xcb_change_window_attributes(xrb_inst_cxn(i), xrb_window_xid(i), m, v);
	xcb_aux_sync(xrb_inst_cxn(i));

	return xrb_window_new(xrb_window_xid(i));
}

// Moves a window, returns the window
static VALUE xrb_window_move(VALUE i, VALUE x, VALUE y) {
	uint32_t m, v[2];

	Check_Type(x, T_FIXNUM);
	Check_Type(y, T_FIXNUM);

	m = XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y;

	v[0] = FIX2INT(x);
	v[1] = FIX2INT(y);

	xcb_configure_window(xrb_inst_cxn(i), xrb_window_xid(i), m, v);
	xcb_aux_sync(xrb_inst_cxn(i));

	return xrb_window_new(xrb_window_xid(i));
}

// Resizes a window, returns the window
static VALUE xrb_window_resize(VALUE i, VALUE width, VALUE height) {
	uint32_t m, v[2];

	Check_Type(width,  T_FIXNUM);
	Check_Type(height, T_FIXNUM);

	m = XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT;

	v[0] = FIX2INT(width);
	v[1] = FIX2INT(height);

	xcb_configure_window(xrb_inst_cxn(i), xrb_window_xid(i), m, v);
	xcb_aux_sync(xrb_inst_cxn(i));

	return xrb_window_new(xrb_window_xid(i));
}

// Teleports a window, returns the window
static VALUE xrb_window_teleport(VALUE i, VALUE x, VALUE y,
                                 VALUE width, VALUE height) {
	uint32_t m, v[4];

	Check_Type(x,      T_FIXNUM);
	Check_Type(y,      T_FIXNUM);
	Check_Type(width,  T_FIXNUM);
	Check_Type(height, T_FIXNUM);

	m = XCB_CONFIG_WINDOW_X
	  | XCB_CONFIG_WINDOW_Y
	  | XCB_CONFIG_WINDOW_WIDTH
	  | XCB_CONFIG_WINDOW_HEIGHT;

	v[0] = FIX2INT(x);
	v[1] = FIX2INT(y);
	v[2] = FIX2INT(width);
	v[3] = FIX2INT(height);

	xcb_configure_window(xrb_inst_cxn(i), xrb_window_xid(i), m, v);
	xcb_aux_sync(xrb_inst_cxn(i));

	return xrb_window_new(xrb_window_xid(i));
}

// Sets a window's border width
static VALUE xrb_window_set_border_width(VALUE i, VALUE width) {
	uint32_t m, v[1];

	Check_Type(width, T_FIXNUM);

	m = XCB_CONFIG_WINDOW_BORDER_WIDTH;
	v[0] = FIX2INT(width);

	xcb_configure_window(xrb_inst_cxn(i), xrb_window_xid(i), m, v);
	xcb_aux_sync(xrb_inst_cxn(i));

	return xrb_window_new(xrb_window_xid(i));
}

// Sets a window's border color
static VALUE xrb_window_set_border_color(VALUE i, VALUE color) {
	uint32_t m, v[1];

	Check_Type(color, T_FIXNUM);

	m = XCB_CW_BORDER_PIXEL;
	v[0] = FIX2INT(color);

	xcb_change_window_attributes(xrb_inst_cxn(i), xrb_window_xid(i), m, v);
	xcb_aux_sync(xrb_inst_cxn(i));

	return xrb_window_new(xrb_window_xid(i));
}

// Puts a window on top
static VALUE xrb_window_stack_above(VALUE i) {
	uint32_t m, v[1];

	m = XCB_CONFIG_WINDOW_STACK_MODE;
	v[0] = XCB_STACK_MODE_BELOW;

	xcb_configure_window(xrb_inst_cxn(i), xrb_window_xid(i), m, v);
	xcb_aux_sync(xrb_inst_cxn(i));

	return xrb_window_new(xrb_window_xid(i));
}

// Puts a window on bottom
static VALUE xrb_window_stack_below(VALUE i) {
	uint32_t m = XCB_CONFIG_WINDOW_STACK_MODE;
	uint32_t v[1] = { XCB_STACK_MODE_BELOW };

	xcb_configure_window(xrb_inst_cxn(i), xrb_window_xid(i), m, v);
	xcb_aux_sync(xrb_inst_cxn(i));

	return xrb_window_new(xrb_window_xid(i));
}

// Creates an Array of Xrb::Windows representing all X windows.
// On failure (or if no windows are found), returns an empty Array.
static VALUE xrb_window_all(VALUE k) {
	xcb_query_tree_cookie_t c;
	xcb_query_tree_reply_t *r;

	xcb_window_t *xids;
	size_t size;

	VALUE ary;

	ary = rb_ary_new();

	c = xcb_query_tree(xrb_klass_cxn(k), xrb_klass_root(k));
	r = xcb_query_tree_reply(xrb_klass_cxn(k), c, NULL);
	if (r == NULL) {
		return ary;
	}

	size = sizeof(xcb_window_t) * r->children_len;
	xids = malloc(size);
	memcpy(xids, xcb_query_tree_children(r), size);

	for (int i = 0; i < r->children_len; i ++) {
		rb_ary_push(ary, xrb_window_new(xids[i]));
	}

	free(r);
	return ary;
}

// Gets the root window.
static VALUE xrb_window_root(VALUE k) {
	return xrb_window_new(xrb_klass_root(k));
}

// Gets the focused window or nil of no windows are focused.
static VALUE xrb_window_focused(VALUE k) {
	xcb_get_input_focus_cookie_t c;
	xcb_get_input_focus_reply_t *r;
	VALUE i;

	c = xcb_get_input_focus(xrb_klass_cxn(k));
	r = xcb_get_input_focus_reply(xrb_klass_cxn(k), c, NULL);
	if (r == NULL) {
		return Qnil;
	}

	i = xrb_window_new(r->focus);

	free(r);
	return i;
}

//	class Xrb::Window < Xrb::Base
//	  attr_reader :id,
//	              :title, :class,
//	              :x, :y,
//	              :width, :height,
//	              :border_width,
//	              :mapped?, :ignored?
//
//	  class << self
//	    undef :new
//
//	    def self.all; end
//	    def self.root; end
//	    def self.focused; end
//	  end
//
//	  def focus; end
//	  def update; end
//	  def kill; end
//	  def kill2; end
//	  def map; end
//	  def unmap; end
//	  def ignore; end
//	  def unignore; end
//	  def move; end
//	  def resize; end
//	  def teleport; end
//	  def set_border_width; end
//	  def set_border_color; end
//	  def stack_above; end
//	  def stack_below; end
//
//	  private
//	  def get_name; end
//	  def get_attributes; end
//	  def get_geometry; end
//	end
void Init_window() {
	VALUE s;

	cXrbWindow = rb_define_class_under(mXrb, "Window", cXrbBase);
	rb_define_alloc_func(cXrbWindow, xrb_window_alloc);

	rb_define_attrs(cXrbWindow, 1, 1, 11,
	                "id",
	                "title", "wm_class",
	                "x", "y",
	                "width", "height",
	                "border_width",
	                "dead?",
	                "mapped?",
	                "ignored?");

	s = rb_singleton_class(cXrbWindow);
	rb_undef_method(s, "new");

	rb_define_method(s, "all",     xrb_window_all,     0);
	rb_define_method(s, "root",    xrb_window_root,    0);
	rb_define_method(s, "focused", xrb_window_focused, 0);

	rb_define_method(cXrbWindow, "focus",    xrb_window_focus,    0);
	rb_define_method(cXrbWindow, "kill",     xrb_window_kill,     0);
	rb_define_method(cXrbWindow, "kill2",    xrb_window_kill2,    0);
	rb_define_method(cXrbWindow, "map",      xrb_window_map,      0);
	rb_define_method(cXrbWindow, "unmap",    xrb_window_unmap,    0);
	rb_define_method(cXrbWindow, "ignore",   xrb_window_ignore,   0);
	rb_define_method(cXrbWindow, "unignore", xrb_window_unignore, 0);

	rb_define_method(cXrbWindow, "move", xrb_window_move, 2);
	rb_define_method(cXrbWindow, "resize", xrb_window_resize, 2);
	rb_define_method(cXrbWindow, "teleport", xrb_window_teleport, 4);

	rb_define_method(cXrbWindow, "set_border_width", xrb_window_set_border_width, 1);
	rb_define_method(cXrbWindow, "set_border_color", xrb_window_set_border_color, 1);

	rb_define_method(cXrbWindow, "stack_above", xrb_window_stack_above, 0);
	rb_define_method(cXrbWindow, "stack_below", xrb_window_stack_below, 0);

	rb_define_private_method(cXrbWindow, "get_attributes", xrb_window_get_attributes, 0);
	rb_define_private_method(cXrbWindow, "get_geometry",   xrb_window_get_geometry,   0);
	rb_define_private_method(cXrbWindow, "get_title",      xrb_window_get_title,      0);
	rb_define_private_method(cXrbWindow, "get_wm_class",   xrb_window_get_wm_class,   0);
}
