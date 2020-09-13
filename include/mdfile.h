#ifndef _MDFILE_H
#define _MDFILE_H
#include <string>
#include <vector>
#define MAXLEN 101
using namespace std;
/*
	All the .md files are saved in \doc, where newly created files will be saved as well.
	After translated, the .html file will also be in the same place.
	Those files in \doc should NOT be moved or renamed in order to maintain the normal operation of the program.
	Functions will be set to derive files from /doc to a specific path.
*/
enum LineType{
	NONE,
	NORMAL, //bold, em, href...
	HEAD,
	UL, //unsorted
	OL, //sorted
	ULH, //ul+head
	OLH	//ol+head
};
struct LineInfo{
	LineType t;
	int level;
	LineInfo():level(0),t(NONE){}
};

enum MyTokenType{
	NORM,
	ITALIC,
	BOLD,
	BEM,
	HEAD1,
	HEAD2,
	HEAD3,
	HEAD4,
	HEAD5,
	HEAD6,
	ULM,
	OLM,
	LINK,
	HREF
};
struct Token{
	string s;
	MyTokenType t;
	int match;//0:no match; 1:left match; 2:right match (only valid for '*' and links)(plus: for ul and ol, means the level) 
	Token():match(0){}
};
class MdFile{
	private:
		string title;
		vector <string> content;
		string filename;
		bool trans;
		int line_nums;
		vector <string> html;
	public:
		MdFile(string title); //creating new file
		MdFile(string title, string filename, bool trans); //loading existing file
		string prepro_s(string s);//do pre-process to the string, make it compatible to the rules of lines
		void add_line_back(string line); //add a line from the end
		bool add_line_mid(string line, int index); //add a line in the middle
		bool modify_line(int id, string line); //modify an existing line
		bool delete_line(int id);
		void display();
		void display_insert(int index);
		void display_chosen(int index); 
		void save_file();
		vector <Token> derive_token(int index); //derive token from a sentence
		vector <Token> check_token(vector <Token> tlist); //check for special circumstances and adjust it
		LineInfo translate_line(int index); //translate line by line according to the token, save to html, and return linetype
		vector <string> translate(); //translate into .html file, return html
		bool export_md(string path); //path is the exported filename of the copy.
		bool export_html(string path);
		string get_title();
		string get_trans_status();
		bool rename_title(string new_title);
		int get_line_nums();
		~MdFile();
};

#endif //_MDFILE_H
