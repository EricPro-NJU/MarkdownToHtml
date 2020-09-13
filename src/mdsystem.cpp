#include "../include/mdsystem.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <iomanip>
#include <windows.h>
/*
	the sysfile are written like this:
	title <tab> trans_or_not <enter>
*/
MdSystem::MdSystem(string filename){
	FILE* pfile = fopen(filename.c_str(),"r");
	this->filename = filename;
	this->filenum = 0;
	this->flist.clear();
	this->current_file=NULL;
	char rc[MAXLEN];
	while(fgets(rc,MAXLEN,pfile)){
		if(rc=="\n") continue;
		string rcs = rc;
		int len = rcs.length();
		string title;
		string trans;
		int i;
		for(i=0;i<len;i++){
			if(rcs[i]!='\t') title+=rcs[i];
			else break;
		}
		i++;
		for(;i<len;i++){
			if(rcs[i]!='\n'&&rcs[i]!='\0') trans+=rcs[i];
			else break;
		}
		string fn = "doc\\";
		fn+=title;
		fn+=".md";
		//cout<<"title: "<<title<<"trans: "<<trans<<endl;
		MdFile newmd(title,fn,(trans=="true")?true:false);
		flist.push_back(newmd);
		filenum++;
	}
}

void MdSystem::display_all(){
	//cout<<"  total file nums: "<<filenum<<endl;
	cout<<'\t'<<setw(5)<<"序号"<<setw(20)<<"标题"<<setw(24)<<"源文件"<<setw(8)<<"行数"<<setw(24)<<"html文件名"<<endl; 
	for(int i=0;i<filenum;i++){
		string name = flist[i].get_title();
		string file = name;
		file += ".md";
		string tr = flist[i].get_trans_status();
		int n = flist[i].get_line_nums();
		cout<<'\t'<<setw(5)<<(i+1)<<setw(20)<<name<<setw(24)<<file
		<<setw(8)<<n<<setw(24)<<tr<<endl; 
	}
}

void MdSystem::display_chosen(int begin, int end, int chosen){
	//cout<<"  total file nums: "<<filenum<<endl;
	cout<<"     "<<setw(5)<<"序号"<<setw(20)<<"标题"<<setw(24)<<"源文件"<<setw(8)<<"行数"<<setw(24)<<"html文件名"<<endl; 
	for(int i=begin-1;i<end;i++){
		string name = flist[i].get_title();
		string file = name;
		file += ".md";
		string tr = flist[i].get_trans_status();
		int n = flist[i].get_line_nums();
		if(chosen==i+1){
			cout<<"---->";
		}
		else{
			cout<<"     ";
		}
		cout<<setw(5)<<(i+1)<<setw(20)<<name<<setw(24)<<file
		<<setw(8)<<n<<setw(24)<<tr; 
		if(chosen==i+1){
			cout<<"<----";
		}
		cout<<endl;
	}
}

MdFile* MdSystem::get_file(string name){
	for(int i=0;i<filenum;i++){
		string title = flist[i].get_title();
		if(title == name){
			MdFile* res = &(flist[i]);
			return res;
		}
	}
	return NULL;
}

MdFile* MdSystem::get_file(int index){
	if(index<=0||index>filenum) return NULL;
	MdFile* mf = &(flist[index-1]);
	return mf;
}

bool MdSystem::open_file(string name){
	MdFile* mf = get_file(name);
	if(mf==NULL) return false;
	else{
		current_file = mf;
		return true;
	}
}

bool MdSystem::close_file(){
	if(current_file==NULL) return false;
	else{
		current_file = NULL;
		return true;
	}
}

bool MdSystem::add_new_file(string name){
	MdFile* mf = get_file(name);
	if(mf!=NULL) return false;
	else{
		MdFile newmf(name);
		flist.push_back(newmf);
		filenum++;
	}
	save_file();
	return true;
}

bool MdSystem::delete_file(string name){
	string md_name = "doc\\";
	md_name += name;
	md_name += ".md";
	string html_name = "doc\\";
	html_name += name;
	html_name += ".html";
	for(int i=0;i<filenum;i++){
		if(flist[i].get_title()==name){
			flist.erase(flist.begin()+i,flist.begin()+i+1);
			filenum--;
			save_file();
			remove(md_name.c_str());
			if(flist[i].get_trans_status()!="未生成") remove(html_name.c_str());
			return true;
		}
	}
	return false;
}

bool MdSystem::rename_file(string ori, string dest){
	MdFile* mf = get_file(ori);
	if(mf==NULL) return false;
	MdFile* mf1 = get_file(dest);
	if(mf1!=NULL) return false;
	if(!mf->rename_title(dest)){
		return false;
	}
	save_file();
	return true;
}

void MdSystem::save_file(){
	FILE* pfile = fopen(filename.c_str(),"w");
	for(int i=0;i<filenum;i++){
		string s;
		s = flist[i].get_title();
		s += '\t';
		if(flist[i].get_trans_status()=="未生成"){
			s += "false\n"; 
		} 
		else{
			s += "true\n";
		}
		fputs(s.c_str(),pfile);
	}
	fclose(pfile);
}

MdFile* MdSystem::get_current_file(){
	return current_file;
}

int MdSystem::get_file_num(){
	return filenum;
}

MdSystem::~MdSystem(){
	flist.clear();
	current_file=NULL;
	filenum=0;
}
