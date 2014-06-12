openfl-scoreloop
================

Basic Scoreloop Haxe/OpenFL native extension

Blackberry only

Install:

<code>haxelib git https://github.com/jalbanesi/openfl-scoreloop.git scoreloop</code>

application.xml

<code>
\<haxelib name="scoreloop" if="blackberry" /\>
</code>

Usage:

```Haxe
  // Init
  Scoreloop.instance.init("game-id", "game-secret", "version", "currency", "language");

  // Submit score
  Scoreloop.instance.addEventListener(ScoreloopEvent.SCORE_SUBMITTED, onScoreSubmitted);
  Scoreloop.instance.submitScore(score, mode);

  // Load leaderboards
  Scoreloop.instance.loadLeaderboards(Scoreloop.LEADERBOARD_TYPE_ALL);
  Scoreloop.instance.addEventListener(ScoreloopEvent.LEADERBOARD_LOADED, onLeaderboardLoaded);


	function onLeaderboardLoaded(evt: ScoreloopEvent): Void
	{
		for (user in evt.leaderboard)
		{
			trace(user.rank + ", " + user.login + " ---> " + user.result);
		}
	}

```
