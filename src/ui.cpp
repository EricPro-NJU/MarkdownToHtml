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
		cout<<" ��ǰ�汾��"<<MDVERSION<<endl;
		cout<<" ��ǰ���棺������"<<endl; 
		cout<<endl;
		cout<<"                 ����̨����MarkDown�༭��"<<endl;
		cout<<"             =================================="<<endl;
		cout<<"             *";
		if(index==1) cout<<"---->";
		else cout<<"     ";
		cout<<"1.�½��հ�MarkDown�ĵ�";
		if(index==1) cout<<"<----";
		else cout<<"     ";
		cout<<"*"<<endl;
		cout<<"             *";
		if(index==2) cout<<"---->";
		else cout<<"     ";
		cout<<"2.�������MarkDown�ĵ�";     
		if(index==2) cout<<"<----";
		else cout<<"     ";
		cout<<"*"<<endl;
		cout<<"             *";     
		if(index==3) cout<<"---->";
		else cout<<"     ";
		cout<<"3.��                ��";     
		if(index==3) cout<<"<----";
		else cout<<"     ";
		cout<<"*"<<endl;
		cout<<"             =================================="<<endl;
		cout<<"      ������ʾ�����¡������������ƶ���꣬����Enterȷ�ϡ�"<<endl; 
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
	cout<<"                 ��лʹ��"<<endl;
	cout<<"           ==================="<<endl;
	cout<<"           *���ڱ���ϵͳ�ļ�..."<<endl;
	cout<<"           *������ϣ������˳�"<<endl;
	cout<<"           ==================="<<endl;
	Sleep(1500);
	return; 
}

void enter_addnew(MdSystem* ms){
	string new_title;
	bool valid_title = false;
	do{
		system("cls");
		cout<<" ��ǰ�汾��"<<MDVERSION<<endl;
		cout<<" ��ǰ���棺�½��հ�MarkDown�ļ�"<<endl; 
		cout<<endl;
		cout<<endl;
		cout<<"                �½��ļ�"<<endl;
		cout<<"      ============================"<<endl;
		cout<<"      ��ӭʹ�ü���MarkDown�༭����"<<endl<<"      (������ֱ�Ӱ�Enter�����ɷ���������)"<<endl;
		cout<<"     *�������½��ļ��ı��⣺";
		getline(cin,new_title);
		if(new_title.empty()){
			return ;
		}
		valid_title = ms->add_new_file(new_title);
		if(!valid_title){
			system("cls");
			cout<<endl<<endl;
			cout<<"                             ����"<<endl;
			cout<<"          ============================================="<<endl;
			cout<<"          ����ʧ�ܣ���������Ƿ����Ѵ����ļ������ͻ��"<<endl;
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
		cout<<"                          ������ʾ"<<endl;
		cout<<"       ============================================"<<endl;
		cout<<"       *�㻹û�д���MarkDown�ļ��أ���ȥ�½�һ���ɣ�"<<endl;
		cout<<"       ============================================"<<endl; 
		cout<<"     ������ʾ������Enter�����½����棬����ESC���ء�"<<endl;
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
		cout<<" ��ǰ�汾��"<<MDVERSION<<endl;
		cout<<" ��ǰ���棺�������MarkDown�ļ�"<<endl; 
		cout<<endl;
		cout<<endl;
		begin = (cur_page - 1) * 10 + 1;
		end = (cur_page*10>total)? total : (cur_page*10);
		ms->display_chosen(begin,end,index);
		cout<<"    ================================================="<<endl;
		cout<<"    ��"<<cur_page<<"ҳ����"<<all_page<<"ҳ����ǰѡ���"<<index<<"���ļ�����"<<total<<"���ļ�"<<endl;
		cout<<"    ������ʾ���������������ƶ����ѡ���ļ�����������������ҳ��Enter�����ļ����飬ESC�˳���"<<endl; 
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
	cout<<"    ��ǰ�ļ�: "<<mf->get_title()<<".md(�༭ģʽ)"<<endl;
	cout<<"(���ļ���ĩβ����ı�������\":q\"�����˳�)"<<endl;
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
	cout<<"    *����ɹ���"<<endl;
	Sleep(1000);
	view_file(ms,mf);
}

void view_file(MdSystem* ms, MdFile* mf){
	system("cls");
	cout<<endl;
	cout<<"    ��ǰ�ļ�: "<<mf->get_title()<<".md(Ԥ��ģʽ)"<<endl;
	cout<<"(��ǰΪԤ��ģʽ������Enter�����˵�������ESC����)"<<endl;
	cout<<endl;
	int len = mf->get_line_nums();
	if(len==0){
		cout<<"  *��ǰ�ĵ��հס�����Enter��ʼ�༭�ĵ���"<<endl;
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
		cout<<endl<<"<MarkDown>�ļ�����˵���"<<endl; 
		cout<<" *W: ���ĵ�ĩβ�����ı�"<<endl;
		cout<<" *I: ���ĵ��в������ı�"<<endl;
		cout<<" *M: �޸��ĵ�ĳһ���ı�"<<endl;
		cout<<" *D: ɾ���ĵ�ĳһ���ı�"<<endl;
		cout<<" *E: ����md�ļ�"<<endl;
		cout<<" *T: ת��ΪHTML��ʽ"<<endl;
		cout<<" *R: ɾ����MarkDown�ļ�"<<endl;
		cout<<" *N: ��������MarkDown�ļ�"<<endl;
		cout<<" *ESC: ������һ��"<<endl;
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
					cout<<"�����ڽ���ɾ��"<<mf->get_title()<<"�ļ��Ĳ�����"<<endl;
					cout<<"��Ӧ���ɵ�html�ļ�Ҳ��һ��ɾ�������ѵ������ļ�����Ӱ�졣"<<endl;
					cout<<"�밴��Enterȷ��ɾ�������������������"<<endl;
					char conf;
					conf=getch();
					if(conf==13){
						bool rm = ms->delete_file(mf->get_title());
						if(rm) {
							cout<<"ɾ���ɹ���"<<endl;
							Sleep(1000);
							enter_display(ms); 
							ms->save_file();
							break;
						}
						else{
							cout<<"���ڲ���Ԥ�ϵĴ���ɾ��ʧ��"<<endl;
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
					cout<<"�����ڽ���������"<<mf->get_title()<<"�ļ��Ĳ�����"<<endl;
					cout<<"������ʹ�������ļ���������Ϊ������"<<endl;
					cout<<"�����±��⣬���������������ֱ�Ӱ���Enter���ɡ�"<<endl;
					cout<<"=======================================================" <<endl;
					cout<<"*�±���: ";
					
					getline(cin,n_title);
					if(n_title.empty()){
						view_file(ms,mf);
						break;
					}
					else{
						bool rn = mf->rename_title(n_title);
						if(rn){
							cout<<"�޸ĳɹ���"<<endl;
							Sleep(1000);
							ms->save_file();
							view_file(ms,mf);
							break;
						}
						else{
							cout<<"�޸�ʧ�ܣ������Ƿ���������"<<endl;
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
		cout<<"    ��ǰ�ļ�: "<<mf->get_title()<<".md(����ģʽ)"<<endl;
		cout<<"(�����������������ƶ����ѡ�����λ�ã���Enter��ʼ���룬��ESC����)"<<endl;
		cout<<"��ǰѡ�����λ��Ϊ";
		if(index == 0){
			cout<<"�ĵ���ʼλ��";
		} 
		else{
			cout<<"��"<<index<<"�к�";
		}
		cout<<"���ĵ���"<<line_num<<"�С�"<<endl;
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
			cout<<"����������ı���"<<endl;
			cout<<"  *  ";
			getline(cin,line);
			mf->add_line_mid(line,index);
			mf->save_file();
			cout<<"  *����ɹ���"<<endl;
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
		cout<<"    ��ǰ�ļ�: "<<mf->get_title()<<".md(�޸�ģʽ)"<<endl;
		cout<<"(�����������������ƶ����ѡ���޸�λ�ã���Enter��ʼ�޸ģ���ESC����)"<<endl;
		cout<<"��ǰѡ���޸�λ��Ϊ";
		cout<<"��"<<index<<"��";
		cout<<"���ĵ���"<<line_num<<"�С�"<<endl;
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
			cout<<"�������޸ĺ���ı���"<<endl;
			cout<<"  *  ";
			getline(cin,line);
			mf->modify_line(index,line);
			mf->save_file();
			cout<<"  *�޸ĳɹ���"<<endl;
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
		cout<<"    ��ǰ�ļ�: "<<mf->get_title()<<".md(ɾ��ģʽ)"<<endl;
		cout<<"(�����������������ƶ����ѡ��ɾ��λ�ã���Enterȷ��ɾ������ESC����)"<<endl;
		cout<<"��ǰѡ��ɾ��λ��Ϊ";
		cout<<"��"<<index<<"��";
		cout<<"���ĵ���"<<line_num<<"�С�"<<endl;
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
			cout<<"���ٴΰ���Enterȷ��ɾ����ע�⣡�˲��������Գ��أ������������������"<<endl;
			op = getch(); 
			if(op!=13) continue; 
			mf->delete_line(index);
			mf->save_file();
			cout<<"  *ɾ���ɹ���"<<endl;
			Sleep(1000);
			view_file(ms,mf);
			return; 
		}
		
	}while(true);
}

void enter_trans(MdSystem* ms, MdFile* mf){
	system("cls");
	cout<<endl;
	cout<<"    ��ǰ�ļ�: "<<mf->get_title()<<".md(�ĵ�����)"<<endl;
	cout<<endl;
	cout<<"   ����Enter��ʼ����html�ĵ�������ESC���ء�"<<endl;
	char ch;
	do{
		ch = getch();
		if(ch==13){
			cout<<"   * ���������ļ�......"<<endl; 
			vector <string> html = mf->translate();
			Sleep(1000);
			cout<<"   * ������ϣ����ڴ�"<<mf->get_trans_status()<<endl;
			Sleep(1000);
			cout<<endl<<"    Ԥ��html�ĵ���"<<endl;
			string title = "doc\\";
			title += mf->get_trans_status();
			system(title.c_str());
			int len = html.size();
			cout<<"=========================================================="<<endl; 
			for(int i=0;i<len;i++){
				cout<<html[i]; 
			} 
			cout<<"=========================================================="<<endl; 
			cout<<" Ԥ����ɣ�";
			cout<<" �����뵼��html�ļ�����·���������������˲��裬����հ׺�Enter���ɣ�"<<endl;
			string path;
			cout<<"  *";
			getline(cin,path);
			if(path.empty()){	
				view_file(ms,mf);
				return;
			} 
			if(!mf->export_html(path)){
				cout<<"  *����ʧ�ܣ�����·���Ƿ���ȷ�����߷���Ȩ�����ޣ�"<<endl;
				cout<<"  *";
				system("pause");
				view_file(ms,mf);
				return;
			}
			else{
				cout<<"  *�����ɹ���"<<endl;
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
	cout<<"  �����뵼�����ļ�����·����������հ׷���"<<endl;
	cout<<"  *";
	getline(cin,path);
	if(path.empty()){	
		view_file(ms,mf);
		return;
	} 
	if(!mf->export_md(path)){
		cout<<"  *����ʧ�ܣ�����·���Ƿ���ȷ�����߷���Ȩ�����ޣ�"<<endl;
		cout<<"  *";
		system("pause");
		view_file(ms,mf);
		return;
	}
	else{
		cout<<"  *�����ɹ���"<<endl;
		Sleep(1000);
		view_file(ms,mf);
		return; 
	}
}
