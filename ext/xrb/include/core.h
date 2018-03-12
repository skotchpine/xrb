#pragma once

#include "xrb.h"
#include "utils.h"

extern VALUE mXrb;

extern VALUE eXrbRuntimeError;
extern VALUE eXrbConnectionError;

//
// ---- Connection ----
//

extern VALUE cXrbConnection;

typedef struct xrb_connection_d {
	xcb_connection_t *cxn;
	xcb_screen_t     *scr;
	xcb_window_t     root;
} xrb_connection_d;

xrb_connection_d *xrb_connection_data(VALUE i);

xcb_connection_t *xrb_connection_cxn( VALUE i);
xcb_screen_t     *xrb_connection_scr( VALUE i);
xcb_window_t      xrb_connection_root(VALUE i);

//
// ---- Base ----
//

extern VALUE cXrbBase;
extern VALUE cXrbBaseSingleton;

// -- Class connection accessors --

VALUE xrb_klass_connection_data( VALUE k);

xcb_connection_t *xrb_klass_cxn( VALUE k);
xcb_screen_t     *xrb_klass_scr( VALUE k);
xcb_window_t      xrb_klass_root(VALUE k);

// -- Instance connection accessors --

VALUE xrb_inst_connection_data( VALUE i);

xcb_connection_t *xrb_inst_cxn( VALUE i);
xcb_screen_t     *xrb_inst_scr( VALUE i);
xcb_window_t      xrb_inst_root(VALUE i);
