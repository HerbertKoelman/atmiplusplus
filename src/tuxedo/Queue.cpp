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

#include <Concurrency.h>
#include <atmi++.h>

using namespace std;

namespace atmi {

  class Sequence : private Mutex {
public:
    Sequence ( long min = 0, long max = 0 ){
      cval = min;
      this->min = min;
      this->max = max;
    };

    long next () {

      long nval = 0;

      lock();
      nval = ++cval;                    // Increment and get next value
      unlock();

      return nval;
    };

    long value () {

      return cval;
    };

private:
    long cval, min, max;

  };

  Queue::Queue ( char *qspace, char *queue, char *reply ) {

    /* Too restrictive
       if ( qspace == NULL || queue == NULL ) {
            throw Exception ( "Invalid values for qspace or queue parameter" );
       }
     */

    this->qspace = qspace;
    this->queue = queue;

    this->flags = TPNOFLAGS;
    qctl.flags = TPNOFLAGS;

    set_message_wait ( true );
    setQoS( TPQQOSDEFAULTPERSIST );

    set_reply_queue ( reply );
  }

/* operations ------------------------------------------------*/

  int Queue::enqueue ( Buffer *data ) {

    return enqueue ( (char *)data->get_buffer(), 0 );
  }

  int Queue::dequeue ( Buffer *data ) {

    int rc = -1;
    long len = 0;
    FBFR32 *b = data->get_buffer();

    if ( (rc = dequeue ( (char **) &b, &len )) != -1 ) {

      data->set_buffer ( b );
    }

    return rc;
  }

  int Queue::enqueue ( char *queue,  char *data, long len ){

    int rc = -1;

    if ( qspace == NULL || queue == NULL ) {

      throw Exception ( catgets ( catd, CATD_ATMI_SET, 19, "Enqueue failed, qspace and queue properties have not been set !!??") );
    }

    // check if we need to switch context
    switch_context ();

    rc = tpenqueue ( qspace, queue, (TPQCTL *) &qctl, data, len, flags );

    updateErrno ();

    if ( rc < 0 ) {
      if ( tperrno == TPEDIAGNOSTIC ) {
        handleDiagnostics ( tperrno, qctl.diagnostic, "Enqueue in %s:%s failed.", qspace, queue );
      } else {
        handleTperrno ( tperrno, "Enqueue on %s:%s failed.", qspace, queue );
      }
    }

    return rc;
  }

  int Queue::enqueue ( char *data, long len ){

    int rc = -1;

    rc = enqueue ( queue, data, len );

    return rc;
  }

  int Queue::dequeueReply ( char **data, long *len ) {

    int rc = -1;

    rc = dequeue ( qctl.replyqueue, data, len );

    return rc;
  }

  int Queue::enqueueReply ( char *data, long len ) {

    int rc = -1;

    rc = enqueue ( qctl.replyqueue, data, len );

    return rc;
  }

  int Queue::dequeue ( char *queue, char **data, long *len ) {

    int rc = -1;

    if ( qspace == NULL || queue == NULL ) {

      throw Exception ( catgets ( catd, CATD_ATMI_SET, 20, "Dequeue failed, qspace and queue properties have not been set !!??") );
    }

    // check if we need to switch context
    switch_context ();

    rc = tpdequeue ( qspace, queue, (TPQCTL *) &qctl, data, len, flags );

    updateErrno ();

    if ( rc < 0 ) {
      if ( tperrno == TPEDIAGNOSTIC ) {
        if ( qctl.diagnostic == QMENOMSG ) {
          rc = QMENOMSG;
        } else {
          handleDiagnostics ( tperrno, qctl.diagnostic, "Dequeue from %s:%s failed.", qspace, queue );
        }
      } else {
        handleTperrno ( tperrno, "Dequeue from %s:%s failed.", qspace, queue );
      }
    }

    return rc;
  }

  int Queue::dequeue ( char **data, long *len ) {

    int rc = -1;

    rc = dequeue ( queue, data, len );

    return rc;
  }

  int Queue::handleDiagnostics ( int _tperrno, int _diagno, const char *msg, ... ) {

    this->diagno = diagno;

    va_list ap;
    va_start ( ap, msg );

    switch ( _diagno ) {
      case QMEINVAL:
      case QMEBADRMID:
      case QMENOTOPEN:
      case QMETRAN:
      case QMEBADMSGID:
      case QMESYSTEM:
      case QMEOS:
      case QMEPROTO:
      case QMEBADQUEUE:
      case QMENOSPACE:
      case QMERELEASE:
      case QMESHARE:
      {
        DiagnosticException err ( _tperrno, _diagno );
        err.setup_message ( msg, ap );
        throw err;
      }
      break;
      case QMEABORTED:
      {
        AbortedException err ( _tperrno, _diagno );
        err.setup_message (msg, ap );
        throw err;
      }
      break;
      /* these are not errors */
      case QMENOMSG:
        // not an error - throw NomsgException ( _tperrno, _diagno, msg, ap );
        break;
      default:
        throw DiagnosticException ( _tperrno, _diagno, catgets ( catd, CATD_ATMI_SET, 33, "Never heard about this diagno %d (tperrno: %d) !!??"), _diagno, _tperrno);
    }

    va_end ( ap );

    return _diagno;
  }

/* properties ------------------------------------------------*/


  void Queue::set_reply_queue( char *reply ) {
    if ( reply != NULL ) {
      qctl.flags = set ( qctl.flags, TPQREPLYQ );
      strncpy(qctl.replyqueue, reply, TMQNAMELEN+1);
    } else {
      qctl.flags = unset ( qctl.flags,  TPQREPLYQ );
    }
  }

  const char *Queue::get_reply_queue() {

    return qctl.replyqueue;
  }

  void Queue::set_message_wait ( bool qwait ) {

    if ( qwait ) {
      qctl.flags = set ( qctl.flags, TPQWAIT);                    // Set QWait bits
    } else {
      qctl.flags = unset ( qctl.flags, TPQWAIT );                    // Unset QWait bits
    }
  }

  void Queue::setQoS ( long qos ) {

    qctl.flags = unset( qctl.flags, TPQDELIVERYQOS | TPQREPLYQOS );


    switch ( qos ) {

      case TPQQOSDEFAULTPERSIST:
        qctl.flags = set ( qctl.flags, TPQDELIVERYQOS | TPQREPLYQOS);
        qctl.delivery_qos = TPQQOSDEFAULTPERSIST;
        qctl.reply_qos  = TPQQOSDEFAULTPERSIST;
        break;

      case TPQQOSPERSISTENT:
        qctl.flags = set ( qctl.flags, TPQDELIVERYQOS | TPQREPLYQOS);
        qctl.delivery_qos = TPQQOSPERSISTENT;
        qctl.reply_qos  = TPQQOSPERSISTENT;
        break;

      case TPQQOSNONPERSISTENT:
        qctl.flags = set ( qctl.flags, TPQDELIVERYQOS | TPQREPLYQOS );
        qctl.delivery_qos = TPQQOSNONPERSISTENT;
        qctl.reply_qos  = TPQQOSNONPERSISTENT;
        break;

      default:
        throw Exception (catgets ( catd, CATD_ATMI_SET, 23, "Unsupported QoS (%d) for /Q operations."), qos);
    }
  }


/* operators -------------------------------------------------*/

} // namespace-end