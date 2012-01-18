if(!Math.randomInt)
{
	Math.randomInt = function(min, max)
	{
		return Math.floor(Math.random() * (max - min + 1)) + min;  
	};
}

var Rand48 = require("./build/default/rand48").Rand48;
var fs = require("fs");
var width = 1000;
var height = 1000;
var pixelCount = (width*height);
var maxWhiteVal = 10;

function generateVals(algo)
{
	var r = new Rand48();
	var vals = [];
	for(var i=0;i<pixelCount;i++) { vals.push(0); }
	
	for(var i=0;i<1000000;i++)
	{
		var num = algo==="rand48" ? r.rand(0, pixelCount) : Math.randomInt(0, pixelCount);
		vals[num]++;
		vals[num] = Math.min(vals[num], maxWhiteVal);
	}
	
	return vals;
}

function writeValsToFile(filename, vals)
{
	var data = "P2\n";
	data += "# random test\n";
	data += width + " " + height + "\n";
	data += maxWhiteVal + "\n";
	for(var y=0;y<height;y++)
	{
		var line = "";
		for(var x=0;x<width;x++)
		{
			if(x>0)
				line += " ";
			
			line += "" + vals[((y*width)+x)];
		}
		data += line + "\n";
	}

	data += "\n";
	fs.writeFileSync(filename, data);
}

for(var i=0;i<5;i++)
{
	writeValsToFile("rand48-" + i + ".pbm", generateVals("rand48"));
}

for(var i=0;i<10;i++)
{
	writeValsToFile("Math.random-" + i + ".pbm", generateVals());
}
