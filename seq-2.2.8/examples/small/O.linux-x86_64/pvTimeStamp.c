/* C code for program pvTimeStampTest, generated by snc from ../pvTimeStamp.st */
#include <string.h>
#include <stddef.h>
#include <stdio.h>
#include <limits.h>

#include "seq_snc.h"

/* Variable declarations */
# line 3 "../pvTimeStamp.st"
static	int test;


/* Function declarations */

#define seqg_var (*(struct seqg_vars *const *)seqg_env)

/* Program init func */
static void seqg_init(PROG_ID seqg_env)
{
	{
# line 3 "../pvTimeStamp.st"
	static int seqg_initvar_test = 0;
	memcpy(&test, &seqg_initvar_test, sizeof(seqg_initvar_test));
	}
}

/****** Code for state "one" in state set "one" ******/

/* Event function for state "one" in state set "one" */
static seqBool seqg_event_one_0_one(SS_ID seqg_env, int *seqg_ptrn, int *seqg_pnst)
{
# line 9 "../pvTimeStamp.st"
	if (seq_delay(seqg_env, 1))
	{
		*seqg_pnst = 0;
		*seqg_ptrn = 0;
		return TRUE;
	}
	return FALSE;
}

/* Action function for state "one" in state set "one" */
static void seqg_action_one_0_one(SS_ID seqg_env, int seqg_trn, int *seqg_pnst)
{
	switch(seqg_trn)
	{
	case 0:
		{
# line 10 "../pvTimeStamp.st"
			printf("sec=%d\n", seq_pvTimeStamp(seqg_env, 0/*test*/).secPastEpoch);
# line 11 "../pvTimeStamp.st"
			test++;
# line 12 "../pvTimeStamp.st"
			seq_pvPutTmo(seqg_env, 0/*test*/, SYNC, DEFAULT_TIMEOUT);
		}
		return;
	}
}

#undef seqg_var

/************************ Tables ************************/

/* Channel table */
static seqChan seqg_chans[] = {
	/* chName, offset, varName, varType, count, eventNum, efId, monitored, queueSize, queueIndex */
	{"pvTimeStampTest", (size_t)&test, "test", P_INT, 1, 1, 0, 1, 0, 0},
};

/* Event masks for state set "one" */
static const seqMask seqg_mask_one_0_one[] = {
	0x00000000,
};

/* State table for state set "one" */
static seqState seqg_states_one[] = {
	{
	/* state name */        "one",
	/* action function */   seqg_action_one_0_one,
	/* event function */    seqg_event_one_0_one,
	/* entry function */    0,
	/* exit function */     0,
	/* event mask array */  seqg_mask_one_0_one,
	/* state options */     (0)
	},
};

/* State set table */
static seqSS seqg_statesets[] = {
	{
	/* state set name */    "one",
	/* states */            seqg_states_one,
	/* number of states */  1
	},
};

/* Program table (global) */
seqProgram pvTimeStampTest = {
	/* magic number */      2002008,
	/* program name */      "pvTimeStampTest",
	/* channels */          seqg_chans,
	/* num. channels */     1,
	/* state sets */        seqg_statesets,
	/* num. state sets */   1,
	/* user var size */     0,
	/* param */             "",
	/* num. event flags */  0,
	/* encoded options */   (0 | OPT_CONN | OPT_NEWEF),
	/* init func */         seqg_init,
	/* entry func */        0,
	/* exit func */         0,
	/* num. queues */       0
};

#define PROG_NAME pvTimeStampTest
#include "seqMain.c"

/* Register sequencer commands and program */
#include "epicsExport.h"
static void pvTimeStampTestRegistrar (void) {
    seqRegisterSequencerCommands();
    seqRegisterSequencerProgram (&pvTimeStampTest);
}
epicsExportRegistrar(pvTimeStampTestRegistrar);
