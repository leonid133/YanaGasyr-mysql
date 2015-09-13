#pragma once
//#include "stdafx.h"

class Parser
{
	public:
		void parseStr( std::string arg );
		std::string n;
		std::string DateTime;
		std::string Date1;
		std::string time1;
		std::string more0;
		std::string type;
		std::string locate;
		std::string filname;
		std::string time2;
		std::string more1;
		std::string more2;
        bool success;

}; 

bool ParseIni(std::string  &result, const std::string patch_file_name, const std::string find_arg );
std::string ParseIni( const std::string patch_file_name, const std::string find_arg );
/*
public class OutputLines
{
	public:
		std::string StartTime;
		std::string StopTime;
		std::string TrackName;
		std::string Long;
		std::string Efir;
};*/