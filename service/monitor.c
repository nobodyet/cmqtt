#include "all.h"
#include "monitor.h"
#include "../include/mysql.sdk.h"
#include <stdint.h>
#include <malloc.h>
#include <assert.h>
#include <pthread.h>

#define LIBNAME		"LZmonitor"

#define LOCK_INIT(m)	pthread_mutex_init(&(m), NULL)	
#define LOCK(m)			pthread_mutex_lock(&(m))
#define UNLOCK(m)		pthread_mutex_unlock(&(m))

#define MAX_BUFFER		256	


struct user {
	int id;
	int token;
	int64_t takecoin;
};

struct monitor {
	int matchid;
	int matchseq;
	int timeout;
	int cb;
	int max;
	int nuse;
	struct user buffer[MAX_BUFFER];
	struct monitor * next;
	pthread_mutex_t lock;
};

struct monitor_G {
	MYSQL mysql;
	struct monitor * head;
	pthread_mutex_t lock;
};


static struct monitor_G * M;

#define QUERY_STR	\
	"SELECT userid,takecoin " \
	"FROM t_jj_user_match " \
	"WHERE (state=%d or (state=%d and rtime+%d<%d)) and matchid=%d and matchseq=%d limit %d"

static void
_recyle_users(struct monitor * m) {
	int i;
	MYSQL_RES * res;	
	MYSQL_ROW row;
	char query[1024];

	sprintf(query, QUERY_STR, M_MATCHOVER, M_MATCHSTART, m->timeout, timeGloble_g, m->matchid, m->matchseq, MAX_BUFFER);
	if ( excuteSql(&M->mysql, query) )
		return;

	res = mysql_store_result(&M->mysql);
	if ( NULL == res )
		return;

	i = 0;
	while ( (row = mysql_fetch_row(res)) ) {
		m->buffer[i].id = atoi(row[0]); 
		m->buffer[i].takecoin = (int64_t)atoll(row[1]);
		++i;
		// 双重验证
		if ( i >= MAX_BUFFER )
			break;
	}
	m->nuse = i;
	if ( m->nuse > 0 ) {
		log("[MONITOR] recyle %d users, matchid:%d, matchseq:%d", m->nuse, m->matchid, m->matchseq);
	}

	mysql_free_result(res);
}


static void *
_worker(void * arg) {
	struct timeval tv1, tv2;
	struct monitor * m;
	struct monitor_G * G = (struct monitor_G *)arg;

	log("[MONITOR] worker thread startup, pid:%ld", pthread_self());

	for ( ;; ) {
		sleep(2);

		gettimeofday(&tv1, NULL);

		LOCK(G->lock);

		m = G->head;
		while ( m ) {
			LOCK(m->lock);
			_recyle_users(m);
			UNLOCK(m->lock);
			m = m->next;
		}

		UNLOCK(G->lock);

		gettimeofday(&tv2, NULL);
		debug("MONITOR.TIMEUSED: %ld us", (tv2.tv_sec-tv1.tv_sec)*1000000 + (tv2.tv_usec-tv1.tv_usec));
	}
	return NULL;
}

static int
_new(lua_State * L) {
	int cb;
	struct monitor * m;

	lua_pushvalue(L, 4);
	cb = luaL_ref(L, LUA_REGISTRYINDEX);
	assert(cb!=LUA_REFNIL);

	m = (struct monitor *)malloc(sizeof(*m));
	LOCK_INIT(m->lock);
	m->cb = cb;
	m->nuse = 0;
	m->matchid = luaL_checkint(L, 1);
	m->matchseq = luaL_checkint(L, 2);
	m->timeout = luaL_checkint(L, 3);

	LOCK(M->lock);
	m->next = M->head;
	M->head = m;
	UNLOCK(M->lock);

	lua_pushlightuserdata(L, m);

	log("[MONITOR] New, matchid:%d, matchseq:%d, timeout:%d, max:%d", m->matchid, m->matchseq, m->timeout, m->max);

	return 1;
}

static int
_heart(lua_State * L) {
	int i;
	struct user * u;
	struct monitor * m = lua_touserdata(L, 1);

	LOCK(m->lock);

	for ( i=0; i<m->nuse; ++i ) {
		u = &m->buffer[i];
		lua_rawgeti(L, LUA_REGISTRYINDEX, m->cb);
		lua_pushnumber(L, u->id);
		lua_pushnumber(L, u->takecoin);
		if ( lua_pcall(L, 2, 0, 0) ) {
			log("error: %s", lua_tostring(L, -1));
			lua_pop(L, 1);
		}
	}

	m->nuse = 0;

	UNLOCK(m->lock);

	return 0;
}

void
monitor_init(void) {
	pthread_t pid;

	M = (struct monitor_G *)malloc(sizeof(*M));

	if ( mysqlConnetInit(&M->mysql) ) {
		log("connect db server FAILED!");
		assert(0);
	}

	LOCK_INIT(M->lock);
	M->head = NULL;
	pthread_create(&pid, NULL, _worker, M);

	log("[MONITOR] startup");
}

void 
monitor_openlib(lua_State * L) {
	static luaL_Reg _libs[] = {
		{"new", _new},
		{"heart", _heart},
		{NULL, NULL}
	};

	luaL_register(L, LIBNAME, _libs);
}

/* END */

