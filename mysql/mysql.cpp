// mysql.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"

#include <string>

#include <iostream>
#include <stdio.h>
#include <vector>
#include <map>
#include <algorithm>
#include <fstream>

#include <clocale>
#include <algorithm>
#include <memory>

#include <ctime>
#include <sstream>

#include <mysql.h>
#include "pars.h"

#include <conio.h> 
#include <locale.h> 
#include <string> 
 
#define UN setlocale(LC_CTYPE, "RU_ru.utf8"); 
#define RU setlocale(LC_CTYPE, "Russian_Russia.1251"); 
#define EN setlocale(LC_CTYPE, "English_United States.866"); 
#define ALRU setlocale(LC_ALL, "rus"); 
#define RU1 setlocale(LC_ALL, "Russian");
#define UTF setlocale (LC_ALL, "ru_RU.UTF-8"); 

void ErrMsg(const std::string error_string)
{				
	std::string logfile = "error_log.txt";
	std::ofstream error_log_filestream(logfile, std::ios_base::app);
	if (!error_log_filestream.is_open()) // если файл не открыт
		std::cout << "Файл лога ошибки не может быть открыт!\n"; // сообщить об этом
	else
	{
		std::cout.rdbuf(error_log_filestream.rdbuf());
		time_t rawtime = {0};
		struct tm * timeinfo ;
		time ( &rawtime );                             
		timeinfo = localtime ( &rawtime ); 
		char time_buff[100];
		strftime (time_buff, 100,"%A %c", timeinfo);
		std::cout << time_buff << ": " << error_string << std::endl;
		
	}
    error_log_filestream.close();
}

void ReciveMsg(const std::string error_string)
{				
	std::string logfile = "recive_log.txt";
	std::ofstream recieve_log_filestream(logfile, std::ios_base::app);
	if (!recieve_log_filestream.is_open()) // если файл не открыт
		std::cout << "Файл лога действий не может быть открыт!\n"; // сообщить об этом
	else
	{
		std::cout.rdbuf(recieve_log_filestream.rdbuf());
		time_t rawtime = {0};
		struct tm * timeinfo ;
		time ( &rawtime );                             
		timeinfo = localtime ( &rawtime ); 
		char time_buff[100];
		strftime (time_buff, 100,"%A %c", timeinfo);
		std::cout << time_buff << ": " << error_string << std::endl;
	}
    recieve_log_filestream.close();
}

void ReadMySQLConnectionINI( std::string &host, std::string &user, 
    std::string &password, std::string &database, int &port,
    std::string &unix_socket, unsigned long &clientflag  )
{
    std::string patch_file_name = "";
	patch_file_name += "connection.txt";
    
		ParseIni( host, patch_file_name, "host" );
        ParseIni( user, patch_file_name, "user" );
        ParseIni( password, patch_file_name, "password" );
        ParseIni( database, patch_file_name, "database" );
        port = atoi( ParseIni( patch_file_name, "port" ).c_str() );// ParseIni( port, fin, "port" );
        ParseIni( unix_socket, patch_file_name, "unix_socket" );
        clientflag =  atoi( ParseIni( patch_file_name, "clientflag").c_str() );
		//std::cout << buff << std::endl; // напечатали эту строку
	
}

void ReadFilepatchINI(std::string &dpm_filepath, std::string &dpm_filename, std::string &aliase_patch )
{
    std::string patch_file_name = "";
	patch_file_name += "connection.txt";

    ParseIni( dpm_filepath, patch_file_name, "dpm_filepath" );
    ParseIni( dpm_filename, patch_file_name, "dpm_filename" );
    ParseIni( aliase_patch, patch_file_name, "aliase_patch" );

}

std::vector<std::pair<std::string, std::string> > ReadAliasesFrom( std::string aliase_patch )
{
    std::vector<std::pair<std::string, std::string> > result_aliases;
    aliase_patch += "Aliases.ini";
    ReciveMsg( aliase_patch );
    std::ifstream fin(aliase_patch); 
	if(!fin)
	{
		std::cerr << "Error" << std::endl;
	}
	else
	{    
        std::string word;
        while( fin >> word && !fin.eof() )
        {
            if(word == "[Aliases]")
            {
               while( fin >> word && !fin.eof() )
               {
                   if(word == "[AltAliases]")
                       break;
                   std::string alias_str;
                   alias_str = word;
                   ReciveMsg(alias_str);
                   std::pair<std::string, std::string> next_alliase;
                   bool aliase_separator_detector = false;
                   bool aliace_start = false;
                   for(auto it_alias_str = alias_str.begin(); it_alias_str!= alias_str.end(); ++it_alias_str)
                   {
                       if(*it_alias_str == '=')
                       {    aliace_start = true; continue;}
                       if(aliace_start)
                       {
                           if(*it_alias_str == '*' )
                           {    aliase_separator_detector = true; continue; }
                           if(aliase_separator_detector)
                           {
                               next_alliase.second += *it_alias_str; 
                               if( *it_alias_str == '\\')
                                   next_alliase.second += '\\';
                           }
                           else
                             next_alliase.first += *it_alias_str;
                       }
                   }
                   result_aliases.push_back(next_alliase);
               }
            }
        }
    }
 	fin.close(); 
    return result_aliases;
}

int _tmain(int argc, _TCHAR* argv[])
{
	RU
	UINT oldcodepage = GetConsoleOutputCP();
	SetConsoleOutputCP(1251);
	
	std::string dpm_filepath = "D:\\tnvdata\\";
    std::string aliase_patch = "D:\\tnvdata\\";
	std::string dpm_filename = "*.dpm";
		
	std::string host = "localhost";
    std::string user = "admin";
    std::string password = "admin";
    std::string database = "cpp_data";
    int port = 3306;
    std::string unix_socket = "0";
    unsigned long clientflag = 0;

    ReciveMsg("Читаем connection.txt");
    ReadMySQLConnectionINI( host, user, password, database, port, unix_socket, clientflag );
    ReciveMsg("Настройки подключения к БД host, user, password, database, port, unix_socket, clientflag ");
    ReciveMsg(host);
    ReciveMsg(user);
    ReciveMsg(password );
    ReciveMsg(database);
    char *ch_port = new char[10];
    itoa(port, ch_port, 10);
    ReciveMsg(ch_port);
    ReciveMsg(unix_socket);
    char *ch_clientflag = new char[10];
    itoa(clientflag, ch_clientflag, 10);
    ReciveMsg(ch_clientflag);
    ReadFilepatchINI( dpm_filepath, dpm_filename, aliase_patch );
    ReciveMsg("Настройки расположения файлов  dpm_filepath, dpm_filename, aliase_patch");
    ReciveMsg(dpm_filepath);
    ReciveMsg(dpm_filename);
    ReciveMsg(aliase_patch);

    	//setlocale(LC_ALL, "rus"); // корректное отображение Кириллицы
    char buff[500]; // буфер промежуточного хранения считываемого из файла текста
	
	ReciveMsg("Ищем файлы в директории");
    
    std::string patch_file_name = dpm_filepath;
	patch_file_name += dpm_filename;

    std::vector<std::string> dpm_file_vector;
    HANDLE hFind;
    WIN32_FIND_DATA FindFileData;

    if((hFind = FindFirstFile(patch_file_name.c_str(), &FindFileData)) != INVALID_HANDLE_VALUE){
        do{
    	    ReciveMsg( FindFileData.cFileName );
            dpm_file_vector.push_back( FindFileData.cFileName );
        }while(FindNextFile(hFind, &FindFileData));
        FindClose(hFind);
    }

          MYSQL *connection, mysql;
          MYSQL_RES *result;
          MYSQL_ROW row;
          int query_state;

          mysql_init(&mysql);
          //connection = mysql_real_connect(&mysql,"host","user",
          //                   "password","database",port,"unix_socket",clientflag);
          //connection = mysql_real_connect(&mysql,"localhost",
          //                   "admin","admin","cpp_data",3306,0,0);
          std::string connect_string;
          connect_string = "Подключаемся к БД \n";
          connect_string += "";
          ReciveMsg( connect_string );
          connection = mysql_real_connect( &mysql,host.c_str(),
            user.c_str(), password.c_str(), database.c_str(), port, unix_socket.c_str(), clientflag );

		  if (connection == NULL) 
		  {
			  ErrMsg( mysql_error(connection) );
			  return 1;
          }

		  std::string set_names_query = "SET NAMES 'cp1251';";
		  query_state = mysql_query(connection, set_names_query.c_str() );
		  if (query_state !=0) 
		  {
			ErrMsg( mysql_error(connection) );
		  }
		  
          std::vector<std::pair<std::string, std::string> > aliases_vector;
            ReciveMsg("читаем алиасы");
            aliases_vector = ReadAliasesFrom( aliase_patch );

		  ReciveMsg( "Update Aliaces" );
          for( auto it_aliases_vector = aliases_vector.begin(); it_aliases_vector != aliases_vector.end(); it_aliases_vector++ )
          {
            std::string sqluery;
            sqluery = " REPLACE INTO `cpp_data`.`aliases` (`aliace`, `patch`) \n VALUES (\"" + it_aliases_vector->first;
            sqluery += "\", \"" + it_aliases_vector->second;
            sqluery +="\" )"; 
            query_state = mysql_query(connection, sqluery.c_str() );
            // ReciveMsg( sqluery );
            /*CREATE TABLE `cpp_data`.`aliases` (
  `aliace` VARCHAR(255) NOT NULL COMMENT '',
  `patch` VARCHAR(255) NULL COMMENT '',
  PRIMARY KEY (`aliace`)  COMMENT '',
  UNIQUE INDEX `aliace_UNIQUE` (`aliace` ASC)  COMMENT '')
ENGINE = InnoDB
DEFAULT CHARACTER SET = utf8
COLLATE = utf8_bin;
*/
              if (query_state !=0) 
		      {
			    ErrMsg( mysql_error(connection) );
		      }
          }

    for(auto it_dpm_file_vector = dpm_file_vector.begin(); it_dpm_file_vector != dpm_file_vector.end(); ++it_dpm_file_vector )
    {
        std::string filename_date;
        std::string filename = *it_dpm_file_vector;
        for(auto it_filename = filename.begin(); it_filename != filename.end(); ++it_filename  )
        {
            if(*it_filename == char(.))
                break;
            filename_date += *it_filename;
        }
        std::string day = "";
	    std::string mon = "";
	    std::string year = "";
        parsfiledatecounter = 0;
        for(auto it_filename_date =filename_date.begin(); it_filename_date!= filename_date.end(); ++it_filename_date)
        {
		    if(*it_filename_date == 46)
		    {	parsfiledatecounter++; }
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
        std::string patch_file_name = dpm_filepath;
        patch_file_name += *it_dpm_file_vector;
        ReciveMsg("открываем файл для чтения");
        ReciveMsg(patch_file_name);
        std::ifstream fin(patch_file_name); // открыли файл для чтения
        Parser *parser1[10000];
        int count = 0;
        if(!fin)
        {
		        ErrMsg( "Файл не открылся" );
                ErrMsg( patch_file_name );
        }
        else
        {
	        while ( !fin.eof() )
	        {
		        fin.getline(buff, 1000);
		        parser1[count] = new Parser;
		        parser1[count++]->parseStr(buff);
	        }
        }
		
        fin.close(); // закрываем файл

        

		  for(int i=0; i<(count); i++)
		  {
			  
              std::string sqluery;
			  sqluery = " REPLACE INTO `cpp_data`.`dbm` (`n`, `DateTime`,`more0`, `type`,`locate`, `filename`, `time2`, `more1`, `more2`) \n VALUES (\"" + parser1[i]->n;
			  sqluery += "\", \"" + parser1[i]->DateTime;
			  sqluery +="\", \"" + parser1[i]->more0;
			  sqluery +="\", \"" + parser1[i]->type;
			  sqluery +="\", \"" + parser1[i]->locate;
			  sqluery +="\", \"" + parser1[i]->filname;
			  sqluery +="\", \"" + parser1[i]->time2;
			  sqluery +="\", \"" + parser1[i]->more1;
			  sqluery +="\", \"" + parser1[i]->more2;
			  sqluery +="\" )"; 
			  
              if( !parser1[i]->success )
              {
                  ErrMsg("----------------");
                  ErrMsg( sqluery );
                  ErrMsg( patch_file_name );
                  ErrMsg("----------------");
                  continue;
              }
              //ReciveMsg( sqluery );
			  query_state = mysql_query(connection, sqluery.c_str() );
			  if (query_state !=0) 
			  {
					ErrMsg("----------------");
                    ErrMsg( mysql_error(connection) );
                    ErrMsg( sqluery );
                    ErrMsg( patch_file_name );
                    ErrMsg("----------------");
			  }
		  }

          
		  /*

          if (query_state !=0) 
		  {
			  std::cout << mysql_error(connection) << std::endl;
			  return 1;
          }

          result = mysql_store_result(connection);
          while (( row = mysql_fetch_row(result)) != NULL) 
		  {
			std::cout <<  row[0] << std::endl;
          }*/
		  /*
		   mysql> create database cpp_data;
      Query OK, 1 row affected (0.00 sec)

      mysql> use cpp_data;
      Database changed
      mysql> create table users(id int, fname varchar(25), sname varchar(25), active bool);
      Query OK, 0 rows affected (0.11 sec)

      mysql> insert into users values (1, 'Qwerko', 'Purko', True);
      Query OK, 1 row affected (0.05 sec)

      mysql> insert into users values (2, 'Asdfko', 'Kurko', False);
      Query OK, 1 row affected (0.01 sec)

      mysql> insert into users values (3, 'Bumko', 'Bomko', True);
      Query OK, 1 row affected (0.03 sec)

      mysql> delimiter //
      mysql> create function user_count() returns int
      -> deterministic
      -> begin
      -> declare c int;
      -> select count(*) into c from users where active = True;
      -> return c;
      -> end
      -> //
      Query OK, 0 rows affected (0.08 sec)

      mysql> delimiter ;
      mysql>
		  */

         // mysql_free_result(result);
    }
    mysql_close(connection);
    ReciveMsg("Завершаем работу.");
	SetConsoleOutputCP(oldcodepage);
    return 0;
          
}

