#pragma once

#include "keyboard.h"
#include "window.h"

#include <xcb/xproto.h>
#include <xcb/xcb_event.h>

extern VALUE cXrbEvent;

extern VALUE cXrbNoEvent;

typedef struct xrb_event_d {
	uint32_t m;
	uint32_t xid;
} xrb_event_d;

// ---- Clipboard ----
extern VALUE cXrbClipboardEvent;
extern VALUE cXrbClipboardClearEvent;
extern VALUE cXrbClipboardRequestEvent;

typedef struct xrb_clipboard_event_d {
	xcb_window_t owner;
	xcb_window_t requestor;
	xcb_atom_t selection;
	xcb_atom_t target;
	xcb_atom_t property;
} xrb_clipboard_event_d;

// ---- Keyboard ----
extern VALUE cXrbKeyDownEvent;
extern VALUE cXrbKeyUpEvent;
extern VALUE cXrbKeymapChangeEvent;

typedef struct xrb_key_event_d {
	xcb_window_t root;
	xcb_window_t event;
	xcb_window_t child;
} xrb_key_event_d;

// ---- Mouse ----
extern VALUE cXrbMouseDownEvent;
extern VALUE cXrbMouseUpEvent;
extern VALUE cXrbMouseInEvent;
extern VALUE cXrbMouseOutEvent;
extern VALUE cXrbMouseMoveEvent;

typedef struct xrb_mouse_event_d {
	xcb_window_t root_window;
	xcb_window_t event_window;
	xcb_window_t child_window;
} xrb_mouse_event_d;

// ---- Drawable ----
VALUE cXrbDrawableExposeEvent;
VALUE cXrbDrawableUnexposeEvent;

typedef struct xrb_drawable_event_d {
	xcb_drawable_t drawable;
} xrb_drawable_event_d;

// ---- Window ----
extern VALUE cXrbCreateEvent;
extern VALUE cXrbDestroyEvent;
extern VALUE cXrbUnmapEvent;
extern VALUE cXrbMapEvent;
extern VALUE cXrbMapRequestEvent;
extern VALUE cXrbVisibleEvent;

typedef struct xrb_window_event_d {
	xcb_window_t parent;
	xcb_window_t window;
	xcb_window_t event;
} xrb_window_event_d;

// ---- Meta ----
extern VALUE cXrbConfigureEvent;
extern VALUE cXrbConfigureRequestEvent;
extern VALUE cXrbGravityEvent;
extern VALUE cXrbResizeRequestEvent;
extern VALUE cXrbCirculateEvent;
extern VALUE cXrbPropertyEvent;
extern VALUE cXrbReparentEvent;
extern VALUE cXrbClientMessageEvent;
extern VALUE cXrbMappingEvent;
