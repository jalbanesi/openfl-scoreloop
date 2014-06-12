package ilmare.extension;

import openfl.events.Event;

class ScoreloopEvent extends Event
{
	public static inline var SCORE_SUBMITTED = "scoreloop_SCORE_SUBMITTED";
	public static inline var LEADERBOARD_LOADED = "scoreloop_LEADERBOARD_LOADED";
	
	public var leaderboard(get, never): Array<ScoreloopLeaderboardItem>;
	var _leaderboard: Array<ScoreloopLeaderboardItem>;
	
	public function new(type : String, arg : String) 
	{
		super(type);
		
		parseArg(type, arg);
	}	
	
	function parseArg(type : String, arg : String) 
	{
		trace("parseArg: " + type + ", " + arg);
		switch(type)
		{
			case LEADERBOARD_LOADED:
				_leaderboard = new Array<ScoreloopLeaderboardItem>();
				var users: Array<String> = arg.split("|");
				trace("users: " + users.length);
				for (user in users)
				{
					var item: ScoreloopLeaderboardItem = new ScoreloopLeaderboardItem();
					var userArray: Array<String> = user.split(",");
					if (userArray.length >= 3)
					{
						item.rank = Std.parseInt(userArray[0]);
						item.result = Std.parseFloat(userArray[2]);
						item.login = userArray[1];
						_leaderboard.push(item);
					}
				}
		}
	}
	
	function get_leaderboard(): Array<ScoreloopLeaderboardItem>
	{
		return _leaderboard;
	}
}