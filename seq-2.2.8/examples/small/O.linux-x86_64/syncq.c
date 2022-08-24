/* C code for program syncqTest, generated by snc from ../syncq.st */
#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include <limits.h>

#include "seq_snc.h"

/* Variable declarations */
struct seqg_vars {
# line 11 "../syncq.st"
	string s[3];
# line 16 "../syncq.st"
	string s1[1];
# line 21 "../syncq.st"
	int n;
};


/* Function declarations */

#define seqg_var (*(struct seqg_vars *const *)seqg_env)

/* Program init func */
static void seqg_init(PROG_ID seqg_env)
{
	{
# line 21 "../syncq.st"
	static int seqg_initvar_n = 0;
	memcpy(&seqg_var->n, &seqg_initvar_n, sizeof(seqg_initvar_n));
	}
}

/****** Code for state "get" in state set "get" ******/

/* Event function for state "get" in state set "get" */
static seqBool seqg_event_get_0_get(SS_ID seqg_env, int *seqg_ptrn, int *seqg_pnst)
{
# line 27 "../syncq.st"
	if (seq_pvGetQ(seqg_env, 0/*s*/))
	{
		*seqg_pnst = 1;
		*seqg_ptrn = 0;
		return TRUE;
	}
	return FALSE;
}

/* Action function for state "get" in state set "get" */
static void seqg_action_get_0_get(SS_ID seqg_env, int seqg_trn, int *seqg_pnst)
{
	switch(seqg_trn)
	{
	case 0:
		{
# line 28 "../syncq.st"
			printf("got: [%s,%s,%s]\n", seqg_var->s[0], seqg_var->s[1], seqg_var->s[2]);
		}
		return;
	}
}

/****** Code for state "pause" in state set "get" ******/

/* Event function for state "pause" in state set "get" */
static seqBool seqg_event_get_0_pause(SS_ID seqg_env, int *seqg_ptrn, int *seqg_pnst)
{
# line 32 "../syncq.st"
	if (seq_delay(seqg_env, 2))
	{
		*seqg_pnst = 0;
		*seqg_ptrn = 0;
		return TRUE;
	}
	return FALSE;
}

/* Action function for state "pause" in state set "get" */
static void seqg_action_get_0_pause(SS_ID seqg_env, int seqg_trn, int *seqg_pnst)
{
	switch(seqg_trn)
	{
	case 0:
		{
		}
		return;
	}
}

/****** Code for state "get" in state set "get1" ******/

/* Event function for state "get" in state set "get1" */
static seqBool seqg_event_get1_1_get(SS_ID seqg_env, int *seqg_ptrn, int *seqg_pnst)
{
# line 39 "../syncq.st"
	if (seq_pvGetQ(seqg_env, 1/*s1*/))
	{
		*seqg_pnst = 1;
		*seqg_ptrn = 0;
		return TRUE;
	}
	return FALSE;
}

/* Action function for state "get" in state set "get1" */
static void seqg_action_get1_1_get(SS_ID seqg_env, int seqg_trn, int *seqg_pnst)
{
	switch(seqg_trn)
	{
	case 0:
		{
# line 40 "../syncq.st"
			printf("got: [%s]\n", seqg_var->s1[0]);
		}
		return;
	}
}

/****** Code for state "pause" in state set "get1" ******/

/* Event function for state "pause" in state set "get1" */
static seqBool seqg_event_get1_1_pause(SS_ID seqg_env, int *seqg_ptrn, int *seqg_pnst)
{
# line 44 "../syncq.st"
	if (seq_delay(seqg_env, 2))
	{
		*seqg_pnst = 0;
		*seqg_ptrn = 0;
		return TRUE;
	}
	return FALSE;
}

/* Action function for state "pause" in state set "get1" */
static void seqg_action_get1_1_pause(SS_ID seqg_env, int seqg_trn, int *seqg_pnst)
{
	switch(seqg_trn)
	{
	case 0:
		{
		}
		return;
	}
}

/****** Code for state "put" in state set "put" ******/

/* Event function for state "put" in state set "put" */
static seqBool seqg_event_put_2_put(SS_ID seqg_env, int *seqg_ptrn, int *seqg_pnst)
{
# line 51 "../syncq.st"
	if (seq_delay(seqg_env, 1))
	{
		*seqg_pnst = 0;
		*seqg_ptrn = 0;
		return TRUE;
	}
	return FALSE;
}

/* Action function for state "put" in state set "put" */
static void seqg_action_put_2_put(SS_ID seqg_env, int seqg_trn, int *seqg_pnst)
{
	switch(seqg_trn)
	{
	case 0:
		{
# line 52 "../syncq.st"
			sprintf(seqg_var->s[0], "%d", seqg_var->n);
# line 53 "../syncq.st"
			sprintf(seqg_var->s[1], "%d", -seqg_var->n);
# line 54 "../syncq.st"
			sprintf(seqg_var->s[2], "%d", 2 * seqg_var->n);
# line 55 "../syncq.st"
			seq_pvPutTmo(seqg_env, 0/*s*/, DEFAULT, DEFAULT_TIMEOUT);
# line 56 "../syncq.st"
			seqg_var->n++;
# line 57 "../syncq.st"
			seq_pvPutTmo(seqg_env, 2/*n*/, DEFAULT, DEFAULT_TIMEOUT);
		}
		return;
	}
}

/****** Code for state "idle" in state set "flush" ******/

/* Event function for state "idle" in state set "flush" */
static seqBool seqg_event_flush_3_idle(SS_ID seqg_env, int *seqg_ptrn, int *seqg_pnst)
{
# line 64 "../syncq.st"
	if (seqg_var->n % 20 == 0)
	{
		*seqg_pnst = 1;
		*seqg_ptrn = 0;
		return TRUE;
	}
	return FALSE;
}

/* Action function for state "idle" in state set "flush" */
static void seqg_action_flush_3_idle(SS_ID seqg_env, int seqg_trn, int *seqg_pnst)
{
	switch(seqg_trn)
	{
	case 0:
		{
# line 65 "../syncq.st"
			printf("flush\n");
# line 66 "../syncq.st"
			seq_pvFlushQ(seqg_env, 0/*s*/);
# line 67 "../syncq.st"
			seq_pvFlushQ(seqg_env, 1/*s1*/);
		}
		return;
	}
}

/****** Code for state "flush" in state set "flush" ******/

/* Event function for state "flush" in state set "flush" */
static seqBool seqg_event_flush_3_flush(SS_ID seqg_env, int *seqg_ptrn, int *seqg_pnst)
{
# line 71 "../syncq.st"
	if (seqg_var->n % 20 != 0)
	{
		*seqg_pnst = 0;
		*seqg_ptrn = 0;
		return TRUE;
	}
	return FALSE;
}

/* Action function for state "flush" in state set "flush" */
static void seqg_action_flush_3_flush(SS_ID seqg_env, int seqg_trn, int *seqg_pnst)
{
	switch(seqg_trn)
	{
	case 0:
		{
		}
		return;
	}
}

#undef seqg_var

/************************ Tables ************************/

/* Channel table */
static seqChan seqg_chans[] = {
	/* chName, offset, varName, varType, count, eventNum, efId, monitored, queueSize, queueIndex */
	{"syncq", offsetof(struct seqg_vars, s), "s", P_STRING, 3, 1, 0, 1, 5, 0},
	{"syncq", offsetof(struct seqg_vars, s1), "s1", P_STRING, 1, 2, 0, 1, 5, 1},
	{"", offsetof(struct seqg_vars, n), "n", P_INT, 1, 3, 0, 1, 0, 0},
};

/* Event masks for state set "get" */
static const seqMask seqg_mask_get_0_get[] = {
	0x00000002,
};
static const seqMask seqg_mask_get_0_pause[] = {
	0x00000000,
};

/* State table for state set "get" */
static seqState seqg_states_get[] = {
	{
	/* state name */        "get",
	/* action function */   seqg_action_get_0_get,
	/* event function */    seqg_event_get_0_get,
	/* entry function */    0,
	/* exit function */     0,
	/* event mask array */  seqg_mask_get_0_get,
	/* state options */     (0)
	},
	{
	/* state name */        "pause",
	/* action function */   seqg_action_get_0_pause,
	/* event function */    seqg_event_get_0_pause,
	/* entry function */    0,
	/* exit function */     0,
	/* event mask array */  seqg_mask_get_0_pause,
	/* state options */     (0)
	},
};

/* Event masks for state set "get1" */
static const seqMask seqg_mask_get1_1_get[] = {
	0x00000004,
};
static const seqMask seqg_mask_get1_1_pause[] = {
	0x00000000,
};

/* State table for state set "get1" */
static seqState seqg_states_get1[] = {
	{
	/* state name */        "get",
	/* action function */   seqg_action_get1_1_get,
	/* event function */    seqg_event_get1_1_get,
	/* entry function */    0,
	/* exit function */     0,
	/* event mask array */  seqg_mask_get1_1_get,
	/* state options */     (0)
	},
	{
	/* state name */        "pause",
	/* action function */   seqg_action_get1_1_pause,
	/* event function */    seqg_event_get1_1_pause,
	/* entry function */    0,
	/* exit function */     0,
	/* event mask array */  seqg_mask_get1_1_pause,
	/* state options */     (0)
	},
};

/* Event masks for state set "put" */
static const seqMask seqg_mask_put_2_put[] = {
	0x00000000,
};

/* State table for state set "put" */
static seqState seqg_states_put[] = {
	{
	/* state name */        "put",
	/* action function */   seqg_action_put_2_put,
	/* event function */    seqg_event_put_2_put,
	/* entry function */    0,
	/* exit function */     0,
	/* event mask array */  seqg_mask_put_2_put,
	/* state options */     (0)
	},
};

/* Event masks for state set "flush" */
static const seqMask seqg_mask_flush_3_idle[] = {
	0x00000008,
};
static const seqMask seqg_mask_flush_3_flush[] = {
	0x00000008,
};

/* State table for state set "flush" */
static seqState seqg_states_flush[] = {
	{
	/* state name */        "idle",
	/* action function */   seqg_action_flush_3_idle,
	/* event function */    seqg_event_flush_3_idle,
	/* entry function */    0,
	/* exit function */     0,
	/* event mask array */  seqg_mask_flush_3_idle,
	/* state options */     (0)
	},
	{
	/* state name */        "flush",
	/* action function */   seqg_action_flush_3_flush,
	/* event function */    seqg_event_flush_3_flush,
	/* entry function */    0,
	/* exit function */     0,
	/* event mask array */  seqg_mask_flush_3_flush,
	/* state options */     (0)
	},
};

/* State set table */
static seqSS seqg_statesets[] = {
	{
	/* state set name */    "get",
	/* states */            seqg_states_get,
	/* number of states */  2
	},

	{
	/* state set name */    "get1",
	/* states */            seqg_states_get1,
	/* number of states */  2
	},

	{
	/* state set name */    "put",
	/* states */            seqg_states_put,
	/* number of states */  1
	},

	{
	/* state set name */    "flush",
	/* states */            seqg_states_flush,
	/* number of states */  2
	},
};

/* Program table (global) */
seqProgram syncqTest = {
	/* magic number */      2002008,
	/* program name */      "syncqTest",
	/* channels */          seqg_chans,
	/* num. channels */     3,
	/* state sets */        seqg_statesets,
	/* num. state sets */   4,
	/* user var size */     sizeof(struct seqg_vars),
	/* param */             "",
	/* num. event flags */  0,
	/* encoded options */   (0 | OPT_CONN | OPT_NEWEF | OPT_REENT | OPT_SAFE),
	/* init func */         seqg_init,
	/* entry func */        0,
	/* exit func */         0,
	/* num. queues */       2
};

#define PROG_NAME syncqTest
#include "seqMain.c"

/* Register sequencer commands and program */
#include "epicsExport.h"
static void syncqTestRegistrar (void) {
    seqRegisterSequencerCommands();
    seqRegisterSequencerProgram (&syncqTest);
}
epicsExportRegistrar(syncqTestRegistrar);