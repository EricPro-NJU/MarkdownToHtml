#include "..\include\mdfile.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <iomanip>
#include <windows.h>
#include <direct.h>

MdFile::MdFile(string title){
	this->title = title;
	this->filename = "doc\\";
	this->filename += title;
	this->filename += ".md";
	this->content.clear();
	this->html.clear();
	this->line_nums=0;
	this->trans = false;
}

MdFile::MdFile(string title, string filename, bool trans){
	this->title = title;
	this->filename = filename;
	this->trans = trans;
	this->html.clear();
	line_nums=0;
	FILE* pfile = fopen(filename.c_str(),"r");
	if(pfile==NULL) {cout<<"Error! cannot open the file"<<endl; return;} //adnormal condition 
	char rc[MAXLEN];
	while(fgets(rc,MAXLEN,pfile)){
		string rs = rc;
		content.push_back(rs);
		line_nums++;
	}
	fclose(pfile);
}

string MdFile::prepro_s(string s){
	int len = s.size();
	for(int i=0;i<len-1;i++){
		if(s[i]=='\n'){
			s.erase(s.begin()+i,s.begin()+i+1);
			len--;
		}
	}
	if(s[len]!='\n'){
		s.insert(s.begin()+len,'\n');
	}
	
	return s;
}

void MdFile::add_line_back(string line){
	string s = prepro_s(line);
	content.push_back(s);
	line_nums++;
}

//add a line AFTER line index. Here index is the line number, 0 means in the beginning.
bool MdFile::add_line_mid(string line, int index){
	string s = prepro_s(line);
	if(index<0||index>line_nums){
		return false;
	}
	else{
		content.insert(content.begin()+index,s);
		line_nums++; 
		return true;
	}
}

bool MdFile::modify_line(int id, string line){
	if(id<=0||id>line_nums){
		return false;
	}
	else{
		string s = prepro_s(line);
		content[id-1] = s;
		return true;
	}
}

bool MdFile::delete_line(int id){
	if(id<=0||id>line_nums){
		return false;
	}
	else{
		content.erase(content.begin()+id-1);
		line_nums--;
		return true;
	}
}

void MdFile::display(){
	for(int i=0;i<line_nums;i++){
		cout<<setw(3)<<i+1<<"  ";
		cout<<content[i];
	}
	//cout<<"Total line number: "<<line_nums<<endl; 
}

void MdFile::display_insert(int index){
	/*
		In order to save space, we only show the last and next 5 lines of line index.
	*/
	int start, end;
	if(index < 5){
		start = 0;
		end = (10>line_nums)? line_nums : 10;
	}
	else if(line_nums - index <=5){
		end = line_nums;
		start = (line_nums-10 < 0)? 0 : (line_nums-10);
	}
	else{
		start = index-5;
		end = index+5;
	}
	for(int i=start;i<end;i++){
		if(index==i){
			cout<<" ***---->（在这一行插入文本）<----***"<<endl;
		}
		cout<<setw(3)<<i+1<<"  ";
		cout<<content[i];
	}
	if(index == line_nums){
		cout<<" ***---->（在这一行插入文本）<----***"<<endl;
	}
}

void MdFile::display_chosen(int index){
	/*
		In order to save space, we only show the last and next 5 lines of line index.
	*/
	int start, end;
	if(index < 5){
		start = 0;
		end = (10>line_nums)? line_nums : 10;
	}
	else if(line_nums - index <=5){
		end = line_nums;
		start = (line_nums-10 < 0)? 0 : (line_nums-10);
	}
	else{
		start = index-5;
		end = index+5;
	}
	for(int i=start;i<end;i++){
		if(i==index-1){
			cout<<"---->";
		}
		else{
			cout<<"     ";
		}
		cout<<setw(3)<<i+1<<"  ";
		cout<<content[i];
	}
}

void MdFile::save_file(){
	FILE* pfile = fopen(filename.c_str(),"w");
	for(int i=0;i<line_nums;i++){
		fputs(content[i].c_str(),pfile);
	}
	fclose(pfile);
}

vector <Token> MdFile::derive_token(int index){
	vector <Token> tlist;
	tlist.clear();
	string s = content[index];
	//cout<<s<<endl;
	if(s=="\n"){
		return tlist;
	}
	int len = s.size();
	int i = 0;
	string cur;
	cur.clear();
	while(i<len){
		if(i==len-1){ //s[len-1] is '\n'
			if(!cur.empty()){
				Token tk;
				tk.s = cur;
				tk.t = NORM;
				tlist.push_back(tk);
			}
			break;
		}
		if(s[i]=='#'){ //head marks
			if(!cur.empty()){
				Token tk;
				tk.s = cur;
				tk.t = NORM;
				tlist.push_back(tk);
				cur.clear();
			}
			cur=+'#';
			int level = 1;
			i++;
			while(s[i]=='#'){
				cur+='#';
				level++;
				i++;
			}
			if(s[i]==' '){
				cur+=' ';
				Token tk;
				tk.s = cur;
				if(level > 6){
					level = 6;
					//[a warning should be put here.]
				}
				tk.t = (MyTokenType)(level+3);
				tlist.push_back(tk);
				cur.clear();
				i++;
				continue;
			}
			else{//those marks should be viewd as normal text
				//[a warning should be put here.]
				continue;
			}
		} 
		else if(s[i]=='*'){ //b and em
			if(!cur.empty()){
				Token tk;
				tk.s = cur;
				tk.t = NORM;
				tlist.push_back(tk);
				cur.clear();
			}
			cur+='*';
			int level = 1;
			i++;
			while(s[i]=='*'&&level<3){
				cur+='*';
				level++;
				i++;
			}
			Token tk;
			tk.s = cur;
			tk.t = (MyTokenType)level;
			tlist.push_back(tk);
			cur.clear();
		}
		else if((s[i]=='+'||s[i]=='-'||s[i]=='*')&&s[i+1]==' '){ //unsorted charts
			if(!cur.empty()){
				Token tk;
				tk.s = cur;
				tk.t = NORM;
				tlist.push_back(tk);
				cur.clear();
			}
			
			
			cur = s[i];
			cur += ' ';
			Token tk;
			tk.s = cur;
			tk.t = ULM;
			if(!tlist.empty()){
				Token last = *(tlist.end()-1); 
				string last_s = last.s;
				if(last.t==NORM){ //identify the level 
					int len = last_s.length();
					bool valid = true;
					int num = 0;
					for(int i=0;i<len;i++){
						if(last_s[i]==' ') num++;
						else{
							valid = false;
							break;
						}
					}
					if(valid) tk.match = num/2+1;
					else tk.match = 1; 
				}
			}
			else tk.match = 1;
			tlist.push_back(tk);
			i+=2;
			cur.clear();
		}
		else if (s[i]>='0'&&s[i]<='9'){ //sorted charts
			if(!cur.empty()){
				Token tk;
				tk.s = cur;
				tk.t = NORM;
				tlist.push_back(tk);
				cur.clear();
			}
			cur+=s[i];
			i++;
			while(s[i]>='0'&&s[i]<='9'){
				cur+=s[i];
				i++;
			}
			if(s[i]=='.'&&s[i+1]==' '){
				cur+=". ";
				i+=2;
				Token tk;
				tk.s=cur;
				tk.t=OLM;
				if(!tlist.empty()){
					Token last = *(tlist.end()-1); 
					string last_s = last.s;
					if(last.t==NORM){ //identify the level 
						int len = last_s.length();
						bool valid = true;
						int num = 0;
						for(int i=0;i<len;i++){
							if(last_s[i]==' ') num++;
							else{
								valid = false;
								break;
							}
						}
						if(valid) tk.match = num/2+1;
						else tk.match = 1; 
					}
				}
				else tk.match = 1;
				tlist.push_back(tk);
				cur.clear();
			}
			else{
				continue;
			}
		}
		else if(s[i]=='['){ //links
			if(!cur.empty()){
				Token tk;
				tk.s = cur;
				tk.t = NORM;
				tlist.push_back(tk);
				cur.clear();
			}
			int j=i;
			while(j<len-1){
				if(s[j]==']') break;
				j++;
			}
			if(j!=len-1){
				for(int k=i;k<=j;k++){
					cur+=s[k];
				}
				Token tk;
				tk.s = cur;
				tk.t = LINK;
				tlist.push_back(tk);
				cur.clear();
				i=j+1;
			}
			else{
				cur+=s[i];
				i++;
				continue;
			}
		}
		else if(s[i]=='('){ //hrefs
			if(!cur.empty()){
				Token tk;
				tk.s = cur;
				tk.t = NORM;
				tlist.push_back(tk);
				cur.clear();
			}
			int j=i;
			while(j<len-1){
				if(s[j]==')') break;
				j++;
			}
			if(j!=len-1){
				for(int k=i;k<=j;k++){
					cur+=s[k];
				}
				Token tk;
				tk.s = cur;
				tk.t = HREF;
				tlist.push_back(tk);
				cur.clear();
				i=j+1;
			}
			else{
				cur+=s[i];
				i++;
				continue;
			}
		}
		else{
			cur+=s[i];
			i++;
			continue;
		}
	}
	tlist = check_token(tlist);
	/*cout<<"Line "<<index<<endl; 
	int nums = tlist.size();
	cout<<"  Token: "<<nums<<endl; 
	for(int i=0;i<nums;i++){
		cout<<"  "<<tlist[i].s<<"  "<<tlist[i].t<<endl; 
	}*/
	/*
		Still some problems should be addressed before translation:
			1. symbol '*' doesn't match.
			2. links and hrefs don't match.
	*/
	return tlist;
} 

vector <Token> MdFile::check_token(vector <Token> tlist){
	int nums = tlist.size();
	int i;
	for(i=0;i<nums;i++){
		if(tlist[i].t==ITALIC&&tlist[i].match==0){
			int j=i+1;
			while(j<nums){
				if(tlist[j].t==ITALIC){
					tlist[i].match=1;
					tlist[j].match=2;
					break;
				}
				j++;
			}
			if(j==nums){
				tlist[i].t=NORM;
			}
		}
		else if(tlist[i].t==BOLD&&tlist[i].match==0){
			int j=i+1;
			while(j<nums){
				if(tlist[j].t==BOLD){
					tlist[i].match=1;
					tlist[j].match=2;
					break;
				}
				j++;
			}
			if(j==nums){ //'*' might be intepreted as ITALIC symbols
				j=i+1;
				while(j<nums){
					if(tlist[j].t==ITALIC){
						tlist[i].s='*';
						tlist[i].t=ITALIC;
						tlist[i].match=1;
						tlist[j].match=2;
						Token tk;
						tk.s='*';
						tk.t=ITALIC;
						tlist.insert(tlist.begin()+i+1,tk);
						nums++;
						break;
					}
					j++;
				}
				if(j==nums){
					tlist[i].t=NORM;
				}
			}
		}
		else if(tlist[i].t==BEM&&tlist[i].match==0){
			int j=i+1;
			while(j<nums){
				if(tlist[j].t==BEM){
					tlist[i].match=1;
					tlist[j].match=2;
					break;
				}
				j++;
			}
			if(j==nums){ //'*' might be intepreted as BOLD/ITALIC symbols
				j=i+1;
				while(j<nums){
					if(tlist[j].t==BOLD){
						tlist[i].s="**";
						tlist[i].t=BOLD;
						tlist[i].match=1;
						tlist[j].match=2;
						Token tk;
						tk.s="*";
						tk.t=ITALIC;
						tlist.insert(tlist.begin()+i+1,tk);
						nums++;
						break;
					}
					j++;
				}
				if(j==nums){
					j=i+1;
					while(j<nums){
						if(tlist[j].t==ITALIC){
							tlist[i].s='*';
							tlist[i].t=ITALIC;
							tlist[i].match=1;
							tlist[j].match=2;
							Token tk;
							tk.s="**";
							tk.t=BOLD;
							tlist.insert(tlist.begin()+i+1,tk);
							nums++;
							break;
						}
						j++;
					}
					if(j==nums){
						tlist[i].t=NORM;
					}
				}
			}
		}
		//links
		else if(tlist[i].t==LINK&&tlist[i].match==0){
			if(tlist[i+1].t==HREF){
				tlist[i].match=1;
				tlist[i+1].match=2;
				continue;
			}
			else{
				tlist[i].t=NORM;
			}
		}
		else if(tlist[i].t==HREF&&tlist[i].match==0){
			tlist[i].t=NORM; //no match
		}
	}
	return tlist;
}

LineInfo MdFile::translate_line(int index){
	LineInfo info;
	vector <Token> tlist = derive_token(index);
	if(tlist.empty())  {
		info.t = NONE;
		info.level = 0;
		return info	;
	}
	vector <string> ending;
	int endnum = 0;
	ending.clear();
	LineType t = NORMAL;
	string h;
	h.clear();
	int nums = tlist.size();
	for(int i=0;i<nums;i++){
		if(tlist[i].t==ITALIC){ //italic
			if(tlist[i].match==1){
				h += "<em>";
			}
			else if(tlist[i].match==2){
				h += "</em>";
			}
		}
		else if(tlist[i].t==BOLD){ //bold
			if(tlist[i].match==1){
				h += "<b>";
			}
			else if(tlist[i].match==2){
				h += "</b>";
			}
		}
		else if(tlist[i].t==BEM){ //b+em
			if(tlist[i].match==1){
				h += "<b><em>";
			}
			else if(tlist[i].match==2){
				h += "</em></b>";
			}
		}
		else if(tlist[i].t==HEAD1){
			h += "<h1>";
			ending.push_back("</h1>");
			endnum++;
			if(t==UL||t==ULH) t=ULH;
			else if(t==OL||t==OLH) t=OLH;
			else t=HEAD;
		}
		else if(tlist[i].t==HEAD2){
			h += "<h2>";
			ending.push_back("</h2>");
			endnum++;
			if(t==UL||t==ULH) t=ULH;
			else if(t==OL||t==OLH) t=OLH;
			else t=HEAD;
		}
		else if(tlist[i].t==HEAD3){
			h += "<h3>";
			ending.push_back("</h3>");
			endnum++;
			if(t==UL||t==ULH) t=ULH;
			else if(t==OL||t==OLH) t=OLH;
			else t=HEAD;
		}
		else if(tlist[i].t==HEAD4){
			h += "<h4>";
			ending.push_back("</h4>");
			endnum++;
			if(t==UL||t==ULH) t=ULH;
			else if(t==OL||t==OLH) t=OLH;
			else t=HEAD;
		}
		else if(tlist[i].t==HEAD5){
			h += "<h5>";
			ending.push_back("</h5>");
			endnum++;
			if(t==UL||t==ULH) t=ULH;
			else if(t==OL||t==OLH) t=OLH;
			else t=HEAD;
		}
		else if(tlist[i].t==HEAD6){
			h += "<h6>";
			ending.push_back("</h6>");
			endnum++;
			if(t==UL||t==ULH) t=ULH;
			else if(t==OL||t==OLH) t=OLH;
			else t=HEAD;
		}
		else if(tlist[i].t==ULM){
			h += "<li>";
			info.level = tlist[i].match;
			ending.push_back("</li>");
			endnum++;
			if(t==HEAD||t==ULH) t=ULH;
			else t=UL;
		}
		else if(tlist[i].t==OLM){
			h += "<li>";
			info.level = tlist[i].match;
			ending.push_back("</li>");
			endnum++;
			if(t==HEAD||t==OLH) t=OLH;
			else t=OL;
		}
		else if(tlist[i].t==LINK){
			string title = tlist[i].s;
			string url = tlist[i+1].s;
			title.erase(title.begin());
			title.erase(title.end()-1);
			url.erase(url.begin());
			url.erase(url.end()-1);
			h += "<a href=\'";
			h += url;
			h += "\'>";
			h += title;
			h += "</a>";
			i++;	
		}
		else{
			h += tlist[i].s;
		}
	}
	for(int i=endnum-1;i>=0;i--){ //Stack
		h += ending[i];
	}
	h+='\n';
	html.push_back(h);
	//cout<<h;
	info.t = t;
	return info;
}

vector <string> MdFile::translate(){
	html.clear();
	//bool ulest=false; //mark the beginning of ul
	//bool olest=false; //mark the beginning of ol
	vector <LineInfo> stack_list;
	int tlevel=0;
	html.push_back("<!doctype html>\n");
	html.push_back("<html>\n<head>\n");
	html.push_back("<meta charset=\'gb2312\'><meta name=\'viewport\' content=\'width=device-width initial-scale=1\'>\n");
	html.push_back("<title>");
	html.push_back(title);
	html.push_back("</title></head>\n<body>\n");
	for(int i=0;i<line_nums;i++){
		LineInfo info = translate_line(i);
		if(info.t==NONE) continue;
		/*if(!ulest&&!olest){
			if(info.t==NORMAL) html.push_back("<br>"); //swtich lines 
			else if(info.t==UL||info.t==ULH){
				tlevel = info.level;
				html.insert(html.end()-1,"<ul>\n");
				ulest = true;
				tlevel = 1;
			}
			else if(info.t==OL||info.t==OLH){
				tlevel = info.level;
				html.insert(html.end()-1,"<ol>\n");
				olest = true;
				tlevel = 1;
			}
		}
		else if(ulest){
			if(info.t==NORMAL){
				html.insert(html.end()-1,"</ul>\n");
				ulest = false;
				tlevel = 0;
				html.push_back("<br>");
			}
			else if(info.t==HEAD){
				html.insert(html.end()-1,"</ul>\n");
				ulest = false;
				tlevel = 0;
			}
			else if(info.t==OL||t==OLH){
				if(info.level>tlevel){
					html
				}
				html.insert(html.end()-1,"</ul>\n<ol>\n");
				ulest = false;
				olest = true;
			}
		}
		else if(olest){
			if(t==NORMAL){
				html.insert(html.end()-1,"</ol>\n");
				olest = false;
				tlevel = 0;
				html.push_back("<br>");
			}
			else if(t==HEAD){
				html.insert(html.end()-1,"</ol>\n");
				olest = false;
				tlevel = 0;
			}
			else if(t==UL||t==ULH){
				html.insert(html.end()-1,"</ol>\n<ul>\n");
				olest = false;
				ulest = true;
			}
		}*/
		if(info.t==NORMAL){
			while(tlevel>0){
				LineInfo bk = *(stack_list.end()-1);
				switch(bk.t){
					case UL: case ULH: html.insert(html.end()-1,"</ul>\n"); break;
					case OL: case OLH: html.insert(html.end()-1,"</ol>\n"); break;
				}
				stack_list.pop_back();
				tlevel--;
			}
			html.push_back("<br>\n");
		}
		else if(info.t==HEAD){
			while(tlevel>0){
				LineInfo bk = *(stack_list.end()-1);
				switch(bk.t){
					case UL: case ULH: html.insert(html.end()-1,"</ul>\n"); break;
					case OL: case OLH: html.insert(html.end()-1,"</ol>\n"); break;
				}
				stack_list.pop_back();
				tlevel--;
			}
		}
		else if(info.t==UL||info.t==ULH){
			if(tlevel==0){
				html.insert(html.end()-1,"<ul>\n");
				stack_list.push_back(info);
				tlevel++;
			}
			else{
				LineInfo bk = *(stack_list.end()-1);
				if(info.level>bk.level) {
					html.insert(html.end()-1,"<ul>\n");
					stack_list.push_back(info);
					tlevel++;
				}
				else if(info.level<bk.level){
					while(!stack_list.empty()){
						LineInfo bk = *(stack_list.end()-1);
						if(bk.level<info.level) {
							html.insert(html.end()-1,"<ul>\n");
							stack_list.push_back(info);
							tlevel++;
							break;
						}
						if(bk.level==info.level&&(bk.t==UL||bk.t==ULH)) break;
						switch(bk.t){
							case UL: case ULH: html.insert(html.end()-1,"</ul>\n"); break;
							case OL: case OLH: html.insert(html.end()-1,"</ol>\n"); break;
						}
						stack_list.pop_back();
						tlevel--;
					}
					
				}
			}
		}
		else if(info.t==OL||info.t==OLH){
			if(tlevel==0){
				html.insert(html.end()-1,"<ol>\n");
				stack_list.push_back(info);
				tlevel++;
			}
			else{
				LineInfo bk = *(stack_list.end()-1);
				if(info.level>bk.level) {
					html.insert(html.end()-1,"<ol>\n");
					stack_list.push_back(info);
					tlevel++;
				}
				else if(info.level<bk.level){
					while(!stack_list.empty()){
						LineInfo bk = *(stack_list.end()-1);
						if(bk.level<info.level) {
							html.insert(html.end()-1,"<ol>\n");
							stack_list.push_back(info);
							tlevel++;
							break;
						}
						if(bk.level==info.level&&(bk.t==OL||bk.t==OLH)) break;
						switch(bk.t){
							case UL: case ULH: html.insert(html.end()-1,"</ul>\n"); break;
							case OL: case OLH: html.insert(html.end()-1,"</ol>\n"); break;
						}
						stack_list.pop_back();
						tlevel--;
					}
					
				}
			}
		}
	}
	/*if(ulest){
		html.push_back("</ul>\n");
		ulest = false;
		tlevel = 0;
	}
	if(olest){
		html.push_back("</ol>\n");
		olest = false;
		tlevel = 0;
	}*/
	
	html.push_back("</body>\n</html>\n");
	string hfile = "doc\\";
	hfile += title;
	hfile += ".html";
	FILE* sfile = fopen(hfile.c_str(),"w");
	int onums = html.size();
	for(int i=0;i<onums;i++){
		fputs(html[i].c_str(),sfile);
	}
	fclose(sfile);
	this->trans = true;
	return html;
}

bool MdFile::export_md(string path){
	char buffer[_MAX_PATH];
	_getcwd(buffer, _MAX_PATH);
	string ori = buffer;
	ori += "\\";
	ori += filename;
	string now = path;
	bool success;
	//cout<<"From \""<<ori<<"\" to \""<<now<<"\""<<endl;
	success=CopyFile(ori.c_str(),now.c_str(),true);
	
	return success;  //some need admin acess
} 

bool MdFile::export_html(string path){
	char buffer[_MAX_PATH];
	_getcwd(buffer, _MAX_PATH);
	string ori = buffer;
	ori += "\\doc\\";
	ori += title;
	ori += ".html"; 
	string now = path;
	bool success;
	//cout<<"From \""<<ori<<"\" to \""<<now<<"\""<<endl;
	success=CopyFile(ori.c_str(),now.c_str(),true);
	
	return success;  //some need admin acess
}

int MdFile::get_line_nums(){
	return line_nums;
}

string MdFile::get_title(){
	return title;
}

string MdFile::get_trans_status(){
	if(trans==true){
		string result = title;
		result += ".html";
		return result; 
	}
	else{
		return "未生成";
	}
}

bool MdFile::rename_title(string new_title){
	string old_file = filename;
	string new_file = "doc\\";
	string old_html = "doc\\";
	old_html += title;
	old_html += ".html";
	string new_html;
	new_file += new_title;
	new_html = new_file;
	new_file += ".md";
	new_html += ".html";
	bool mdr = rename(old_file.c_str(),new_file.c_str());
	if(mdr){
		return false;
	}
	if(trans){	
		bool htmlr = rename(old_html.c_str(),new_html.c_str());
		if(htmlr){
			return false;
		}
	}
	title = new_title;
	filename = new_file;
	return true;
}

MdFile::~MdFile(){
	content.clear();
	html.clear();
}
