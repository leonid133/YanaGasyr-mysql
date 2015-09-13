#pragma once

#include "stdafx.h"

#include <string>
#include "pars.h"
#include <ctime>
#include <iostream>
#include <fstream>

void Parser::parseStr( std::string arg )
{
	int parscounter = 0;
	
	n = "";
	DateTime = "";
	Date1 = "";
	time1 = "";
	type = "";
	locate = "";
	filname = "";
	time2 = "";
	more1 = "";
	more2 = " ";
    success = false;
	
	for ( int i= 0, i_end = arg.length()-1; i < i_end; ++i )
	{
		if(arg[i] == NULL || arg[i] == 343 || arg[i] == 344 || arg[i] == 28 || arg[i] == '\0' || arg[i] == char(0))
			continue;
		if(arg[i] == 9)
		{	parscounter++; i++;}
		switch(parscounter)	
		{
			case 0:
				n +=arg[i];
			break;
				
			case 1:
				if(arg[i]==32) parscounter++;
				else
					Date1 += arg[i];
			break;

			case 2:
				time1 += arg[i];
			break;
			
			case 3:
				more0 += arg[i];
			break;
			
			case 4:
				type += arg[i];
			break;

			case 5:
				
                if( arg[i] == 92 )
                    this->locate += "\\\\";
                else if( arg[i] == 34 )
                    this->locate += "\\\"";
                else
                    this->locate += arg[i];
				if( arg[i] == 92 )
				    {filname = "";}
				filname += arg[i];
			break;

			case 6:
				time2 += arg[i];
			break;

			case 7:
				
                if(arg[i] == 9)
				{parscounter++;}
                else if( arg[i] == 34 )
                    more1 += "\\\"";
				else more1 += arg[i];
			break;

			case 8:
				if( arg[i] == 34 )
                    more2 += "\\\"";
                else	
                    more2 += arg[i];
			break;
			
			default:;
		}
	}
	/*
	if(more2[more2.length()] == '0x1c')
	{
		more2= " ";
	}
		*/
	/*
	time_t rawtime = {0};
	struct tm * timeinfo ;
	time ( &rawtime );                             
	timeinfo = localtime ( &rawtime ); 
	*/							
	/*
	long long day, mon, year, hour, min, sec;
	sscanf( Date1.c_str(),"%2d.%2d.%4d", &day, &mon, &year );
	//sscanf( time1.c_str(),"%2d:%2d:%2d", &hour, &min, &sec );
				
	std::string date_time;
	date_time = std::to_string( year );
	date_time += "-";
	date_time += std::to_string( mon );
	date_time += "-";
	date_time += std::to_string( day );
	date_time += " ";
	date_time += time1;
				
	DateTime = date_time;*/
	std::string day = "";
	std::string mon = "";
	std::string year = "";
	parscounter = 0;
	for ( int i= 0, i_end = Date1.length(); i < i_end; ++i )
	{
		if(Date1[i] == 46)
		{	parscounter++; i++; }
		switch( parscounter )	
		{
			case 0:
				day += Date1[i];
			break;
			case 1:
				mon += Date1[i];
			break;
			case 2:
				year += Date1[i];
			break;
		}
	}
    if( parscounter == 2 )
    {
	    DateTime = year;
	    DateTime += "-";
	    DateTime += mon;
	    DateTime += "-";
	    DateTime += day;
	    DateTime += " ";
	    DateTime += time1;
        success = true;
    }
    std::string correct_filename = "";
    for ( int i= 0, i_end = filname.length(); i < i_end; ++i )
	{
        if( filname[i] != 92 )
        correct_filename += filname[i];
    }
 }; 


bool ParseIni( std::string &result, const std::string patch_file_name, const std::string find_arg )
{
    bool find_done = false;
    std::ifstream fin(patch_file_name); 
	if(!fin)
	{
		std::cerr << "Error" << std::endl;
	}
	else
	{    
        std::string word;
        while(fin >> word)
        {
            if(word == find_arg)
            {
                //std::cout << word ;
                fin >> word;
               // std::cout << word ;
                fin >> word;
                //std::cout << word << std::endl;
                result = word;
                find_done = true;
            }
        }
    }
 	fin.close(); 
    return find_done;
};

std::string ParseIni( const std::string patch_file_name, const std::string find_arg )
{
    std::string result;
    std::ifstream fin(patch_file_name); 
	if(!fin)
	{
		std::cerr << "Error" << std::endl;
	}
	else
	{    
        std::string word;
        while(fin >> word)
        {
            if(word == find_arg)
            {
                //std::cout << word ;
                fin >> word;
                //std::cout << word ;
                fin >> result;
                //std::cout << result << std::endl;
            }
        }
    }
 	fin.close(); 
    return result;
};


