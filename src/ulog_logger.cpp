#include <sys/types.h>
#include <iostream>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <userlog.h>
#include <atmi/logger.hpp>

using namespace std ;

namespace atmi {

  ulog_logger::ulog_logger ( LoggingLevel level ){

    setup_level_names();

    set_log_level ( level );
  }

  void ulog_logger::log ( LoggingLevel at, const char *msg, va_list args){

    if ( get_log_level() <= at) {

        memset ( buffer, 0, sizeof (buffer) );

        vsnprintf ( buffer, sizeof(buffer), msg, args );
cout << "vsnprintf returned: " << msg << endl;

        string m = LEVELS[at]+ ": " + string (msg);
cout << "print message : " << m << endl;
        userlog ( (char *)m.c_str());
cout << "done" << endl;
    }
  }
}