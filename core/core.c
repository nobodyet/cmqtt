#define LOG_TAG		"db"
#include "../include/debug.h"

#include "db.h"
#include "all.h"
#include <stdint.h>
#include <assert.h>

#define DBLIBNAME		"LZdb"



int core_init(){
  int ret =0 ;
  
  return ret;
}


int db_init(MYSQL * db) {
  int ret;

  MYSQL * _db = db;
  ret = mysqlConnetInit(_db)?1:0;

	return ret; 
}



/* END */

