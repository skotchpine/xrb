#pragma once

#include "core.h"

#include <xcb/xcb_aux.h>

extern VALUE cXrbWindow;

typedef struct xrb_window_d {
	xcb_window_t xid;
} xrb_window_d;

extern VALUE xrb_window_new(xcb_window_t xid);
extern xcb_window_t xrb_window_xid(VALUE i);
