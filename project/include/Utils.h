#ifndef SCORELOOP_H
#define SCORELOOP_H

#include <hx/CFFI.h>
#include <scoreloop/scoreloopcore.h>
#include <scoreloop/sc_init.h>
#include <vector>
#include <string>
#include <cstdio>
#include <sstream>

namespace scoreloop {
	
	
	void HandleEvent (int event);
	bool Initialize (const char* gameIdentifier, const char *gameSecret, const char *gameVersion, const char *currency, const char *languages);
	bool SubmitScore(int scoreResult, int mode);	
	void SubmitScoreCompletionCallback(void *cookie, SC_Error_t completionStatus);
	void LoadLeaderboard(const SC_ScoresSearchList_t searchList, unsigned int count);
	void LoadLeaderboardCompletionCallback(void *cookie, SC_Error_t completionStatus);
	
	extern "C" void scoreloop_dispatch_event( const char *sEvent , const char *sArg );
}


#endif