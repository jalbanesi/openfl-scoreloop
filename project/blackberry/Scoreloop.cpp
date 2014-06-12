#include "Utils.h"


namespace scoreloop 
{
	static const char* const SCORE_SUBMITTED = "scoreloop_SCORE_SUBMITTED";
	static const char* const LEADERBOARD_LOADED = "scoreloop_LEADERBOARD_LOADED";

	
	SC_Error_t errCode;
	SC_Client_h client;
	SC_InitData_t initData;

    SC_Score_h score;
    SC_UserController_h userController;
    SC_ScoreController_h scoreController;
    SC_ScoresController_h scoresController;	

	
	void HandleEvent (int data) 
	{
		bps_event_t *event = (bps_event_t *)data;
		
		if (bps_event_get_domain(event) == SC_GetBPSEventDomain (&initData)) {
			
			SC_HandleBPSEvent (&initData, event);		
		}
	}
	
	
	bool Initialize (const char* gameIdentifier, const char *gameSecret, const char *gameVersion, const char *currency, const char *languages) 
	{	
		SC_InitData_Init (&initData);
		SC_Error_t errCode = SC_Client_New(&client, &initData, gameIdentifier, gameSecret, gameVersion, currency, languages);
		return (errCode == 0);	
	}
	
	
	bool SubmitScore(int scoreResult, int mode)
	{
	    SC_Error_t rc = SC_Client_CreateScoreController(client, &scoreController, SubmitScoreCompletionCallback, &client);
		if (rc != SC_OK) 
		{
			return false;
		}

		/* Create and configure the score */
		rc = SC_Client_CreateScore(client, &score);
		if (rc != SC_OK) 
		{
			SC_ScoreController_Release(scoreController); /* Cleanup Controller */
			return false;
		}
		SC_Score_SetResult(score, scoreResult);
		SC_Score_SetMode(score, mode);

		/* Submit the score */
		rc = SC_ScoreController_SubmitScore(scoreController, score);
		if (rc != SC_OK) 
		{
			SC_ScoreController_Release(scoreController); /* Cleanup Controller */
			SC_Score_Release(score); /* Cleanup Score */
			return false;
		}
		
		printf("Submitting score\n");
		
		return true;
	}	

	void SubmitScoreCompletionCallback(void *cookie, SC_Error_t completionStatus)
	{
		/* Check completion status */
		if (completionStatus != SC_OK) 
		{
			SC_ScoreController_Release(scoreController); /* Cleanup Controller */
			SC_Score_Release(score); /* Cleanup Score */
			return;
		}
		printf("Done submitting Score\n");

		/* Cleanup Controller */
		SC_ScoreController_Release(scoreController);

		/* Request the leaderboard here just for demonstration purposes */
		//LoadLeaderboard(app, score, SC_SCORES_SEARCH_LIST_ALL, 15);

		/* Cleanup Score */
		SC_Score_Release(score);
		
		//scoreloop_dispatch_event( SCORE_SUBMITTED , "" );
		scoreloop_dispatch_event(SCORE_SUBMITTED, "" );
	}	

	void LoadLeaderboard(const SC_ScoresSearchList_t searchList, unsigned int count)
	{
		/* Create a ScoresController */
		SC_Error_t rc = SC_Client_CreateScoresController(client, &scoresController, LoadLeaderboardCompletionCallback, &client);
		if (rc != SC_OK) 
		{
			return;
		}

		/* Configure the Controller */
		SC_ScoresController_SetMode(scoresController, SC_Score_GetMode(score));
		rc = SC_ScoresController_SetSearchList(scoresController, searchList);
		if (rc != SC_OK) 
		{
			SC_ScoresController_Release(scoresController); /* Cleanup Controller */
			return;
		}

		/* Load the Leaderboard for the given score and count */
		rc = SC_ScoresController_LoadScoresAroundScore(scoresController, score, count);
		if (rc != SC_OK) 
		{
			SC_ScoresController_Release(scoresController); /* Cleanup Controller */
			return;
		}
		//LOG("Loading Leaderboard...");
	}

	void LoadLeaderboardCompletionCallback(void *cookie, SC_Error_t completionStatus)
	{
		/* Check completion status */
		if (completionStatus != SC_OK) 
		{
			SC_ScoresController_Release(scoresController); /* Cleanup Controller */
			return;
		}

		/* Just log the scores here for demonstration purposes */
		SC_ScoreList_h scoreList = SC_ScoresController_GetScores(scoresController);
		if (scoreList == NULL) 
		{
			SC_ScoresController_Release(scoresController); /* Cleanup Controller */
			return;
		}
		printf("Done loading Leaderboard\n");

		/* Get the score formatter here - remember that you need to add a
		* scoreloop/SLScoreFormatter.strings file to your asset files in order to retrieve a formatter.
		*/
		/*SC_ScoreFormatter_h scoreFormatter = SC_Client_GetScoreFormatter(app->client);
		if (!scoreFormatter) 
		{
			SC_ScoresController_Release(app->scoresController); // Cleanup Controller 
			return;
		}*/

		std::string leaderboard = "";
		std::stringstream s;
		
		unsigned int i, numScores = SC_ScoreList_GetCount(scoreList);
		
		printf("numScores: %d\n", numScores);
		for (i = 0; i < numScores; ++i) 
		{
			SC_Score_h score = SC_ScoreList_GetAt(scoreList, i);
			SC_User_h user = SC_Score_GetUser(score);
			SC_String_h login = user ? SC_User_GetLogin(user) : NULL;
			int rank = SC_Score_GetRank(score);
			int result = SC_Score_GetResult(score);
			const char* loginName = login ? SC_String_GetData(login) : "<unknown>";

			using namespace std;
			
			
			s << rank << "," << loginName << "," << result << "|";
				//leaderboard = string(rank) + string(",") + string(loginName) + string(",") + string(result) + string("|");
			
			/*
			leaderboard += rank;
			leaderboard += ",";
			leaderboard += loginName;
			leaderboard += ",";
			leaderboard += result;
			leaderboard += "|";
			*/
			/*SC_String_h formattedScore;
			
			// Format the score - we take ownership of string
			SC_Error_t rc = SC_ScoreFormatter_FormatScore(scoreFormatter, score, SC_SCORE_FORMAT_DEFAULT, &formattedScore);
			if (rc != SC_OK) 
			{
				return;
			}
			LOG(" Rank: %d, Result: %s, User: %s", SC_Score_GetRank(score), SC_String_GetData(formattedScore),
					login ? SC_String_GetData(login) : "<unknown>");

			// Release the string 
			SC_String_Release(formattedScore);*/
		}

		/* Cleanup Controller */
		SC_ScoresController_Release(scoresController);
		
		leaderboard = s.str();
		printf("leaderboard string: %s\n", leaderboard.c_str());

		scoreloop_dispatch_event(LEADERBOARD_LOADED, leaderboard.c_str());
		
		/* Set an Award as achieved here just for demonstration purposes */
		//AchieveAward(app, AN_AWARD_ID);
	}

		
	
}