#ifndef STATIC_LINK
#define IMPLEMENT_API
#endif

#if defined(HX_WINDOWS) || defined(HX_MACOS) || defined(HX_LINUX)
#define NEKO_COMPATIBLE
#endif


#include "Utils.h"


using namespace scoreloop;


AutoGCRoot *eval_scoreloop_onEvent = 0;
extern "C" void scoreloop_dispatch_event( const char *sEvent , const char *sArg )
{
	val_call2(
		eval_scoreloop_onEvent->get( ) ,
		alloc_string( sEvent ) ,
		alloc_string( sArg )
	);
}		


static value scoreloop_initialize (value gameIdentifier, value gameSecret, value gameVersion, value currency, value languages) 
{
	bool result = Initialize (val_string (gameIdentifier), val_string (gameSecret), val_string (gameVersion), val_string (currency), val_string (languages));
	return alloc_bool (result);
}
DEFINE_PRIM (scoreloop_initialize, 5);


static value scoreloop_submit_score(value score, value mode) 
{
	bool result = SubmitScore(val_int(score), val_int(mode));
	
	return alloc_bool(result);
}
DEFINE_PRIM (scoreloop_submit_score, 2);


static value scoreloop_load_leaderboard(value type, value count) 
{
	bool result = true;
	
	if (strcmp(val_string(type), "all") == 0)
		LoadLeaderboard(SC_SCORES_SEARCH_LIST_ALL, val_int(count));
	else if (strcmp(val_string(type), "24h") == 0)
		LoadLeaderboard(SC_SCORES_SEARCH_LIST_24H, val_int(count));
	else if (strcmp(val_string(type), "country") == 0)
		LoadLeaderboard(SC_SCORES_SEARCH_LIST_USER_COUNTRY, val_int(count));
	
	return alloc_bool(result);
}
DEFINE_PRIM (scoreloop_load_leaderboard, 2);



// Callback
static value scoreloop_set_event_callback(value onCall)
{
	eval_scoreloop_onEvent = new AutoGCRoot( onCall );
	return alloc_bool( true );
}
DEFINE_PRIM( scoreloop_set_event_callback , 1 );


static void scoreloop_process_event (value event) 
{
	HandleEvent (val_int (event));
	
}
DEFINE_PRIM (scoreloop_process_event, 1);



extern "C" void scoreloop_main () 
{
	
	val_int(0); // Fix Neko init
	
}
DEFINE_ENTRY_POINT (scoreloop_main);



extern "C" int scoreloop_register_prims () 
{ 
	return 0; 
}

