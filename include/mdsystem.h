#ifndef _MDSYSTEM_H
#define _MDSYSTEM_H
#include "mdfile.h"

class MdSystem{
	private:
		int filenum;
		string filename; //system file
		vector <MdFile> flist;
		MdFile* current_file; 
	public:
		MdSystem(string filename);
		MdFile* get_file(string name);
		MdFile* get_file(int index); 
		bool open_file(string name);
		bool close_file();
		bool add_new_file(string name);
		bool delete_file(string name);
		bool rename_file(string ori, string dest);
		void display_all();
		void display_chosen(int begin, int end, int chosen);
		void save_file();
		MdFile* get_current_file(); 
		int get_file_num();
		~MdSystem();
};

#endif //_MDSYSTEM_H
