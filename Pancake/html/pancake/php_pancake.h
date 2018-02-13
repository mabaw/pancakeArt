/*
  +----------------------------------------------------------------------+
  | PHP Version 5                                                        |
  +----------------------------------------------------------------------+
  | This source file is subject to BSD 3-Clause license,                 |
  | that is bundled with this package in the file LICENSE, and is        |
  | available through the world-wide-web at the following url:           |
  | http://opensource.org/licenses/BSD-3-Clause.                         |
  | If you did not receive a copy of the BSD license and are unable to   |
  | obtain it through the world-wide-web, please send a note to          |
  | robert@xarg.org so we can mail you a copy immediately.               |
  +----------------------------------------------------------------------+
  | Author: Robert Eisele <robert@xarg.org>                              |
  +----------------------------------------------------------------------+
*/

#ifndef PHP_PANCAKE_H
#define PHP_PANCAKE_H

#define PHP_PANCAKE_VERSION "1.1.0"
#define PHP_PANCAKE_EXTNAME "pancake"

#ifdef ZTS
# include "TSRM.h"
#endif

PHP_MINFO_FUNCTION(pancake);

PHP_FUNCTION(to_pancake);

extern zend_module_entry pancake_module_entry;
#define phpext_pancake_ptr &pancake_module_entry

#endif

