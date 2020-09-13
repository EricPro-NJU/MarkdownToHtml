#include "../include/ui.h"
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <iomanip>
#include <windows.h>
#include <conio.h>

void enter_main(MdSystem* ms){
	int index=1;
	char ch;
	bool do_exit=false;
	do{
		system("cls");
		cout<<" 当前版本："<<MDVERSION<<endl;
		cout<<" 当前界面：主界面"<<endl; 
		cout<<endl;
		cout<<"                 控制台简易MarkDown编辑器"<<endl;
		cout<<"             =================================="<<endl;
		cout<<"             *";
		if(index==1) cout<<"---->";
		else cout<<"     ";
		cout<<"1.新建空白MarkDown文档";
		if(index==1) cout<<"<----";
		else cout<<"     ";
		cout<<"*"<<endl;
		cout<<"             *";
		if(index==2) cout<<"---->";
		else cout<<"     ";
		cout<<"2.浏览所有MarkDown文档";     
		if(index==2) cout<<"<----";
		else cout<<"     ";
		cout<<"*"<<endl;
		cout<<"             *";     
		if(index==3) cout<<"---->";
		else cout<<"     ";
		cout<<"3.退                出";     
		if(index==3) cout<<"<----";
		else cout<<"     ";
		cout<<"*"<<endl;
		cout<<"             =================================="<<endl;
		cout<<"      操作提示：按下“↑”“↓”移动光标，按下Enter确认。"<<endl; 
		ch = getch();
		
		if(ch=='H'){
			if(index==1) continue;
			else{
				index--;
				continue;
			}
		}
		if(ch=='P'){
			if(index==3) continue;
			else{
				index++;
				continue;
			}
		}
		if(ch==13){
			switch(index){
				case 1: enter_addnew(ms); break;
				case 2: enter_display(ms); break;
				case 3: enter_exit(ms); do_exit = true; break;
			}
		}
	}while(!do_exit);
}

void enter_exit(MdSystem* ms){
	system("cls");
	ms->save_file();
	cout<<endl<<endl;
	cout<<"                 感谢使用"<<endl;
	cout<<"           ==================="<<endl;
	cout<<"           *正在保存系统文件..."<<endl;
	cout<<"           *保存完毕，正在退出"<<endl;
	cout<<"           ==================="<<endl;
	Sleep(1500);
	return; 
}

void enter_addnew(MdSystem* ms){
	string new_title;
	bool valid_title = false;
	do{
		system("cls");
		cout<<" 当前版本："<<MDVERSION<<endl;
		cout<<" 当前界面：新建空白MarkDown文件"<<endl; 
		cout<<endl;
		cout<<endl;
		cout<<"                新建文件"<<endl;
		cout<<"      ============================"<<endl;
		cout<<"      欢迎使用简易MarkDown编辑器！"<<endl<<"      (不输入直接按Enter，即可返回主界面)"<<endl;
		cout<<"     *请输入新建文件的标题：";
		getline(cin,new_title);
		if(new_title.empty()){
			return ;
		}
		valid_title = ms->add_new_file(new_title);
		if(!valid_title){
			system("cls");
			cout<<endl<<endl;
			cout<<"                             错误"<<endl;
			cout<<"          ============================================="<<endl;
			cout<<"          创建失败！请检查标题是否与已存在文件标题冲突！"<<endl;
			cout<<"          ============================================="<<endl;
			cout<<"          *";
			system("pause"); 
		}
	}while(!valid_title);
	ms->open_file(new_title);
	MdFile* the_file = ms->get_current_file();
	edit_file(ms,the_file);
}

void enter_display(MdSystem* ms){
	system("cls");
	int total = ms->get_file_num();
	if(total==0){
		cout<<endl<<endl;
		cout<<"                          错误提示"<<endl;
		cout<<"       ============================================"<<endl;
		cout<<"       *你还没有创建MarkDown文件呢！快去新建一个吧！"<<endl;
		cout<<"       ============================================"<<endl; 
		cout<<"     操作提示：按下Enter进入新建界面，按下ESC返回。"<<endl;
		char ch1;
		do{
			ch1 = getch();
		} while(ch1!=13&&ch1!=27);
		if(ch1==13){
			enter_addnew(ms);
		}
		return;
	}
	int cur_page = 1;
	int all_page = (total%10==0)? (total/10) : (total/10+1);
	int index = 1;
	int begin, end;
	char ch;
	do{
		system("cls");
		cout<<" 当前版本："<<MDVERSION<<endl;
		cout<<" 当前界面：浏览所有MarkDown文件"<<endl; 
		cout<<endl;
		cout<<endl;
		begin = (cur_page - 1) * 10 + 1;
		end = (cur_page*10>total)? total : (cur_page*10);
		ms->display_chosen(begin,end,index);
		cout<<"    ================================================="<<endl;
		cout<<"    第"<<cur_page<<"页，共"<<all_page<<"页，当前选择第"<<index<<"个文件，共"<<total<<"个文件"<<endl;
		cout<<"    操作提示：“↑”“↓”移动光标选择文件，“←”“→”翻页，Enter进入文件详情，ESC退出。"<<endl; 
		ch = getch();
		if(ch == 'H'){
			if(index==1) continue;
			if(index%10==1){
				cur_page --;
			}
			index --;
		}
		if(ch == 'P'){
			if(index==total) continue;
			if(index%10==0){
				cur_page ++;
			}
			index++;
		}
		if(ch == 'K'){
			if(cur_page==1) continue;
			cur_page--;
			index = (cur_page - 1) * 10 + 1;
		}
		if(ch == 'M'){
			if(cur_page==all_page) continue;
			cur_page++;
			index = (cur_page - 1) * 10 + 1;
		}
		if(ch == 13){
			MdFile* mf = ms->get_file(index);
			ms -> open_file(mf->get_title());
			view_file(ms,mf);
			return;
		}
		if(ch == 27){
			return;
		}
	}while(true);
} 

void edit_file(MdSystem* ms, MdFile* mf){
	system("cls");
	cout<<endl;
	cout<<"    当前文件: "<<mf->get_title()<<".md(编辑模式)"<<endl;
	cout<<"(在文件的末尾添加文本，输入\":q\"保存退出)"<<endl;
	cout<<endl;
	mf->display();
	string add_text;
	do{
		cout<<setw(3)<<(mf->get_line_nums()+1)<<"  ";
		getline(cin,add_text);
		if(add_text!=":q"){
			mf->add_line_back(add_text);
		}
	} while(add_text!=":q");
	mf->save_file();
	cout<<"    *保存成功！"<<endl;
	Sleep(1000);
	view_file(ms,mf);
}

void view_file(MdSystem* ms, MdFile* mf){
	system("cls");
	cout<<endl;
	cout<<"    当前文件: "<<mf->get_title()<<".md(预览模式)"<<endl;
	cout<<"(当前为预览模式，按下Enter呼出菜单，按下ESC返回)"<<endl;
	cout<<endl;
	int len = mf->get_line_nums();
	if(len==0){
		cout<<"  *当前文档空白。按下Enter开始编辑文档。"<<endl;
		char op;
		do{
		
			op = getch();
			if(op==13){
				edit_file(ms,mf);
				return;
			}
			if(op==27){
				enter_display(ms);
				return;
			}
		}while(true);
		
	}
	mf->display();
	char ch;
	do{
		ch = getch();
	}while(ch!=13&&ch!=27);
	if(ch==27){
		enter_display(ms);
	}
	else if(ch==13){
		cout<<endl<<"<MarkDown>文件管理菜单："<<endl; 
		cout<<" *W: 在文档末尾插入文本"<<endl;
		cout<<" *I: 在文档中部插入文本"<<endl;
		cout<<" *M: 修改文档某一行文本"<<endl;
		cout<<" *D: 删除文档某一行文本"<<endl;
		cout<<" *E: 导出md文件"<<endl;
		cout<<" *T: 转换为HTML格式"<<endl;
		cout<<" *R: 删除该MarkDown文件"<<endl;
		cout<<" *N: 重命名该MarkDown文件"<<endl;
		cout<<" *ESC: 返回上一级"<<endl;
		char ch;
		bool valid;
		do{
			ch = getch();
			valid = true;
			string n_title;
			switch(ch){
				case 'w': edit_file(ms,mf); valid=true; break;
				case 'i': add_mid_file(ms,mf); valid=true; break;
				case 'm': modify_file(ms,mf); valid=true; break;
				case 'd': delete_line_file(ms,mf); valid=true; break;
				case 't': enter_trans(ms,mf); valid=true; break;
				case 'e': enter_export_md(ms,mf); valid=true; break;
				case 'r':
					valid = true;
					cout<<endl<<"======================================================="<<endl;
					cout<<"您正在进行删除"<<mf->get_title()<<"文件的操作。"<<endl;
					cout<<"对应生成的html文件也将一并删除，但已导出的文件不受影响。"<<endl;
					cout<<"请按下Enter确认删除，按其他任意键返回"<<endl;
					char conf;
					conf=getch();
					if(conf==13){
						bool rm = ms->delete_file(mf->get_title());
						if(rm) {
							cout<<"删除成功！"<<endl;
							Sleep(1000);
							enter_display(ms); 
							ms->save_file();
							break;
						}
						else{
							cout<<"由于不可预料的错误，删除失败"<<endl;
							Sleep(2000);
							enter_display(ms);
							break;
						}
				
					}
					else{
						view_file(ms,mf);
						break;
					}
				case 'n':
					valid = true;
					cout<<endl<<"======================================================="<<endl;
					cout<<"您正在进行重命名"<<mf->get_title()<<"文件的操作。"<<endl;
					cout<<"您不能使用已有文件的名字来为其命名"<<endl;
					cout<<"输入新标题，如果不想重命名，直接按下Enter即可。"<<endl;
					cout<<"=======================================================" <<endl;
					cout<<"*新标题: ";
					
					getline(cin,n_title);
					if(n_title.empty()){
						view_file(ms,mf);
						break;
					}
					else{
						bool rn = mf->rename_title(n_title);
						if(rn){
							cout<<"修改成功！"<<endl;
							Sleep(1000);
							ms->save_file();
							view_file(ms,mf);
							break;
						}
						else{
							cout<<"修改失败，请检查是否有重名！"<<endl;
							system("pause");
							view_file(ms,mf);
							break;
						} 
					}
				//todo
				case 27: enter_display(ms); valid=true; break;
				default: valid = false;
			}
		} while(!valid);
	}
}

void add_mid_file(MdSystem *ms,MdFile* mf){
	int line_num = mf->get_line_nums();
	int index = 0;
	char ch;
	do{
		system("cls");
		cout<<endl;
		cout<<"    当前文件: "<<mf->get_title()<<".md(插入模式)"<<endl;
		cout<<"(按“↑”“↓”键移动光标选择插入位置，按Enter开始插入，按ESC返回)"<<endl;
		cout<<"当前选择插入位置为";
		if(index == 0){
			cout<<"文档起始位置";
		} 
		else{
			cout<<"第"<<index<<"行后";
		}
		cout<<"，文档共"<<line_num<<"行。"<<endl;
		cout<<endl;
		cout<<"=================================================="<<endl;
		mf->display_insert(index);
		cout<<"=================================================="<<endl;
		ch = getch();
		if(ch == 'H'){
			if(index==0) continue;
			index --;
		}
		else if(ch == 'P'){
			if(index==line_num) continue;
			index ++;
		}
		else if(ch == 27){
			view_file(ms,mf);
			return;
		}
		else if(ch == 13){
			string line; 
			cout<<endl;
			cout<<"请输入插入文本："<<endl;
			cout<<"  *  ";
			getline(cin,line);
			mf->add_line_mid(line,index);
			mf->save_file();
			cout<<"  *插入成功！"<<endl;
			Sleep(1000);
			view_file(ms,mf);
			return; 
		}
		
	}while(true);
}

void modify_file(MdSystem* ms, MdFile* mf){
	int line_num = mf->get_line_nums();
	int index = 1;
	char ch;
	do{
		system("cls");
		cout<<endl;
		cout<<"    当前文件: "<<mf->get_title()<<".md(修改模式)"<<endl;
		cout<<"(按“↑”“↓”键移动光标选择修改位置，按Enter开始修改，按ESC返回)"<<endl;
		cout<<"当前选择修改位置为";
		cout<<"第"<<index<<"行";
		cout<<"，文档共"<<line_num<<"行。"<<endl;
		cout<<endl;
		cout<<"=================================================="<<endl;
		mf->display_chosen(index);
		cout<<"=================================================="<<endl;
		ch = getch();
		if(ch == 'H'){
			if(index==1) continue;
			index --;
		}
		else if(ch == 'P'){
			if(index==line_num) continue;
			index ++;
		}
		else if(ch == 27){
			view_file(ms,mf);
			return;
		}
		else if(ch == 13){
			string line; 
			cout<<endl;
			cout<<"请输入修改后的文本："<<endl;
			cout<<"  *  ";
			getline(cin,line);
			mf->modify_line(index,line);
			mf->save_file();
			cout<<"  *修改成功！"<<endl;
			Sleep(1000);
			view_file(ms,mf);
			return; 
		}
		
	}while(true);
}

void delete_line_file(MdSystem* ms, MdFile* mf){
	int line_num = mf->get_line_nums();
	int index = 1;
	char ch;
	do{
		system("cls");
		cout<<endl;
		cout<<"    当前文件: "<<mf->get_title()<<".md(删除模式)"<<endl;
		cout<<"(按“↑”“↓”键移动光标选择删除位置，按Enter确认删除，按ESC返回)"<<endl;
		cout<<"当前选择删除位置为";
		cout<<"第"<<index<<"行";
		cout<<"，文档共"<<line_num<<"行。"<<endl;
		cout<<endl;
		cout<<"=================================================="<<endl;
		mf->display_chosen(index);
		cout<<"=================================================="<<endl;
		ch = getch();
		if(ch == 'H'){
			if(index==1) continue;
			index --;
		}
		else if(ch == 'P'){
			if(index==line_num) continue;
			index ++;
		}
		else if(ch == 27){
			view_file(ms,mf);
			return;
		}
		else if(ch == 13){
			char op; 
			cout<<endl;
			cout<<"请再次按下Enter确认删除（注意！此操作不可以撤回），按其他任意键返回"<<endl;
			op = getch(); 
			if(op!=13) continue; 
			mf->delete_line(index);
			mf->save_file();
			cout<<"  *删除成功！"<<endl;
			Sleep(1000);
			view_file(ms,mf);
			return; 
		}
		
	}while(true);
}

void enter_trans(MdSystem* ms, MdFile* mf){
	system("cls");
	cout<<endl;
	cout<<"    当前文件: "<<mf->get_title()<<".md(文档解释)"<<endl;
	cout<<endl;
	cout<<"   按下Enter开始生成html文档，按下ESC返回。"<<endl;
	char ch;
	do{
		ch = getch();
		if(ch==13){
			cout<<"   * 正在生成文件......"<<endl; 
			vector <string> html = mf->translate();
			Sleep(1000);
			cout<<"   * 生成完毕！正在打开"<<mf->get_trans_status()<<endl;
			Sleep(1000);
			cout<<endl<<"    预览html文档："<<endl;
			string title = "doc\\";
			title += mf->get_trans_status();
			system(title.c_str());
			int len = html.size();
			cout<<"=========================================================="<<endl; 
			for(int i=0;i<len;i++){
				cout<<html[i]; 
			} 
			cout<<"=========================================================="<<endl; 
			cout<<" 预览完成，";
			cout<<" 请输入导出html文件名（路径）：（若跳过此步骤，输入空白后Enter即可）"<<endl;
			string path;
			cout<<"  *";
			getline(cin,path);
			if(path.empty()){	
				view_file(ms,mf);
				return;
			} 
			if(!mf->export_html(path)){
				cout<<"  *导出失败！请检查路径是否正确，或者访问权限受限！"<<endl;
				cout<<"  *";
				system("pause");
				view_file(ms,mf);
				return;
			}
			else{
				cout<<"  *导出成功！"<<endl;
				Sleep(1000);
				view_file(ms,mf);
				return; 
			}
			return;
		}
		if(ch==27){
			view_file(ms,mf);
			return;
		}
	} while(ch!=27&&ch!=13);
	 
}

void enter_export_md(MdSystem* ms, MdFile* mf){
	string path;
	cout<<"  请输入导出的文件名（路径），输入空白返回"<<endl;
	cout<<"  *";
	getline(cin,path);
	if(path.empty()){	
		view_file(ms,mf);
		return;
	} 
	if(!mf->export_md(path)){
		cout<<"  *导出失败！请检查路径是否正确，或者访问权限受限！"<<endl;
		cout<<"  *";
		system("pause");
		view_file(ms,mf);
		return;
	}
	else{
		cout<<"  *导出成功！"<<endl;
		Sleep(1000);
		view_file(ms,mf);
		return; 
	}
}
