/*
 * Copyright (C) 2006 - herbert koelman
 *
 * Thread is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * Thread is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Thread; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA  02110-1301  USA
 */

#include <string>
#include <map>
#include <memory>
#include <exception>
#include <yaml.h>

using namespace std;

#ifndef __CONFIG_PARSER__
#define __CONFIG_PARSER__

namespace atmi {

  class YamlConfig;
  typedef auto_ptr<YamlConfig> AYamlConfig;

  typedef enum yaml_node_types_e {
    YAML_NODE,
    YAML_LEAF,
    YAML_SEQ_LEAF
  } yaml_node_types_t;

  class YamlException : public std::exception {
    public:
      YamlException ( const char *msg ) throw () : message(msg) {
      };

      const char *what () const throw(){
        return message;
      };

    private:
      const char *message;
  };

  /** This class parses a yaml formattted file and return a map of key/value entries found in the yaml file
   *
   * @author herbert
   * @since v3.0
   */
  class YamlConfig {
    public:
      /**
       * Create an instance ready to read the given YAML formatted file.
       *
       * @param file yaml formatted configuration file
       */
      YamlConfig ( const char *file ) throw (YamlException);
      ~YamlConfig();

      /**
       * Build a map made of string key and values.
       *
       * The key entries are forming a unique path to a value. Enumerated values for a key are seperated by a white spaces. When the parsing is done you can get the result through the get_properties method.
       *
       * @param root a prefix to all subsequent nodes.
       */
      void parse ( string root );

      /**
       * Build a map made of string key and values.
       *
       * The key entries are forming a unique path to a value. Enumerated values for a key are seperated by a white spaces. When the parsing is done you can get the result through the get_properties method.
       *
       */
      void parse ();

      /**
       * search the value for the given key. If not found the default value is returned instead.
       *
       * @param key map entry key
       * @param dvalue value returned if key doesn't exist
       * @return the string value of the key (or default if not found)
       */
      string get_string_property ( const string key, string dvalue ) ;

      /**
       * search the value for the given key. If not found an exception is returned
       *
       * @param key map entry key
       * @return the string value of the key
       * @trows out_of_range if the searched key was not found
       */
      string get_string_property ( const string key ) ;

      /** 
       * search the value for the given key. If not found the default value integer is retruned.
       *
       * if a value was found it is converted into an integer using atoi().
       *
       * @param key searched map entry key
       * @param dvalue value returned if key doesn't exist
       * @return the integer value of the key.
       */
      int get_int_property ( const string key, int dvalue );

      /**
       * search the value for the given key. If not found an exception is returned
       *
       * @param key map entry key
       * @return the integer value of the key
       * @trows out_of_range if the searched key was not found
       */
      int get_int_property ( const string key ) ;

      /**
       * This map will contain the result of the parsing
       *
       * @return a map of key/value pairs
       */
      const map<string,string> get_properties ();

      /**
       * Print content on stdout
       */
      void print () ;

    private:

      /** this map will be populated by the call to parse() method
       */
      map<string, string> properties;

      yaml_node_types_t node_type;
      int last_token;

      FILE*f;
      yaml_parser_t parser;
      yaml_token_t token;  /* new variable */
  };
}
#endif
