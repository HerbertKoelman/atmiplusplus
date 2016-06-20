/*
 * ATMI++
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



#ifndef _ATMI_PLUS_PLUS_H_
#define _ATMI_PLUS_PLUS_H_

#include <userlog.h>
#include <signal.h>
#include <Usignal.h>

#include <atmi/exceptions.hpp>
#include <atmi/logger.hpp>
#include <atmi/tuxedo.hpp>
#include <atmi/buffers.hpp>
#include <atmi/config.h>

//! \namespace atmi 
namespace atmi {

  /** @return the library's current version */
  extern "C" const char *cpp_atmi_version();

}
#endif