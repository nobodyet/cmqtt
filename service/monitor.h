#ifndef __MOZIT_MONITOR_H
#define __MOZIT_MONITOR_H

// for lua
#include "../lua/lua.h"
#include "../lua/lualib.h"
#include "../lua/lauxlib.h"

void monitor_init(void);
void monitor_openlib(lua_State * L);

#endif /* #ifndef __MOZIT_MONITOR_H */

