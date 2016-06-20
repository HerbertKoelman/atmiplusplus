/*
   $Id: Queue.C 79 2007-08-18 17:30:26Z hkoelman $

   Tuxedo queue manipulation helper class.
 */
#include <limits.h>
#include <stdarg.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <atmi.h>
#include <fml32.h>
#include <atmi/atmi++.hpp>

using namespace std;

namespace atmi {

  queue_stream::queue_stream ( atmi::queue *q ) : queue_stream ( q, 1024 ){
  }

  queue_stream::queue_stream ( atmi::queue *queue, long bs ): _count(0) {

    _buffer_size = bs;
    encode_base64( false );
    if ( queue == NULL ) {
      throw atmi_exception ( "queue_stream needs a queue to by initialized. Passed queue was NULL." );
    }

    this->_queue = queue;
  }

  void queue_stream::encode_base64( bool b ){

    _flags = b ? TPNOFLAGS : TPEX_STRING;
  };

/* operations ------------------------------------------------*/


/* properties ------------------------------------------------*/


/* operators -------------------------------------------------*/

  ostream& operator<<(ostream& out, queue_stream& qs){

    int   rc      = -1;
    char *message = NULL;
    long  len     = qs._buffer_size, size = qs._buffer_size, s = 0;
    char *buffer  = new char[size];

    try{
      qs._count = 0; // it's a friend, I can access private class data

      // the use of const_cast avoids the warnings on GCC/Linux
      message = qs.allocate ( const_cast<char *>("CARRAY"), NULL, len );

      while ( rc !=  QMENOMSG ) {

        try {

          qs._queue->begin ();
          rc = qs._queue->dequeue ( &message, &len );

          // Make sure we have data
          if ( rc != QMENOMSG && len > 0 ) {

            s = size;

            rc = tpexport ( message, len, buffer, &s, qs._flags);

            if ( rc < 0 ) {

              // Give it chance to be corrected
              if ( tperrno == TPELIMIT ) {
                s += s;

                delete buffer;
                buffer = new char[s];
                memset ( buffer, 0, s);
                rc = tpexport ( message, len, buffer, &s, qs._flags );
              }

              // If we still don't succeed to export buffer we throw an exception and exit the program.
              if ( rc < 0 ) {
                throw tuxedo_exception ( tperrno, "TPEXPORT failed. Check buffer size (last buffer size was %d bytes).", s );
              }
            }

            out.write ( buffer, s ) << endl;
            qs._count++;

          }
          qs._queue->commit ();

        } catch ( timeout_exception &timeout) {
          qs._queue->abort();
        }
      }

      qs.free ( message );
      delete  buffer;

    } catch (... ) {
      qs._queue->abort();
      qs.free ( message );
      delete buffer;
      throw;
    }

    return out;
  }

  istream& operator>>(istream& in, queue_stream& qs){

    int rc = -1;
    long len = qs.buffer_size ();
    string line;
    char *message = NULL;

    try {
      qs._count = 0;

      message = (char *)qs.allocate (const_cast<char *>("CARRAY"), NULL, len );

      while (getline ( in, line)) {

        rc = tpimport ( (char *)line.c_str(), line.size(), (char **) &message, &len, qs._flags );

        if ( rc < 0 ) {
          throw tuxedo_exception ( tperrno, "TPIMPORT failed. Was this message exported using tpexport ?" );
        }

        try{ // to en dequeue message.

          qs._queue->begin();
          rc = qs._queue->enqueue ( (char *)message, len );
          qs._queue->commit();

          qs._count++;

        } catch ( diagnostic_exception &diagErr ) {
          qs._queue->abort ();
          throw;
        } catch (tuxedo_exception &tuxErr ) {
          qs._queue->abort ();
          throw;
        }

      }

      qs.free ( message );

    } catch (tuxedo_exception &terr ) {
      qs.free ( message );
      throw;
    } 

    return in;
  }

} // namespace-end