{
	"targets":
	[
		{
			"target_name": "rand48",
			"sources": [ "rand48.cc" ],
			"include_dirs" : [ "<!(node -e \"require('nan')\")" ]
		}
	]
}