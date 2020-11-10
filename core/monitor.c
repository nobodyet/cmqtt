

#include "monitor.h"

#define LOCK_INIT(m) pthread_mutex_init(&(m), NULL)
#define LOCK(m) pthread_mutex_lock(&(m))
#define UNLOCK(m) pthread_mutex_unlock(&(m))

#define MAX_BUFFER 256

struct user
{
	int id;
	int token;
	int64_t takecoin;
};

struct monitor
{
	int matchid;
	int matchseq;
	int timeout;
	int cb;
	int max;
	int nuse;
	struct user buffer[MAX_BUFFER];
	struct monitor *next;
	pthread_mutex_t lock;
};

struct monitor_G
{
	MYSQL mysql;
	struct monitor *head;
	pthread_mutex_t lock;
};

static struct monitor_G *M;

#define QUERY_STR             \
	"SELECT userid,takecoin " \
	"FROM t_jj_user_match "   \
	"WHERE (state=%d or (state=%d and rtime+%d<%d)) and matchid=%d and matchseq=%d limit %d"

static void
_recyle_users(struct monitor *m)
{
	int i;
	MYSQL_RES *res;
	MYSQL_ROW row;
	char query[1024];

	sprintf(query, QUERY_STR, M_MATCHOVER, M_MATCHSTART, m->timeout, timeGloble_g, m->matchid, m->matchseq, MAX_BUFFER);
	if (excuteSql(&M->mysql, query))
		return;

	res = mysql_store_result(&M->mysql);
	if (NULL == res)
		return;

	i = 0;
	while ((row = mysql_fetch_row(res)))
	{
		m->buffer[i].id = atoi(row[0]);
		m->buffer[i].takecoin = (int64_t)atoll(row[1]);
		++i;
		// 双重验证
		if (i >= MAX_BUFFER)
			break;
	}
	m->nuse = i;
	if (m->nuse > 0)
	{
		log("[MONITOR] recyle %d users, matchid:%d, matchseq:%d", m->nuse, m->matchid, m->matchseq);
	}

	mysql_free_result(res);
}

void monitor_init(void)
{
	pthread_t pid;

	M = (struct monitor_G *)malloc(sizeof(*M));

	if (mysqlConnetInit(&M->mysql))
	{
		log("connect db server FAILED!");
		assert(0);
	}

	LOCK_INIT(M->lock);
	M->head = NULL;
	pthread_create(&pid, NULL, _worker, M);

	log("[MONITOR] startup");
}

/* END */
