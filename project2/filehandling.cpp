#include "filehandling.h"
#include <regex>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <iostream>

using namespace std::tr1;


board stringToBoard(std::string fileInfo){
	int err;

	regex reName("Name[^;]+");
	regex reQuotes("\"[^\"]+\"");
	regex reTerrain("Terrain[^}]+\\}");
	regex reWindow("Window[^}]+\\}");

	regex reChars("Chars[^}]+\\}");
	regex reColors("Colors[^}]+\\}");
	regex reInitial("Initial[^}]+\\}");
	regex reXrange("Xrange[^;]+\\;");
	regex reYrange("Yrange[^;]+\\;");
	regex reLowHigh("-?[0-9]+\\.\\.[0-9]+");
	regex reAlive("Alive[^;]+;");
	regex reDead("Dead[^;]+;");
	regex reParens("\\([^\\)]+\\)");
	fileInfo = compress(fileInfo);

	smatch container;
	
	//Handles retrieval of name from the string containing the file
	std::string name = "";
	regex_search(fileInfo, container, reName);
	if(container.size() == 1){
		regex_search(container[0].str(), container, reQuotes);
		if(container.size() == 1){
			name = trimQuotes(container[0].str());
		}
	}

	//Handles retrieval of Terrain information from the string containing the file
	regex_search(fileInfo, container, reTerrain);
	if(container.size() != 1){
		cerr << "Error with the terrain portion" << '\n';
		throw -1;
	}
	smatch xrange;
	regex_search(container[0].str(), xrange, reXrange);
	if(xrange.size() != 1){
		cerr << "Error with the Xrange portion of terrain" << '\n';
		throw -1;
	}
	regex_search(xrange[0].str(), xrange, reLowHigh);
	if(xrange.size() != 1){
		cerr << "Error with the Xrange portion of terrain" << '\n';
		throw -1;
	}
	std::string x = xrange.str();

	smatch yrange;
	regex_search(container[0].str(), yrange, reYrange);
	if(xrange.size() != 1){
		cerr << "Error with the Yrange portion of terrain" << '\n';
		throw -1;
	}
	regex_search(yrange[0].str(), yrange, reLowHigh);
	if(xrange.size() != 1){
		cerr << "Error with the Yrange portion of terrain" << '\n';
		throw -1;
	}
	std::string y = yrange.str();

	int xlow;
	int xhigh;
	int ylow; 
	int yhigh;
	if((sscanf(x.c_str(), "%d..%d", &xlow, &xhigh)) != 2){
		cerr << "Error reading range of x values" << '\n';
		throw -1;
	}
	if((sscanf(y.c_str(), "%d..%d", &ylow, &yhigh)) != 2){
		cerr << "Error reading range of y values" << '\n';
		throw -1;
	}

	//Handles retrieval of Chars information from the string containing the file
	regex_search(fileInfo, container, reChars);
	if(container.size() != 1){
		cerr << "Error with the Chars portion" << '\n';
		throw -1;
	}
	int alive = 64, dead = 126;

	char * aliveLoc = strstr(container.str().c_str(), "Alive=");
	if(aliveLoc){
		if((err = sscanf(aliveLoc, "Alive=%d", &alive)) != 1){
			cerr << "Error reading Alive value from Chars portion, default will be used" << '\n';
			alive = 64;
		}
	}else{
		cerr << "Could not find location of Alive value, default will be used" << '\n';
		alive = 64;
	}
	char * deadLoc = strstr(container.str().c_str(), "Dead=");
	if(deadLoc){
		if((sscanf(deadLoc, "Dead=%d", &dead)) != 1){
			cerr << "Error reading Dead value from Chars portion. default will be used" << '\n';
			dead = 126;
		}
	}else{
		cerr << "Could not find location of Dead value, default will be used" << '\n';
		dead = 126;
	}

	//SETS UP BOARD WITH PROVIDED INFORMATION
	board gameBoard(name, xhigh, xlow, yhigh, ylow, (char) alive, (char) dead);

	//Handles retrieval of Window information from the string containing the file
	regex_search(fileInfo, container, reWindow);
	if(container.size() == 1){

		smatch xrange;
		regex_search(container[0].str(), xrange, reXrange);
		if(xrange.size() != 1){
			cerr << "Error with the Xrange portion of terrain" << '\n';
			throw -1;
		}
		regex_search(xrange[0].str(), xrange, reLowHigh);
		if(xrange.size() != 1){
			cerr << "Error with the Xrange portion of terrain" << '\n';
			throw -1;
		}
		std::string x = xrange.str();

		smatch yrange;
		regex_search(container[0].str(), yrange, reYrange);
		if(xrange.size() != 1){
			cerr << "Error with the Yrange portion of terrain" << '\n';
			throw -1;
		}
		regex_search(yrange[0].str(), yrange, reLowHigh);
		if(xrange.size() != 1){
			cerr << "Error with the Yrange portion of terrain" << '\n';
			throw -1;
		}
		std::string y = yrange.str();

		int xlow = gameBoard.getXMin();
		int xhigh = gameBoard.getXMax();
		int ylow = gameBoard.getYMin();
		int yhigh = gameBoard.getYMax();
		if((sscanf(x.c_str(), "%d..%d", &xlow, &xhigh)) != 2){
			cerr << "Error reading range of x values, default will be used" << '\n';
			xlow = gameBoard.getXMin();
			xhigh = gameBoard.getXMax();
		}
		if(xlow < gameBoard.getXMin()){
			cerr << "Window X-Low was out of range of the terrain, default will be used\n";
			xlow = gameBoard.getXMin();
		}
		if(xhigh > gameBoard.getXMax()){
			cerr << "Window X-High was out of range of the terrain, default will be used\n";
			xhigh = gameBoard.getXMax();
		}
		if((sscanf(y.c_str(), "%d..%d", &ylow, &yhigh)) != 2){
			cerr << "Error reading range of y values" << '\n';
			throw -1;
		}
		if(ylow < gameBoard.getYMin()){
			cerr << "Window Y-Low was out of range of the terrain, default will be used\n";
			ylow = gameBoard.getYMin();
		}
		if(yhigh > gameBoard.getYMax()){
			cerr << "Window Y-High was out of range of the terrain, default will be used\n";
			yhigh = gameBoard.getYMax();
		}

		gameBoard.setWinWidth(xhigh, xlow);
		gameBoard.setWinHeight(yhigh, ylow);
	}

	//Handles retrieval of Colors information from the string containing the file
	regex_search(fileInfo, container, reColors);
	if(container.size() != 1){
		cerr << "Error with the Colors portion" << '\n';
		throw -1;
	}
	smatch aliveMatch;
	smatch deadMatch;

	std::string aliveColor;
	std::string deadColor;
	//Alive color

	int r, g, b;
	regex_search(container[0].str(), aliveMatch, reAlive);
	if(aliveMatch.size() == 1){

		regex_search(aliveMatch[0].str(), aliveMatch, reParens);
		if(aliveMatch.size() != 1){ 
			cerr << "Error attempting to read Alive color value from Colors portion, default will be used" << '\n';
			gameBoard.setAliveColor(255, 255, 255);
		}
		aliveColor = aliveMatch.str();
		int r, b ,g;
		if((sscanf(aliveColor.c_str(), "(%d,%d,%d)", &r, &g, &b)) != 3){
			cerr << "RBG value for Alive in Colors portion was not valid, default will be used" << '\n';
			gameBoard.setAliveColor(255, 255, 255);
		}
		if(r < 0 || r > 255 || b < 0 || b > 255 || g < 0 || g > 255){
			cerr << "RBG value for Alive in Colors portion was not valid, default will be used" << '\n';
			gameBoard.setAliveColor(255, 255, 255);
		}else{
			gameBoard.setAliveColor(r, g, b);
		}
		
	}else{
		cout << "Alive portion not found, default color will be used\n";
		gameBoard.setAliveColor(255, 255, 255);
	}

	//Dead color
	regex_search(container[0].str(), deadMatch, reDead);
	if(deadMatch.size() == 1){
		regex_search(deadMatch[0].str(), deadMatch, reParens);
		if(deadMatch.size() != 1){
			cerr << "Error attempting to read Dead color value from Colors portion, default will be used" << '\n';
			gameBoard.setDeadColor(64 , 64, 64);
		}
		deadColor = deadMatch.str();
		if((sscanf(deadColor.c_str(), "(%d,%d,%d)", &r, &g, &b)) != 3){
			cerr << "RBG value for Dead in Colors portion was not valid, default will be used." << '\n';
			gameBoard.setDeadColor(64 , 64, 64);
		}
		else if(r < 0 || r > 255 || b < 0 || b > 255 || g < 0 || g > 255){
			cerr << "RBG value for Dead in Colors portion was not valid" << '\n';
			gameBoard.setDeadColor(64 , 64, 64);
		}else{
			gameBoard.setDeadColor(r, g, b);
		}
	}else{
		cerr << "Dead color not found default will be used\n";
		gameBoard.setDeadColor(0 , 0, 0);
	}

	//Handles retrieval of Initial information from the string containing the file
	regex_search(fileInfo, container, reInitial);
	if(container.size() != 1){
		cerr << "Error with Initial portion\n";
		throw -1;
	}
	std::string temp = container.str();
	regex_iterator<std::string::iterator> regit (temp.begin(), temp.end(), reParens);
	regex_iterator<std::string::iterator> end;

	while(regit != end){
		int xc;
		int yc;
		if((sscanf(regit->str().c_str(), "(%d, %d)", &xc, &yc)) != 2){
			cerr << "Error reading X, Y coordinate" << '\n';
			continue;
		}
		if(xc < xlow || xc > xhigh || yc < ylow || yc > yhigh){
			cerr << "Coordinate in " << regit->str() << " was not in range of the terrain." << '\n';
			continue;
		}
		gameBoard.setAlive(xc, yc);
		regit++;
	}
	return gameBoard;
}


std::string getStringFromFile(const char *filename){
	ifstream inf(filename);
	if(inf){
		std::string fileData;
		inf.seekg(0, ios::end);
		fileData.resize(inf.tellg());
		inf.seekg(0, ios::beg);
		inf.read(&fileData[0], fileData.size());
		inf.close();
		return fileData;
	}else{
		cerr << "Error reading file: " << filename << '\n';
		throw -1;
	}
}
std::string getStringFromStdIn(){
	try{
		std::string fileData = "";
		char c;
		int i = 0;
		cin.get(c);
		while(!cin.eof()){
			fileData += c;
			cin.get(c);
		}
		return fileData;
	}catch(int e){
		cerr << "Error getting string from stdin\n";
		throw -1;
	}
}
std::string trimQuotes(std::string toTrim){
	string finalString;
	finalString.resize(toTrim.size() - 2);
	int j = 0;
	for(int i = 0; i < toTrim.size(); i++){
		if('\"' != toTrim[i]){
			finalString[j] = toTrim[i];
			j++;
		}
	}
	return finalString;
}
std::string compress(std::string data){
	std::string noComm;
	noComm.resize(data.size());
	bool isComment = false;
	bool inQoutes = false;
	int j = 0;
	for(int i = 0; i < data.size(); i++){
		if('#' == data[i] && !inQoutes){
			isComment = true;
		}
		else if(isComment){
			if('\n' == data[i]){
				isComment = false;
			}
		}
		else{
			if((data[i] == ' ' || data[i] == '\t' || data[i] == '\n') && !inQoutes)
				continue;
			noComm[j] = data[i];
			j++;
			if(data[i] == '\"' && !inQoutes){
				inQoutes = true;
			}
			else if(data[i] == '\"' && inQoutes){
				inQoutes = false;
			}
		}
	}
	noComm[j] = '\0';
	return noComm;
}