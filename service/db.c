#define LOG_TAG		"db"
#include "../include/debug.h"

#include "db.h"
#include "all.h"
#include <stdint.h>
#include <assert.h>

#define DBLIBNAME		"LZdb"

static MYSQL * _db = NULL;



int
db_init(MYSQL * db) {
	_db = db;
	return 0==mysqlConnetInit(_db)?1:0;
}



/* END */

