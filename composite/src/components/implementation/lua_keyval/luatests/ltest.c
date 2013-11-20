#include <cos_component.h>
#include <print.h>
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <stdlib.h>


// some dummy functions to remove the compiling warning

//  Table specific string value lookup from the lua_State
char* luakv_getstring_bytable (char *key, char *table_name)
{
	return NULL;
}

//  Search for a string value from the default table
char* luakv_getstring (char *key)
{
	return NULL;
}

// Table specific lookup for an int value
int luakv_getnumber_bytable (char *key, char *table_name)
{
	return 0;

}

// Search for a number value in the default table
int luakv_getnumber (char *key)
{
	return 0;
}

// Store a string value in the specified table
void luakv_putstring_bytable (char *key, char *value, char *table_name)
{

}

// Store a string value in the default table
void luakv_putstring (char *key, char *value)
{
	
}

// Store an int value in the specified table
void luakv_putnumber_bytable (char *key, int intvalue, char *table_name)
{

}

// Store an int value in the default table
void luakv_putnumber (char *key, int intvalue)
{

}


void
cos_init(void)
{
	printc("We welcome our new Lua overlords!\n");
	// Creates the lua state
	lua_State *L = luaL_newstate();

	// Pushes an int onto the Lua stack, pops it, 
	// Testing executing a string of Lua
	// returns a value which pushes it onto the stack, then
	// prints it
	int tmp = luaL_dostring(L, "return 'Our printed string'");
	const char *str = lua_tostring(L, -1);
	printc("Lua returned value is %s\n", str);

	// tests loding configuration from a file
	// loads the file, then executes it, which converts the variables
	// in the file to lua parsable data
	if (luaL_loadfile(L, "/home/cos/research/composite/src/components/implementation/lua_keyval/luatests/test.txt") || lua_pcall(L, 0, 0, 0)){
		// if either call fails, will return non-zero error code and print
		// it out here
		printc("error %s", lua_tostring(L, -1));
	}

	lua_getglobal(L, "width");
	int width = lua_tonumber(L, -1);
	printc("Width: %d\n", width);

	// Fibonacci tests
	if (luaL_loadfile(L, "/home/cos/research/composite/src/components/implementation/lua_keyval/luatests/foo.lua") || lua_pcall(L, 0, 0, 0)){
		// if either call fails, will return non-zero error code and print
		// it out here
		printc("error %s", lua_tostring(L, -1));
	}

	lua_getglobal(L, "fib");
	int fib_num = 20;
	lua_pushnumber(L,fib_num);
	lua_pcall(L,1,1,0);
	int new_num = (int)lua_tonumber(L, -1);
	printc("Fib of %d is %d\n", fib_num, new_num);
	
	// Linked list tests
	int i = 0;	
	for (i = 0; i < 10; i++) {
		lua_getglobal(L,"linked_list");
		lua_pushnumber(L,40000);
		lua_pcall(L,1,0,0);
		const char *str = lua_tostring(L,-1);
		printc("Lua string is %s\n", str); 
		printc("garbage %d\n", lua_gc(L, LUA_GCCOUNT,0));
	
		lua_getglobal(L,"clear_list");
		lua_pcall(L,0,0,0);
		// lua_gc(L, LUA_GCCOLLECT,9);
		printc("garbage %d\n", lua_gc(L, LUA_GCCOUNT,0));
	}
	// Closes lua
	lua_close(L);
	printc("End of Lua file\n");
	return;
}
