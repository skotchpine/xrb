#include "core.h"

#include "connection.c"
#include "base.c"

VALUE mXrb;

VALUE eXrbRuntimeError;
VALUE eXrbConnectionError;

//	module Xrb
//	  class RuntimeError < StdError; end
//	  class ConnectionError < StdError; end
//
//	  init_xrb_connection
//	  init_xrb_base
//	end
extern void Init_core() {
	mXrb = rb_define_module("Xrb");

	eXrbConnectionError = rb_define_class_under(mXrb, "ConnectionError", rb_eStandardError);
	eXrbRuntimeError    = rb_define_class_under(mXrb, "RuntimeError",     rb_eRuntimeError);

	init_xrb_connection();
	init_xrb_base();
}
