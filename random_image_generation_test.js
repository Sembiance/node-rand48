"use strict";

if(!Math.randomInt)
	Math.randomInt = (min, max) => Math.floor(Math.random() * (max - min + 1)) + min;

const Rand48 = require("./build/default/rand48").Rand48;
const fs = require("fs");
const width = 1000;
const height = 1000;
const pixelCount = (width*height);
const maxWhiteVal = 10;

function generateVals(algo)
{
	const r = new Rand48();
	const vals = [];
	for(let i=0;i<pixelCount;i++)
		vals.push(0);
	
	for(let i=0;i<1000000;i++)
	{
		const num = algo==="rand48" ? r.rand(0, pixelCount) : Math.randomInt(0, pixelCount);
		vals[num]++;
		vals[num] = Math.min(vals[num], maxWhiteVal);
	}
	
	return vals;
}

function writeValsToFile(filename, vals)
{
	let data = "P2\n";
	data += "# random test\n";
	data += width + " " + height + "\n";
	data += maxWhiteVal + "\n";
	for(let y=0;y<height;y++)
	{
		let line = "";
		for(let x=0;x<width;x++)
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

for(let i=0;i<5;i++)
	writeValsToFile("rand48-" + i + ".pbm", generateVals("rand48"));

for(let i=0;i<10;i++)
	writeValsToFile("Math.random-" + i + ".pbm", generateVals());
