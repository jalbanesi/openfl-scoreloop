package ilmare.extension;
import cpp.rtti.FieldNumericIntegerLookup;
import openfl.events.EventDispatcher;
import openfl.events.Event;

#if cpp
import cpp.Lib;
import openfl.events.SystemEvent;
#elseif neko
import neko.Lib;
#end

#if (android && openfl)
import openfl.utils.JNI;
#end


class Scoreloop extends EventDispatcher
{
	public static inline var LEADERBOARD_TYPE_ALL: String = "all";
	public static inline var LEADERBOARD_TYPE_24H: String = "24h";
	public static inline var LEADERBOARD_TYPE_COUNTRY: String = "country";
	
	
	private static var _bInitialized : Bool;
	
	public static var instance(get, never): Scoreloop;
	
	static var _instance: Scoreloop;
	static function get_instance(): Scoreloop
	{
		if (_instance == null)
		{
			_instance = new Scoreloop();
		}
		return _instance;
	}
	
	function new() // setCallback
	{
		super();
		scoreloop_set_event_callback( _onEvent_from_java );
	}
	
	function _onEvent_from_java( sEvent : String , sArg : String ) : Void // onConnectFromCpp
	{
		_emit( sEvent , sArg );
	}

	function _emit( sEvent_type : String , sArg : String = null ) : Void
	{
		dispatchEvent(new ScoreloopEvent(sEvent_type, sArg));
	}

	
	public function init(gameIdentifier:String, gameSecret:String, gameVersion:String, currency:String, languages:String):Bool 
	{
		var result = scoreloop_initialize (gameIdentifier, gameSecret, gameVersion, currency, languages);
		
		if (result) 
		{	
			openfl.Lib.current.stage.addEventListener (SystemEvent.SYSTEM, stage_onSystemEvent);	
		}
		
		return result;		
	}
	
	public function submitScore(score: Int, mode: Int): Bool
	{
		var result = scoreloop_submit_score(score, mode);
		
		return result;		
	}
	
	public function loadLeaderboards(type: String, count: Int = 10): Void
	{
		scoreloop_load_leaderboard(type, count);
	}
	
	// Event Handlers
	
	private static function stage_onSystemEvent(event:SystemEvent):Void 
	{
		scoreloop_process_event(event.data);
	}
	
	
	
	
	// Native Methods
	private static var scoreloop_initialize = Lib.load ("scoreloop", "scoreloop_initialize", 5);
	private static var scoreloop_submit_score = Lib.load ("scoreloop", "scoreloop_submit_score", 2);
	private static var scoreloop_load_leaderboard = Lib.load ("scoreloop", "scoreloop_load_leaderboard", 2);
	
	
	private static var scoreloop_process_event = Lib.load ("scoreloop", "scoreloop_process_event", 1);
	private static var scoreloop_set_event_callback = Lib.load ("scoreloop", "scoreloop_set_event_callback", 1); // cpp_cb_connect
	
	
}