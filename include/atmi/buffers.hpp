/*
 * buffers
 *
 * Copyright (C) 2006 - herbert koelman
 *
 * Tuxedo is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * Logging Tuxedo is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Tuxedo; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA  02110-1301  USA
 */


#ifndef __ATMI_BUFFERS__
#define __ATMI_BUFFERS__

#include <typeinfo>
#include <fml.h>
#include <fml32.h>
#include <string>
#include <string.h>
#include <memory>

using namespace std;

namespace atmi {

//----------------------------------------------------------------------------------------------
  class field;
  class buffer;

  typedef char * Carray;
  typedef auto_ptr<Carray> ACarray;
  typedef auto_ptr<buffer> Abuffer;

  /**
   * FML buffer
   *
   */
  class buffer : public tuxedo {

    public:

      friend class field;

      /** Allocates a buffer of 1024bytes */
      buffer ();

      /** Create a new buffer reference.
       *
       * Memory allocation (alloc/free) are not handled by the instance create by this constructor. We assume that
       * the given buffer was allocated elsewhere. And it will be deallocated later.
       *
       * @param b set FML32 buffer reference
       */
      buffer ( FBFR32 *b );

      /** Allocates a buffer of byte size.
       * @param len bytes space of field value in bytres
       */
      buffer ( FLDLEN32 len );

      /** default destructor
       *
       * If memory was allocated by this instance then it will be freed. Otherwise the buffer is not deallocated.
       */
      ~buffer ();

      /**
       * @return true if it's a FMLTYPE32 buffer type
       * @throw atmi_exception if we failed to get buffer type (tptypes)
       */
      static inline bool is_fml32_buffer( char *buffer );

      /** @return the size of the buffer (in bytes) */
      long size ();

      /** @return number of bytes stored into the buffer */
      long used ();

      /** @return number of bytes not yet used into the buffer */
      long unused ();


      /** Removes unused space allocation
       */
      void pack ();

      /** Extends the buffer size with the given bytes
       *
       */
      void extend ();

      /** Extends the buffer size with the given bytes
       *
       * @param extent size in bytes of the extent
       * @throw buffer_exception upon failure
       */
      void resize ( long extent );

      /** Set a field's value into the buffer
       *
       * @param f a pointer to the field for which to change the value
       * @throw buffer_exception upon failure
       */
      field *set    ( field *f );

      /** Adds a field into the buffer
       *
       * @param f a pointer to the field to add into the buffer
       * @throw buffer_exception upon failure
       */
      field *add    ( field *f );         // FLDID fieldid, char *value, FLDLEN len );

      /** Appends a field to the buffer
       *
       * @param f a pointer to the field to add into the buffer
       */
      field *append ( field *f );         // FLDID fieldid, char *value, FLDLEN len );

      /** Removes the field from the buffer
       *
       * @param f a pointer to the field to remove
       * @throw buffer_exception upon failure
       */
      void remove ( field *f );         // FLDID fieldid );

      /** Get the field value set in the buffer
       * @param f the field to set
       * @return the field when set
       * @throw buffer_exception upon failure
       */
      field *get ( field *f );

      /** Get the field value set in the buffer
       * @param f the field to set
       * @param occ the field occurence to set
       * @return the field when set
       * @throw buffer_exception upon failure
       */
      field *get ( field *f, FLDOCC32 occ);

      /** @return the current check sum of the buffer
       *
       * @throw buffer_exception when calcutaion fails.
       */
      long chksum();

      /** @return the number of occurences of the field into the buffer */
      FLDOCC32 occurences ( const field *f );

      /** @return the number of fields into the buffer */
      FLDOCC32 num ();

      /** print on stdout the content of the buffer. Mainly used for debugging
       */
      void print ();

      /** @return return FML32 buffer reference
       */
      FBFR32 * get_buffer ();

      /** Set a new buffer reference. Previous buffer is deallocated before assign the passed value.
       * @param b set FML32 buffer reference
       */
      void set_buffer ( FBFR32 *b );

      /**
       * @return true if this instance willfree allocated memory buffer.
       */
      bool is_handling_memory();

      /** @param b If true then destructor will free referenced buffer.
       */
      void set_handling_memory ( bool b);

      // operators -------------------------------------------------------------------------------

      /** Checks equality of two buffers (based upon chksum)
       * @return true if both checksums are equal.
       */
      bool operator== ( buffer &);

      /** Copies the content of a fielded buffer into another
       */
      buffer &operator= (buffer &);

    private:

      /** FML buffer reference */
      FBFR32 *_buffer;

      /** if true then buffer was allocated by the buffer instance (thus it can be freed when needed).
       */
      bool _allocated;

      /** value by which the buffer size will be extended */
      long _extent;
  };

// ---------------------------------------------------------------------------------


/** Abstract class to manipulate a field
 *
 */
  class field {

    public:

      friend class buffer;

      /** Default destructor
       */
      virtual ~field () {
      };

      /**
       *  Possible values are:
       *  FLD_SHORT       0       short int
       *  FLD_LONG        1       long int
       *  FLD_CHAR        2       character
       *  FLD_FLOAT       3       single-precision float
       *  FLD_DOUBLE      4       double-precision float
       *  FLD_STRING      5       string - null terminated
       *  FLD_CARRAY      6       character array
       *  FLD_PTR         9       pointer to a buffer
       *  FLD_FML32      10       embedded FML32 buffer
       *  FLD_VIEW32     11       embedded VIEW32 buffer
       *  FLD_MBSTRING   12       multibyte character array
       *
       * @return the field's type
       */
      int type ();

      /** @return the name of the fml field type
       */
      const char * tname ();

      /** @return the field ID of the field
       */
      FLDID32 id ();

      /** Extracts the field number from the field identifier.
       *
       * @return field number.
       */
      int number ();

      /** @return the name of the field */
      const char *name ();

      /** @return the fields occurence (as last found in a buffer)*/
      FLDOCC32 occurence ();

      /** @return the last Ferror32 value */
      int error ();

      /** @return the length of the field's value
       */
      virtual FLDLEN32  length () = 0;

      virtual long needed ();

      /** @return a string describing the field
       */
      const char * what ();

      virtual field &operator= ( const string & ) {
        return *this;
      };

    protected:

      int handleFerror(int, const char *, ...);

      virtual void setFocc ( FLDOCC32 occ );

      /** utility method to setup field's data
       */
      virtual void setup ( FLDID32 fid );

      /** Retrieves the value of the field found into the buffer
       *
       * @param b buffer from which to retrieve the field's value
       */
      virtual int get ( buffer *b ) = 0;

      /** Retrieves the value of the field's occurence found into the buffer
       *
       * Upon success the value of focc is set to the retrieved occurence.
       *
       * @param b buffer from which to retrieve the field's value
       * @param occ occurence to retreive
       */
      virtual int get ( buffer *b, FLDOCC32 occ ) = 0;

      /** add the fiels into the buffer
       *
       * When successfull the value of focc is set
       *
       * @param b buffer in which to add the field.
       */
      virtual int add ( buffer *b ) = 0;

      /** set the value of the field's value
       *
       * @param b the buffer in which the value must be changed
       */
      virtual int set ( buffer *b ) = 0;

      /** removes the field from the buffer
       *
       * @param b the buffer from which to remove the field
       */
      virtual int remove ( buffer *b );

    private:
      FLDID32 fid;  // field ID
      FLDOCC32 focc;                    // field occurence
      char *fname;
      int last_err;
  };

/** Template that handles non pointer data types (short, long, char, double, ...)
 *
 */
  template <class T> class Tfield : public field {
    public:

      /** Constructs a Tfield for the passed field id
       *
       * The search is done in the tables identified by FLDTBLDIR32  and FIELDTBLS32
       *
       * @param fid the fml field id to setup (as defined in the FML tables)
       */
      Tfield ( FLDID32 fid ) {

        /** This array is used to check that FML type matches template Tfield's type */
        const char *TYPEID_NAMES[5] = { typeid(short).name(), typeid(long).name(), typeid(char).name(), typeid(float).name(), typeid(double).name() };

        setup ( fid );
        // check type matching
        if ( type() > 5 ) {
          throw atmi_exception ( "This template doesn't support the given type Tfield<%s>.", tname ());
        } else if ( strcmp (typeid(this->value).name (), TYPEID_NAMES[type()]) != 0 ) {
          throw atmi_exception ( "Tfield %s's value is of type %s and the FML table decalares a type %s.", name (), typeid(this->value).name (), tname());
        }
      }

      /** Constructs a Tfield for the passed name
       *
       * The search is done in the tables identified by FLDTBLDIR32  and FIELDTBLS32
       *
       * @param n the fml field name to setup (as defined in the FML tables)
       */
      Tfield ( const char *n ) {

        setup ( (FLDID32) Fldid32 ( const_cast<char *>(n) ) );

        // check type matching
        if ( strcmp (typeid(this->value).name (), tname ()) != 0 ) {
          throw atmi_exception ( "Tfield value is of type %s and the FML table decalares a type %s for %s.", typeid(this->value).name (), tname(), name());
        }
      }

      virtual ~Tfield() {
      };

      /** @return de length (or size) of the field's data
       */
      virtual FLDLEN32 length () {

        return sizeof ( value );
      };

      /** Assigns a value to the field
       * ...
       * Tfield<int> f (fid);
       * f = 156 ; // Set value to 156
       * ...
       */
      T operator= ( T v) {

        value = v;

        return *this;
      };

      /** casts the field value
       * ...
       * Tfield<int> f (fid);
       * f = 156 ;
       * int x = f ; // returns 156
       * ...
       */
      operator T(){

        return value;
      };

    protected:

      virtual int set ( buffer *b) {

        int rc = -1;

        rc = Fchg32 ( b->get_buffer(), id(), occurence(), (char *) &value, length() );
        if ( rc < 0 ) {
          throw buffer_exception ( Ferror32, "FCHG32 Tfield::set failed for field %s (id: %d, occ: %d)", name(), id(), occurence() );
        }

        return rc;
      };

      virtual int add ( buffer *b) {

        int rc = -1;
        try {
          if ( (needed () + b->used()) >= b->size()) {
            b->extend ();
          }
        } catch ( buffer_exception buffErr ) {
          throw  atmi_exception ( "Add failed to estimate needed memory extension. Original message was : %s", buffErr.what() );
        };

        rc = Fadd32 ( b->get_buffer(), id(), (char *) &value, length() );
        if ( rc < 0 ) {
          throw buffer_exception ( Ferror32, "FADD32 Tfield::add failed for field %s (id: %d, occ: %d)", name(), id(), occurence() );
        } else {
          setFocc ( (b->occurences ( this ) == 0 ? 0 : b->occurences ( this )-1));
        }

        return rc;
      };

      virtual int get ( buffer *b ){

        return get ( b, occurence() );
      };

      virtual int get ( buffer *b, FLDOCC32 occ ){

        int rc = -1;
        FLDLEN32 l = length();
        setFocc (occ);

        rc = Fget32 ( b->get_buffer(), id(), occurence(), (char *) &value, &l );
        if ( rc == -1 ) {
          throw buffer_exception (Ferror32, "FGET32 failed to get field %s (id: %d, occ: %d).", name(), id(), occurence() );
        }

        return rc;
      };

      T value;

  };


/** Specialization of template Tfield which handles string typed fields.
 *
 * This class is using a string object to hold and handle string data
 */
  template <> class Tfield<string>: public field {
    public:

      /** Constructs a Tfield for the passed field id
       *
       * The search is done in the tables identified by FLDTBLDIR32  and FIELDTBLS32
       *
       * @param fid the fml field id to setup (as defined in the FML tables)
       */
      Tfield ( FLDID32 fid ) {

        setup ( fid );

        // check taht we have a string declaration in the FML table
        if ( type () != 5 ) {
          throw atmi_exception ( "Tfield %s's value is of type string and the FML table decalares a type %s.", name (), tname());
        }
      }

      /** Constructs a Tfield for the passed name
       *
       * The search is done in the tables identified by FLDTBLDIR32  and FIELDTBLS32
       *
       * @param n the fml field name to setup (as defined in the FML tables)
       */
      Tfield ( const char *n ) {

        setup ( (FLDID32) Fldid32 ( const_cast<char *>(n) ) );

        // check type matching
        if ( type() !=5 ) {
          throw atmi_exception ( "Tfield value is of type string and the FML table decalares a type %s for %s.", tname(), name());
        }
      }

      virtual ~Tfield() {
      };

      /** @return the string's length
       */
      virtual FLDLEN32 length () {

        return (FLDLEN32)value.length();
      };

      /** @return the string's size
       */
      virtual FLDLEN32 size () {

        return (FLDLEN32)value.size();
      };

      /** @return a C string (with \0) of current string's content.
       */
      const char* c_str ( ) const {
        return value.c_str();
      }

      /** Appends a copy of the argument to the string.
       *
       * The new string content is the content existing in the string object before the call followed by the content of
       * the argument.
       *
       * The append member function provides a similar functionality with additional options.
       *
       * @param str a copy of the content of this object is appended to the object's content.
       * @return *this
       */
      string& operator+= ( const string& str ){

        return value += str;
      }

      /** Appends a copy of the argument to the string.
       *
       * The new string content is the content existing in the string object before the call followed by the content of
       * the argument.
       *
       * The append member function provides a similar functionality with additional options.
       *
       * @param  s  a pointer to an array containing a null-terminated character sequence (C string), which is appended to the object's content.
       * @return *this
       */
      string& operator+= ( const char* s ){

        return value += s;
      }

      /** Appends a copy of the argument to the string.
       *
       * The new string content is the content existing in the string object before the call followed by the content of
       * the argument.
       *
       * The append member function provides a similar functionality with additional options.
       *
       * @param c   character. This single character is appended to the string object's content.
       * @return *this
       */
      string& operator+= ( char c ){
        return value += c;
      }

      /**
       * Returns a reference the character at position pos in the string.
       *
       * The function actually returns data()[ pos ].
       *
       * The at member function has the same behavior as this operator function, except that at also performs a range
       * check.
       *
       * @param pos position within the string of the character to be retrieved. Notice that the first character in the string has a position of 0, not 1. size_t is an unsigned integral type.
       * @return The character at the specified position in the string.
       */
      const char& operator[] ( size_t pos ) const {

        return value[pos];
      }

      /**
       * Returns a reference the character at position pos in the string.
       *
       * The function actually returns data()[ pos ].
       *
       * The at member function has the same behavior as this operator function, except that at also performs a range
       * check.
       *
       * @param pos position within the string of the character to be retrieved. Notice that the first character in the string has a position of 0, not 1. size_t is an unsigned integral type.
       * @return The character at the specified position in the string.
       */
      char& operator[] ( size_t pos ){

        return value[pos];
      }

      /**
       * string& operator= ( char c );
       *
       * String assignment
       * Sets a copy of the argument as the new content for the string object.
       *
       * The previous content is dropped.
       *
       * The assign member function provides a similar functionality with additional options.
       *
       * @param s   a pointer to an array containing a null-terminated character sequence (C string), which is copied as the new content for the object.
       */
      virtual Tfield<string> &operator= ( const char* s ) {

        value = s;
        return *this;
      };

      /**
       * string& operator= ( char c );
       *
       * String assignment
       * Sets a copy of the argument as the new content for the string object.
       *
       * The previous content is dropped.
       *
       * The assign member function provides a similar functionality with additional options.
       *
       * @param str a copy of the content of this object is used as the new content for the object.
       */
      virtual Tfield<string> &operator= ( const string &str ) {

        value = str;

        return *this;
      };

      /**
       * String assignment
       * Sets a copy of the argument as the new content for the string object.
       *
       * The previous content is dropped.
       *
       * The assign member function provides a similar functionality with additional options.
       *
       * @param  c  the content is set to a single character.
       */
      virtual Tfield<string> &operator= ( char &c ) {

        value = c;

        return *this;
      };
      /** casts the field value
       */
      operator string(){

        return value;
      };

      /** @return a null terminated const char *
         operator const char*() {

         return value.c_str();
         };
       */

    protected:

      virtual int set ( buffer *b) {

        int rc = -1;

        rc = Fchg32 ( b->get_buffer(), id(), occurence(), (char *) value.c_str(), length() );
        if ( rc < 0 ) {
          throw buffer_exception ( Ferror32, "FCHG32 Tfield::set failed for field %s (id: %d, occ: %d)", name(), id(), occurence() );
        }

        return rc;
      };

      virtual int add ( buffer *b) {

        int rc = -1;
        if ( length () > 0 ) {
          try {
            if ( (needed () + b->used()) >= b->size()) {
              b->extend ();
            }
          } catch ( buffer_exception buffErr ) {
            throw  atmi_exception ( "Add failed to estimate needed memory extension. Original message was : %s", buffErr.what() );
          };

          rc = Fadd32 ( b->get_buffer(), id(), (char *) value.c_str(), length() );
          if ( rc < 0 ) {
            throw buffer_exception ( Ferror32, "FADD32 Tfield::add failed for field %s (id: %d, occ: %d)", name(), id(), occurence() );
          } else {
            setFocc ( (b->occurences ( this ) == 0 ? 0 : b->occurences ( this )-1));
          }
        } else {
          throw atmi_exception ("field %s's value is empty !!?? Cannot add an empty field value.", name ());
        }

        return rc;
      };

      virtual int get ( buffer *b ){

        return get ( b, occurence() );
      };

      virtual int get ( buffer *b, FLDOCC32 occ ){

        int rc = -1;
        FLDLEN32 l = 0;
        char *v = NULL;
        setFocc(occ);

        v = Fgetsa32 ( b->get_buffer(), id(), occurence(), &l );
        if ( v != NULL ) {

          rc=0;
          value = v;                    // copy v into string value
          delete v;

        } else {
          throw buffer_exception (Ferror32, "FGETSA32 Tfield::get failed to get field %s (id: %d, occ: %d).", name(), id(), occurence() );
        }

        return rc;
      };

      string value;
  };

/** Specialization of template Tfield which handles CARRAY typed fields.
 *
 * This class is using a string object to hold and handle char * data
 */
  template <> class Tfield<char *>: public field {
    public:

      /** Constructs a Tfield for the passed field id
       *
       * The search is done in the tables identified by FLDTBLDIR32  and FIELDTBLS32
       *
       * @param fid the fml field id to setup (as defined in the FML tables)
       */
      Tfield ( FLDID32 fid ) : len (0), value (NULL) {

        setup ( fid );

        // check taht we have a string declaration in the FML table
        if ( type () != 6 ) {
          throw atmi_exception ( "Tfield %s's value is of type carray and the FML table decalares a type %s.", name (), tname());
        }
      }

      /** Constructs a Tfield for the passed name
       *
       * The search is done in the tables identified by FLDTBLDIR32  and FIELDTBLS32
       *
       * @param n the fml field name to setup (as defined in the FML tables)
       */
      Tfield ( const char *n ) : len (0), value (NULL) {

        setup ( (FLDID32) Fldid32 ( const_cast<char *>(n) ) );

        // check type matching
        if ( type() != 6 ) {
          throw atmi_exception ( "Tfield value is of type carray and the FML table decalares a type %s for %s.", tname(), name());
        }
      }

      virtual ~Tfield() {

        if ( value != NULL) delete[] value;
      };

      /** @return returns the size of the carray
       */
      virtual FLDLEN32 length () {

        return len;
      };

      /** Assigns a value to the field
       */
      void setCarray ( const char * c, long s ) {

        if ( value == NULL ) delete value;

        len = s;
        value = new char [len];
        memcpy ( value, c, len );
      };

      /**
       * @return new allocated  pointer to the value of the carray
         virtual ACarray getCarrayAlloc () {

              Carray carray = new char[length()];
              memcpy ( carray, value, length () );

         return new ACarray ( &carray );
         };
       */

      /** copy (memcpy) the field's value into carray. If size is smaller than the length of the field,
       * then only size characters are copied. If size is bigger then the field's length, then length characters
       * are copied.
       *
       * @param carray pointer to a character buffer
       * @param size size of the buffer
       */
      virtual void getCarray ( char *carray, long size ) {

        memcpy ( carray, value, ( size < length () ? size : length() ) );
      };

      /** copy the field's value
       */
      Tfield<char *> &operator= ( Tfield<char*> &carray ){

        setCarray ( carray.value, carray.len );

        return *this;
      };

    protected:

      virtual int set ( buffer *b) {

        int rc = -1;

        rc = Fchg32 ( b->get_buffer(), id(), occurence(), value, length() );
        if ( rc < 0 ) {
          throw buffer_exception ( Ferror32, "FCHG32 Tfield::set failed for field %s (id: %d, occ: %d)", name(), id(), occurence() );
        }

        return rc;
      };

      virtual int add ( buffer *b) {

        int rc = -1;
        if ( length () > 0 ) {
          try {
            if ( (needed () + b->used()) >= b->size()) {
              b->extend ();
            }
          } catch ( buffer_exception buffErr ) {
            throw  atmi_exception ( "Add failed to estimate needed memory extension. Original message was : %s", buffErr.what() );
          };

          rc  = Fadd32 ( b->get_buffer(), id(), value, length() );
          if ( rc < 0 ) {
            throw buffer_exception ( Ferror32, "FADD32 Tfield::add failed for field %s (id: %d, occ: %d)", name(), id(), occurence() );
          } else {
            setFocc ( (b->occurences ( this ) == 0 ? 0 : b->occurences ( this )-1));
          }

        } else {
          throw atmi_exception ( "field %s's value is empty !!?? Cannot add an empty field value.", name () );
        }

        return rc;
      };

      virtual int get ( buffer *b ){

        return get ( b, occurence() );
      };

      virtual int get ( buffer *b, FLDOCC32 occ ){

        int rc = -1;
        FLDLEN32 l = 0;
        setFocc(occ);
        char *v = NULL;

        v = Fgetalloc32 ( b->get_buffer(), id(), occurence(), &l );
        if ( v != NULL ) {

          len = l;
          if ( value != NULL ) delete[] value;

          value = new char[len];
          memcpy (value, v, len);

          rc = 0;
          delete[] v;

        } else {
          throw buffer_exception (Ferror32, "FGETSA32 Tfield::get failed to get field %s (id: %d, occ: %d).", name(), id(), occurence() );
        }

        return rc;
      };

      FLDLEN32 len;
      char    *value;
  };

/** Helper that handles the operator << between output streams and field value */
  ostream &operator<< ( ostream &o, Tfield<string> &f );


}
#endif
